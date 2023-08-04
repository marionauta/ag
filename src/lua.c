#ifndef __AG_LUA_C__
#define __AG_LUA_C__

#include <stdio.h>
#include <stdlib.h>

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

lua_State *ag_lua_state_from_file(const char *filename);
void ag_lua_state_close(lua_State *L);
const char *ag_lua_read_string(lua_State *L, const char *name);

lua_State *ag_lua_state_from_file(const char *filename) {
  lua_State *L = luaL_newstate();
  if (L == NULL) {
    printf("ERROR: failed to create lua state\n");
    return NULL;
  }
  luaL_openlibs(L);
  if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0)) {
    printf("ERROR: %s\n", lua_tostring(L, -1));
    lua_close(L);
    return NULL;
  }
  printf("INFO: SCRIPT: Loaded '%s' successfully\n", filename);
  return L;
}

void ag_lua_state_close(lua_State *L) {
  if (L) {
    lua_close(L);
  }
}

const char *ag_lua_read_string(lua_State *L, const char *name) {
  if (L == NULL) {
    printf("ERROR: null lua state\n");
    return "";
  }
  lua_getglobal(L, name);
  if (!lua_isstring(L, -1)) {
    printf("ERROR: '%s' global is not a string\n", name);
    return "";
  }
  const char *result = lua_tostring(L, -1);
  lua_pop(L, -1);
  return result;
}

#endif // __AG_LUA_C__
