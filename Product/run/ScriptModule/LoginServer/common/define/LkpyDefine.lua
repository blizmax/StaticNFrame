g_lkpyDefine = {}

-- 协议编号范围
g_lkpyDefine.game_type = 4700
g_lkpyDefine.proto_id_begin = 4701        -- 起始
g_lkpyDefine.proto_id_end = 4800          -- 结束



-- 桌子号范围
g_lkpyDefine.table_id_begin = 2140000
g_lkpyDefine.table_id_end = 2150000 - 1

-- 桌子人数上限
g_lkpyDefine.max_user = 4

-- 分数类型
g_lkpyDefine.score_type_jetton = 1       	-- 筹码
g_lkpyDefine.score_type_money = 2			-- 钻石

-- 桌子状态
g_lkpyDefine.state_ready = 1
g_lkpyDefine.state_play = 2
g_lkpyDefine.state_tide = 3


-- 时间
g_lkpyDefine.time_ready = 2
g_lkpyDefine.time_play = 60 * 5
g_lkpyDefine.time_tide = 10

----
g_lkpyDefine.fire_interval = 0.2			-- 每次开火的时间间隔(秒)
g_lkpyDefine.ion_need_power = 100			--
g_lkpyDefine.freeze_time = 10				-- 冰冻时间(秒)
g_lkpyDefine.freeze_cool_down = 10			-- 冰冻技能冷却时间
g_lkpyDefine.range_bomb_radius = 400		-- 
g_lkpyDefine.lock_time = 10					-- 锁定技能持续时间(秒)
g_lkpyDefine.lock_cool_down = 10			-- 锁定技能冷却时间(秒)



g_lkpyDefine.table_type = {}           --
g_lkpyDefine.table_type['junior'] = 1	-- 摇钱树
g_lkpyDefine.table_type['medium'] = 2	-- 李逵劈鱼
g_lkpyDefine.table_type['serior'] = 3
g_lkpyDefine.table_type['high'] = 4

g_lkpyDefine.init_data = {}
g_lkpyDefine.init_data[1] = {}
g_lkpyDefine.init_data[2] = {}
g_lkpyDefine.init_data[3] = {}
g_lkpyDefine.init_data[4] = {}

