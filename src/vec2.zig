const std = @import("std");
const expectEqual = std.testing.expectEqual;

pub const tools = @import("tools.zig");

pub const Vec2 = struct {
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

test "origin" {
    const v = Vec2.origin();
    try expectEqual(v.x, 0);
    try expectEqual(v.y, 0);
}

test "length" {
    const v1 = Vec2{ .x = 1, .y = 0 };
    try expectEqual(v1.length(), 1.0);
    const v2 = Vec2{ .x = 0, .y = 1 };
    try expectEqual(v2.length(), 1.0);
    const v3 = Vec2{ .x = 1, .y = 1 };
    try expectEqual(v3.length(), @sqrt(2.0));
    const v4 = Vec2{ .x = 3, .y = 4 };
    try expectEqual(v4.length(), 5.0);
}

test "normalised" {
    const v = Vec2.random(100, 100).normalised();
    try expectEqual(v.length(), 1.0);
}
