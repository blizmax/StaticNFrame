g_yqsDefine = {}

-- 协议编号范围
g_yqsDefine.game_type = 4900
g_yqsDefine.proto_id_begin = 4901        -- 起始
g_yqsDefine.proto_id_end = 5000          -- 结束



-- 桌子号范围
g_yqsDefine.table_id_begin = 2150000
g_yqsDefine.table_id_end = 2160000 - 1

-- 桌子人数上限
g_yqsDefine.max_user = 4

-- 分数类型
g_yqsDefine.score_type_jetton = 1       	-- 筹码
g_yqsDefine.score_type_money = 2			-- 钻石

-- 桌子状态
g_yqsDefine.state_ready = 1
g_yqsDefine.state_play = 2
g_yqsDefine.state_tide = 3


--时间
g_yqsDefine.time_ready = 2
g_yqsDefine.time_play = 60 * 5
g_yqsDefine.time_tide = 10

----
g_yqsDefine.fire_interval = 0.2				-- 每次开火的时间间隔(秒)
g_yqsDefine.ion_need_power = 100			--
g_yqsDefine.freeze_time = 10				-- 冰冻技能持续时间(秒)
g_yqsDefine.freeze_cool_down = 10			-- 冰冻技能冷却时间(秒)
g_yqsDefine.range_bomb_radius = 400			-- 
g_yqsDefine.lock_time = 10					-- 锁定技能持续时间(秒)
g_yqsDefine.lock_cool_down = 10				-- 锁定技能冷却时间(秒)

g_yqsDefine.boom_kill_type = 10				-- 炸弹鱼可以杀死的类型(小于该值的都可以)
g_yqsDefine.boom_kill_count = 10			-- 炸弹鱼可以杀死的数量



g_yqsDefine.table_type = {}           --
g_yqsDefine.table_type['junior'] = 1	-- 摇钱树
g_yqsDefine.table_type['medium'] = 2
g_yqsDefine.table_type['serior'] = 3
--g_yqsDefine.table_type['high'] = 4

g_yqsDefine.init_data = {}
g_yqsDefine.init_data[1] = {}
g_yqsDefine.init_data[2] = {}
g_yqsDefine.init_data[3] = {}
--g_yqsDefine.init_data[4] = {}

--g_yqsDefine.init_data[1]['tableid'] = {2141001}     --这里用table，防止后面有需要搭建多个同样类型的
g_yqsDefine.init_data[1]['maxuser'] = 4
g_yqsDefine.init_data[1]['bankerid'] = 1
g_yqsDefine.init_data[1]['bankername'] = '小甜心'
g_yqsDefine.init_data[1]['bankerfaceid'] = "90001_1001"
g_yqsDefine.init_data[1]['banklimit'] = 200000
g_yqsDefine.init_data[1]['minenter'] = 1*100
g_yqsDefine.init_data[1]['maxenter'] = 1000000000
g_yqsDefine.init_data[1]['tabletype'] = 1
g_yqsDefine.init_data[1]['bankerjetton'] = 999999999

--g_yqsDefine.init_data[2]['tableid'] = {2142001}     --这里用table，防止后面有需要搭建多个同样类型的
g_yqsDefine.init_data[2]['maxuser'] = 4
g_yqsDefine.init_data[2]['bankerid'] = 1
g_yqsDefine.init_data[2]['bankername'] = '甜甜圈'
g_yqsDefine.init_data[2]['bankerfaceid'] = "90002_1001"
g_yqsDefine.init_data[2]['banklimit'] = 200000
g_yqsDefine.init_data[2]['minenter'] = 10*100
g_yqsDefine.init_data[2]['maxenter'] = 1000000000
g_yqsDefine.init_data[2]['tabletype'] = 2
g_yqsDefine.init_data[2]['bankerjetton'] = 999999999

