const std = @import("std");

const c = @cImport({
    @cInclude("stdlib.h");
    @cInclude("time.h");
});

const rl = @cImport({
    @cInclude("raylib.h");
});

const rg = @cImport({
    @cInclude("raygui.h");
});

const ag = struct {
    usingnamespace @import("agent.zig");
    usingnamespace @import("config.zig");
    usingnamespace @import("world.zig");
    usingnamespace @import("tools.zig");
};

const scripting = @import("scripting.zig");

const PATCH_LENGTH = 40;
const WORLD_WIDTH = PATCH_LENGTH * ag.World.WIDTH;
const WORLD_HEIGHT = WORLD_WIDTH;

const TARGET_FPS = 60;
const TOOLBAR_HEIGHT = 60.0;
const WINDOW_WIDTH = WORLD_WIDTH;
const WINDOW_HEIGHT = WORLD_HEIGHT + TOOLBAR_HEIGHT;
const WINDOW_TITLE = "Agents";

pub fn main() !void {
    const allocator = std.heap.page_allocator;

    c.srand(@truncate(@as(u64, @bitCast(c.time(0)))));

    rl.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

    var args = try std.process.argsWithAllocator(allocator);
    defer args.deinit();

    _ = args.skip(); // skip program name
    var state: ?scripting.State = null;
    if (args.next()) |filename| {
        state = try scripting.state_from_file(filename.ptr);
    }
    defer scripting.state_close(state);
    if (state) |L| {
        const title = scripting.read_string(L, "window_title") catch "untitled";
        rl.SetWindowTitle(rl.TextFormat("%s - %s", title, WINDOW_TITLE));
    }

    defer rl.CloseWindow();
    rl.SetTargetFPS(TARGET_FPS);
    const world_render_texture = rl.LoadRenderTexture(WORLD_WIDTH, WORLD_HEIGHT);
    defer rl.UnloadRenderTexture(world_render_texture);

    var config = ag.Config.new(TARGET_FPS / 2);
    var world = ag.World.init(allocator);
    defer world.deinit();
    ag_world_setup(&world, state);
    var seconds_since_tick: f64 = 0;

    const should_close = false;
    while (!should_close and !rl.WindowShouldClose()) {
        const tick_time_delta = 1.0 / @as(f64, @floatFromInt(config.ticks_per_second));
        seconds_since_tick += rl.GetFrameTime();
        const should_tick = !world.is_done() and seconds_since_tick > tick_time_delta;
        if (config.should_tick() and should_tick) {
            seconds_since_tick = 0;
            const new_world = world.tick(on_agent_tick, on_patch_tick);
            world.deinit();
            world = new_world;
        }

        window_handle_events(&config, &world, state);

        if (world.is_done()) {
            config.running = false;
        }

        rl.BeginTextureMode(world_render_texture);
        ag_world_render(&world);
        rl.EndTextureMode();

        rl.BeginDrawing();
        defer rl.EndDrawing();
        rl.ClearBackground(rl.BLACK);
        rl.DrawTexture(world_render_texture.texture, 0, TOOLBAR_HEIGHT, rl.WHITE);
        ag_toolbar_render(&config, &world, state);
    }
}

fn ag_world_setup(world: *ag.World, state: ?scripting.State) void {
    world.destroy();
    if (state) |L| {
        const agent_count: usize = @intFromFloat(scripting.read_number(L, "agent_count") catch 0);
        world.spawn_agents_setup(agent_count, on_agent_setup);
    }
}

fn on_agent_setup(agent: *ag.Agent, world: ag.World) void {
    _ = world;
    agent.randomise_position(ag.World.WIDTH, ag.World.HEIGHT);
}

fn on_agent_tick(agent: *ag.Agent, world: ag.World) void {
    _ = world;
    agent.randomise_direction();
    agent.move_forward(1);
}

fn on_patch_tick(patch: *ag.Patch, world: ag.World) void {
    _ = world;
    if (patch.is_alive()) {
        return;
    }
    if (@mod(c.rand(), 100) < 3) {
        patch.properties[ag.Patch.HAS_GREEN] = 1;
    }
}

fn ag_patch_render(patch: ag.Patch) void {
    const x = @as(c_int, @intFromFloat(patch.position.x * PATCH_LENGTH));
    const y = @as(c_int, @intFromFloat(patch.position.y * PATCH_LENGTH));
    const color = if (patch.is_alive()) rl.GREEN else rl.BROWN;
    rl.DrawRectangle(x, y, PATCH_LENGTH, PATCH_LENGTH, color);
}

fn agent_render(agent: ag.Agent) void {
    const x = @mod(@as(c_int, @intFromFloat(agent.position.x * PATCH_LENGTH)), WORLD_WIDTH);
    const y = @mod(@as(c_int, @intFromFloat(agent.position.y * PATCH_LENGTH)), WORLD_HEIGHT);
    rl.DrawCircle(x, y, 2, rl.WHITE);
}

fn ag_world_render(world: *const ag.World) void {
    for (world.patches.items()) |patch| {
        ag_patch_render(patch);
    }
    for (world.agents.items()) |agent| {
        agent_render(agent);
    }
}

fn ag_toolbar_render(config: *ag.Config, world: *ag.World, state: ?scripting.State) void {
    _ = rg.GuiPanel(.{ .width = WINDOW_WIDTH, .height = TOOLBAR_HEIGHT }, null);

    if (rg.GuiButton(
        .{ .x = 20, .y = 20, .width = 80, .height = 20 },
        rg.GuiIconText(rg.ICON_RESTART, if (world.is_new()) "Restart" else "Setup"),
    ) > 0) {
        ag_world_setup(world, state);
    }
    _ = rg.GuiToggle(
        .{ .x = 110, .y = 20, .width = 60, .height = 20 },
        "Go",
        &config.running,
    );
    _ = rg.GuiLabel(
        .{ .x = 180, .y = 8, .width = 200, .height = 20 },
        rl.TextFormat("Ticks per second: %zu", config.ticks_per_second),
    );
    {
        var tps: f32 = @floatFromInt(config.ticks_per_second);
        _ = rg.GuiSlider(
            .{ .x = 180, .y = 25, .width = 150, .height = 15 },
            null,
            null,
            &tps,
            1,
            TARGET_FPS,
        );
        config.ticks_per_second = @intFromFloat(tps);
    }
}

fn window_handle_events(config: *ag.Config, world: *ag.World, state: ?scripting.State) void {
    if (rl.IsKeyPressed(rl.KEY_R)) {
        ag_world_setup(world, state);
    }
    if (rl.IsKeyPressed(rl.KEY_ENTER)) {
        config.running = !config.running;
    }
}
