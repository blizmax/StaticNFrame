
g_fqzsDefine = {}

g_fqzsDefine.game_type = 4300   --和协议号是同一个号段的

g_fqzsDefine.table_type = {}           --
g_fqzsDefine.table_type['junior'] = 1
g_fqzsDefine.table_type['medium'] = 2
g_fqzsDefine.table_type['serior'] = 3
g_fqzsDefine.table_type['high'] = 4
g_fqzsDefine.table_type['fu01'] = 5
g_fqzsDefine.table_type['fu02'] = 6

g_fqzsDefine.min_jetton = {}
g_fqzsDefine.min_jetton[1] = 10000
g_fqzsDefine.min_jetton[2] = 10000
g_fqzsDefine.min_jetton[3] = 10000
g_fqzsDefine.min_jetton[4] = 10000

g_fqzsDefine.max_jetton = {}
g_fqzsDefine.max_jetton[1] = -1
g_fqzsDefine.max_jetton[2] = -1
g_fqzsDefine.max_jetton[3] = -1
g_fqzsDefine.max_jetton[4] = -1   --没有上线

g_fqzsDefine.banker_limit = {}
g_fqzsDefine.banker_limit[1] = 200000
g_fqzsDefine.banker_limit[2] = 200000
g_fqzsDefine.banker_limit[3] = 200000
g_fqzsDefine.banker_limit[4] = 200000

g_fqzsDefine.state_ready = 1
g_fqzsDefine.state_start = 2
g_fqzsDefine.state_play = 3
g_fqzsDefine.state_count = 4
g_fqzsDefine.state_end = 5

g_fqzsDefine.time_start = 10
g_fqzsDefine.time_play = 10
g_fqzsDefine.time_count = 15
g_fqzsDefine.time_end = 3

g_fqzsDefine.history_count = 32	-- 历史记录最多32条

-- 下注类型
g_fqzsPourID = {}
g_fqzsPourID.yan_zi = 1
g_fqzsPourID.ge_zi = 2
g_fqzsPourID.kong_que = 3
g_fqzsPourID.lao_ying = 4
g_fqzsPourID.tu_zi = 5
g_fqzsPourID.hou_zi = 6
g_fqzsPourID.xiong_mao = 7
g_fqzsPourID.shi_zi = 8
g_fqzsPourID.fei_qing = 9
g_fqzsPourID.zou_shou = 10
g_fqzsPourID.sha_yu = 11

-- 特殊的抽奖项
g_fqzsSpecialPrize ={}
g_fqzsSpecialPrize.jin_sha = 12
g_fqzsSpecialPrize.ku_lou = 13		-- 通赔
g_fqzsSpecialPrize.zha_dan = 14		-- 通杀

-- 下注类型对应的倍数
g_fqzsPourMul = {
	[g_fqzsPourID.yan_zi] = 6,
	[g_fqzsPourID.ge_zi] = 8,
	[g_fqzsPourID.kong_que] = 8,
	[g_fqzsPourID.lao_ying] = 12,
	
	[g_fqzsPourID.tu_zi] = 6,
	[g_fqzsPourID.hou_zi] = 8,
	[g_fqzsPourID.xiong_mao] = 8,
	[g_fqzsPourID.shi_zi] = 12,

	[g_fqzsPourID.fei_qing] = 2,
	[g_fqzsPourID.zou_shou] = 2,
	[g_fqzsPourID.sha_yu] = 24,
	[g_fqzsSpecialPrize.jin_sha] = 100,
}

--以下add by landy ，主要用于计算开奖的概率和奖池
g_fqzsDefine.rand_list = {
	{g_fqzsPourID.yan_zi,1536},
	{g_fqzsPourID.ge_zi,1189},
	{g_fqzsPourID.kong_que,1189},
	{g_fqzsPourID.lao_ying,792},
	{g_fqzsPourID.tu_zi,1536},
	{g_fqzsPourID.hou_zi,1189},
	{g_fqzsPourID.xiong_mao,1189},
	{g_fqzsPourID.shi_zi,792},
	{g_fqzsPourID.sha_yu,396},   --原本是400，先改为0
	{g_fqzsSpecialPrize.jin_sha,96},   --原本是100， 先改为0
	{g_fqzsSpecialPrize.ku_lou,50},
	{g_fqzsSpecialPrize.zha_dan,50}
}

g_fqzsDefine.chance_list = {}
g_fqzsDefine.chance_list[g_fqzsPourID.yan_zi] = 1586
g_fqzsDefine.chance_list[g_fqzsPourID.ge_zi] = 1189
g_fqzsDefine.chance_list[g_fqzsPourID.kong_que] = 1189
g_fqzsDefine.chance_list[g_fqzsPourID.lao_ying] = 792
g_fqzsDefine.chance_list[g_fqzsPourID.tu_zi] = 1586
g_fqzsDefine.chance_list[g_fqzsPourID.hou_zi] = 1189
g_fqzsDefine.chance_list[g_fqzsPourID.xiong_mao] = 1189
g_fqzsDefine.chance_list[g_fqzsPourID.shi_zi] = 792
g_fqzsDefine.chance_list[g_fqzsPourID.sha_yu] = 396
g_fqzsDefine.chance_list[g_fqzsSpecialPrize.jin_sha] = 96
g_fqzsDefine.chance_list[g_fqzsSpecialPrize.ku_lou] = 0
g_fqzsDefine.chance_list[g_fqzsSpecialPrize.zha_dan] = 0