--g_yqsDefine.init_data[3]['tableid'] = {2143001}     --这里用table，防止后面有需要搭建多个同样类型的
g_yqsDefine.init_data[3]['maxuser'] = 4
g_yqsDefine.init_data[3]['bankerid'] = 1
g_yqsDefine.init_data[3]['bankername'] = '奈何情深'
g_yqsDefine.init_data[3]['bankerfaceid'] = "90003_1001"
g_yqsDefine.init_data[3]['banklimit'] = 200000
g_yqsDefine.init_data[3]['minenter'] = 100*100
g_yqsDefine.init_data[3]['maxenter'] = 1000000000
g_yqsDefine.init_data[3]['tabletype'] = 3
g_yqsDefine.init_data[3]['bankerjetton'] = 999999999

--g_yqsDefine.init_data[4]['tableid'] = {2144001}     --这里用table，防止后面有需要搭建多个同样类型的
--g_yqsDefine.init_data[4]['maxuser'] = 999
--g_yqsDefine.init_data[4]['bankerid'] = 1
--g_yqsDefine.init_data[4]['bankername'] = '不服来战'
--g_yqsDefine.init_data[4]['bankerfaceid'] = "90004_1001"
--g_yqsDefine.init_data[4]['banklimit'] = 100000000
--g_yqsDefine.init_data[4]['minenter'] =60000 --3000000
--g_yqsDefine.init_data[4]['maxenter'] = -1
--g_yqsDefine.init_data[4]['tabletype'] = 4
--g_yqsDefine.init_data[4]['bankerjetton'] = 999999999


---------------------------------------------------------

g_yqsDefine.max_catch_fish = 5				-- 一次最多可以捉到5条鱼
g_yqsDefine.max_fish = 35

-- 玩家状态
g_yqsDefine.player_state = {}
g_yqsDefine.player_state.ready = 1			-- 准备,所有刚进入房间的玩家都是这个状态
g_yqsDefine.player_state.playing = 2		-- 游戏中,只要玩家开炮,状态就会改成playing

-- 鱼的路径类型
g_yqsDefine.path_type_small = 0
g_yqsDefine.path_type_big = 1
g_yqsDefine.path_type_huge = 2
g_yqsDefine.path_type_special = 3

g_yqsDefine.path_type_array = 4

g_yqsDefine.path_type_ex1 = 5
g_yqsDefine.path_type_ex2 = 6
g_yqsDefine.path_type_ex3 = 7

g_yqsDefine.path_type_slash =8

g_yqsDefine.path_ex1_max_frame = 1900
g_yqsDefine.path_ex2_max_frame = 1880
--g_yqsDefine.path_ex3_max_frame = 1950
g_yqsDefine.path_ex3_max_frame = 701


g_yqsDefine.path_max_size = {
	[g_yqsDefine.path_type_small] = 208,
	[g_yqsDefine.path_type_big] = 130,
	[g_yqsDefine.path_type_huge] = 62,
	[g_yqsDefine.path_type_special] = 24,
}

g_yqsDefine.path_frame_interval = 0.031	-- 路径上每帧刷新的时间间隔(秒)

-- 炮台类型
g_yqsDefine.cannon_type_xinshou = 0		-- 新手炮
g_yqsDefine.cannon_type_ziyuan = 1      	-- 紫苑炮
g_yqsDefine.cannon_type_hanguang = 2    	-- 寒光炮
g_yqsDefine.cannon_type_haiyan = 3      	-- 海炎炮
g_yqsDefine.cannon_type_shenpan = 4     	-- 审判炮
g_yqsDefine.cannon_type_duyao = 5       	-- 毒药炮
g_yqsDefine.cannon_type_binghun = 6     	-- 冰魂炮
g_yqsDefine.cannon_type_nongyuan = 7    	-- 龙渊炮
g_yqsDefine.cannon_type_anmie = 8       	-- 黯灭炮
g_yqsDefine.cannon_type_nengliang = 9   	-- 能量炮

-- 缺省炮台类型
g_yqsDefine.cannon_type_default = g_yqsDefine.cannon_type_xinshou
-- 缺省炮管类型
g_yqsDefine.cannon_tubenum_default = 2

