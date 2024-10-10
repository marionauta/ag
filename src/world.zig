const std = @import("std");

const Agent = @import("agent.zig").Agent;
const agent_group = @import("agent_group.zig");
const AgentGroup = agent_group.AgentGroup;
const AgentUpdate = agent_group.AgentUpdate;

pub const World = struct {
    pub const WIDTH = 16;
    pub const HEIGHT = WIDTH;
    pub const PATCHES_COUNT = (WIDTH * HEIGHT);

    agents: AgentGroup,
    patches: AgentGroup,
    ticks: usize = 0,

    pub fn init(allocator: std.mem.Allocator) World {
        var world = World{
            .agents = AgentGroup.init(allocator),
            .patches = AgentGroup.init(allocator),
        };
        const patches = world.patches.spawn_count(PATCHES_COUNT);
        setup_patches(patches);
        return world;
    }

    pub fn clone(self: *const World) World {
        return .{
            .agents = self.agents.clone(),
            .patches = self.patches.clone(),
            .ticks = self.ticks,
        };
    }

    pub fn destroy(self: *World) void {
        self.agents.destroy();
        setup_patches(self.patches.list.items);
    }

    pub fn deinit(self: *World) void {
        self.agents.deinit();
        self.patches.deinit();
    }

    pub fn spawn_agents(self: *World, count: usize) []Agent {
        return self.agents.spawn_count(count);
    }

    pub fn spawn_agents_setup(self: *World, count: usize, setup_function: AgentUpdate) void {
        const agents = self.spawn_agents(count);
        for (agents) |*agent| {
            setup_function(agent, self.*);
        }
    }

    pub fn kill_agent(self: *World, agent: *const Agent) void {
        self.agents.ag_agent_group_kill(agent);
    }

    pub fn kill_agent_at(self: *World, index_to_kill: usize) void {
        self.agents.ag_agent_group_kill_at(index_to_kill);
    }

    pub fn tick(self: World, agent_update: AgentUpdate, patch_update: AgentUpdate) World {
        var new_world = self.clone();
        new_world.agents.perform(self, agent_update);
        new_world.patches.perform(self, patch_update);
        new_world.ticks += 1;
        return new_world;
    }

    pub fn is_done(self: World) bool {
        for (self.patches.items()) |patch| {
            if (!patch.is_alive()) {
                return false;
            }
        }
        return true;
    }

    pub fn is_new(self: World) bool {
        return self.ticks == 0;
    }
};

fn setup_patches(patches: []Agent) void {
    for (0..World.WIDTH) |col| {
        for (0..World.HEIGHT) |row| {
            const index = row * World.WIDTH + col;
            const patch = &patches[index];
            patch.position.x = @floatFromInt(col);
            patch.position.y = @floatFromInt(row);
            patch.properties[Agent.HAS_GREEN] = 0;
        }
    }
}
