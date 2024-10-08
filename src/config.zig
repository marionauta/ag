const std = @import("std");
const p = std.debug.print;

pub const Config = extern struct {
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

export fn ag_config_new(tps: usize) Config {
    return Config.new(tps);
}

export fn ag_config_should_tick(config: *const Config) bool {
    return config.should_tick();
}
