const stdlib = @cImport({
    @cInclude("stdlib.h");
});

pub fn double_random(max: f64) f64 {
    const real_max = max * 2;
    const res = (real_max / stdlib.RAND_MAX) * @as(f64, @floatFromInt(stdlib.rand()));
    return res - max;
}
