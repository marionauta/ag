#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "raygui.h"
#include "raylib.h"

#include "config.c"
#include "tools.c"
#include "world.c"

#define PATCH_LENGTH 40
#define WORLD_WIDTH (PATCH_LENGTH * AG_WORLD_WIDTH)
#define WORLD_HEIGHT WORLD_WIDTH

#define TARGET_FPS 60
#define TOOLBAR_HEIGHT 60.0
#define WINDOW_WIDTH WORLD_WIDTH
#define WINDOW_HEIGHT (TOOLBAR_HEIGHT + WORLD_HEIGHT)

void ag_world_setup(World *world);
void ag_world_render(const World *world);
void ag_toolbar_render(Config *config, World *world);
void ag_window_handle_events(Config *config, World *world);
void _on_agent_setup(Agent *agent, const World *world);
void _on_agent_tick(Agent *agent, const World *world);
void _on_patch_tick(Patch *patch, const World *world);

int main(void) {
  srand(time(0));

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Agents");
  SetTargetFPS(TARGET_FPS);
  RenderTexture world_render_texture =
      LoadRenderTexture(WORLD_WIDTH, WORLD_HEIGHT);

  Config config = {.ticks_per_second = TARGET_FPS / 2};
  World world = {0};
  ag_world_setup(&world);
  float seconds_since_tick = 0;

  bool should_close = false;
  while (!should_close && !WindowShouldClose()) {
    double tick_time_delta = 1.0 / config.ticks_per_second;
    seconds_since_tick += GetFrameTime();
    bool should_tick = seconds_since_tick > tick_time_delta;
    if (ag_config_should_tick(&config) && should_tick) {
      seconds_since_tick = 0;
      World new_world = ag_world_tick(&world, _on_agent_tick, _on_patch_tick);
      ag_world_destroy(&world);
      world = new_world;
    }

    // Handle keyboard events
    ag_window_handle_events(&config, &world);

    // Render world into texture
    BeginTextureMode(world_render_texture);
    ag_world_render(&world);
    EndTextureMode();

    // Render whole window
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(world_render_texture.texture, 0, TOOLBAR_HEIGHT, WHITE);
    ag_toolbar_render(&config, &world);
    EndDrawing();

    if (ag_world_is_done(&world)) {
      config.running = false;
    }
  }

  ag_world_destroy(&world);
  UnloadRenderTexture(world_render_texture);
  CloseWindow();
}

void ag_world_setup(World *world) {
  if (world == NULL) {
    return;
  }
  ag_world_destroy(world);
  *world = ag_world_new();
  ag_world_spawn_agents(world, 1000, _on_agent_setup);
}

void _on_agent_setup(Agent *agent, const World *world) {
  UNUSED(world);
  ag_agent_randomise_position(agent, AG_WORLD_WIDTH, AG_WORLD_HEIGHT);
}

void _on_agent_tick(Agent *agent, const World *world) {
  UNUSED(world);
  ag_agent_randomise_direction(agent);
  ag_agent_move_forward(agent, 1);
}

void _on_patch_tick(Patch *patch, const World *world) {
  UNUSED(world);
  if (patch->properties[AG_PATCH_HAS_GREEN]) {
    return;
  }
  if ((rand() % 100) < 3) {
    patch->properties[AG_PATCH_HAS_GREEN] = true;
  }
}

void ag_patch_render(const Patch patch) {
  int x = (int)(patch.position.x * PATCH_LENGTH);
  int y = (int)(patch.position.y * PATCH_LENGTH);
  Color color = (patch.properties[AG_PATCH_HAS_GREEN]) ? GREEN : BROWN;
  DrawRectangle(x, y, PATCH_LENGTH, PATCH_LENGTH, color);
}

void ag_agent_render(const Agent agent) {
  const int x = int_mod((int)(agent.position.x * PATCH_LENGTH), WORLD_WIDTH);
  const int y = int_mod((int)(agent.position.y * PATCH_LENGTH), WORLD_HEIGHT);
  DrawCircle(x, y, 2, WHITE);
}

void ag_world_render(const World *world) {
  for (size_t index = 0; index < world->patches.count; index++) {
    Patch patch = world->patches.as[index];
    ag_patch_render(patch);
  }
  for (size_t index = 0; index < world->agents.count; index++) {
    Agent agent = world->agents.as[index];
    ag_agent_render(agent);
  }
}

void ag_toolbar_render(Config *config, World *world) {
  GuiPanel((Rectangle){.width = WINDOW_WIDTH, .height = TOOLBAR_HEIGHT}, NULL);
  {
    Rectangle rec = {.x = 20, .y = 20, .width = 80, .height = 20};
    if (GuiButton(rec, GuiIconText(ICON_RESTART, "Setup"))) {
      ag_world_setup(world);
    }
  }
  {
    Rectangle rec = {.x = 110, .y = 20, .width = 60, .height = 20};
    GuiToggle(rec, "Go", &config->running);
  }
  {
    double height = 20;
    char label_text[50];
    sprintf(label_text, "Ticks per second: %zu", config->ticks_per_second);
    Rectangle rec = {.x = 180, .y = 8, .width = 200, .height = height};
    GuiLabel(rec, label_text);
  }
  {
    float tps = (float)config->ticks_per_second;
    Rectangle rec = {.x = 180, .y = 25, .width = 150, .height = 15};
    int res = GuiSlider(rec, NULL, NULL, &tps, 1, TARGET_FPS);
    printf("res = %d\n", res);
    config->ticks_per_second = tps;
  }
}

void ag_window_handle_events(Config *config, World *world) {
  if (IsKeyPressed(KEY_R)) {
    ag_world_setup(world);
  }
  if (IsKeyPressed(KEY_ENTER)) {
    config->running = !config->running;
  }
}
