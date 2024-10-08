#ifndef __AG_AGENT_H__
#define __AG_AGENT_H__

#include <stdbool.h>
#include <stdint.h>

#include "tools.h"
#include "vec2.h"

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

#endif // __AG_AGENT_H__