g_yqsDefine.item_type_freeze = 1			-- 冰冻
g_yqsDefine.item_type_lock = 2				-- 锁定

g_yqsDefine.scene_id_default = 1			-- 默认场景编号

g_yqsDefine.scene_id_max = 7				-- 场景编号最大值

--g_yqsDefine.robot_jetton = {{10000, 200000},{50000, 1000000}, {300000,10000000},{500000, 100000000}}
g_yqsDefine.robot_jetton = {{100, 200000},{1000, 1000000}, {10000,10000000},{60000, 100000000}}

g_yqsDefine.fish_speed_default = 0.5		-- 鱼游走的缺省速度

g_yqsDefine.boss_yao_qian_shu = 46				-- 摇钱树
g_yqsDefine.boss_list = {
	g_yqsDefine.boss_yao_qian_shu
}

-- 鱼群列表
g_yqsDefine.shoal_list = {

    {
        --河豚*皇 和海盗船
		pathType = g_yqsDefine.path_type_ex1,
		speed = 1,
        {cnt = 9, type = 1},
		{cnt = 9, type = 6},
		{cnt = 1, type = 25},
        {cnt = 9, type = 2},
		{cnt = 9, type = 5},
		{cnt = 1, type = 26},

    },
    {
        --美人鱼和章鱼
    	pathType = g_yqsDefine.path_type_ex1,
		speed = 1,
        {cnt = 9, type = 2},
		{cnt = 9, type = 5},
		{cnt = 1, type = 22},
        {cnt = 9, type = 3},
		{cnt = 9, type = 4},
		{cnt = 1, type = 23},
    },
    {
         --金龙 和 金蝉
		pathType = g_yqsDefine.path_type_ex1,
		speed = 1,
        {cnt = 9, type = 1},
		{cnt = 9, type = 4},
		{cnt = 1, type = 24},
        {cnt = 9, type = 2},
		{cnt = 9, type = 3},
		{cnt = 1, type = 33},
    },
    {
        --大鱼[河豚*白] 大鱼[河豚*金] 大鱼[海豚] 大鱼[金龙*白]
        pathType = g_yqsDefine.path_type_ex2,
	    speed = 1,
	    {cnt = 30, type = 1},
        {cnt = 8, type = 1},
        {cnt = 1, type = 17},
        {cnt = 8, type = 1},
        {cnt = 1, type = 18},
        {cnt = 8, type = 1},
        {cnt = 1, type = 21},
        {cnt = 8, type = 1},
        {cnt = 1, type = 20},
    },
    {
		pathType = g_yqsDefine.path_type_ex3,
		speed = 1,
        {cnt = 25, type = 1},
        {cnt = 15, type = 6},
        {cnt = 1, type = 33},
    },
    {
		pathType = g_yqsDefine.path_type_ex3,
		speed = 1,
        {cnt = 25, type = 2},
        {cnt = 15, type = 5},
        {cnt = 1, type = 33},
    },
    {
		pathType = g_yqsDefine.path_type_ex3,
		speed = 1,
        {cnt = 25, type = 3},
        {cnt = 15, type = 4},
        {cnt = 1, type = 33},
    },
}

g_yqsDefine.yqs_create_list = {}
g_yqsDefine.yqs_create_list[1] = {}
g_yqsDefine.yqs_create_list[2] = {}
g_yqsDefine.yqs_create_list[3] = {}

