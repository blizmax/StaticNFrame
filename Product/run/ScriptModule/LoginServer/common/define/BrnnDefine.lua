

--牛牛的牌型是三位数，第一位是花色后面两位是牌大小
--小王是：515，大王是525

g_brnnDefine = {}

g_brnnDefine.game_type = 2500   --和协议号是同一个号段的

g_brnnDefine.pour_type = {1,2,3,4}

g_brnnDefine.table_common = 777777

g_brnnDefine.init_sit = 6      --初始化的6个座位
g_brnnDefine.page_item = 8     --每一页显示的牌桌的数量
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


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建
g_brnnDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_brnnDefine.init_data[1] = {}
g_brnnDefine.init_data[2] = {}
--g_brnnDefine.init_data[3] = {}
--g_brnnDefine.init_data[4] = {}

g_brnnDefine.init_data[1]['tableid'] = {2051001}     --这里用table，防止后面有需要搭建多个同样类型的
g_brnnDefine.init_data[1]['maxuser'] = 999
g_brnnDefine.init_data[1]['bankerid'] = 1
g_brnnDefine.init_data[1]['bankername'] = '青醉'
g_brnnDefine.init_data[1]['bankernamelist'] = {'青醉','狂想曲','wx100','江山美人','财神爷','赢他一个亿','发发发','百万梦想'}
g_brnnDefine.init_data[1]['faceid'] = "90001_1001"
g_brnnDefine.init_data[1]['banklimit'] = 200000
g_brnnDefine.init_data[1]['enterlimit'] = 0
g_brnnDefine.init_data[1]['entermax'] = -1
g_brnnDefine.init_data[1]['tabletype'] = 1
g_brnnDefine.init_data[1]['bankjetton'] = 5000000
g_brnnDefine.init_data[1]['tablename'] = "荣耀厅01"
g_brnnDefine.init_data[1]['multiple'] = 5
g_brnnDefine.init_data[1]['limitred'] = 999999999

g_brnnDefine.init_data[2]['tableid'] = {2052001}    --对应的tableid
g_brnnDefine.init_data[2]['maxuser'] = 999
g_brnnDefine.init_data[2]['bankerid'] = 1
g_brnnDefine.init_data[2]['bankername'] = '千年三梦'
g_brnnDefine.init_data[2]['bankernamelist'] = {'echo','why me','o-o','千年三梦','后悔无期','长生不老','梦想家','来一发'}
g_brnnDefine.init_data[2]['faceid'] = "90002_1001"
g_brnnDefine.init_data[2]['banklimit'] = 200000
g_brnnDefine.init_data[2]['enterlimit'] = 0
g_brnnDefine.init_data[2]['entermax'] = -1
g_brnnDefine.init_data[2]['tabletype'] = 2
g_brnnDefine.init_data[2]['bankjetton'] = 500000
g_brnnDefine.init_data[2]['tablename'] = "荣耀厅02"
g_brnnDefine.init_data[2]['multiple'] = 5
g_brnnDefine.init_data[2]['limitred'] = 999999999

