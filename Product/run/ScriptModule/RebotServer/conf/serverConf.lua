g_servername = "run_dwc858" --这是每一个游戏的标志
g_serverid = 1
g_serverip = "192.168.2.121"
g_gamename = "dwcgame"
g_httpport = 6005
g_tcpport = 6003
g_websocketport = 6007
g_udpport = 6103      --目前暂时不支持udp
g_tcptimeout = 6000
g_websockettimeout = 6000
g_tcpmaxthreads = 5
g_httpmaxthreads = 2
g_workmaxthreads = 40
g_websocketmaxthreads = 2
g_serverType = "login"
g_isDebug = 1
g_dbtype = 'mysql'
g_timeOut = 1000    --定时器设置的超时时间，单位是毫秒

g_caiPiaoData = "redis"        --说明是从缓存中获取的，如果是http，那么就是从http的接口获取

g_dbHost2 = '119.23.29.164'
g_dbUser2 = 'gamer'
g_dbPassword2 = "btc!+ngkp"
g_dbPort2 = 6006
g_dbDatabase2 = 'db_CblTest'

g_webUrl = "https://20998888.com"  --游戏的URL，


g_dbHost = '127.0.0.1'
g_dbUser = 'user_yj'
g_dbPassword = "cgt=+erhk"
g_dbPort = 6001
g_dbDatabase = 'db_new_wuxiandai'


--[[

]]

g_qrcodeUrl = ""       --生成二维码的url,调用改接口，生成对应的推广二维码
g_redisInfo = {}
g_redisInfo.redis_one = 6     --玩家属性
g_redisInfo.redis_two = 7     --
g_redisInfo.redis_three = 8   --
g_redisInfo.redis_four = 9   --这个数据是设置历史记录的
g_redisInfo.host = "127.0.0.1"
g_redisInfo.port = 6379
g_redisInfo.pass = ""

g_signKey = "w2ctsethjcyu7r9b"
g_markTime = g_markTime or {}
function ServerInit()  --在函数启动的时候，会调用该函数，获取服务器的配置信息
	--返回以下11个参数
	return g_workmaxthreads,g_timeOut, g_serverid,g_serverip,g_tcpport,g_httpport,g_websocketport,g_udpport,g_tcptimeout,g_websockettimeout,
	g_tcpmaxthreads,g_httpmaxthreads,g_websocketmaxthreads,g_isDebug
end

--以下是Utils服务器的配置，放在同一个配置文件中。

g_utilsTimout = 1000     --这个定时器设置超时间短一些。
g_utilsmaxthreads = 10


function serveUtilsInit()
	return g_utilsTimout, g_utilsmaxthreads,g_isDebug
end