g_yqsDefine.fishCfg_list = {
	{	--小鱼期
		{cnt = 30, fishid = {1,1,1,1,2,2,2,2,3,4,4,6,6,6,6,7,7,7,7,13}},
		{cnt = 30, fishid = {1,3,3,3,4,4,4,5,5,5,6,6,6,8,8,9,9,15}},
		{cnt = 30, fishid = {1,1,1,3,3,4,4,5,5,7,7,7,8,8,9,9,17}},
		{cnt = 30, fishid = {1,1,1,2,2,2,2,5,5,5,6,7,9,9,9,18,19}},
		--前奏
		{cnt = 10, fishid = {1,3,3,3,4,4,4,8,8,11,13,18}},
		{cnt = 10, fishid = {1,1,1,9,9,9,13,15,16,20}},
		{cnt = 10, fishid = {1,2,2,2,5,5,5,9,12,13,21}},
		{cnt = 10, fishid = {1,3,3,4,4,6,6,8,8,10,17,22}},
		--大鱼期
		{cnt = 10, fishid = {1,2,5,5,7,7,8,8,9,11,13,23,24,25}},
		{cnt = 10, fishid = {1,1,2,2,3,6,6,10,10,15,31,26,33}},
		--小鱼期
		{cnt = 25, fishid = {1,3,3,3,4,4,4,5,5,5,7,7,7,8,8,8,9,9,9,14}},
		{cnt = 30, fishid = {1,1,1,2,2,2,3,3,3,5,5,5,6,6,6,7,7,7,9,15}},
		{cnt = 30, fishid = {1,1,1,2,2,2,4,4,4,8,8,8,9,9,9,11,16}},
	},
	{	--小鱼期
		{cnt = 30, fishid = {1,1,1,3,3,3,5,5,5,7,7,7,17}},
		{cnt = 30, fishid = {1,2,2,2,4,4,4,6,6,6,8,8,8,15}},
		{cnt = 30, fishid = {1,1,1,2,2,2,3,3,3,9,9,9,11}},
		{cnt = 30, fishid = {1,5,5,5,6,6,6,7,7,7,8,8,8,13,14,17}},
		--前奏
		{cnt = 10, fishid = {1,4,4,7,7,7,12,13}},
		{cnt = 10, fishid = {1,1,1,2,2,2,9,10,11}},
		{cnt = 10, fishid = {1,3,3,4,4,4,12,13,15}},
		{cnt = 10, fishid = {1,5,5,6,6,8,8,15,16}},
		--大鱼期
		{cnt = 10, fishid = {1,7,7,8,8,9,9,11,17,18,19,20}},
		{cnt = 10, fishid = {1,2,2,3,3,4,4,5,5,13,15,17,26,31,33}},
		--小鱼期
		{cnt = 25, fishid = {1,1,2,2,3,3,5,7,7,7,8,8,8,9,9,9,10,14}},
		{cnt = 30, fishid = {1,3,4,4,4,5,5,6,6,6,7,9,12}},
		{cnt = 30, fishid = {1,1,2,2,2,4,4,7,8,8,10,10,17}},
	},
	{	--小鱼期
		{cnt = 30, fishid = {1,1,1,2,2,2,4,4,4,6,6,6,8,8,10,13}},
		{cnt = 30, fishid = {1,1,1,3,3,3,5,5,5,7,7,7,8,9,9,14}},
		{cnt = 30, fishid = {1,2,2,2,3,3,3,4,4,4,6,6,6,7,7,7,15}},
		{cnt = 10, fishid = {1,1,1,5,5,5,13,15,16}},
		{cnt = 30, fishid = {1,1,1,2,2,2,3,3,3,4,4,4,5,5,6,6,6,17,18,19}},
		{cnt = 30, fishid = {1,2,3,3,3,4,4,4,7,7,8,8,9,9,10,10,13,20,21,22,22,24,25,31}},
		--前奏
		{cnt = 10, fishid = {1,2,2,3,3,6,6,9,9,12,11,16}},
		{cnt = 10, fishid = {1,1,3,3,5,5,7,7,13,14}},
		{cnt = 10, fishid = {1,2,2,4,4,6,6,10,11,15,17}},
		--大鱼期
		{cnt = 10, fishid = {1,2,7,7,8,8,12,13,15,16,17,18,19,20,21,25,26,33}},
	},
}

for i=1,3  do
    local cfg = g_yqsDefine.fishCfg_list[i]
	for _, item in ipairs(cfg) do
		if 0 < #item.fishid then
			for j = 1, item.cnt do
				table.insert(g_yqsDefine.yqs_create_list[i], item.fishid)
			end
		end
	end
end


