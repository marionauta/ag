#ifndef __AG_SIMULATION__
#define __AG_SIMULATION__

#include <stdlib.h>

#include "agent.c"
#include "agent_group.c"
#include "world.c"

void ag_simulation_run(World *world, AgentUpdate agent_update,
                       AgentUpdate patch_update);

void ag_simulation_run(World *world, AgentUpdate agent_update,
                       AgentUpdate patch_update) {
  ag_agent_group_perform(world->agents, agent_update);
  ag_agent_group_perform(world->patches, patch_update);
}

#endif // __AG_SIMULATION__
