

--MasterServer 服务器，管理多个登录服务器， 一个或多个世界服务器
Server[#Server+1] = {
		ServerName = "MasterServer";
		ServerType = NF_ST_MASTER;
		ServerId = 1;		--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;
		WorldId = 1;
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort;
		MaxConnectNum = 100;
		WorkThreadNum = 1;
		Security = false;
	};
	
	--WorldServer 世界服务器，负责一个大区， 可能会有很多小区
Server[#Server+1] = {
		ServerName = "WorldServer";
		ServerType = NF_ST_WORLD;
		ServerId = 2;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 100;			--代表一个世界服务器
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 100;
		WorkThreadNum = 1;
		Security = false;
	};
	
	--WorldServer 世界服务器，负责一个大区， 可能会有很多小区
Server[#Server+1] = {
		ServerName = "WorldServer";
		ServerType = NF_ST_WORLD;
		ServerId = 3;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 200;			--代表一个世界服务器
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 100;
		WorkThreadNum = 1;
		Security = false;
	};
	
	--LoginServer 登录服务器，负责登录连接
Server[#Server+1] = {
		ServerName = "LoginServer";
		ServerType = NF_ST_LOGIN;
		ServerId = 4;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 100;			--需要与一个世界服务器的WorldId一样， 这样他下面的小服务器的连接才会发过来
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 1000;
		WorkThreadNum = 5;
		Security = false;
	};
	
	--LoginServer 登录服务器，负责登录连接
Server[#Server+1] = {
		ServerName = "LoginServer";
		ServerType = NF_ST_LOGIN;
		ServerId = 5;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 100;			--需要与一个世界服务器的WorldId一样， 这样他下面的小服务器的连接才会发过来
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 1000;
		WorkThreadNum = 5;
		Security = false;
	};
	
	--LoginServer 登录服务器，负责登录连接
Server[#Server+1] = {
		ServerName = "LoginServer";
		ServerType = NF_ST_LOGIN;
		ServerId = 6;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 200;			--需要与一个世界服务器的WorldId一样， 这样他下面的小服务器的连接才会发过来
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 1000;
		WorkThreadNum = 5;
		Security = false;
	};
	
	--LoginServer 登录服务器，负责登录连接
Server[#Server+1] = {
		ServerName = "LoginServer";
		ServerType = NF_ST_LOGIN;
		ServerId = 7;			--对每一个服务器来说都是唯一的， 应用程序需要通过这个ServerId才能知道需要加载的配置是他
		MasterId = 1;			--需要与MasterServer的MasterId保持一致
		WorldId = 200;			--需要与一个世界服务器的WorldId一样， 这样他下面的小服务器的连接才会发过来
		ZoneId = 1;
		ServerIp = MasterIP;
		ServerPort = MasterPort+#Server;
		MaxConnectNum = 1000;
		WorkThreadNum = 5;
		Security = false;
	};
