
g_bcbmDefine = {}

g_bcbmDefine.game_type = 4200   --和协议号是同一个号段的

g_bcbmDefine.table_type = {}           --
g_bcbmDefine.table_type['junior'] = 1
g_bcbmDefine.table_type['medium'] = 2
g_bcbmDefine.table_type['serior'] = 3
g_bcbmDefine.table_type['high'] = 4
g_bcbmDefine.table_type['fu01'] = 5
g_bcbmDefine.table_type['fu02'] = 6

g_bcbmDefine.min_jetton = {}
g_bcbmDefine.min_jetton[1] = 10000
g_bcbmDefine.min_jetton[2] = 10000
g_bcbmDefine.min_jetton[3] = 10000
g_bcbmDefine.min_jetton[4] = 10000

g_bcbmDefine.max_jetton = {}
g_bcbmDefine.max_jetton[1] = -1
g_bcbmDefine.max_jetton[2] = -1
g_bcbmDefine.max_jetton[3] = -1
g_bcbmDefine.max_jetton[4] = -1   --没有上线

g_bcbmDefine.banker_limit = {}
g_bcbmDefine.banker_limit[1] = 200000
g_bcbmDefine.banker_limit[2] = 200000
g_bcbmDefine.banker_limit[3] = 200000
g_bcbmDefine.banker_limit[4] = 200000

g_bcbmDefine.game_type = 4200
g_bcbmDefine.state_ready = 1
g_bcbmDefine.state_start = 2
g_bcbmDefine.state_play = 3
g_bcbmDefine.state_count = 4
g_bcbmDefine.state_end = 5


g_bcbmDefine.time_start = 4
g_bcbmDefine.time_play = 15
g_bcbmDefine.time_count = 12
g_bcbmDefine.time_end = 10

g_bcbmDefine.history_count = 32	-- 历史记录最多32条

-- 下注类型
g_bcbmPourID = {
	'bao_shi_jie_1',
	'bao_ma_1',
	'ben_chi_1',
	'da_zhong_1',
	'bao_shi_jie_2',
	'bao_ma_2',
	'ben_chi_2',
	'da_zhong_2',
}

g_bcbmPourID.bao_shi_jie_1 = 1
g_bcbmPourID.bao_ma_1 = 2
g_bcbmPourID.ben_chi_1 = 3
g_bcbmPourID.da_zhong_1 = 4
g_bcbmPourID.bao_shi_jie_2 = 5
g_bcbmPourID.bao_ma_2 = 6
g_bcbmPourID.ben_chi_2 = 7
g_bcbmPourID.da_zhong_2 = 8


-- 下注类型对应的倍数
g_bcbmPourMul = {
	[g_bcbmPourID.da_zhong_2] = 5,
	[g_bcbmPourID.ben_chi_2] = 5,
	[g_bcbmPourID.bao_ma_2] = 5,
	[g_bcbmPourID.bao_shi_jie_2] = 5,
	[g_bcbmPourID.da_zhong_1] = 10,
	[g_bcbmPourID.ben_chi_1] = 40,
	[g_bcbmPourID.bao_ma_1] = 30,
	[g_bcbmPourID.bao_shi_jie_1] = 20,
}

-- 开奖列表
g_bcbmPrizeList = {
	g_bcbmPourID.ben_chi_1,
	g_bcbmPourID.ben_chi_2,
	g_bcbmPourID.da_zhong_1,
	g_bcbmPourID.da_zhong_2,
	g_bcbmPourID.bao_shi_jie_1,   --landy 先去掉
	g_bcbmPourID.bao_shi_jie_2,
	g_bcbmPourID.bao_ma_1,        --landy 先去掉
	g_bcbmPourID.bao_ma_2
}

--add by landy 下面是添加用于计算奖池和概率的

g_bcbmDefine.rand_list = {
	{g_bcbmPourID.ben_chi_1,248},
	{g_bcbmPourID.ben_chi_2,1983},
	{g_bcbmPourID.da_zhong_1,993},
	{g_bcbmPourID.da_zhong_2,1983},
	{g_bcbmPourID.bao_shi_jie_1,496},
	{g_bcbmPourID.bao_shi_jie_2,1983},
	{g_bcbmPourID.bao_ma_1,331},
	{g_bcbmPourID.bao_ma_2,1983}
}