g_fqzsDefine.all_pour_list = {
g_fqzsPourID.yan_zi,
g_fqzsPourID.ge_zi,
g_fqzsPourID.kong_que,
g_fqzsPourID.lao_ying,
g_fqzsPourID.tu_zi,
g_fqzsPourID.hou_zi,
g_fqzsPourID.xiong_mao,
g_fqzsPourID.shi_zi ,
g_fqzsPourID.fei_qing ,
g_fqzsPourID.zou_shou,
g_fqzsPourID.sha_yu,
g_fqzsSpecialPrize.jin_sha,
--g_fqzsSpecialPrize.ku_lou,
--g_fqzsSpecialPrize.zha_dan
 }



--以上add by landy
-----------
-- 抽奖列表
g_fqzsPrizeList = {}
local function addPrize(prize, cnt)
	if nil == cnt or 1 > cnt then
		cnt = 1
	end
	for i = 1, cnt do
		table.insert(g_fqzsPrizeList, prize)
	end
end
addPrize(g_fqzsSpecialPrize.jin_sha)
addPrize(g_fqzsPourID.tu_zi, 3)
addPrize(g_fqzsPourID.xiong_mao, 3)
--addPrize(g_fqzsSpecialPrize.zha_dan)
addPrize(g_fqzsPourID.hou_zi, 3)
addPrize(g_fqzsPourID.shi_zi, 3)
addPrize(g_fqzsPourID.sha_yu, 1)
addPrize(g_fqzsPourID.lao_ying, 3)
addPrize(g_fqzsPourID.kong_que, 3)
--addPrize(g_fqzsSpecialPrize.ku_lou, 1)
addPrize(g_fqzsPourID.ge_zi, 3)
addPrize(g_fqzsPourID.yan_zi, 3)


g_fqzsDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_fqzsDefine.init_data[1] = {}
g_fqzsDefine.init_data[2] = {}
--g_fqzsDefine.init_data[3] = {}
--g_fqzsDefine.init_data[4] = {}
--g_fqzsDefine.init_data[5] = {}
--g_fqzsDefine.init_data[6] = {}

g_fqzsDefine.init_data[1]['tableid'] = {2081001}     --这里用table，防止后面有需要搭建多个同样类型的
g_fqzsDefine.init_data[1]['maxuser'] = 999
g_fqzsDefine.init_data[1]['bankerid'] = 1
g_fqzsDefine.init_data[1]['bankername'] = '不会打中文'
g_fqzsDefine.init_data[1]['bankernamelist'] = {'不会打中文','A太阳数码','幸运草','Love TY','红桃心','希田梅子','黄小花','YanKee','时^^光','端木剑一','Magical X','四月芳菲'}
g_fqzsDefine.init_data[1]['faceid'] = "90001_1001"
g_fqzsDefine.init_data[1]['banklimit'] = 200000
g_fqzsDefine.init_data[1]['enterlimit'] = -1
--g_fqzsDefine.init_data[1]['enterlimit'] = 1000
g_fqzsDefine.init_data[1]['tabletype'] = 1
g_fqzsDefine.init_data[1]['bankjetton'] = 500000
g_fqzsDefine.init_data[1]['tablename'] = "荣耀厅01"
g_fqzsDefine.init_data[1]['multiple'] = 5
g_fqzsDefine.init_data[1]['limitred']  = 999999999

g_fqzsDefine.init_data[2]['tableid'] = {2082001}    --对应的tableid
g_fqzsDefine.init_data[2]['maxuser'] = 999
g_fqzsDefine.init_data[2]['bankerid'] = 1
g_fqzsDefine.init_data[2]['bankername'] = '陈氏太极'
g_fqzsDefine.init_data[2]['bankernamelist'] = {'追忆','陈氏太极','枕头菌','KUKU','爱上好心情','那些年','男人本色','CoCol','陌路人','随遇而安','至尊宝','玩家101'}
g_fqzsDefine.init_data[2]['faceid'] = "90002_1001"
g_fqzsDefine.init_data[2]['banklimit'] = 200000
g_fqzsDefine.init_data[2]['enterlimit'] = -1
--g_fqzsDefine.init_data[2]['enterlimit'] = 100000
g_fqzsDefine.init_data[2]['tabletype'] = 2
g_fqzsDefine.init_data[2]['bankjetton'] = 500000
g_fqzsDefine.init_data[2]['tablename'] = "荣耀厅02"
g_fqzsDefine.init_data[2]['multiple'] = 5
g_fqzsDefine.init_data[2]['limitred']  = 999999999

