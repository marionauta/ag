#ifndef __AG_AGENT_GROUP__
#define __AG_AGENT_GROUP__

#include <stdbool.h>
#include <stdlib.h>

#include "agent.c"

typedef struct AgentGroup {
  Agent *as;
  size_t count;
} AgentGroup;

AgentGroup ag_agent_group_new(void);
void ag_agent_group_destroy(AgentGroup *group);
bool ag_agent_group_spawn_count(AgentGroup *group, size_t count);
void ag_agent_group_perform(AgentGroup group, AgentUpdate update);

AgentGroup ag_agent_group_new(void) {
  return (AgentGroup){
      .as = NULL,
      .count = 0,
  };
}

void ag_agent_group_destroy(AgentGroup *group) {
  if (group->as != NULL) {
    free(group->as);
  }
  group->count = 0;
}

// Returns true if the operation was successful, false if not.
bool ag_agent_group_spawn_count(AgentGroup *group, size_t count) {
  size_t old_count = group->count;
  size_t new_count = group->count + count;
  Agent *newptr = realloc(group->as, AG_AGENT_SIZE * new_count);
  if (newptr == NULL) {
    return false;
  }
  group->as = newptr;
  group->count = new_count;
  for (size_t index = old_count; index < new_count; index++) {
    group->as[index] = ag_agent_new();
    group->as[index].properties[AG_AGENT_IS_ALIVE] = true;
  }
  return true;
}

void ag_agent_group_perform(AgentGroup group, AgentUpdate update) {
  for (size_t index = 0; index < group.count; index++) {
    Agent *agent = &group.as[index];
    if (!ag_agent_is_alive(agent)) {
      continue;
    }
    update(&group.as[index]);
  }
}

#endif // __AG_AGENT_GROUP__