g_bcbmDefine.chance_list = {}
g_bcbmDefine.chance_list[g_bcbmPourID.bao_shi_jie_1] = 496
g_bcbmDefine.chance_list[g_bcbmPourID.bao_ma_1] = 331
g_bcbmDefine.chance_list[g_bcbmPourID.ben_chi_1] = 248
g_bcbmDefine.chance_list[g_bcbmPourID.da_zhong_1] = 993
g_bcbmDefine.chance_list[g_bcbmPourID.bao_shi_jie_2] = 1983
g_bcbmDefine.chance_list[g_bcbmPourID.bao_ma_2] = 1983
g_bcbmDefine.chance_list[g_bcbmPourID.ben_chi_2] = 1983
g_bcbmDefine.chance_list[g_bcbmPourID.da_zhong_2] = 1983
--[[g_bcbmDefine.rand_list = {
	{g_bcbmPourID.ben_chi_1,500},
	{g_bcbmPourID.ben_chi_2,2000},
	{g_bcbmPourID.da_zhong_1,1000},
	{g_bcbmPourID.da_zhong_2,2000},
	{g_bcbmPourID.bao_shi_jie_1,250},
	{g_bcbmPourID.bao_shi_jie_2,2000},
	{g_bcbmPourID.bao_ma_1,250},
	{g_bcbmPourID.bao_ma_2,2000}
}--]]

----add by landy end

g_bcbmDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_bcbmDefine.init_data[1] = {}
g_bcbmDefine.init_data[2] = {}
--g_bcbmDefine.init_data[3] = {}
--g_bcbmDefine.init_data[4] = {}
--g_bcbmDefine.init_data[5] = {}
--g_bcbmDefine.init_data[6] = {}

g_bcbmDefine.init_data[1]['tableid'] = {2071001}     --这里用table，防止后面有需要搭建多个同样类型的
g_bcbmDefine.init_data[1]['maxuser'] = 999
g_bcbmDefine.init_data[1]['bankerid'] = 1
g_bcbmDefine.init_data[1]['bankername'] = '良辰奉陪'
g_bcbmDefine.init_data[1]['bankernamelist'] = {'良辰奉陪','LoveBaby','太帅了不起','蜗牛','潇洒走一回','玩机达人','Jerry','美*美','万里有云','小浪哥','知十三秋'}
g_bcbmDefine.init_data[1]['faceid'] = "90001_1001"
g_bcbmDefine.init_data[1]['banklimit'] = 200000
g_bcbmDefine.init_data[1]['enterlimit'] = -1
g_bcbmDefine.init_data[1]['tabletype'] = 1
g_bcbmDefine.init_data[1]['bankjetton'] = 500000
g_bcbmDefine.init_data[1]['tablename'] = "荣耀厅01"
g_bcbmDefine.init_data[1]['multiple'] = 5
g_bcbmDefine.init_data[1]['limitred']  = 999999999

g_bcbmDefine.init_data[2]['tableid'] = {2072001}    --对应的tableid
g_bcbmDefine.init_data[2]['maxuser'] = 999
g_bcbmDefine.init_data[2]['bankerid'] = 1
g_bcbmDefine.init_data[2]['bankername'] = '木木夕'
g_bcbmDefine.init_data[2]['bankernamelist'] = {'天然呆','木木夕','20180808','zuoqin','流&浪','记忆中有你','YLLLLLL','小不点','遇见你以后','奔驰哥','佛公子'}
g_bcbmDefine.init_data[2]['faceid'] = "90002_1001"
g_bcbmDefine.init_data[2]['banklimit'] = 200000
g_bcbmDefine.init_data[2]['enterlimit'] = -1
g_bcbmDefine.init_data[2]['entermax'] = 100000
g_bcbmDefine.init_data[2]['tabletype'] = 2
g_bcbmDefine.init_data[2]['bankjetton'] = 500000
g_bcbmDefine.init_data[2]['tablename'] = "荣耀厅02"
g_bcbmDefine.init_data[2]['multiple'] = 5
g_bcbmDefine.init_data[2]['limitred']  = 999999999