--[[
g_brnnDefine.init_data[3]['tableid'] = {2053001}    --对应的tableid
g_brnnDefine.init_data[3]['maxuser'] = 999
g_brnnDefine.init_data[3]['bankerid'] = 1
g_brnnDefine.init_data[3]['bankername'] = '赌&神'
g_brnnDefine.init_data[3]['bankernamelist'] = {'赌&神','pwd2000','TECH ME','出入江湖','三贱客','可儿','100cc','九姑娘'}
g_brnnDefine.init_data[3]['faceid'] = "90003_1001"
g_brnnDefine.init_data[3]['banklimit'] = 200000
g_brnnDefine.init_data[3]['enterlimit'] = 0
g_brnnDefine.init_data[3]['entermax'] = -1
g_brnnDefine.init_data[3]['tabletype'] = 3
g_brnnDefine.init_data[3]['bankjetton'] = 888888800
g_brnnDefine.init_data[3]['tablename'] = "传奇厅01"
g_brnnDefine.init_data[3]['multiple'] = 10
g_brnnDefine.init_data[3]['limitred'] = 999999999


g_brnnDefine.init_data[4]['tableid'] = {2054001}    --对应的tableid
g_brnnDefine.init_data[4]['maxuser'] = 999
g_brnnDefine.init_data[4]['bankerid'] = 1
g_brnnDefine.init_data[4]['bankername'] = '北京男孩'
g_brnnDefine.init_data[4]['bankernamelist'] = {'mygod','北京男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
g_brnnDefine.init_data[4]['faceid'] = "90004_1001"
g_brnnDefine.init_data[4]['banklimit'] = 200000
g_brnnDefine.init_data[4]['enterlimit'] = 0
g_brnnDefine.init_data[4]['entermax'] = -1
g_brnnDefine.init_data[4]['tabletype'] = 4
g_brnnDefine.init_data[4]['bankjetton'] = 888888800
g_brnnDefine.init_data[4]['tablename'] = "传奇厅02"
g_brnnDefine.init_data[4]['multiple'] = 10
g_brnnDefine.init_data[4]['limitred'] = 999999999
]]



g_brnnDefine.table_type = {}           --
g_brnnDefine.table_type['junior'] = 1
g_brnnDefine.table_type['medium'] = 2
g_brnnDefine.table_type['serior'] = 3
g_brnnDefine.table_type['high'] = 4

g_brnnDefine.game_type = 2500

g_brnnDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_brnnDefine.state_start = 2
g_brnnDefine.state_perflop = 3   --摇豆子时间
g_brnnDefine.state_pour = 4      --下注的时间
g_brnnDefine.state_count = 5      --结算时间
g_brnnDefine.state_end = 6      --结束的状态

g_brnnDefine.state_start_wait = 11    
g_brnnDefine.state_perflop_wait = 12


g_brnnDefine.robot_range = { {1000, 1000000}, {1000, 1000000},{1000,1000000},{1000, 1000000},{1000, 1000000},{1000, 1000000}}






g_brnnDefine.perflop_time = 3    --开始的时候，摇杯子的时间
g_brnnDefine.pour_time = 15      --下注的时间
g_brnnDefine.count_time = 15

g_brnnDefine.ready_time = 8

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
g_brnncard_type.wu_hua = 15    --五花牛


g_brnncard_type.win_rate = {}  --这个是低倍场赔率
g_brnncard_type.win_rate[0] = 2
g_brnncard_type.win_rate[1] = 2
g_brnncard_type.win_rate[2] = 2
g_brnncard_type.win_rate[3] = 2
g_brnncard_type.win_rate[4] = 2
g_brnncard_type.win_rate[5] = 2
g_brnncard_type.win_rate[6] = 2
g_brnncard_type.win_rate[7] = 3
g_brnncard_type.win_rate[8] = 3
g_brnncard_type.win_rate[9] = 3
g_brnncard_type.win_rate[10] = 4
g_brnncard_type.win_rate[11] = 4
g_brnncard_type.win_rate[12] = 4
g_brnncard_type.win_rate[13] = 4
g_brnncard_type.win_rate[14] = 5
g_brnncard_type.win_rate[15] = 6


g_brnncard_type.win_rate_high = {}  --这个是高倍场赔率
g_brnncard_type.win_rate_high[0] = 2
g_brnncard_type.win_rate_high[1] = 2
g_brnncard_type.win_rate_high[2] = 3
g_brnncard_type.win_rate_high[3] = 4
g_brnncard_type.win_rate_high[4] = 5
g_brnncard_type.win_rate_high[5] = 6
g_brnncard_type.win_rate_high[6] = 7
g_brnncard_type.win_rate_high[7] = 8
g_brnncard_type.win_rate_high[8] = 9
g_brnncard_type.win_rate_high[9] = 10
g_brnncard_type.win_rate_high[10] = 11
g_brnncard_type.win_rate_high[11] = 11
g_brnncard_type.win_rate_high[12] = 11
g_brnncard_type.win_rate_high[13] = 11
g_brnncard_type.win_rate_high[14] = 11
g_brnncard_type.win_rate_high[15] = 11

-- 不算本金的最大赔付倍数,用于下注时判断玩家或庄家的筹码是否够赔
g_brnnDefine.pour_pay_mul = g_brnncard_type.win_rate[#g_brnncard_type.win_rate] - 1


--N局不行动踢出房间
g_brnnDefine.MaxNoAction = 20