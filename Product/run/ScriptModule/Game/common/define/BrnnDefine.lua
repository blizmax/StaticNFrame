

--牛牛的牌型是三位数，第一位是花色后面两位是牌大小
--小王是：515，大王是525


g_brnnDefine = {}

g_brnnDefine.pour_type = {1,2,3,4}


g_brnnDefine.win_type = {
	[1] = {
		[1] = {321,331,341},
		[2] = {212,213,214},
		[3] = {312,313,314},
		[4] = {101}
		},
	[2] = {
		[1] = {312,332,342},
		[2] = {212,223,224},
		[3] = {321,323,324},
		[4] = {102}
	},
	[3] = {
		[1] = {313,323,343},
		[2] = {213,223,234},
		[3] = {331,332,334},
		[4] = {103}
	},
	[4] = {
		[1] = {314,324,334},
		[2] = {214,224,234},
		[3] = {341,342,343},
		[4] = {104}
	}
}

g_brnnDefine.table_common = 777777

g_brnnDefine.init_sit = 6      --初始化的6个座位
g_brnnDefine.page_item = 8   --每一页显示的牌桌的数量
g_brnnDefine.min_bankjetton = 100000    --最小上庄的倍数
g_brnnDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_brnnDefine.min_pour_common = 100  --普通场最小下注
g_brnnDefine.min_pour_grade = 10000  --高级场最小下注
g_brnnDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_brnnDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_brnnDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_brnnDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_brnnDefine.pour2_rate = 10
	g_brnnDefine.pour3_rate = 50
	
	g_brnnDefine.min_pour_common = 100
	g_brnnDefine.min_pour_grade = 1000
end


g_brnnDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_brnnDefine.state_start = 2
g_brnnDefine.state_perflop = 3   --摇豆子时间
g_brnnDefine.state_pour = 4      --下注的时间
g_brnnDefine.state_count = 5      --结算时间


g_brnnDefine.perflop_time = 3    --开始的时候，摇杯子的时间
g_brnnDefine.pour_time = 15      --下注的时间
--g_brnnDefine.count_time = 15
g_brnnDefine.count_time = 15

g_brnnDefine.chat_txt = 1
g_brnnDefine.chat_img = 2
g_brnnDefine.chat_voice = 3


g_brnnDefine.type_ydc = 1       --欢乐赢
g_brnnDefine.type_lxc = 2

g_brnnDefine.leave_dismiss = 1   --房主解散房间
g_brnnDefine.leave_timeout = 2   --五轮没下注被踢出

g_brnnDefine.quit_count  = 10     --几轮没下注后就被踢出

g_brnnDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_brnnDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_brnnDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励


g_brnncard_type = {}

g_brnncard_type.wu_niu = 0     --无牛
g_brnncard_type.niu_yi = 1     --牛一
g_brnncard_type.niu_er = 2     --牛二
g_brnncard_type.niu_san = 3    --牛三
g_brnncard_type.niu_si = 4     --牛四
g_brnncard_type.niu_wu = 5     --牛五
g_brnncard_type.niu_liu = 6    --牛六
g_brnncard_type.niu_qi = 7     --牛七
g_brnncard_type.niu_ba = 8     --牛八
g_brnncard_type.niu_jiu = 9    --牛九
g_brnncard_type.niu_niu = 10   --牛牛
g_brnncard_type.yin_niu = 11   --银牛
g_brnncard_type.jin_niu = 12   --金牛
g_brnncard_type.wu_xiao = 13   --五小牛
g_brnncard_type.za_dan = 14    --炸弹


g_brnncard_type.win_rate = {}  --这个是赔率
g_brnncard_type.win_rate[0] = 2
g_brnncard_type.win_rate[1] = 2
g_brnncard_type.win_rate[2] = 2
g_brnncard_type.win_rate[3] = 2
g_brnncard_type.win_rate[4] = 2
g_brnncard_type.win_rate[5] = 2
g_brnncard_type.win_rate[6] = 2
g_brnncard_type.win_rate[7] = 3
g_brnncard_type.win_rate[8] = 4
g_brnncard_type.win_rate[9] = 5
g_brnncard_type.win_rate[10] = 6
g_brnncard_type.win_rate[11] = 7
g_brnncard_type.win_rate[12] = 8
g_brnncard_type.win_rate[13] = 9
g_brnncard_type.win_rate[14] = 9