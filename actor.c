#ifndef __AG_ACTOR__
#define __AG_ACTOR__

#include <stdbool.h>
#include <stdint.h>

#include "tools.c"
#include "vector2.c"

typedef struct Actor {
  Vector2 position;
  Vector2 direction;
  int32_t properties[10];
} Actor;

#define AG_ACTOR_SIZE (sizeof(Actor))

#define AG_ACTOR_IS_ALIVE 0
#define AG_PATCH_HAS_GREEN 0

typedef void (*ActorUpdate)(Actor *);

Actor ag_actor_new(void);
void ag_actor_randomise_direction(Actor *actor);
void ag_actor_move_direction(Actor *actor, Vector2 direction);
void ag_actor_move_forward(Actor *actor, double amount);
bool ag_actor_is_alive(const Actor *actor);

Actor ag_actor_new(void) {
  Actor actor = {
      .position = vector2_origin(),
      .direction = vector2_origin(),
      .properties = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };
  return actor;
}

void ag_actor_randomise_direction(Actor *actor) {
  double max = 1.0;
  actor->direction = vector2_random(max, max);
  vector2_normalise(&actor->direction);
}

void ag_actor_move_direction(Actor *actor, Vector2 direction) {
  actor->position.x += direction.x;
  actor->position.y += direction.y;
}

void ag_actor_move_forward(Actor *actor, double amount) {
  Vector2 direction = vector2_normalised(actor->direction);
  Vector2 movement = {.x = direction.x * amount, .y = direction.y * amount};
  ag_actor_move_direction(actor, movement);
}

bool ag_actor_is_alive(const Actor *actor) {
  return actor->properties[AG_ACTOR_IS_ALIVE] > 0;
}

#endif // __AG_ACTOR__
