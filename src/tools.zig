const stdlib = @cImport({
    @cInclude("stdlib.h");
});

pub fn double_random(max: f64) f64 {
    return (max / stdlib.RAND_MAX) * @as(f64, @floatFromInt(stdlib.rand()));
}
