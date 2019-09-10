local ZoneIP = "127.0.0.1"

function InitGameServer()
	--WorldServer 世界服务器，负责一个大区， 可能会有很多小区
	WorldServer3 = {
			ServerName = "WorldServer3",
			ServerType = NF_ST_WORLD,
			ServerId = 3,		--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = ZoneIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100,
			WorkThreadNum = 1,
			Security = false,
			WebSocket = false,
		};

	--GameServer 游戏服务器, 一般一个小区只有一个， 负责游戏非战斗的业务， 连接世界服务器
	GameServer4 = {
			ServerName = "GameServer4",
			ServerType = NF_ST_GAME,
			ServerId = 4,			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = ZoneIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100,
			LuaWorkThreadNum = 2,
			LuaTcpThreadNum = 4,
			WorkThreadNum = 1,
			Security = false,
			WebSocket = false,
		};
		
	--GameServer 游戏服务器, 一般一个小区只有一个， 负责游戏非战斗的业务， 连接世界服务器
	GameServer5 = {
			ServerName = "GameServer5",
			ServerType = NF_ST_GAME,
			ServerId = 5,			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = ZoneIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100,
			WorkThreadNum = 1,
			Security = false,
			WebSocket = false,
		};
		
	--ProxyServer 网管服务器， 负责与外部的客户端连接, 转发数据
	ProxyServer6 = {
			ServerName = "ProxyServer_6",
			ServerType = NF_ST_PROXY,
			ServerId = 6,			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = MasterIP,
			ServerPort = 6003,
			MaxConnectNum = 10000,
			WorkThreadNum = 5,
			Security = false,
			WebSocket = false,
		};
		
	--ProxyServer 网管服务器， 负责与外部的客户端连接, 转发数据
	ProxyServer7 = {
			ServerName = "ProxyServer_7",
			ServerType = NF_ST_PROXY,
			ServerId = 7,			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = MasterIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100000,
			WorkThreadNum = 5,
			Security = false,
			WebSocket = false,
		};
	
	RegisterServer(WorldServer3)
	RegisterServer(GameServer4)
	--RegisterServer(GameServer5)
	RegisterServer(ProxyServer6)
	--RegisterServer(ProxyServer7)
end
	
