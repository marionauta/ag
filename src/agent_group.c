#ifndef __AG_AGENT_GROUP__
#define __AG_AGENT_GROUP__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "agent.h"

#define AG_AGENT_GROUP_CAPACITY_CHUNKS 50

typedef struct AgentGroup {
  Agent *as;
  size_t count;
  size_t __capacity;
} AgentGroup;

typedef struct World World; // defined in world.c
typedef void (*AgentUpdate)(Agent *agent, const World *world);

AgentGroup ag_agent_group_new(void);
AgentGroup ag_agent_group_copy(const AgentGroup *group);
void ag_agent_group_destroy(AgentGroup *group);
Agent *ag_agent_group_spawn_count(AgentGroup *group, const size_t count);
void ag_agent_group_kill(AgentGroup *group, const Agent *agent);
void ag_agent_group_kill_at(AgentGroup *group, const size_t index_to_kill);
void ag_agent_group_perform(AgentGroup *group, const World *world,
                            const AgentUpdate update);

AgentGroup ag_agent_group_new(void) {
  return (AgentGroup){
      .as = NULL,
      .count = 0,
      .__capacity = 0,
  };
}

AgentGroup ag_agent_group_copy(const AgentGroup *group) {
  AgentGroup result = ag_agent_group_new();
  Agent *newptr = malloc(AG_AGENT_SIZE * group->__capacity);
  assert(newptr != NULL);
  result.as = newptr;
  result.count = group->count;
  result.__capacity = group->__capacity;
  memcpy(result.as, group->as, AG_AGENT_SIZE * group->count);
  return result;
}

void ag_agent_group_destroy(AgentGroup *group) {
  free(group->as);
  group->count = 0;
  group->__capacity = 0;
}

// Returns a pointer to the newly created agents,
// or NULL if there was an error.
Agent *ag_agent_group_spawn_count(AgentGroup *group, const size_t count) {
  const size_t old_count = group->count;
  const size_t new_count = group->count + count;
  size_t new_capacity = group->__capacity;
  while (new_count > new_capacity) {
    new_capacity += AG_AGENT_GROUP_CAPACITY_CHUNKS;
  }
  if (new_capacity != group->__capacity) {
    Agent *newptr = realloc(group->as, AG_AGENT_SIZE * new_capacity);
    if (newptr == NULL) {
      fprintf(stderr, "ERROR: Unable to spawn %zu agents.", count);
      return NULL;
    }
    group->as = newptr;
    group->__capacity = new_capacity;
  }
  for (size_t index = old_count; index < new_count; index++) {
    group->as[index] = ag_agent_new();
  }
  group->count = new_count;
  return &group->as[old_count];
}

void ag_agent_group_kill(AgentGroup *group, const Agent *agent) {
  for (size_t index = 0; index < group->count; index++) {
    if (&group->as[index] == agent) {
      ag_agent_group_kill_at(group, index);
      break;
    }
  }
}

void ag_agent_group_kill_at(AgentGroup *group, const size_t index_to_kill) {
  assert(index_to_kill < group->count && "Index larger that the group count");
  const size_t new_count = group->count - 1;
  const size_t to_move = AG_AGENT_SIZE * (new_count - index_to_kill);
  memmove(&group->as[index_to_kill], &group->as[index_to_kill + 1], to_move);
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
