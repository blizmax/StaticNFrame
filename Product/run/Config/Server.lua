MasterIP = "127.0.0.1"
MasterPort = 6110

--�����������б� ServerId, ServerType ��������
ServerList = {}


--��������ᱻ����ִ��
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

	--MasterServer ����������������¼�������� һ���������������
	MasterServer = {
			ServerName = "MasterServer_1",
			ServerType = NF_ST_MASTER,
			ServerId = 1,		--��ÿһ����������˵����Ψһ�ģ� Ӧ�ó�����Ҫͨ�����ServerId����֪����Ҫ���ص���������
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

	--LoginServer ��¼�������������¼����
	LoginServer2 = {
			ServerName = "LoginServer_2",
			ServerType = NF_ST_LOGIN,
			ServerId = 2,			--��ÿһ����������˵����Ψһ�ģ� Ӧ�ó�����Ҫͨ�����ServerId����֪����Ҫ���ص���������
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