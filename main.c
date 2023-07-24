#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "actor_group.c"
#include "simulation.c"

void _on_actor_tick(Actor *actor) {
  ag_actor_randomise_direction(actor);
  ag_actor_move_forward(actor, 1);
}

void _on_patch_tick(Actor *patch) {
  // ...
}

int main(void) {
  srand(time(0));

  Simulation simulation = ag_simulation_new();
  ag_actor_group_spawn_count(&simulation.actors, 2);
  printf("count: %zu\n", simulation.actors.count);
  for (size_t tick = 0; tick <= 100; tick++) {
    ag_simulation_run(&simulation, _on_actor_tick, _on_patch_tick);
  }

  for (size_t index = 0; index < simulation.actors.count; index++) {
    Actor actor = simulation.actors.as[index];
    printf("position (%.2f, %.2f)\n", actor.position.x, actor.position.y);
  }

  ag_simulation_destroy(&simulation);
}
