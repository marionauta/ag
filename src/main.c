#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Wextra"
#define RAYGUI_NO_ICONS
#define RAYGUI_IMPLEMENTATION
#include "vendor/raygui.h"
#pragma clang diagnostic pop

#include "config.c"
#include "tools.c"
#include "world.c"

#define TARGET_FPS 60
#define TOOLBAR_HEIGHT 60.0
#define PATCH_LENGTH 40
#define SCREEN_WIDTH (PATCH_LENGTH * AG_WORLD_WIDTH)
#define SCREEN_HEIGHT (TOOLBAR_HEIGHT + (PATCH_LENGTH * AG_WORLD_HEIGHT))

void ag_world_render(const World *world);
void ag_toolbar_render(Config *config);
void _on_agent_setup(Agent *agent, const World *world);
void _on_agent_tick(Agent *agent, const World *world);
void _on_patch_tick(Patch *patch, const World *world);

int main(void) {
  srand(time(0));

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Agents");
  SetTargetFPS(TARGET_FPS);

  Config config = {.ticks_per_second = TARGET_FPS / 2};
  World world = ag_world_new();
  ag_world_spawn_agents(&world, 1000, _on_agent_setup);
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

    BeginDrawing();
    ClearBackground(BLACK);
    ag_toolbar_render(&config);
    ag_world_render(&world);
    EndDrawing();

    if (ag_world_is_done(&world)) {
      // should_close = true;
    }
  }

  ag_world_destroy(&world);
  CloseWindow();
}

void _on_agent_setup(Agent *agent, const World *world) {
  UNUSED(world);
  ag_agent_randomise_position(agent, SCREEN_WIDTH, SCREEN_HEIGHT);
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
  int y = (int)(patch.position.y * PATCH_LENGTH) + TOOLBAR_HEIGHT;
  Color color = (patch.properties[AG_PATCH_HAS_GREEN]) ? GREEN : BROWN;
  DrawRectangle(x, y, PATCH_LENGTH, PATCH_LENGTH, color);
}

void ag_agent_render(const Agent agent) {
  const int x = int_mod((int)(agent.position.x * PATCH_LENGTH), SCREEN_WIDTH);
  const int y = int_mod((int)(agent.position.y * PATCH_LENGTH), SCREEN_HEIGHT) +
                TOOLBAR_HEIGHT;
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

void ag_toolbar_render(Config *config) {
  GuiPanel((Rectangle){.width = SCREEN_WIDTH, .height = TOOLBAR_HEIGHT}, NULL);
  {
    float tps = (float)config->ticks_per_second;
    Rectangle rec = {.x = 20, .y = 20, .width = 100, .height = 20};
    config->ticks_per_second = (size_t)GuiSlider(rec, NULL, NULL, tps, 0, 60);
  }
  {
    double height = 20;
    char label_text[50];
    sprintf(label_text, "Ticks per second: %zu", config->ticks_per_second);
    Rectangle rec = {.x = 130,
                     .y = (TOOLBAR_HEIGHT - height) / 2,
                     .width = 200,
                     .height = height};
    GuiLabel(rec, label_text);
  }
}
