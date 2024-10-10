const std = @import("std");
const expect_equal = std.testing.expectEqual;

const Agent = @import("agent.zig").Agent;
const World = @import("world.zig").World;

pub const AgentUpdate = fn (*Agent, World) void;

pub const AgentGroup = struct {
    list: std.ArrayList(Agent),

    pub fn init(allocator: std.mem.Allocator) AgentGroup {
        return .{ .list = std.ArrayList(Agent).init(allocator) };
    }

    pub fn clone(self: AgentGroup) AgentGroup {
        const list = self.list.clone() catch unreachable;
        return .{ .list = list };
    }

    pub fn destroy(self: *AgentGroup) void {
        self.list.clearRetainingCapacity();
    }

    pub fn deinit(self: *AgentGroup) void {
        self.list.deinit();
    }

    pub fn items(self: AgentGroup) []Agent {
        return self.list.items;
    }

    pub fn spawn_count(self: *AgentGroup, amount: usize) []Agent {
        return self.list.addManyAsSlice(amount) catch unreachable;
    }

    pub fn perform(self: *AgentGroup, world: World, update: AgentUpdate) void {
        for (self.list.items) |*agent| {
            update(agent, world);
        }
    }

    pub fn count(self: AgentGroup) usize {
        return self.list.items.len;
    }

    pub fn kill(self: *AgentGroup, agent: *const Agent) void {
        for (self.items, 0..) |element, index| {
            if (&element == agent) {
                self.kill_at(index);
                break;
            }
        }
    }

    pub fn kill_at(self: *AgentGroup, index_to_kill: usize) void {
        _ = self.orderedRemove(index_to_kill);
    }
};

test "spawn_count + count" {
    var group = AgentGroup.init(std.testing.allocator);
    defer group.deinit();
    _ = group.spawn_count(23);
    try expect_equal(23, group.count());
    _ = group.spawn_count(19);
    try expect_equal(42, group.count());
}

test "clone" {
    var ag = AgentGroup.init(std.testing.allocator);
    defer ag.deinit();
    const as = ag.spawn_count(1);
    as[0].properties[Agent.IS_ALIVE] = 42;
    var cp = ag.clone();
    defer cp.deinit();
    ag.list.items[0].properties[Agent.IS_ALIVE] = 0;
    try std.testing.expectEqual(42, cp.list.items[0].properties[Agent.IS_ALIVE]);
}