--[[
g_bcbmDefine.init_data[3]['tableid'] = {2073001}    --对应的tableid
g_bcbmDefine.init_data[3]['maxuser'] = 999
g_bcbmDefine.init_data[3]['bankerid'] = 1
g_bcbmDefine.init_data[3]['bankername'] = '系统庄'
g_bcbmDefine.init_data[3]['bankernamelist'] = {'我是大爷','萌面侠','土豆土豆','小帅哥','蒲公英的舞蹈','寻觅过往','不美不萌','听风讲你','酷哥哥'}
g_bcbmDefine.init_data[3]['faceid'] = "90003_1001"
g_bcbmDefine.init_data[3]['banklimit'] = 200000
g_bcbmDefine.init_data[3]['enterlimit'] = 0
g_bcbmDefine.init_data[3]['entermax'] = -1
g_bcbmDefine.init_data[3]['tabletype'] = 3
g_bcbmDefine.init_data[3]['bankjetton'] = 888888800
g_bcbmDefine.init_data[3]['tablename'] = "荣耀厅03"
g_bcbmDefine.init_data[3]['multiple'] = 5
g_bcbmDefine.init_data[3]['limitred']  = 999999999

g_bcbmDefine.init_data[4]['tableid'] = {2074001}    --对应的tableid
g_bcbmDefine.init_data[4]['maxuser'] = 999
g_bcbmDefine.init_data[4]['bankerid'] = 1
g_bcbmDefine.init_data[4]['bankername'] = '系统庄'
g_bcbmDefine.init_data[4]['bankernamelist'] = {'天然呆','木木夕','20180808','zuoqin','流&浪','记忆中有你','YLLLLLL','小不点','遇见你以后','奔驰哥','佛公子'}
g_bcbmDefine.init_data[4]['faceid'] = "90004_1001"
g_bcbmDefine.init_data[4]['banklimit'] = 200000
g_bcbmDefine.init_data[4]['enterlimit'] = 0
g_bcbmDefine.init_data[4]['entermax'] = -1
g_bcbmDefine.init_data[4]['tabletype'] = 4
g_bcbmDefine.init_data[4]['bankjetton'] = 888888800
g_bcbmDefine.init_data[4]['tablename'] = "荣耀厅04"
g_bcbmDefine.init_data[4]['multiple'] = 5
g_bcbmDefine.init_data[4]['limitred']  = 999999999

g_bcbmDefine.init_data[5]['tableid'] = {2075001}    --对应的tableid
g_bcbmDefine.init_data[5]['maxuser'] = 999
g_bcbmDefine.init_data[5]['bankerid'] = 1
g_bcbmDefine.init_data[5]['bankername'] = '系统庄'
g_bcbmDefine.init_data[5]['bankernamelist'] = {'我是大爷','萌面侠','土豆土豆','小帅哥','蒲公英的舞蹈','寻觅过往','不美不萌','听风讲你','酷哥哥'}
g_bcbmDefine.init_data[5]['faceid'] = "90003_1001"
g_bcbmDefine.init_data[5]['banklimit'] = 200000
g_bcbmDefine.init_data[5]['enterlimit'] = 0
g_bcbmDefine.init_data[3]['entermax'] = -1
g_bcbmDefine.init_data[5]['tabletype'] = 5
g_bcbmDefine.init_data[5]['bankjetton'] = 888888800
g_bcbmDefine.init_data[5]['tablename'] = "荣耀厅05"
g_bcbmDefine.init_data[5]['multiple'] = 5
g_bcbmDefine.init_data[5]['limitred']  = 999999999

g_bcbmDefine.init_data[6]['tableid'] = {2076001}    --对应的tableid
g_bcbmDefine.init_data[6]['maxuser'] = 999
g_bcbmDefine.init_data[6]['bankerid'] = 1
g_bcbmDefine.init_data[6]['bankername'] = '系统庄'
g_bcbmDefine.init_data[6]['bankernamelist'] = {'天然呆','木木夕','20180808','zuoqin','流&浪','记忆中有你','YLLLLLL','小不点','遇见你以后','奔驰哥','佛公子'}
g_bcbmDefine.init_data[6]['faceid'] = "90004_1001"
g_bcbmDefine.init_data[6]['banklimit'] = 200000
g_bcbmDefine.init_data[6]['enterlimit'] = 0
g_bcbmDefine.init_data[4]['entermax'] = -1
g_bcbmDefine.init_data[6]['tabletype'] = 6
g_bcbmDefine.init_data[6]['bankjetton'] = 888888800
g_bcbmDefine.init_data[6]['tablename'] = "荣耀厅06"
g_bcbmDefine.init_data[6]['multiple'] = 5
g_bcbmDefine.init_data[6]['limitred']  = 999999999
]]
g_bcbmDefine.remainpourjetton = 10    --最大下注与庄家身上筹码的比例

g_bcbmDefine.robot_range = { {10000, 1000000}, {10000, 1000000},{10000,1000000},{10000, 1000000},{10000,1000000},{10000,1000000}}
