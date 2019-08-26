#include "lua_openlibs.h"

#define LUA_CJSONLIBNAME "cjson"
#define LUA_LUASQLNAME "luasql.mysql"  
#define LUA_LPEG "lpeg"  
#define LUA_LUASOCKET "socket.core"
#define LUA_LUALFS "lfs"
#define LUA_LUAPB "pb"

LUALIB_API int luaopen_socket_core(lua_State *L);
LUALIB_API int luaopen_luasql_mysql(lua_State *L);
LUALIB_API int luaopen_lfs(lua_State *L);
LUALIB_API int luaopen_cjson(lua_State *l);
LUALIB_API int luaopen_lpeg(lua_State *L);
LUALIB_API int luaopen_pb(lua_State *L);

static const luaL_Reg lj_lib_load2[] = {
  { LUA_CJSONLIBNAME, luaopen_cjson },
  { LUA_LUASQLNAME, luaopen_luasql_mysql },
  { LUA_LPEG, luaopen_lpeg },
  { LUA_LUASOCKET, luaopen_socket_core },
  { LUA_LUALFS, luaopen_lfs },
  { LUA_LUAPB, luaopen_pb },

  { NULL,		NULL }
};

void luaL_openlibs2(lua_State *L)
{
	const luaL_Reg *lib;
	for (lib = lj_lib_load2; lib->func; lib++) {
		lua_pushcfunction(L, lib->func);
		lua_pushstring(L, lib->name);
		lua_call(L, 1, 0);
	}
	lua_pop(L, 1);
}