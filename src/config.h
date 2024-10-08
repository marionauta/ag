#ifndef __AG_CONFIG__
#define __AG_CONFIG__

#include <stdbool.h>
#include <stdlib.h>

typedef struct Config {
  bool running;
  size_t ticks_per_second;
} Config;

bool ag_config_should_tick(const Config *config);

#endif // __AG_CONFIG__
