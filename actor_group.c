#ifndef __AG_ACTOR_GROUP__
#define __AG_ACTOR_GROUP__

#include <stdbool.h>
#include <stdlib.h>

#include "actor.c"

typedef struct ActorGroup {
  Actor *as;
  size_t count;
} ActorGroup;

ActorGroup ag_actor_group_new(void);
void ag_actor_group_destroy(ActorGroup *group);
bool ag_actor_group_spawn_count(ActorGroup *group, size_t count);
void ag_actor_group_perform(ActorGroup group, ActorUpdate update);

ActorGroup ag_actor_group_new(void) {
  return (ActorGroup){
      .as = NULL,
      .count = 0,
  };
}

void ag_actor_group_destroy(ActorGroup *group) {
  if (group->as != NULL) {
    free(group->as);
  }
  group->count = 0;
}

// Returns true if the operation was successful, false if not.
bool ag_actor_group_spawn_count(ActorGroup *group, size_t count) {
  size_t old_count = group->count;
  size_t new_count = group->count + count;
  Actor *newptr = realloc(group->as, AG_ACTOR_SIZE * new_count);
  if (newptr == NULL) {
    return false;
  }
  group->as = newptr;
  group->count = new_count;
  for (size_t index = old_count; index < new_count; index++) {
    group->as[index] = ag_actor_new();
    group->as[index].properties[AG_ACTOR_IS_ALIVE] = true;
  }
  return true;
}

void ag_actor_group_perform(ActorGroup group, ActorUpdate update) {
  for (size_t index = 0; index < group.count; index++) {
    Actor *actor = &group.as[index];
    if (!ag_actor_is_alive(actor)) {
      continue;
    }
    update(&group.as[index]);
  }
}

#endif // __AG_ACTOR_GROUP__
