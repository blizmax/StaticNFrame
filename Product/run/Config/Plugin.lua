-- server type
NF_ST_NONE = 0;    		-- NONE
NF_ST_MASTER = 1;    	--
NF_ST_LOGIN = 2;    	--
NF_ST_WORLD = 3;    	--
NF_ST_GAME = 4;    		--
NF_ST_PROXY = 5;    	--
NF_ST_MATCH = 6;
NF_ST_BATTLE = 7;
NF_ST_UNION_MATCH = 8;
NF_ST_MAX = 9;    		--

NF_LOG_LEVEL_TRACE = 0 --    trace = 0,
NF_LOG_LEVEL_DEBUG = 1 --    debug = 1,
NF_LOG_LEVEL_INFO = 2  --    info = 2,
NF_LOG_LEVEL_WARN = 3  --    warn = 3,
NF_LOG_LEVEL_ERROR = 4 --    err = 4,


LogLevel = NF_LOG_LEVEL_TRACE;
LoadPlugin = 
{
	AllServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFTestPlugin",
			"NFMysqlPlugin",
			--"NFProxyServerPlugin",
			--"NFProxyClientPlugin",
			--"NFGameServerPlugin",
			"NFLuaScriptPlugin",
		};
		ServerType = NF_ST_NONE;
		
	},
};


