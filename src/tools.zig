const stdlib = @cImport({
    @cInclude("stdlib.h");
});

pub fn double_random(max: f64) f64 {
    const sign = @as(f64, if (stdlib.rand() > stdlib.RAND_MAX / 2) -1 else 1);
    return sign * (max / stdlib.RAND_MAX) * @as(f64, @floatFromInt(stdlib.rand()));
}
