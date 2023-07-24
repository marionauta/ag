#ifndef __AG_AGENT__
#define __AG_AGENT__

#include <stdbool.h>
#include <stdint.h>

#include "tools.c"
#include "vector2.c"

typedef struct Agent {
  Vector2 position;
  Vector2 direction;
  int32_t properties[10];
} Agent;

#define AG_AGENT_SIZE (sizeof(Agent))

#define AG_AGENT_IS_ALIVE 0
#define AG_PATCH_HAS_GREEN 0

typedef void (*AgentUpdate)(Agent *);

Agent ag_agent_new(void);
void ag_agent_randomise_direction(Agent *agent);
void ag_agent_move_direction(Agent *agent, Vector2 direction);
void ag_agent_move_forward(Agent *agent, double amount);
bool ag_agent_is_alive(const Agent *agent);

Agent ag_agent_new(void) {
  Agent agent = {
      .position = vector2_origin(),
      .direction = vector2_origin(),
      .properties = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };
  return agent;
}

void ag_agent_randomise_direction(Agent *agent) {
  double max = 1.0;
  agent->direction = vector2_random(max, max);
  vector2_normalise(&agent->direction);
}

void ag_agent_move_direction(Agent *agent, Vector2 direction) {
  agent->position.x += direction.x;
  agent->position.y += direction.y;
}

void ag_agent_move_forward(Agent *agent, double amount) {
  Vector2 direction = vector2_normalised(agent->direction);
  Vector2 movement = {.x = direction.x * amount, .y = direction.y * amount};
  ag_agent_move_direction(agent, movement);
}

bool ag_agent_is_alive(const Agent *agent) {
  return agent->properties[AG_AGENT_IS_ALIVE] > 0;
}

#endif // __AG_AGENT__
