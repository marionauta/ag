const Vec2 = @import("vec2.zig").Vec2;

pub const Patch = Agent;

pub const Agent = struct {
    pub const IS_ALIVE = 0;
    pub const HAS_GREEN = IS_ALIVE;

    position: Vec2,
    direction: Vec2,
    properties: [10]i32,

    pub fn new() Agent {
        return .{
            .position = Vec2.origin(),
            .direction = Vec2.origin(),
            .properties = [_]i32{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        };
    }

    pub fn randomise_position(self: *Agent, max_x: f64, max_y: f64) void {
        self.position = Vec2.random(max_x, max_y);
    }

    pub fn randomise_direction(self: *Agent) void {
        self.direction = Vec2.random(1.0, 1.0).normalised();
    }

    pub fn move_direction(self: *Agent, direction: Vec2) void {
        self.position.x += direction.x;
        self.position.y += direction.y;
    }

    pub fn move_forward(self: *Agent, amount: f64) void {
        const direction = self.direction.normalised();
        self.move_direction(direction.times(amount));
    }

    pub fn is_alive(self: Agent) bool {
        return self.properties[IS_ALIVE] > 0;
    }
};