--[[
g_fqzsDefine.init_data[3]['tableid'] = {2083001}    --对应的tableid
g_fqzsDefine.init_data[3]['maxuser'] = 999
g_fqzsDefine.init_data[3]['bankerid'] = 1
g_fqzsDefine.init_data[3]['bankername'] = '纯妹'
g_fqzsDefine.init_data[3]['bankernamelist'] = {'纯妹','幸福人生','Damon','白天不懂夜的黑','风继续吹','财小姐','单令高胚','我是小蕊','Administrator','微笑人生','上善若水','莉莉安'}
g_fqzsDefine.init_data[3]['faceid'] = "90003_1001"
g_fqzsDefine.init_data[3]['banklimit'] = 200000
g_fqzsDefine.init_data[3]['enterlimit'] = -1
--g_fqzsDefine.init_data[3]['enterlimit'] = 1000000
g_fqzsDefine.init_data[3]['tabletype'] = 3
g_fqzsDefine.init_data[3]['bankjetton'] = 888888800
g_fqzsDefine.init_data[3]['tablename'] = "荣耀厅03"
g_fqzsDefine.init_data[3]['multiple'] = 5
g_fqzsDefine.init_data[3]['limitred']  = 999999999

g_fqzsDefine.init_data[4]['tableid'] = {2084001}    --对应的tableid
g_fqzsDefine.init_data[4]['maxuser'] = 999
g_fqzsDefine.init_data[4]['bankerid'] = 1
g_fqzsDefine.init_data[4]['bankername'] = '木月'
g_fqzsDefine.init_data[4]['bankernamelist'] = {'木月','奧利奧','我是大侦探','小红娘','永恒纪元','战天下','屠龙传说','杨桃儿','草莓蛋糕','Daisy','老K','月光之夏'}
g_fqzsDefine.init_data[4]['faceid'] = "90004_1001"
g_fqzsDefine.init_data[4]['banklimit'] = 200000
g_fqzsDefine.init_data[4]['enterlimit'] = -1
--g_fqzsDefine.init_data[4]['enterlimit'] = 1000000
g_fqzsDefine.init_data[4]['tabletype'] = 4
g_fqzsDefine.init_data[4]['bankjetton'] = 888888800
g_fqzsDefine.init_data[4]['tablename'] = "荣耀厅04"
g_fqzsDefine.init_data[4]['multiple'] = 5
g_fqzsDefine.init_data[4]['limitred']  = 999999999

g_fqzsDefine.init_data[5]['tableid'] = {2085001}    --对应的tableid
g_fqzsDefine.init_data[5]['maxuser'] = 999
g_fqzsDefine.init_data[5]['bankerid'] = 1
g_fqzsDefine.init_data[5]['bankername'] = '杜甫的诗歌'
g_fqzsDefine.init_data[5]['bankernamelist'] = {'杜甫的诗歌','东方红','刚好','监管科好看','很过分冬天','温热发货成功','合肥师范学院','还敢打我','爱到伤心'}
g_fqzsDefine.init_data[5]['faceid'] = "90003_1001"
g_fqzsDefine.init_data[5]['banklimit'] = 200000
g_fqzsDefine.init_data[5]['enterlimit'] = -1
--g_fqzsDefine.init_data[3]['enterlimit'] = 1000000
g_fqzsDefine.init_data[5]['tabletype'] = 5
g_fqzsDefine.init_data[5]['bankjetton'] = 888888800
g_fqzsDefine.init_data[5]['tablename'] = "荣耀厅05"
g_fqzsDefine.init_data[5]['multiple'] = 5
g_fqzsDefine.init_data[5]['limitred']  = 999999999

g_fqzsDefine.init_data[6]['tableid'] = {2086001}    --对应的tableid
g_fqzsDefine.init_data[6]['maxuser'] = 999
g_fqzsDefine.init_data[6]['bankerid'] = 1
g_fqzsDefine.init_data[6]['bankername'] = 'ACVG基本金属'
g_fqzsDefine.init_data[6]['bankernamelist'] = {'ACVG基本金属','鬼地方','广东','好几款','地方规划','捂捂盖盖','好好读书','发给','热播后','上官府邸','艾斯'}
g_fqzsDefine.init_data[6]['faceid'] = "90004_1001"
g_fqzsDefine.init_data[6]['banklimit'] = 200000
g_fqzsDefine.init_data[6]['enterlimit'] = -1
--g_fqzsDefine.init_data[4]['enterlimit'] = 1000000
g_fqzsDefine.init_data[6]['tabletype'] = 6
g_fqzsDefine.init_data[6]['bankjetton'] = 888888800
g_fqzsDefine.init_data[6]['tablename'] = "荣耀厅06"
g_fqzsDefine.init_data[6]['multiple'] = 5
g_fqzsDefine.init_data[6]['limitred']  = 999999999
]]
g_fqzsDefine.remainpourjetton = 10    --最大下注与庄家身上筹码的比例


g_fqzsDefine.robot_range = { {1000, 1000000}, {1000, 1000000},{1000,1000000},{1000, 1000000},{1000, 1000000},{1000, 1000000}}

g_fqzsDefine.game_lockstate = {}
g_fqzsDefine.game_lockstate.lock_no = 1
g_fqzsDefine.game_lockstate.lock_yes = 2