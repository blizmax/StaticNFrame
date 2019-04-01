-- server type
NF_ST_NONE = 0;    		-- NONE
NF_ST_MASTER = 1;    	--
NF_ST_LOGIN = 2;    	--
NF_ST_WORLD = 3;    	--
NF_ST_GAME = 4;    		--
NF_ST_PROXY = 5;    	--
NF_ST_PROXY_INNER = 6;    	--
NF_ST_REBOT = 7;		--
NF_ST_MAX = 9;    		--

LoadPlugin = 
{
	AllServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFTestPlugin",
			"NFMonitorPlugin",
			--"NFSqlitePlugin",
			--"NFMysqlPlugin",
			"NFMongoPlugin",
			
			"NFLuaScriptPlugin",
			
			--"NFMasterServerPlugin",
			
			--"NFLoginClientPlugin",
			--"NFLoginServerPlugin",
			
			--"NFWorldClientPlugin",
			--"NFWorldServerPlugin",
			
			--"NFGameClientPlugin",
			--"NFGameServerPlugin",
			
			--"NFProxyServerPlugin",
			--"NFProxyClientPlugin",
			
			--"NFGameLogicPlugin",
			--"NFLoginLogicPlugin",
			--"NFProxyLogicPlugin",

		};
		ServerType = NF_ST_NONE;
	},
	MasterServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMongoPlugin",
			
			"NFMasterServerPlugin",
		};
		ServerType = NF_ST_MASTER;
	},
	LoginServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMongoPlugin",
			
			"NFLoginClientPlugin",
			"NFLoginServerPlugin",
			
			"NFLoginLogicPlugin",
		};
		ServerType = NF_ST_LOGIN;
	},
	WorldServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMongoPlugin",
			
			"NFWorldClientPlugin",
			"NFWorldServerPlugin",
			
			--"NFLuaScriptPlugin",
		};
		ServerType = NF_ST_WORLD;
	},
	ProxyServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			
			"NFProxyServerPlugin",
			"NFProxyClientPlugin",
			
			"NFProxyLogicPlugin",
		};
		ServerType = NF_ST_PROXY;
	},
	GameServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMongoPlugin",
			
			"NFGameClientPlugin",
			"NFGameServerPlugin",
			
			"NFGameLogicPlugin",
			
			--"NFLuaScriptPlugin",
		};
		ServerType = NF_ST_GAME;
	},
	RebotServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFActorPlugin",
			"NFMongoPlugin",
			
			--"NFLuaScriptPlugin",
		};
		ServerType = NF_ST_REBOT;
	},
};


