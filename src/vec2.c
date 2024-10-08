#ifndef __AG_VEC2__
#define __AG_VEC2__

#include <math.h>

#include "tools.h"

typedef struct Vec2 {
  double x, y;
} Vec2;

Vec2 ag_vec2_origin(void);
Vec2 ag_vec2_random(const double max_x, const double max_y);
double ag_vec2_length(const Vec2 *vector);
void ag_vec2_normalise(Vec2 *vector);
Vec2 ag_vec2_normalised(Vec2 vector);

Vec2 ag_vec2_origin(void) { return (Vec2){.x = 0, .y = 0}; }

Vec2 ag_vec2_random(const double max_x, const double max_y) {
  return (Vec2){
      .x = double_random(max_x),
      .y = double_random(max_y),
  };
}

double ag_vec2_length(const Vec2 *vector) {
  return sqrt(pow(vector->x, 2) + pow(vector->y, 2));
}

void ag_vec2_normalise(Vec2 *vector) {
  double length = ag_vec2_length(vector);
  vector->x /= length;
  vector->y /= length;
}

Vec2 ag_vec2_normalised(Vec2 vector) {
  ag_vec2_normalise(&vector);
  return vector;
}

#endif // __AG_VEC2__
