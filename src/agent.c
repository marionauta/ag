#ifndef __AG_AGENT__
#define __AG_AGENT__

#include <stdbool.h>
#include <stdint.h>

#include "tools.c"
#include "vec2.c"

typedef struct Agent {
  Vec2 position;
  Vec2 direction;
  int32_t properties[10];
} Agent;

#define Patch Agent

#define AG_AGENT_SIZE (sizeof(Agent))

#define AG_AGENT_IS_ALIVE 0
#define AG_PATCH_HAS_GREEN 0

Agent ag_agent_new(void);
void ag_agent_randomise_position(Agent *agent, const double max_x,
                                 const double max_y);
void ag_agent_randomise_direction(Agent *agent);
void ag_agent_move_direction(Agent *agent, const Vec2 direction);
void ag_agent_move_forward(Agent *agent, const double amount);
bool ag_agent_is_alive(const Agent *agent);

Agent ag_agent_new(void) {
  Agent agent = {
      .position = ag_vec2_origin(),
      .direction = ag_vec2_origin(),
      .properties = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };
  return agent;
}

void ag_agent_randomise_position(Agent *agent, const double max_x,
                                 const double max_y) {
  agent->position = ag_vec2_random(max_x, max_y);
}

void ag_agent_randomise_direction(Agent *agent) {
  double max = 1.0;
  agent->direction = ag_vec2_random(max, max);
  ag_vec2_normalise(&agent->direction);
}

void ag_agent_move_direction(Agent *agent, const Vec2 direction) {
  agent->position.x += direction.x;
  agent->position.y += direction.y;
}

void ag_agent_move_forward(Agent *agent, const double amount) {
  Vec2 direction = ag_vec2_normalised(agent->direction);
  Vec2 movement = {.x = direction.x * amount, .y = direction.y * amount};
  ag_agent_move_direction(agent, movement);
}

bool ag_agent_is_alive(const Agent *agent) {
  return agent->properties[AG_AGENT_IS_ALIVE] > 0;
}

#endif // __AG_AGENT__
