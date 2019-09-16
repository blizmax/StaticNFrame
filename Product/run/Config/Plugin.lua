-- server type
NF_ST_NONE = 0;    		-- NONE
NF_ST_MASTER = 1;    	--
NF_ST_LOGIN = 2;    	--
NF_ST_WORLD = 3;    	--
NF_ST_GAME = 4;    		--
NF_ST_PROXY = 5;    	--
NF_ST_HTTP = 6;			-- WEB SERVER
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
			
			------------------------------------------------
			-----服务器基础逻辑引擎-------------------------
			"NFGameLogicPlugin",
			"NFLoginLogicPlugin",
			"NFProxyLogicPlugin",
			"NFWorldLogicPlugin",
			--------------------------------------------------------
			-----服务器游戏具体内容逻辑引擎-------------------------
			-----可以动态加载的引擎---------------------------------
			"NFLuaScriptPlugin",
			--"NFLuaThreadScriptPlugin",

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
			
			"NFLoginClientPlugin",
			"NFLoginServerPlugin",
			
			"NFLoginLogicPlugin",
		};
		ServerType = NF_ST_LOGIN;
	},
	WorldServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFNetPlugin",
			"NFMonitorPlugin",
			
			"NFWorldClientPlugin",
			"NFWorldServerPlugin",
			
			"NFWorldLogicPlugin",
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
	RebotServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			
			"NFRebotPlugin",
			"NFLuaScriptPlugin",
			--"NFLuaThreadScriptPlugin",
		};
		ServerType = NF_ST_REBOT;
	},
};


