const std = @import("std");
const Agent = @import("agent.zig").Agent;

pub const AgentGroup = struct {
    list: std.ArrayList(Agent),

    pub fn init(allocator: std.mem.Allocator) AgentGroup {
        return .{ .list = std.ArrayList(Agent).init(allocator) };
    }

    pub fn items(self: AgentGroup) []Agent {
        return self.list.items;
    }

    pub fn clone(self: AgentGroup) AgentGroup {
        const list = self.list.clone() catch unreachable;
        return .{ .list = list };
    }

    pub fn destroy(self: *AgentGroup) void {
        self.list.clearAndFree();
    }

    pub fn spawn_count(self: *AgentGroup, count: usize) []Agent {
        return self.list.addManyAsSlice(count) catch unreachable;
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

test "clone" {
    var ag = AgentGroup.init(std.testing.allocator);
    defer ag.destroy();
    const as = ag.spawn_count(1);
    as[0] = 42;
    var cp = try ag.clone();
    defer cp.destroy();
    ag.list.items[0].properties[Agent.IS_ALIVE] = 0;
    try std.testing.expectEqual(42, cp.list.items[0].properties[Agent.IS_ALIVE]);
}
