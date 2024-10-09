pub const Config = struct {
    running: bool,
    ticks_per_second: usize,

    pub fn new(tps: usize) @This() {
        return .{
            .running = false,
            .ticks_per_second = tps,
        };
    }

    pub fn should_tick(self: @This()) bool {
        return self.running and self.ticks_per_second > 0;
    }
};
