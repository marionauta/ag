const stdlib = @cImport({
    @cInclude("stdlib.h");
});

pub export fn double_random(max: f64) f64 {
    const sign = @as(f64, if (stdlib.rand() > stdlib.RAND_MAX / 2) -1 else 1);
    return sign * (max / stdlib.RAND_MAX) * @as(f64, @floatFromInt(stdlib.rand()));
}

pub export fn int_mod(a: i64, b: i64) i64 {
    return @mod(a, b);
}
