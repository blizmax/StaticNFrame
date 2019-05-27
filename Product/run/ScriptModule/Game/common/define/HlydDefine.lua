--注意：如果是线上版本：比分是1:1万
--公共普通场：下注额是100,1000,1万，公共高级场：下注额是：10万,10万,100万
--公共场上庄最低分是：1000万，私人房最低上庄分是500万
--创建私人房的时候，最低下注额分别是：100,1000,1万

--如果是地推版本：比分是1:100
--公共普通场：下注额是100,500,1000，公共高级场：下注额是：1000,5000,1万
--公共场上庄最低分是：10万，私人房最低上庄分是: 5万
--创建私人房的时候，最低下注额分别是：100,500,1000

g_hlydDefine = {}

g_hlydDefine.pour_type = {
101,102,103,104,                                   --压单一个类型的
212,213,214,223,224,234,                           --压前压后
312,313,314,321,323,324,331,332,334,341,342,343    --压前防后
}

g_hlydDefine.win_type = {
	[1] = {
		[1] = {321,331,341,402,404},
		[2] = {212,213,214,401},
		[3] = {312,313,314},
		[4] = {101}
		},
	[2] = {
		[1] = {312,332,342,401,403},
		[2] = {212,223,224,402},
		[3] = {321,323,324},
		[4] = {102}
	},
	[3] = {
		[1] = {313,323,343,402,404},
		[2] = {213,223,234,403},
		[3] = {331,332,334},
		[4] = {103}
	},
	[4] = {
		[1] = {314,324,334,401,403},
		[2] = {214,224,234,404},
		[3] = {341,342,343},
		[4] = {104}
	}
}

g_hlydDefine.table_common = 155555 --888888
g_hlydDefine.table_grade = 156666  --666666
g_hlydDefine.table_lxc = 999999

g_hlydDefine.init_sit = 5      --初始化的8个座位
g_hlydDefine.page_item = 8   --每一页显示的牌桌的数量
g_hlydDefine.min_bankjetton = 100000    --最小上庄的倍数
g_hlydDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_hlydDefine.min_pour_common = 100  --普通场最小下注
g_hlydDefine.min_pour_grade = 10000  --高级场最小下注
g_hlydDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_hlydDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_hlydDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_hlydDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_hlydDefine.pour2_rate = 10
	g_hlydDefine.pour3_rate = 50
	
	g_hlydDefine.min_pour_common = 100
	g_hlydDefine.min_pour_grade = 1000

	g_hlydDefine.pour_type = {
101,102,103,104,                                   --压单一个类型的
212,213,214,223,224,234,                           --压前压后
312,313,314,321,323,324,331,332,334,341,342,343,   --压前防后
401,402,403,404                                    --这个是单筒
}
	
end

g_hlydDefine.sys_rate = 5       --系统的抽水，
g_hlydDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_hlydDefine.state_start = 2
g_hlydDefine.state_perflop = 3   --摇豆子时间
g_hlydDefine.state_pour = 4      --下注的时间
g_hlydDefine.state_count = 5      --结算时间

g_hlydDefine.state_dissolve = 11      --结算时间

g_hlydDefine.perflop_time = 3    --开始的时候，摇杯子的时间
g_hlydDefine.pour_time = 15      --下注的时间
g_hlydDefine.count_time = 15


g_hlydDefine.chat_txt = 1
g_hlydDefine.chat_img = 2
g_hlydDefine.chat_voice = 3


g_hlydDefine.type_ydc = 1       --欢乐赢
g_hlydDefine.type_lxc = 2       --积分场

g_hlydDefine.leave_dismiss = 1   --房主解散房间
g_hlydDefine.leave_timeout = 2   --五轮没下注被踢出

g_hlydDefine.quit_count  = 10     --几轮没下注后就被踢出

g_hlydDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_hlydDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_hlydDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励

g_hlydDefine.pay_type = {}
g_hlydDefine.pay_owner = 1
g_hlydDefine.pay_everyone = 2