--g_lkpyDefine.init_data[1]['tableid'] = {2141001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lkpyDefine.init_data[1]['maxuser'] = 999
g_lkpyDefine.init_data[1]['bankerid'] = 1
g_lkpyDefine.init_data[1]['bankername'] = '小甜心'
g_lkpyDefine.init_data[1]['bankerfaceid'] = "90001_1001"
g_lkpyDefine.init_data[1]['banklimit'] = 1000000
g_lkpyDefine.init_data[1]['minenter'] = 10000
g_lkpyDefine.init_data[1]['maxenter'] = 1000000
g_lkpyDefine.init_data[1]['tabletype'] = 1
g_lkpyDefine.init_data[1]['bankerjetton'] = 999999999

--g_lkpyDefine.init_data[2]['tableid'] = {2142001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lkpyDefine.init_data[2]['maxuser'] = 999
g_lkpyDefine.init_data[2]['bankerid'] = 1
g_lkpyDefine.init_data[2]['bankername'] = '甜甜圈'
g_lkpyDefine.init_data[2]['bankerfaceid'] = "90002_1001"
g_lkpyDefine.init_data[2]['banklimit'] = 10000000
g_lkpyDefine.init_data[2]['minenter'] = 100000
g_lkpyDefine.init_data[2]['maxenter'] = 10000000
g_lkpyDefine.init_data[2]['tabletype'] = 2
g_lkpyDefine.init_data[2]['bankerjetton'] = 999999999

--g_lkpyDefine.init_data[3]['tableid'] = {2143001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lkpyDefine.init_data[3]['maxuser'] = 999
g_lkpyDefine.init_data[3]['bankerid'] = 1
g_lkpyDefine.init_data[3]['bankername'] = '奈何情深'
g_lkpyDefine.init_data[3]['bankerfaceid'] = "90003_1001"
g_lkpyDefine.init_data[3]['banklimit'] = 100000000
g_lkpyDefine.init_data[3]['minenter'] = 1000000
g_lkpyDefine.init_data[3]['maxenter'] = 100000000
g_lkpyDefine.init_data[3]['tabletype'] = 3
g_lkpyDefine.init_data[3]['bankerjetton'] = 999999999

--g_lkpyDefine.init_data[4]['tableid'] = {2144001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lkpyDefine.init_data[4]['maxuser'] = 999
g_lkpyDefine.init_data[4]['bankerid'] = 1
g_lkpyDefine.init_data[4]['bankername'] = '不服来战'
g_lkpyDefine.init_data[4]['bankerfaceid'] = "90004_1001"
g_lkpyDefine.init_data[4]['banklimit'] = 100000000
g_lkpyDefine.init_data[4]['minenter'] = 3000000
g_lkpyDefine.init_data[4]['maxenter'] = -1
g_lkpyDefine.init_data[4]['tabletype'] = 4
g_lkpyDefine.init_data[4]['bankerjetton'] = 999999999


---------------------------------------------------------

g_lkpyDefine.max_catch_fish = 5				-- 一次最多可以捉到5条鱼
g_lkpyDefine.max_fish = 35

-- 玩家状态
g_lkpyDefine.player_state = {}
g_lkpyDefine.player_state.ready = 1			-- 准备,所有刚进入房间的玩家都是这个状态
g_lkpyDefine.player_state.playing = 2		-- 游戏中,只要玩家开炮,状态就会改成playing

-- 鱼的路径类型
g_lkpyDefine.path_type_small = 0
g_lkpyDefine.path_type_big = 1
g_lkpyDefine.path_type_huge = 2
g_lkpyDefine.path_type_special = 3
g_lkpyDefine.path_type_ex1 = 5
g_lkpyDefine.path_type_ex2 = 6
g_lkpyDefine.path_type_ex3 = 7

g_lkpyDefine.path_ex1_max_frame = 1950
g_lkpyDefine.path_ex2_max_frame = 1950
--g_lkpyDefine.path_ex3_max_frame = 1950
g_lkpyDefine.path_ex3_max_frame = 1000


g_lkpyDefine.path_max_size = {
	[g_lkpyDefine.path_type_small] = 208,
	[g_lkpyDefine.path_type_big] = 130,
	[g_lkpyDefine.path_type_huge] = 62,
	[g_lkpyDefine.path_type_special] = 24,
}

g_lkpyDefine.path_frame_interval = 0.031	-- 路径上每帧刷新的时间间隔(秒)

-- 炮台类型
g_lkpyDefine.cannon_type_xinshou = 0		-- 新手炮
g_lkpyDefine.cannon_type_ziyuan = 1      	-- 紫苑炮
g_lkpyDefine.cannon_type_hanguang = 2    	-- 寒光炮
g_lkpyDefine.cannon_type_haiyan = 3      	-- 海炎炮
g_lkpyDefine.cannon_type_shenpan = 4     	-- 审判炮
g_lkpyDefine.cannon_type_duyao = 5       	-- 毒药炮
g_lkpyDefine.cannon_type_binghun = 6     	-- 冰魂炮
g_lkpyDefine.cannon_type_nongyuan = 7    	-- 龙渊炮
g_lkpyDefine.cannon_type_anmie = 8       	-- 黯灭炮
g_lkpyDefine.cannon_type_nengliang = 9   	-- 能量炮

-- 缺省炮台类型
g_lkpyDefine.cannon_type_default = g_lkpyDefine.cannon_type_xinshou
-- 缺省炮管类型
g_lkpyDefine.cannon_tubenum_default = 2

g_lkpyDefine.item_type_freeze = 1			-- 冰冻
g_lkpyDefine.item_type_lock = 2				-- 锁定

g_lkpyDefine.scene_id_default = 1			-- 默认场景编号

g_lkpyDefine.scene_id_max = 7				-- 场景编号最大值

g_lkpyDefine.robot_jetton = {{10000, 200000},{50000, 1000000}, {300000,10000000},{500000, 100000000}}

g_lkpyDefine.fish_speed_default = 0.25		-- 鱼游走的缺省速度

g_lkpyDefine.boss_li_kui = 20				-- 李逵
g_lkpyDefine.boss_list = {
	g_lkpyDefine.boss_li_kui
}

-- 鱼群列表
g_lkpyDefine.shoal_list = {
	{
		pathType = g_lkpyDefine.path_type_ex1,
		speed = 1,
		{cnt = 30, type = 1},
		{cnt = 20, type = 2},
		{cnt = 10, type = 3},
		{cnt = 1, type = 20},
		{cnt = 30, type = 1},
		{cnt = 20, type = 2},
		{cnt = 10, type = 3},
		{cnt = 1, type = 20},
	},
	{
		pathType = g_lkpyDefine.path_type_ex2,
		speed = 1,
		{cnt = 30, type = 1},
		{cnt = 8, type = 2},
		{cnt = 1, type = 20},
		{cnt = 8, type = 3},
		{cnt = 1, type = 20},
		{cnt = 8, type = 4},
		{cnt = 1, type = 20},
		{cnt = 8, type = 5},
		{cnt = 1, type = 20},
		{cnt = 8, type = 6},
		{cnt = 1, type = 20},
	},
	--[[{
		pathType = g_lkpyDefine.path_type_ex3,
		speed = 1,
		{cnt = 30, type = 6},
		{cnt = 25, type = 2},
		{cnt = 20, type = 3},
		{cnt = 15, type = 4},
		{cnt = 30, type = 6},
		{cnt = 25, type = 2},
		{cnt = 20, type = 3},
		{cnt = 15, type = 4},
		{cnt = 1, type = 20},
		{cnt = 1, type = 20},
	},
--]]
}

