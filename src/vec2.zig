const tools = struct {
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
};

pub export fn ag_vec2_origin() Vec2 {
    return Vec2.origin();
}

pub export fn ag_vec2_random(max_x: f64, max_y: f64) Vec2 {
    return Vec2.random(max_x, max_y);
}

pub export fn ag_vec2_length(vector: *const Vec2) f64 {
    return vector.length();
}

pub export fn ag_vec2_normalise(vector: *Vec2) void {
    return vector.normalise();
}

pub export fn ag_vec2_normalised(vector: Vec2) Vec2 {
    var v = vector;
    v.normalise();
    return v;
}
