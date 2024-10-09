const std = @import("std");
pub const agent = @import("agent.zig");

pub const AgentGroup = std.ArrayList(agent.Agent);

pub fn ag_agent_group_new(allocator: std.mem.Allocator) AgentGroup {
    return AgentGroup.init(allocator);
}

pub fn ag_agent_group_copy(group: *const AgentGroup) AgentGroup {
    const as = group.clone() catch AgentGroup.init(std.heap.c_allocator);
    return as;
}

pub export fn ag_agent_group_destroy(group: *AgentGroup) void {
    group.clearRetainingCapacity();
}

pub fn spawn_count(self: *AgentGroup, count: usize) []agent.Agent {
    return self.addManyAsSlice(count) catch unreachable;
}

pub export fn ag_agent_group_kill(group: *AgentGroup, a: *const agent.Agent) void {
    for (group.items, 0..) |element, index| {
        if (&element == a) {
            ag_agent_group_kill_at(group, index);
            break;
        }
    }
}

pub export fn ag_agent_group_kill_at(group: *AgentGroup, index_to_kill: usize) void {
    _ = group.orderedRemove(index_to_kill);
}
