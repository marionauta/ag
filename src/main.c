#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "agent_group.c"
#include "world.c"

void _print_agent_position(Agent *agent, const World *world) {
  printf("position (%.2f, %.2f)\n", agent->position.x, agent->position.y);
}

void _on_agent_tick(Agent *agent, const World *world) {
  ag_agent_randomise_direction(agent);
  ag_agent_move_forward(agent, 1);
}

void _on_patch_tick(Agent *patch, const World *world) {
  if (patch->properties[AG_PATCH_HAS_GREEN]) {
    return;
  }
  if ((rand() % 100) < 3) {
    patch->properties[AG_PATCH_HAS_GREEN] = true;
  }
}

int main(void) {
  srand(time(0));

  World world = ag_world_new();
  ag_world_spawn_agents(&world, 2, NULL);

  for (size_t tick = 0; tick <= 100; tick++) {
    World new_world = ag_world_tick(&world, _on_agent_tick, _on_patch_tick);
    ag_world_destroy(&world);
    world = new_world;
  }

  ag_agent_group_perform(&world.agents, &world, _print_agent_position);
  ag_world_destroy(&world);
}
