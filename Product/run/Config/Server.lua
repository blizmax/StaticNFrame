MasterIP = "127.0.0.1"
MasterPort = 6110

--服务器配置列表， ServerId, ServerType 必须配置
ServerList = {}


--这个函数会被程序执行
function InitServer()
	InitGlobalServer()
	InitGameServer()
end

function GetNewPort()
	local port = MasterPort
	MasterPort = MasterPort + 1
	return port
end

function RegisterServer(server)
	table.insert(ServerList, server)
end

function InitGlobalServer()

	--MasterServer 服务器，管理多个登录服务器， 一个或多个世界服务器
	MasterServer = {
			ServerName = "MasterServer_1",
			ServerType = NF_ST_MASTER,
			ServerId = 1,		--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = MasterIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100,
			WorkThreadNum = 1,
			MysqlIp = "47.75.217.27",
			MysqlPort = 6001,
			MysqlDbName = "db_gaoyi",
			MysqlUser = "user_yj",
			MysqlPassword = "cgt=+erhk",
			Security = false,
			WebSocket = false,
			HttpPort = 6105,
	};

	--LoginServer 登录服务器，负责登录连接
	LoginServer2 = {
			ServerName = "LoginServer_2",
			ServerType = NF_ST_LOGIN,
			ServerId = 2,			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
			ServerIp = MasterIP,
			ServerPort = GetNewPort(),
			MaxConnectNum = 100000,
			WorkThreadNum = 5,
			LuaWorkThreadNum = 1,
			LuaTcpThreadNum = 2,
			Security = false,
			WebSocket = false,
		};

	RegisterServer(MasterServer)
	RegisterServer(LoginServer2)
end