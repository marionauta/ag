#ifndef __AG_VECTOR2__
#define __AG_VECTOR2__

#include <math.h>

#include "tools.c"

typedef struct Vector2 {
  double x, y;
} Vector2;

Vector2 vector2_origin(void);
Vector2 vector2_random(const double max_x, const double max_y);
double vector2_length(const Vector2 *vector);
void vector2_normalise(Vector2 *vector);
Vector2 vector2_normalised(Vector2 vector);

Vector2 vector2_origin(void) { return (Vector2){.x = 0, .y = 0}; }

Vector2 vector2_random(const double max_x, const double max_y) {
  return (Vector2){
      .x = double_random(max_x),
      .y = double_random(max_y),
  };
}

double vector2_length(const Vector2 *vector) {
  return sqrt(pow(vector->x, 2) + pow(vector->y, 2));
}

void vector2_normalise(Vector2 *vector) {
  double length = vector2_length(vector);
  vector->x /= length;
  vector->y /= length;
}

Vector2 vector2_normalised(Vector2 vector) {
  vector2_normalise(&vector);
  return vector;
}

#endif // __AG_VECTOR2__
