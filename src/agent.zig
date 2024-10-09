const vec2 = @import("vec2.zig");
const tools = vec2.tools;

pub const Agent = struct {
    pub const IS_ALIVE = 0;
    pub const HAS_GREEN = IS_ALIVE;

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
        return self.properties[IS_ALIVE] > 0;
    }
};

pub const Patch = Agent;
