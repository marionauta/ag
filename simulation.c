#ifndef __AG_SIMULATION__
#define __AG_SIMULATION__

#include <stdlib.h>

#include "actor.c"
#include "actor_group.c"

// #define SIMULATION_WIDTH 16
// #define SIMULATION_HEIGHT SIMULATION_WIDTH

typedef struct Simulation {
  // Actor patches[SIMULATION_HEIGHT][SIMULATION_WIDTH];
  ActorGroup actors;
} Simulation;

Simulation ag_simulation_new(void);
void ag_simulation_destroy(Simulation *simulation);
void ag_simulation_run(Simulation *simulation, ActorUpdate actor_update);

Simulation ag_simulation_new() {
  Simulation simulation = {
      .actors = ag_actor_group_new(),
  };
  // for (int row = 0; row < SIMULATION_HEIGHT; row++) {
  //   for (int col = 0; col < SIMULATION_WIDTH; col++) {
  //     simulation.patches[row][col] = ag_actor_new();
  //   }
  // }
  return simulation;
}

void ag_simulation_destroy(Simulation *simulation) {
  return ag_actor_group_destroy(&simulation->actors);
}

void ag_simulation_run(Simulation *simulation, ActorUpdate actor_update) {
  ag_actor_group_perform(simulation->actors, actor_update);
}

#endif // __AG_SIMULATION__
