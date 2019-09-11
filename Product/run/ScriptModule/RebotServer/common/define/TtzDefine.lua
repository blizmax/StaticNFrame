

--牛牛的牌型是三位数，第一位是花色后面两位是牌大小
--小王是：515，大王是525

g_TtzDefine = {}

g_TtzDefine.game_type = 8500   --和协议号是同一个号段的

g_TtzDefine.pour_type = {1,2,3}

g_TtzDefine.table_common = 777777

g_TtzDefine.init_sit = 6      --初始化的6个座位
g_TtzDefine.page_item = 8     --每一页显示的牌桌的数量
g_TtzDefine.min_bankjetton = 100000    --最小上庄的倍数
g_TtzDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_TtzDefine.min_pour_common = 100  --普通场最小下注
g_TtzDefine.min_pour_grade = 10000  --高级场最小下注
g_TtzDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_TtzDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_TtzDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_TtzDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_TtzDefine.pour2_rate = 10
	g_TtzDefine.pour3_rate = 50
	
	g_TtzDefine.min_pour_common = 100
	g_TtzDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建
g_TtzDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_TtzDefine.init_data[1] = {}
--g_TtzDefine.init_data[2] = {}
--g_brnnDefine.init_data[3] = {}
--g_brnnDefine.init_data[4] = {}

g_TtzDefine.init_data[1]['tableid'] = {2281001}     --这里用table，防止后面有需要搭建多个同样类型的
g_TtzDefine.init_data[1]['maxuser'] = 999
g_TtzDefine.init_data[1]['bankerid'] = 1
g_TtzDefine.init_data[1]['bankername'] = '青醉'
g_TtzDefine.init_data[1]['bankernamelist'] = {'青醉','狂想曲','wx100','江山美人','财神爷','赢他一个亿','发发发','百万梦想'}
g_TtzDefine.init_data[1]['faceid'] = "90001_1001"
g_TtzDefine.init_data[1]['banklimit'] = 200000
g_TtzDefine.init_data[1]['enterlimit'] = 0
g_TtzDefine.init_data[1]['entermax'] = -1
g_TtzDefine.init_data[1]['tabletype'] = 1
g_TtzDefine.init_data[1]['bankjetton'] = 500000
g_TtzDefine.init_data[1]['tablename'] = "荣耀厅01"
g_TtzDefine.init_data[1]['multiple'] = 2
g_TtzDefine.init_data[1]['limitred'] = 50000000
--[[
g_TtzDefine.init_data[2]['tableid'] = {2282001}    --对应的tableid
g_TtzDefine.init_data[2]['maxuser'] = 999
g_TtzDefine.init_data[2]['bankerid'] = 1
g_TtzDefine.init_data[2]['bankername'] = '千年三梦'
g_TtzDefine.init_data[2]['bankernamelist'] = {'echo','why me','o-o','千年三梦','后悔无期','长生不老','梦想家','来一发'}
g_TtzDefine.init_data[2]['faceid'] = "90002_1001"
g_TtzDefine.init_data[2]['banklimit'] = 200000
g_TtzDefine.init_data[2]['enterlimit'] = 0
g_TtzDefine.init_data[2]['entermax'] = -1
g_TtzDefine.init_data[2]['tabletype'] = 2
g_TtzDefine.init_data[2]['bankjetton'] = 888888800
g_TtzDefine.init_data[2]['tablename'] = "荣耀厅02"
g_TtzDefine.init_data[2]['multiple'] = 5
g_TtzDefine.init_data[2]['limitred'] = 999999999
]]

g_TtzDefine.table_type = {}           --
g_TtzDefine.table_type['junior'] = 1
g_TtzDefine.table_type['medium'] = 2
g_TtzDefine.table_type['serior'] = 3
g_TtzDefine.table_type['high'] = 4

g_TtzDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_TtzDefine.state_start = 2
g_TtzDefine.state_perflop = 3    --摇骰子发牌状态
g_TtzDefine.state_pour = 4       --下注状态
g_TtzDefine.state_opencard = 5   --开牌状态
g_TtzDefine.state_count = 6      --结算时间状态

g_TtzDefine.state_start_wait = 11    
g_TtzDefine.state_perflop_wait = 12


