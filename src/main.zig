const std = @import("std");

const c = @cImport({
    @cInclude("stdlib.h");
    @cInclude("time.h");
});

const rl = @cImport({
    @cInclude("raylib.h");
});

const ag = struct {
    usingnamespace @import("config.zig");
    usingnamespace @cImport({
        @cInclude("world.c");
    });
};
// const AG_WORLD_WIDTH = 16;

const PATCH_LENGTH = 40;
const WORLD_WIDTH = PATCH_LENGTH * ag.AG_WORLD_WIDTH;
const WORLS_HEIGHT = WORLD_WIDTH;

const TARGET_FPS = 60;
const WINDOW_WIDTH = WORLD_WIDTH;
const WINDOW_HEIGHT = WORLS_HEIGHT;

pub fn main() void {
    c.srand(@truncate(@as(u64, @bitCast(c.time(0)))));

    rl.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Agents");
    defer rl.CloseWindow();

    var config = ag.Config.new(10);
    config.running = true;
    var world = ag.World{};
    defer ag.ag_world_destroy(&world);
    ag_world_setup(&world);

    var seconds_since_tick: f64 = 0;

    const should_close = false;
    while (!should_close and !rl.WindowShouldClose()) {
        const tick_time_delta = 1.0 / @as(f64, @floatFromInt(config.ticks_per_second));
        seconds_since_tick += rl.GetFrameTime();
        const should_tick = seconds_since_tick > tick_time_delta;
        if (config.should_tick() and should_tick) {
            seconds_since_tick = 0;
            const new_world = ag.ag_world_tick(&world, on_agent_tick, on_patch_tick);
            ag.ag_world_destroy(&world);
            world = new_world;
        }

        if (ag.ag_world_is_done(&world)) {
            config.running = false;
        }

        rl.BeginDrawing();
        defer rl.EndDrawing();
        rl.ClearBackground(rl.BLACK);
        ag_world_render(&world);
    }
}

fn ag_world_setup(world: *ag.World) void {
    ag.ag_world_destroy(world);
    world.* = ag.ag_world_new();
    ag.ag_world_spawn_agents(world, 1000, on_agent_setup);
}

export fn on_agent_setup(agent: ?*ag.Agent, world: ?*const ag.World) void {
    _ = world;
    ag.ag_agent_randomise_position(agent, ag.AG_WORLD_WIDTH, ag.AG_WORLD_WIDTH);
}

export fn on_agent_tick(agent: ?*ag.Agent, world: ?*const ag.World) void {
    _ = world;
    ag.ag_agent_randomise_direction(agent);
    ag.ag_agent_move_forward(agent, 1);
}

export fn on_patch_tick(patch: ?*ag.Patch, world: ?*const ag.World) void {
    _ = world;
    if (ag.ag_agent_is_alive(patch)) {
        return;
    }
    if (ag.double_random(100.0) < 0.03) {
        patch.?.properties[ag.AG_PATCH_HAS_GREEN] = 1;
    }
}

fn ag_patch_render(patch: ag.Patch) void {
    const x = @as(c_int, @intFromFloat(patch.position.x * PATCH_LENGTH));
    const y = @as(c_int, @intFromFloat(patch.position.y * PATCH_LENGTH));
    const color = if (ag.ag_agent_is_alive(&patch)) rl.GREEN else rl.BROWN;
    rl.DrawRectangle(x, y, PATCH_LENGTH, PATCH_LENGTH, color);
}

fn agent_render(agent: ag.Agent) void {
    const x = @mod(@as(c_int, @intFromFloat(agent.position.x * PATCH_LENGTH)), WORLD_WIDTH);
    const y = @mod(@as(c_int, @intFromFloat(agent.position.y * PATCH_LENGTH)), WORLS_HEIGHT);
    rl.DrawCircle(x, y, 2, rl.WHITE);
}

fn ag_world_render(world: *const ag.World) void {
    for (0..world.patches.count) |index| {
        const patch = world.patches.as[index];
        ag_patch_render(patch);
    }
    for (0..world.agents.count) |index| {
        const agent = world.agents.as[index];
        agent_render(agent);
    }
}
