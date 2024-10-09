const std = @import("std");
const Agent = @import("agent.zig").Agent;
const AgentGroup = @import("agent_group.zig").AgentGroup;

pub const AgentUpdate = fn (*Agent, *const World) void;

pub const World = struct {
    pub const WIDTH = 16;
    pub const HEIGHT = WIDTH;
    pub const PATCHES_COUNT = (WIDTH * HEIGHT);

    agents: AgentGroup,
    patches: AgentGroup,
    ticks: usize,

    pub fn new(allocator: std.mem.Allocator) World {
        var world = World{
            .agents = AgentGroup.init(allocator),
            .patches = AgentGroup.init(allocator),
            .ticks = 0,
        };
        _ = world.patches.spawn_count(PATCHES_COUNT);
        _setup_patches(&world.patches);
        return world;
    }

    pub fn copy(self: *const World) World {
        return .{
            .agents = self.agents.clone(),
            .patches = self.patches.clone(),
            .ticks = self.ticks,
        };
    }

    pub fn destroy(self: *World) void {
        self.agents.destroy();
        self.patches.destroy();
    }

    pub fn spawn_agents(self: *World, count: usize, setup_function: AgentUpdate) void {
        const agents = self.agents.spawn_count(count);
        for (agents) |*agent| {
            setup_function(agent, self);
        }
    }

    pub fn kill_agent(self: *World, agent: *const Agent) void {
        self.agents.ag_agent_group_kill(agent);
    }

    pub fn kill_agent_at(self: *World, index_to_kill: usize) void {
        self.agents.ag_agent_group_kill_at(index_to_kill);
    }

    pub fn tick(self: *const World, agent_update: AgentUpdate, patch_update: AgentUpdate) World {
        var new_world = self.copy();
        for (new_world.agents.items()) |*agent| {
            agent_update(agent, self);
        }
        for (new_world.patches.items()) |*patch| {
            patch_update(patch, self);
        }
        new_world.ticks += 1;
        return new_world;
    }

    pub fn is_done(self: *World) bool {
        for (self.patches.items()) |patch| {
            if (!patch.is_alive()) {
                return false;
            }
        }
        return true;
    }
};

fn _setup_patches(group: *AgentGroup) void {
    for (0..World.WIDTH) |col| {
        for (0..World.HEIGHT) |row| {
            const index = row * World.WIDTH + col;
            const patch = &group.list.items[index];
            patch.position.x = @floatFromInt(col);
            patch.position.y = @floatFromInt(row);
        }
    }
}
