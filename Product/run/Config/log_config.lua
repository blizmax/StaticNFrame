
NF_LOG_LEVEL_TRACE = 0 --    trace = 0,
NF_LOG_LEVEL_DEBUG = 1 --    debug = 1,
NF_LOG_LEVEL_INFO = 2  --    info = 2,
NF_LOG_LEVEL_WARN = 3  --    warn = 3,
NF_LOG_LEVEL_ERROR = 4 --    err = 4,


LogLevel = NF_LOG_LEVEL_TRACE;				--log等级配置		当前日志输出级别，大于此等于级别的才会输出到console
LogFlushLevel = NF_LOG_LEVEL_TRACE;			--log刷新等级配置, 当前日志输出级别，大于此等于级别的才会刷新到LOG文件里


NFLogId = {
	--0-100是基础框架层LOG
	NF_LOG_DEFAULT = 0,						--默认LOG
	NF_LOG_SYSTEMLOG = 1,					--系统LOG
	NF_LOG_ACTOR_PLUGIN = 2,				--Actor 引擎 
	NF_LOG_KERNEL_PLUGIN = 3,				--kernel 引擎
	NF_LOG_LUA_PLUGIN = 4,					--lua 引擎
	NF_LOG_MONGO_PLUGIN = 5,				--mongo 引擎
	NF_LOG_MONITOR_PLUGIN = 5,				--monitor 引擎
	NF_LOG_MYSQL_PLUGIN = 7,				--mysql 引擎
	NF_LOG_NET_PLUGIN = 8,					--net 引擎
	NF_LOG_SQLITE_PLUGIN = 9,				--sqlite 引擎
	NF_LOG_TEST_PLUGIN = 10,				--test 引擎
	NF_LOG_PROXY_LOGIC_PLUGIN = 11,			--proxy 引擎
	
	
	NF_LOG_LOAD_CONFIG = 21,				--加载配置
	NF_LOG_PROTOBUF_PARSE = 22,				--protobuf解析出错
	NF_LOG_RECV_MSG = 23,					--接受消息
	NF_LOG_RECV_MSG_JSON_PRINTF = 24,		--接受消息Json输出
	NF_LOG_PLUGIN_MANAGER = 100,			--引擎加载器
	NF_LOG_MAX_SYSTEM_PLUGIN = 100,

	------------------------------------------------------------------------------------------
	NF_LOG_BEGIN_SERVER_LOG = 101,			--101-1000是系统框架层LOG
	
	NF_LOG_SERVER_CONNECT_SERVER = 101,		--服务器与服务器连接LOG
	NF_LOG_SERVER_NOT_HANDLE_MESSAGE = 102,		--服务器未处理消息的LOG
	NF_LOG_BEHAVIOR_LOGIC_LOG = 103,		--行为LOG日志
	
	NF_LOG_END_SERVER_LOG = 1000,			--
	
	-------------------------------------------------------------------------
	

	NF_LOG_BEGIN_LOGIC_LOG = 1001,			--1001以后是逻辑框架层
	NF_LOG_PROXY_RECV_MSG_LOG = 1001,		--网关接受消息LOG
	NF_LOG_PROXY_SEND_MSG_LOG = 1002,		--网关发送消息LOG
	NF_LOG_WORLD_RECV_MSG_LOG = 1003,		--WORLD接受消息LOG
	NF_LOG_WORLD_SEND_MSG_LOG = 1004,		--WORLD发送消息LOG
	NF_LOG_GAME_RECV_MSG_LOG = 1005,		--GAME接受消息LOG
	NF_LOG_GAME_SEND_MSG_LOG = 1006,		--GAME发送消息LOG
	NF_LOG_LOGIN_MODULE_LOG = 1102,			--登陆相关LOG打印
	
	
	NF_LOG_MAX_ID = 10240, --最大LOGID
}

