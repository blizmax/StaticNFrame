#ifndef _LUA_OPENLIBS_H
#define _LUA_OPENLIBS_H

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

LUALIB_API void luaL_openlibs2(lua_State *L);

#endif



