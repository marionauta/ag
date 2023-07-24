#ifndef __AG_WORLD__
#define __AG_WORLD__

#include "agent_group.c"

#define AG_WORLD_WIDTH 16
#define AG_WORLD_HEIGHT AG_WORLD_WIDTH
#define AG_PATCHES_COUNT (AG_WORLD_WIDTH * AG_WORLD_HEIGHT)

typedef struct World {
  AgentGroup agents;
  AgentGroup patches;
} World;

typedef void (*AgentUpdate)(Agent *agent, const World *world);

World ag_world_new(void);
World ag_world_copy(const World *world);
void ag_world_destroy(World *world);
void ag_world_spawn_agents(World *world, size_t count, AgentUpdate setup);
World ag_world_tick(const World *world, AgentUpdate agent_update,
                    AgentUpdate patch_update);

void ag_agent_group_perform(AgentGroup *group, const World *world,
                            AgentUpdate update);

void _setup_patch(Agent *patch, const World *world) {
  patch->properties[AG_PATCH_HAS_GREEN] = true;
}

World ag_world_new(void) {
  World world = {
      .agents = ag_agent_group_new(),
      .patches = ag_agent_group_new(),
  };
  ag_agent_group_spawn_count(&world.patches, AG_PATCHES_COUNT);
  ag_agent_group_perform(&world.patches, &world, _setup_patch);
  return world;
}

World ag_world_copy(const World *world) {
  return (World){
      .agents = ag_agent_group_copy(&world->agents),
      .patches = ag_agent_group_copy(&world->patches),
  };
}

void ag_world_destroy(World *world) {
  return ag_agent_group_destroy(&world->agents);
  return ag_agent_group_destroy(&world->patches);
}

void ag_world_spawn_agents(World *world, size_t count, AgentUpdate setup) {
  Agent *agents = ag_agent_group_spawn_count(&world->agents, count);
  if (agents == NULL || setup == NULL) {
    return;
  }
  for (size_t index = 0; index < count; index++) {
    setup(&agents[index], world);
  }
}

World ag_world_tick(const World *world, AgentUpdate agent_update,
                    AgentUpdate patch_update) {
  World new_world = ag_world_copy(world);
  ag_agent_group_perform(&new_world.agents, world, agent_update);
  ag_agent_group_perform(&new_world.patches, world, patch_update);
  return new_world;
}

// AgentGroup

void ag_agent_group_perform(AgentGroup *group, const World *world,
                            AgentUpdate update) {
  for (size_t index = 0; index < group->count; index++) {
    update(&group->as[index], world);
  }
}

#endif // __AG_WORLD__
