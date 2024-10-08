#ifndef __AG_VEC2_H__
#define __AG_VEC2_H__

#include "tools.h"

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

Vec2 ag_vec2_origin(void);
Vec2 ag_vec2_random(const double max_x, const double max_y);
double ag_vec2_length(const Vec2 *vector);
void ag_vec2_normalise(Vec2 *vector);
Vec2 ag_vec2_normalised(Vec2 vector);

#endif // __AG_VEC2_H__
