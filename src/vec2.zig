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

    pub fn times(self: Vec2, amount: f64) Vec2 {
        return .{ .x = self.x * amount, .y = self.y * amount };
    }
};

test "origin" {
    const v = Vec2.origin();
    try expectEqual(0.0, v.x);
    try expectEqual(0.0, v.y);
}

test "length" {
    const v1 = Vec2{ .x = 1, .y = 0 };
    try expectEqual(1.0, v1.length());
    const v2 = Vec2{ .x = 0, .y = 1 };
    try expectEqual(1.0, v2.length());
    const v3 = Vec2{ .x = 1, .y = 1 };
    try expectEqual(@sqrt(2.0), v3.length());
    const v4 = Vec2{ .x = 3, .y = 4 };
    try expectEqual(5.0, v4.length());
}

test "normalised" {
    const v = Vec2.random(100, 100).normalised();
    try expectEqual(1.0, v.length());
}

test "times" {
    const v = Vec2.random(100, 100).normalised();
    const t = v.times(2).times(4);
    try expectEqual(8.0, t.length());
}
