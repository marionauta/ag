#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "agent_group.c"
#include "simulation.c"

void _on_agent_tick(Agent *agent) {
  ag_agent_randomise_direction(agent);
  ag_agent_move_forward(agent, 1);
}

void _on_patch_tick(Agent *patch) {
  if (patch->properties[AG_PATCH_HAS_GREEN]) {
    return;
  }
  if ((rand() % 100) < 3) {
    patch->properties[AG_PATCH_HAS_GREEN] = true;
  }
}

int main(void) {
  srand(time(0));

  Simulation simulation = ag_simulation_new();
  ag_agent_group_spawn_count(&simulation.agents, 2);
  printf("count: %zu\n", simulation.agents.count);
  for (size_t tick = 0; tick <= 100; tick++) {
    ag_simulation_run(&simulation, _on_agent_tick, _on_patch_tick);
  }

  for (size_t index = 0; index < simulation.agents.count; index++) {
    Agent agent = simulation.agents.as[index];
    printf("position (%.2f, %.2f)\n", agent.position.x, agent.position.y);
  }

  ag_simulation_destroy(&simulation);
}