--	配置要打印的LOG
--	display 是否打印log
--	level 打印log最低等级
--  logname 打印时显示的名字
--  guid 只打印一个或几个玩家的LOG 打印几个玩家的LOG系统guid={guid1, guid2}
LogInfo = {
	--基础框架层LOG
	{logid = NFLogId.NF_LOG_DEFAULT, display=true, level=0, logname = "DefaultLog", guid = {0}, desc = "默认LOG"},
	{logid = NFLogId.NF_LOG_SYSTEMLOG, display=true, level=0, logname = "SystemLog", guid = 0, desc = "系统LOG"},
	{logid = NFLogId.NF_LOG_ACTOR_PLUGIN, display=false, level=0, logname = "ActorLog", guid = 0, desc = "Actor引擎LOG"},
	{logid = NFLogId.NF_LOG_KERNEL_PLUGIN, display=true, level=0, logname = "KernelLog", guid = 0, desc = "Kernel引擎LOG"},
	{logid = NFLogId.NF_LOG_LUA_PLUGIN, display=false, level=0, logname = "LuaLog", guid = 0, desc = "Lua引擎LOG"},
	{logid = NFLogId.NF_LOG_MONGO_PLUGIN, display=false, level=0, logname = "MongoLog", guid = 0, desc = "Mongo引擎LOG"},
	{logid = NFLogId.NF_LOG_MONITOR_PLUGIN, display=false, level=0, logname = "MonitorLog", guid = 0, desc = "Monitor引擎LOG"},
	{logid = NFLogId.NF_LOG_MYSQL_PLUGIN, display=true, level=0, logname = "MysqlLog", guid = 0, desc = "Mysql引擎LOG"},
	{logid = NFLogId.NF_LOG_NET_PLUGIN, display=true, level=0, logname = "NetLog", guid = 0, desc = "Net引擎LOG"},
	{logid = NFLogId.NF_LOG_SQLITE_PLUGIN, display=true, level=0, logname = "SqliteLog", guid = 0, desc = "Sqlite引擎LOG"},
	{logid = NFLogId.NF_LOG_TEST_PLUGIN, display=false, level=0, logname = "TestLog", guid = 0, desc = "test引擎LOG"},
	{logid = NFLogId.NF_LOG_PROXY_LOGIC_PLUGIN, display=true, level=0, logname = "ProxyLog", guid = 0, desc = "proxy引擎LOG"},
	-------------------------------------------------------------------------------------------------
	{logid = NFLogId.NF_LOG_LOAD_CONFIG, display=true, level=0, logname = "LoadConfigLog", guid = 0, desc = "加载配置LOG"},
	{logid = NFLogId.NF_LOG_PROTOBUF_PARSE, display=true, level=0, logname = "ProtobufParse", guid = 0, desc = "protobuf解析出错LOG"},
	{logid = NFLogId.NF_LOG_RECV_MSG, display=false, level=0, logname = "RecvMsg", guid = 0, desc = "接受消息LOG"},
	{logid = NFLogId.NF_LOG_RECV_MSG_JSON_PRINTF, display=false, level=0, logname = "RecvMsgJson", guid = 0, desc = "接受消息Json输出LOG"},
	{logid = NFLogId.NF_LOG_PLUGIN_MANAGER, display=false, level=0, logname = "PluginLog", guid = 0, desc = "引擎管理LOG"},
	--服务器层架构LOG
	
	{logid = NFLogId.NF_LOG_SERVER_CONNECT_SERVER, display=true, level=0, logname = "ServerConnectServerLog", guid = 0, desc = "服务器与服务器连接LOG"},
	{logid = NFLogId.NF_LOG_SERVER_NOT_HANDLE_MESSAGE, display=true, level=0, logname = "NotHandleMessageLog", guid = 0, desc = "服务器未处理消息的LOG"},
	{logid = NFLogId.NF_LOG_BEHAVIOR_LOGIC_LOG, display=true, level=0, logname = "BehaviorLog", guid = 0, desc = "行为LOG日志"},
	--逻辑代码LOG
	--------------------网关服务器LOG信息
	{logid = NFLogId.NF_LOG_PROXY_RECV_MSG_LOG, display=false, level=0, logname = "ProxyRecvMsg", guid = 0, desc = "网关接受消息LOG"},
	{logid = NFLogId.NF_LOG_PROXY_SEND_MSG_LOG, display=false, level=0, logname = "ProxySendMsg", guid = 0, desc = "网关发送消息LOG"},
	{logid = NFLogId.NF_LOG_WORLD_RECV_MSG_LOG, display=true, level=0, logname = "WorldRecvMsg", guid = 0, desc = "WORLD接受消息LOG"},
	{logid = NFLogId.NF_LOG_WORLD_SEND_MSG_LOG, display=true, level=0, logname = "WorldSendMsg", guid = 0, desc = "WORLD发送消息LOG"},
	{logid = NFLogId.NF_LOG_GAME_RECV_MSG_LOG, display=false, level=0, logname = "GameRecvMsg", guid = 0, desc = "GAME接受消息LOG"},
	{logid = NFLogId.NF_LOG_GAME_SEND_MSG_LOG, display=false, level=0, logname = "GameSendMsg", guid = 0, desc = "GAME发送消息LOG"},
	
	--------------------游戏服务器LOG信息
	{logid = NFLogId.NF_LOG_LOGIN_MODULE_LOG, display=true, level=0, logname = "LoginModule", guid = 0, desc = "登陆相关LOG打印"},
}
