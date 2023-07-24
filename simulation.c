#ifndef __AG_SIMULATION__
#define __AG_SIMULATION__

#include <stdlib.h>

#include "agent.c"
#include "agent_group.c"

#define SIMULATION_WIDTH 16
#define SIMULATION_HEIGHT SIMULATION_WIDTH

typedef struct Simulation {
  Agent patches[SIMULATION_HEIGHT][SIMULATION_WIDTH];
  AgentGroup agents;
} Simulation;

Simulation ag_simulation_new(void);
void ag_simulation_destroy(Simulation *simulation);
void ag_simulation_run(Simulation *simulation, AgentUpdate agent_update,
                       AgentUpdate patch_update);

Simulation ag_simulation_new() {
  Simulation simulation = {
      .agents = ag_agent_group_new(),
  };
  for (size_t row = 0; row < SIMULATION_HEIGHT; row++) {
    for (size_t col = 0; col < SIMULATION_WIDTH; col++) {
      simulation.patches[row][col] = ag_agent_new();
      simulation.patches[row][col].properties[AG_PATCH_HAS_GREEN] = true;
    }
  }
  return simulation;
}

void ag_simulation_destroy(Simulation *simulation) {
  return ag_agent_group_destroy(&simulation->agents);
}

void ag_simulation_run(Simulation *simulation, AgentUpdate agent_update,
                       AgentUpdate patch_update) {
  ag_agent_group_perform(simulation->agents, agent_update);
  for (size_t row = 0; row < SIMULATION_HEIGHT; row++) {
    for (size_t col = 0; col < SIMULATION_WIDTH; col++) {
      Agent *patch = &simulation->patches[row][col];
      patch_update(patch);
    }
  }
}

#endif // __AG_SIMULATION__
