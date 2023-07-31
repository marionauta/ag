#ifndef __AG_TOOLS__
#define __AG_TOOLS__

#include <stdlib.h>

#define UNUSED(x) ((void)sizeof((void)(x), 0))

double double_random(const double max);
int int_mod(const int a, const int b);

double double_random(const double max) {
  return (rand() > RAND_MAX / 2 ? -1 : 1) * (max / RAND_MAX) * rand();
}

int int_mod(const int a, const int b) { return ((a % b) + b) % b; }

#endif // __AG_TOOLS__
