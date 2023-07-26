#ifndef __AG_WORLD__
#define __AG_WORLD__

#include <stdbool.h>

#include "agent_group.c"

#define AG_WORLD_WIDTH 16
#define AG_WORLD_HEIGHT AG_WORLD_WIDTH
#define AG_PATCHES_COUNT (AG_WORLD_WIDTH * AG_WORLD_HEIGHT)

typedef struct World {
  AgentGroup agents;
  AgentGroup patches;
} World;

World ag_world_new(void);
World ag_world_copy(const World *world);
void ag_world_destroy(World *world);
void ag_world_spawn_agents(World *world, size_t count, const AgentUpdate setup);
World ag_world_tick(const World *world, const AgentUpdate agent_update,
                    const AgentUpdate patch_update);

void _setup_patches(AgentGroup *group);

World ag_world_new(void) {
  World world = {
      .agents = ag_agent_group_new(),
      .patches = ag_agent_group_new(),
  };
  ag_agent_group_spawn_count(&world.patches, AG_PATCHES_COUNT);
  _setup_patches(&world.patches);
  return world;
}

World ag_world_copy(const World *world) {
  return (World){
      .agents = ag_agent_group_copy(&world->agents),
      .patches = ag_agent_group_copy(&world->patches),
  };
}

void ag_world_destroy(World *world) {
  ag_agent_group_destroy(&world->agents);
  ag_agent_group_destroy(&world->patches);
}

void ag_world_spawn_agents(World *world, size_t count,
                           const AgentUpdate setup) {
  Agent *agents = ag_agent_group_spawn_count(&world->agents, count);
  if (agents == NULL || setup == NULL) {
    return;
  }
  for (size_t index = 0; index < count; index++) {
    setup(&agents[index], world);
  }
}

World ag_world_tick(const World *world, const AgentUpdate agent_update,
                    const AgentUpdate patch_update) {
  World new_world = ag_world_copy(world);
  ag_agent_group_perform(&new_world.agents, world, agent_update);
  ag_agent_group_perform(&new_world.patches, world, patch_update);
  return new_world;
}

// Patches

void _setup_patches(AgentGroup *group) {
  for (size_t col = 0; col < AG_WORLD_WIDTH; col++) {
    for (size_t row = 0; row < AG_WORLD_HEIGHT; row++) {
      size_t index = row * AG_WORLD_WIDTH + col;
      Patch *patch = &group->as[index];
      patch->position.x = (double)col;
      patch->position.y = (double)row;
      patch->properties[AG_PATCH_HAS_GREEN] = false;
    }
  }
}

#endif // __AG_WORLD__
