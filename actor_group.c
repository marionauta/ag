#ifndef __AG_ACTOR_GROUP__
#define __AG_ACTOR_GROUP__

#include <stdlib.h>

#include "actor.c"

typedef struct ActorGroup {
  Actor *as;
  size_t count;
} ActorGroup;

ActorGroup ag_actor_group_new(void);
void ag_actor_group_destroy(ActorGroup *group);
void ag_actor_group_spawn_count(ActorGroup *group, size_t count);
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

void ag_actor_group_spawn_count(ActorGroup *group, size_t count) {
  size_t old_count = group->count;
  if (group->as == NULL) {
    group->as = malloc(AG_ACTOR_SIZE * count);
    group->count = count;
  } else {
    size_t new_count = group->count + count;
    group->as = realloc(group->as, new_count);
    group->count = new_count;
  }
  for (size_t index = old_count; index < group->count; index++) {
    group->as[index] = ag_actor_new();
  }
}

void ag_actor_group_perform(ActorGroup group, ActorUpdate update) {
  for (size_t index = 0; index < group.count; index++) {
    update(&group.as[index]);
  }
}

#endif // __AG_ACTOR_GROUP__
