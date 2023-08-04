#include <lauxlib.h>
#include <lua.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.c"

typedef struct Config {
  bool running;
  size_t ticks_per_second;
} Config;

bool ag_config_should_tick(Config *config);
void ag_configure_window_title(lua_State *L);

bool ag_config_should_tick(Config *config) {
  return config->running && (config->ticks_per_second > 0);
}

void ag_configure_window_title(lua_State *L) {
  const char *title = ag_lua_read_string(L, "window_title");
  if (title && strlen(title) > 0) {
    SetWindowTitle(title);
  }
}
