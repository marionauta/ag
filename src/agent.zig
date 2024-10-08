const vec2 = @import("vec2.zig");
const tools = vec2.tools;

pub const AG_AGENT_IS_ALIVE = 0;
pub const AG_PATCH_HAS_GREEN = 0;

pub const Agent = extern struct {
    position: vec2.Vec2,
    direction: vec2.Vec2,
    properties: [10]i32,

    pub fn new() Agent {
        return .{
            .position = vec2.Vec2.origin(),
            .direction = vec2.Vec2.origin(),
            .properties = [_]i32{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        };
    }

    pub fn randomise_position(self: *Agent, max_x: f64, max_y: f64) void {
        self.position = vec2.Vec2.random(max_x, max_y);
    }

    pub fn randomise_direction(self: *Agent) void {
        self.direction = vec2.Vec2.random(1.0, 1.0).normalised();
    }

    pub fn move_direction(self: *Agent, direction: vec2.Vec2) void {
        self.position.x += direction.x;
        self.position.y += direction.y;
    }

    pub fn move_forward(self: *Agent, amount: f64) void {
        const dir = self.direction.normalised();
        const movement = vec2.Vec2{ .x = dir.x * amount, .y = dir.y * amount };
        self.move_direction(movement);
    }

    pub fn is_alive(self: *const Agent) bool {
        return self.properties[AG_AGENT_IS_ALIVE] > 0;
    }
};

pub const Patch = Agent;

pub export fn ag_agent_new() Agent {
    return Agent.new();
}

pub export fn ag_agent_randomise_position(self: *Agent, max_x: f64, max_y: f64) void {
    return self.randomise_position(max_x, max_y);
}

pub export fn ag_agent_randomise_direction(self: *Agent) void {
    return self.randomise_direction();
}

pub export fn ag_agent_move_direction(self: *Agent, direction: vec2.Vec2) void {
    return self.move_direction(direction);
}

pub export fn ag_agent_move_forward(self: *Agent, amount: f64) void {
    return self.move_forward(amount);
}

pub export fn ag_agent_is_alive(self: *const Agent) bool {
    return self.is_alive();
}
