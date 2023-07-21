#ifndef __AG_TOOLS__
#define __AG_TOOLS__

#include <stdlib.h>

double double_random(double max);

double double_random(double max) {
  return (rand() > RAND_MAX / 2 ? -1 : 1) * (max / RAND_MAX) * rand();
}

#endif // __AG_TOOLS__
