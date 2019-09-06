g_lhdbDefine = {}

g_lhdbDefine.game_type = 4500
g_lhdbDefine.table_type = {}           --
g_lhdbDefine.table_type['junior'] = 1
g_lhdbDefine.table_type['medium'] = 2
g_lhdbDefine.table_type['serior'] = 3

g_lhdbDefine.init_data = {}
g_lhdbDefine.init_data[1] = {}
g_lhdbDefine.init_data[2] = {}
g_lhdbDefine.init_data[3] = {}
g_lhdbDefine.init_data[4] = {}

g_lhdbDefine.init_data[1]['tableid'] = {2121001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lhdbDefine.init_data[1]['maxuser'] = 999
g_lhdbDefine.init_data[1]['bankerid'] = 1
g_lhdbDefine.init_data[1]['bankername'] = '琥珀'
g_lhdbDefine.init_data[1]['faceid'] = "90001_1001"
g_lhdbDefine.init_data[1]['banklimit'] = 1000000
g_lhdbDefine.init_data[1]['enterlimit'] = 10000
g_lhdbDefine.init_data[1]['tabletype'] = 1
g_lhdbDefine.init_data[1]['bankjetton'] = 999999999

g_lhdbDefine.init_data[2]['tableid'] = {2122001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lhdbDefine.init_data[2]['maxuser'] = 999
g_lhdbDefine.init_data[2]['bankerid'] = 1
g_lhdbDefine.init_data[2]['bankername'] = '珍珠'
g_lhdbDefine.init_data[2]['faceid'] = "90002_1001"
g_lhdbDefine.init_data[2]['banklimit'] = 10000000
g_lhdbDefine.init_data[2]['enterlimit'] = 100000
g_lhdbDefine.init_data[2]['tabletype'] = 2
g_lhdbDefine.init_data[2]['bankjetton'] = 999999999

g_lhdbDefine.init_data[3]['tableid'] = {2123001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lhdbDefine.init_data[3]['maxuser'] = 999
g_lhdbDefine.init_data[3]['bankerid'] = 1
g_lhdbDefine.init_data[3]['bankername'] = '钻石'
g_lhdbDefine.init_data[3]['faceid'] = "90003_1001"
g_lhdbDefine.init_data[3]['banklimit'] = 100000000
g_lhdbDefine.init_data[3]['enterlimit'] = 1000000
g_lhdbDefine.init_data[3]['tabletype'] = 3
g_lhdbDefine.init_data[3]['bankjetton'] = 999999999

g_lhdbDefine.init_data[4]['tableid'] = {2124001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lhdbDefine.init_data[4]['maxuser'] = 999
g_lhdbDefine.init_data[4]['bankerid'] = 1
g_lhdbDefine.init_data[4]['bankername'] = '夺宝'
g_lhdbDefine.init_data[4]['faceid'] = "90004_1001"
g_lhdbDefine.init_data[4]['banklimit'] = 100000000
g_lhdbDefine.init_data[4]['enterlimit'] = 1000000
g_lhdbDefine.init_data[4]['tabletype'] = 4
g_lhdbDefine.init_data[4]['bankjetton'] = 999999999


g_lhdbDefine.state_play = 1

g_lhdbDefine.time_play = 10

g_lhdbDefine.round_one = 1
g_lhdbDefine.round_two = 2
g_lhdbDefine.round_three = 3
g_lhdbDefine.round_four = 4

g_lhdbDefineGem = {}
g_lhdbDefineGem.invalid = 0		-- 无效
g_lhdbDefineGem.bit = 1			-- 钻头
g_lhdbDefineGem.round = {
	[g_lhdbDefine.round_one] = {2, 3, 4, 5, 6},		-- 第一关使用的宝石列表
	[g_lhdbDefine.round_two] = {7, 8, 9, 10, 11},	-- 第二关使用的宝石列表
	[g_lhdbDefine.round_three] = {12, 13, 14, 15, 16}	-- 第三关使用的宝石列表
}

g_lhdbDefine.link_length = {
	[g_lhdbDefine.round_one] = 4,
	[g_lhdbDefine.round_two] = 5,
	[g_lhdbDefine.round_three] = 6,
}

g_lhdbDefine.board_length = {
	[g_lhdbDefine.round_one] = 15,
	[g_lhdbDefine.round_two] = 15,
	[g_lhdbDefine.round_three] = 15,
}

-- 宝石赔率
-- 说明
-- gem:宝石
-- startGem: 那一局的起始宝石 例如第一局是g_lhdbDefineGem.round[g_lhdbDefine.round_one][1]
-- len: 相连多少个
-- startLen: 相连多少个起 例如第一局是 g_lhdbDefine.link_length[g_lhdbDefine.round_one]
-- g_lhdbDefine.gem_mul[gem - startGem + 1][len - startLen + 1]
g_lhdbDefine.gem_mul = {}
g_lhdbDefine.gem_mul[1] = {0.2, 0.4, 0.5, 0.8, 1, 2, 3, 5, 10, 20, 50}
g_lhdbDefine.gem_mul[2] = {0.4, 0.5, 1, 2, 3, 5, 10, 20, 50, 75, 120}
g_lhdbDefine.gem_mul[3] = {0.5, 1, 2, 4, 8, 16, 50, 100, 200, 500, 800}
g_lhdbDefine.gem_mul[4] = {1, 3, 5, 6, 10, 75, 100, 1000, 2000, 5000, 8000}
g_lhdbDefine.gem_mul[5] = {2, 5, 10, 50, 100, 200, 500, 2000, 5000, 10000, 10000}

-- 第四关 有五个宝箱 各个宝箱的金额等于本场游戏得分乘以以下倍数
g_lhdbDefine.round_four_prize_mul = {1.2, 1.1, 1, 0.9, 0.8}

g_lhdbDefine.round_four_limit_score = 10	-- 第四关 当玩家总积分少于此值时时使用此值计算宝箱分数

g_lhdbDefine.save_data_days = 30		-- 保存数据多少天