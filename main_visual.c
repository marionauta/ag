#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define __AG_USING_RAYLIB__

#include "tools.c"
#include "world.c"

#define TICKS_PER_SECOND 100
#define TICK_TIME_DELTA (1.0 / TICKS_PER_SECOND)

#define PATCH_LENGTH 40
#define SCREEN_WIDTH (PATCH_LENGTH * AG_WORLD_WIDTH)
#define SCREEN_HEIGHT (PATCH_LENGTH * AG_WORLD_HEIGHT)

void ag_world_render(const World *world);
void _on_agent_tick(Agent *agent, const World *world);
void _on_patch_tick(Patch *patch, const World *world);

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Agents");
  SetTargetFPS(60);

  World world = ag_world_new();
  ag_world_spawn_agents(&world, 1000, NULL);
  float seconds_since_tick = 0;

  bool should_close = false;
  while (!should_close && !WindowShouldClose()) {
    seconds_since_tick += GetFrameTime();
    bool should_tick = seconds_since_tick > TICK_TIME_DELTA;
    if (should_tick) {
      seconds_since_tick = 0;
      World new_world = ag_world_tick(&world, _on_agent_tick, _on_patch_tick);
      world = new_world;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    ag_world_render(&world);
    EndDrawing();
    WaitTime(TICK_TIME_DELTA);

    if (ag_world_is_done(&world)) {
      should_close = true;
    }
  }

  ag_world_destroy(&world);
  CloseWindow();
}

void _on_agent_tick(Agent *agent, const World *world) {
  ag_agent_randomise_direction(agent);
  ag_agent_move_forward(agent, 1);
}

void _on_patch_tick(Patch *patch, const World *world) {
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
  const int x = int_mod((int)(agent.position.x * PATCH_LENGTH), SCREEN_WIDTH);
  const int y = int_mod((int)(agent.position.y * PATCH_LENGTH), SCREEN_HEIGHT);
  DrawRectangle(x, y, 4, 4, WHITE);
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
