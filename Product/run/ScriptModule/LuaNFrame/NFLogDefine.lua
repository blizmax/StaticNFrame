NF_LOG_LEVEL_TRACE = 0 --    trace = 0,
NF_LOG_LEVEL_DEBUG = 1 --    debug = 1,
NF_LOG_LEVEL_INFO = 2  --    info = 2,
NF_LOG_LEVEL_WARN = 3  --    warn = 3,
NF_LOG_LEVEL_ERROR = 4 --    err = 4,


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
	NF_LOG_PROXY_RECV_HEART_LOG = 1007,		--网关接受心跳消息LOG
	NF_LOG_LOGIN_MODULE_LOG = 1102,			--登陆相关LOG打印
	
	---------lua系统的LOG------------------------
	NF_LOG_GAME_HALL_LOG = 1200,			--大厅LOG
	NF_LOG_GAME_AGENT_LOG = 1300,			--代理系统LOG
	NF_LOG_GAME_Barccatat_LOG = 1400,		--百家乐LOG
	NF_LOG_GAME_Brnn_LOG = 1500,			--百人牛牛LOG
	NF_LOG_GAME_Bcbm_LOG = 1600,			--奔驰宝马LOG
	NF_LOG_GAME_Caipiao_LOG = 1700,			--彩票LOG
	NF_LOG_GAME_Ddz_LOG = 1800,				--斗地主LOG
	NF_LOG_GAME_DouNiu_LOG = 1900,			--斗牛LOG
	NF_LOG_GAME_DouNiutz_LOG = 2000,		--抢庄牛牛LOG
	NF_LOG_GAME_Ernn_LOG = 2100,			--二人牛牛LOG
	NF_LOG_GAME_Ersh_LOG = 2200,			--二人梭哈LOG
	NF_LOG_GAME_Ffc_LOG = 2300,				--分分彩LOG
	NF_LOG_GAME_Forest_LOG = 2400,			--ForestLOG
	NF_LOG_GAME_Fqzs_LOG = 2500,			--飞禽走兽LOG
	NF_LOG_GAME_Fruit_LOG = 2600,			--百人水果LOG
	NF_LOG_GAME_FruitBat_LOG = 2700,		--水果拉霸LOG
	NF_LOG_GAME_Gdmj_LOG = 2800,			--广东麻将LOG
	NF_LOG_GAME_HongHei_LOG = 2900,			--红黑LOG
	NF_LOG_GAME_Lhd_LOG = 3000,				--龙虎斗LOG
	NF_LOG_GAME_Psz_LOG = 3100,				--拼三张LOG
	NF_LOG_GAME_Robot_LOG = 3200,			--机器人LOG
	NF_LOG_GAME_ShzBat_LOG = 3300,			--水浒传LOG
	NF_LOG_GAME_Scibo_LOG = 3400,			--赛宝LOG
	NF_LOG_GAME_Slhb_LOG = 3500,			--扫雷红包LOG
	NF_LOG_GAME_Texas_LOG = 3600,			--德州LOG
	NF_LOG_GAME_Ttz_LOG = 3700,				--推筒子LOG
	NF_LOG_GAME_Yqs_LOG = 3800,				--捕鱼LOG
	NF_LOG_MAX_ID = 10240, --最大LOGID
}