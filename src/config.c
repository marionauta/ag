#include <stdbool.h>
#include <stdlib.h>

typedef struct Config {
  bool running;
  size_t ticks_per_second;
} Config;

bool ag_config_should_tick(Config *config);

bool ag_config_should_tick(Config *config) {
  return config->running && (config->ticks_per_second > 0);
}
