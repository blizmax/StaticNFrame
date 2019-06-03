g_servername = "run_gdmj" --这是每一个游戏的标志
g_serverid = 1
g_serverip = "192.168.2.121"
g_gamename = "德州扑克"
g_httpport = 7051
g_tcpport = 7101
g_websocketport = 5350
g_udpport = 7102      --目前暂时不支持udp
g_tcptimeout = 6000
g_websockettimeout = 6000
g_tcpmaxthreads = 5
g_httpmaxthreads = 2
g_websocketmaxthreads = 2
g_serverType = "login"
g_isDebug = 1
g_dbtype = 'mysql'

--g_dbHost2 = 'limitlessdb2.mysql.rds.aliyuncs.com'
--g_dbUser2 = 'mjuser'
--g_dbPassword2 = "tywl_mj_6789"
--g_dbPort2 = 3306
--g_dbDatabase2 = 'db_mmmj'

g_dbHost = '23.91.101.148'
g_dbUser = 'user_yj'
g_dbPassword = "cgt=+erhk"
g_dbPort = 3306
g_dbDatabase = 'db_rpmj'

g_redisInfo = {}
g_redisInfo.redis_one = 1     --玩家属性
g_redisInfo.redis_two = 2     --
g_redisInfo.redis_three = 3   --
g_redisInfo.redis_four = 4   --这个数据是设置历史记录的
g_signKey = "w2ctsethjcyu7r9b"
g_markTime = {}
function ServerInit()  --在函数启动的时候，会调用该函数，获取服务器的配置信息
	--返回以下11个参数
	return g_serverid,g_serverip,g_tcpport,g_httpport,g_websocketport,g_udpport,g_tcptimeout,g_websockettimeout,
	g_tcpmaxthreads,g_httpmaxthreads,g_websocketmaxthreads,g_isDebug
end

g_redisIndex = {

	["default"] = {index = g_redisInfo.redis_three, des = "default_"} --签到奖励
	
}