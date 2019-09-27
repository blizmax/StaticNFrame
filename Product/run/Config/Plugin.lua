-- server type
NF_ST_NONE = 0;    		-- NONE
NF_ST_MASTER = 1;    	--
NF_ST_LOGIN = 2;    	--
NF_ST_WORLD = 3;    	--
NF_ST_GAME = 4;    		--
NF_ST_PROXY = 5;    	--
NF_ST_WEB = 6;			-- WEB SERVER
NF_ST_LOCATION = 7; 	--LOCATION SERVER
NF_ST_LOG = 8; 			--日志采集服务器 
NF_ST_REBOT = 15; 		--机器人模拟客户端
NF_ST_MAX = 20;    		--

LoadPlugin = 
{
	TestServer = {
		ServerPlugins = {
			-------------------------
			-----基础框架引擎-------------
			"NFKernelPlugin",
			"NFTestPlugin",
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
			"NFMysqlPlugin",
			------------------------------------------
			
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

			"NFWebClientPlugin",
			"NFWebServerPlugin",
			
			"NFLocationClientPlugin",
			"NFLocationServerPlugin",

			"NFLogClientPlugin",
			"NFLogServerPlugin",
			------------------------------------------------
			-----服务器基础逻辑引擎-------------------------
			"NFGameLogicPlugin",
			"NFLoginLogicPlugin",
			"NFProxyLogicPlugin",
			"NFWorldLogicPlugin",
			--------------------------------------------------------
			-----服务器游戏具体内容逻辑引擎-------------------------
			-----可以动态加载的引擎---------------------------------
			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",

		};
		ServerType = NF_ST_NONE;
	},
	MasterServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			"NFActorPlugin",
			"NFMysqlPlugin",
			
			"NFMasterServerPlugin",
		};
		ServerType = NF_ST_MASTER;
	},
	LoginServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			"NFActorPlugin",

			"NFLoginClientPlugin",
			"NFLoginServerPlugin",
			
			"NFLoginLogicPlugin",

			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_LOGIN;
	},
	WorldServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			"NFActorPlugin",
			
			"NFWorldClientPlugin",
			"NFWorldServerPlugin",
			
			"NFWorldLogicPlugin",

			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_WORLD;
	},
	ProxyServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			
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
			"NFMonitorPlugin",
			"NFMysqlPlugin",
			
			"NFGameClientPlugin",
			"NFGameServerPlugin",
			
			"NFGameLogicPlugin",
			
			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_GAME;
	},
	LocationServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",

			"NFLocationClientPlugin",
			"NFLocationServerPlugin",
			
			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_LOCATION;
	},
	LogServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",

			"NFLogClientPlugin",
			"NFLogServerPlugin",
			
			"NFLuaScriptPlugin",
			--"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_LOG;
	},
	WebServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",

			"NFWebClientPlugin",
			"NFWebServerPlugin",
			
			"NFLuaScriptPlugin",
			--"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_WEB;
	},
	RebotServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			
			"NFRebotPlugin",
			--"NFLuaScriptPlugin",
			"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_REBOT;
	},
};