g_TtzDefine.robot_range = { {20000, 800000}, {20000, 800000},{20000,800000},{20000, 800000},{20000, 800000},{20000, 800000}}

g_Ttzcard_type = {}
--普通点数 0 - 9			           --value值  牌型等级大小	
g_Ttzcard_type.var_0 = 0     
g_Ttzcard_type.var_1 = 1     
g_Ttzcard_type.var_2 = 2     
g_Ttzcard_type.var_3 = 3    
g_Ttzcard_type.var_4 = 4     
g_Ttzcard_type.var_5 = 5     
g_Ttzcard_type.var_6 = 6    
g_Ttzcard_type.var_7 = 7   
g_Ttzcard_type.var_8 = 8     
g_Ttzcard_type.var_9 = 9
--特殊牌型二八杠
g_Ttzcard_type.erbagang = 10
--对子点数 0 - 9
g_Ttzcard_type.duizi_0 = 30  
g_Ttzcard_type.duizi_1 = 21  
g_Ttzcard_type.duizi_2 = 22   
g_Ttzcard_type.duizi_3 = 23   
g_Ttzcard_type.duizi_4 = 24   
g_Ttzcard_type.duizi_5 = 25    
g_Ttzcard_type.duizi_6 = 26    
g_Ttzcard_type.duizi_7 = 27    
g_Ttzcard_type.duizi_8 = 28    
g_Ttzcard_type.duizi_9 = 29   

g_TtzCardTypeArray = {               --用于通过下标快速获取牌型
	g_Ttzcard_type.var_0,
	g_Ttzcard_type.var_1,
	g_Ttzcard_type.var_2,
	g_Ttzcard_type.var_3,
	g_Ttzcard_type.var_4,
	g_Ttzcard_type.var_5,
	g_Ttzcard_type.var_6,
	g_Ttzcard_type.var_7,
	g_Ttzcard_type.var_8,
	g_Ttzcard_type.var_9,

	g_Ttzcard_type.duizi_0,
	g_Ttzcard_type.duizi_1,
	g_Ttzcard_type.duizi_2,
	g_Ttzcard_type.duizi_3,
	g_Ttzcard_type.duizi_4,
	g_Ttzcard_type.duizi_5,
	g_Ttzcard_type.duizi_6,
	g_Ttzcard_type.duizi_7,
	g_Ttzcard_type.duizi_8,
	g_Ttzcard_type.duizi_9,
}

g_TtzDefine.perflop_time = 4    --开始的时候，摇杯子的时间
g_TtzDefine.pour_time = 15      --下注的时间
g_TtzDefine.count_time = 5
g_TtzDefine.opencard_time = 5   --开牌时间
g_TtzDefine.ready_time = 0

g_TtzDefine.chat_txt = 1
g_TtzDefine.chat_img = 2
g_TtzDefine.chat_voice = 3


g_TtzDefine.type_ydc = 1       --欢乐赢
g_TtzDefine.type_lxc = 2

g_TtzDefine.leave_dismiss = 1   --房主解散房间
g_TtzDefine.leave_timeout = 2   --五轮没下注被踢出

g_TtzDefine.quit_count  = 10     --几轮没下注后就被踢出

g_TtzDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_TtzDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_TtzDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励


g_Ttzcard_type.win_rate = {}  --这个是低倍场赔率
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_0] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_1] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_2] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_3] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_4] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_5] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_6] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_7] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_8] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.var_9] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.erbagang] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_0] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_1] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_2] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_3] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_4] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_5] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_6] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_7] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_8] = 2
g_Ttzcard_type.win_rate[g_Ttzcard_type.duizi_9] = 2


g_Ttzcard_type.win_rate_high = {}  --这个是高倍场赔率
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_0] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_1] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_2] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_3] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_4] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_5] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_6] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_7] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_8] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.var_9] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.erbagang] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_0] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_1] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_2] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_3] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_4] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_5] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_6] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_7] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_8] = 4
g_Ttzcard_type.win_rate_high[g_Ttzcard_type.duizi_9] = 4

-- 不算本金的最大赔付倍数,用于下注时判断玩家或庄家的筹码是否够赔
g_TtzDefine.pour_pay_mul = g_Ttzcard_type.win_rate[#g_Ttzcard_type.win_rate] - 1

--N局不行动踢出房间
g_TtzDefine.MaxNoAction = 20