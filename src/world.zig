const ag = @import("agent_group.zig");

pub const AG_WORLD_WIDTH = 16;
pub const AG_WORLD_HEIGHT = AG_WORLD_WIDTH;
pub const AG_PATCHES_COUNT = (AG_WORLD_WIDTH * AG_WORLD_HEIGHT);

const Agent = ag.agent.Agent;
pub const AgentUpdate = fn (*ag.agent.Agent, *const World) void;

pub const World = struct {
    agents: ag.AgentGroup,
    patches: ag.AgentGroup,
    ticks: usize,

    pub fn new() World {
        var world = World{
            .agents = ag.ag_agent_group_new(),
            .patches = ag.ag_agent_group_new(),
            .ticks = 0,
        };
        _ = ag.spawn_count(&world.patches, AG_PATCHES_COUNT);
        _setup_patches(&world.patches);
        return world;
    }

    pub fn copy(self: *const World) World {
        return .{
            .agents = ag.ag_agent_group_copy(&self.agents),
            .patches = ag.ag_agent_group_copy(&self.patches),
            .ticks = self.ticks,
        };
    }

    pub fn destroy(self: *World) void {
        ag.ag_agent_group_destroy(&self.agents);
        ag.ag_agent_group_destroy(&self.patches);
    }

    pub fn spawn_agents(self: *World, count: usize, setup_function: AgentUpdate) void {
        const agents = ag.spawn_count(&self.agents, count);
        for (agents) |*agent| {
            setup_function(agent, self);
        }
    }

    pub fn kill_agent(self: *World, agent: *const Agent) void {
        ag.ag_agent_group_kill(&self.agents, agent);
    }

    pub fn kill_agent_at(self: *World, index_to_kill: usize) void {
        ag.ag_agent_group_kill_at(&self.agents, index_to_kill);
    }

    pub fn tick(self: *const World, agent_update: AgentUpdate, patch_update: AgentUpdate) World {
        var new_world = self.copy();
        for (new_world.agents.items) |*agent| {
            agent_update(agent, self);
        }
        for (new_world.patches.items) |*patch| {
            patch_update(patch, self);
        }
        new_world.ticks += 1;
        return new_world;
    }

    pub fn is_done(self: *World) bool {
        for (self.patches.items) |patch| {
            if (!patch.is_alive()) {
                return false;
            }
        }
        return true;
    }
};

fn _setup_patches(group: *ag.AgentGroup) void {
    for (0..AG_WORLD_WIDTH) |col| {
        for (0..AG_WORLD_HEIGHT) |row| {
            const index = row * AG_WORLD_WIDTH + col;
            const patch = &group.items[index];
            patch.position.x = @floatFromInt(col);
            patch.position.y = @floatFromInt(row);
        }
    }
}
