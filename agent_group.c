#ifndef __AG_AGENT_GROUP__
#define __AG_AGENT_GROUP__

#include <stdlib.h>
#include <string.h>

#include "agent.c"

typedef struct AgentGroup {
  Agent *as;
  size_t count;
} AgentGroup;

typedef struct World World; // defined in world.c
typedef void (*AgentUpdate)(Agent *agent, const World *world);

AgentGroup ag_agent_group_new(void);
AgentGroup ag_agent_group_copy(const AgentGroup *group);
void ag_agent_group_destroy(AgentGroup *group);
Agent *ag_agent_group_spawn_count(AgentGroup *group, const size_t count);
void ag_agent_group_perform(AgentGroup *group, const World *world,
                            const AgentUpdate update);

AgentGroup ag_agent_group_new(void) {
  return (AgentGroup){
      .as = NULL,
      .count = 0,
  };
}

AgentGroup ag_agent_group_copy(const AgentGroup *group) {
  AgentGroup result = ag_agent_group_new();
  Agent *newptr = malloc(AG_AGENT_SIZE * group->count);
  if (newptr == NULL) {
    abort();
  }
  result.as = newptr;
  result.count = group->count;
  memcpy(result.as, group->as, AG_AGENT_SIZE * group->count);
  return result;
}

void ag_agent_group_destroy(AgentGroup *group) {
  free(group->as);
  group->count = 0;
}

// Returns a pointer to the newly created agents,
// or NULL if there was an error.
Agent *ag_agent_group_spawn_count(AgentGroup *group, const size_t count) {
  size_t old_count = group->count;
  size_t new_count = group->count + count;
  Agent *newptr = realloc(group->as, AG_AGENT_SIZE * new_count);
  if (newptr == NULL) {
    return NULL;
  }
  group->as = newptr;
  group->count = new_count;
  for (size_t index = old_count; index < new_count; index++) {
    group->as[index] = ag_agent_new();
  }
  return &group->as[old_count];
}

void ag_agent_group_perform(AgentGroup *group, const World *world,
                            const AgentUpdate update) {
  if (update == NULL) {
    return;
  }
  for (size_t index = 0; index < group->count; index++) {
    update(&group->as[index], world);
  }
}

#endif // __AG_AGENT_GROUP__
