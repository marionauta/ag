pub const tools = struct {
    usingnamespace @import("tools.zig");
};

pub const Vec2 = extern struct {
    x: f64,
    y: f64,

    pub fn origin() @This() {
        return .{ .x = 0, .y = 0 };
    }

    pub fn random(max_x: f64, max_y: f64) @This() {
        return .{
            .x = tools.double_random(max_x),
            .y = tools.double_random(max_y),
        };
    }

    pub fn length(self: Vec2) f64 {
        return @sqrt((self.x * self.x) + (self.y * self.y));
    }

    pub fn normalise(self: *Vec2) void {
        const len = self.length();
        self.x /= len;
        self.y /= len;
    }

    pub fn normalised(self: Vec2) Vec2 {
        var v = self;
        v.normalise();
        return v;
    }
};
