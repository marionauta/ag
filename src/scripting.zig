const lua = @cImport({
    @cInclude("lauxlib.h");
    @cInclude("lua.h");
    @cInclude("lualib.h");
});

pub const State = *lua.lua_State;

const ScriptingError = error{
    FailedStateCreation,
    FailedFileRead,
    NotAString,
    NotANumber,
};

pub fn state_from_file(filename: [*]const u8) ScriptingError!State {
    const L = lua.luaL_newstate();
    if (L == null) {
        return ScriptingError.FailedStateCreation;
    }
    lua.luaL_openlibs(L);
    if (lua.luaL_loadfile(L, filename) > 0 or lua.lua_pcall(L, 0, 0, 0) > 0) {
        lua.lua_close(L);
        return ScriptingError.FailedFileRead;
    }
    if (L) |state| {
        return state;
    } else {
        return ScriptingError.FailedStateCreation;
    }
}

pub fn state_close(L: ?State) void {
    if (L) |state| {
        lua.lua_close(state);
    }
}

pub fn read_string(L: State, name: [*]const u8) ScriptingError![*]const u8 {
    lua.lua_getglobal(L, name);
    if (lua.lua_isstring(L, -1) == 0) {
        return ScriptingError.NotAString;
    }
    const result = lua.lua_tolstring(L, -1, null);
    lua.lua_pop(L, -1);
    return result;
}

pub fn read_number(state: State, name: [*]const u8) ScriptingError!f64 {
    lua.lua_getglobal(state, name);
    if (lua.lua_isnumber(state, -1) == 0) {
        return ScriptingError.NotANumber;
    }
    const result = lua.lua_tonumber(state, -1);
    lua.lua_pop(state, -1);
    return result;
}
