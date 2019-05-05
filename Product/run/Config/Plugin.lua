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
	KernelServer = {
		ServerPlugins = {
			-------------------------
			-----基础框架引擎-------------
			"NFKernelPlugin",
			"NFMonitorPlugin",
		};
		ServerType = NF_ST_NONE;
	},
	
	AllServer = {
		ServerPlugins = {
			-------------------------
			-----基础框架引擎-------------
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			"NFSqlitePlugin",
			"NFNoSqlPlugin",
			"NFMysqlPlugin",
			"NFMongoPlugin",
			------------------------------------------
			--"NFTestPlugin",
			
			--"NFLuaScriptPlugin",
			--------------------------------------------
			-----服务器架构引擎-------------------------
			"NFMasterServerPlugin",
			
			"NFLoginClientPlugin",
			"NFLoginServerPlugin",
			
			"NFWorldClientPlugin",
			"NFWorldServerPlugin",
			
			"NFGameClientPlugin",
			"NFGameServerPlugin",
			
			"NFProxyServerPlugin",
			"NFProxyClientPlugin",
			
			------------------------------------------------
			-----服务器基础逻辑引擎-------------------------
			"NFGameLogicPlugin",
			"NFLoginLogicPlugin",
			"NFProxyLogicPlugin",
			"NFWorldLogicPlugin",
			--------------------------------------------------------
			-----服务器游戏具体内容逻辑引擎-------------------------
			-----可以动态加载的引擎---------------------------------
			"NFGameHumanPlugin"

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


