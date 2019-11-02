
g_fenfencaiDefine = {}
g_fenfencaiDefine.game_type = 7200
g_fenfencaiDefine.pour_type = {1,2,3, 4, 5, 6, 7} -- 龙、虎、和、大、小、单、双
g_fenfencaiDefine.awardresult = {"龙", "虎", "和", "大", "小", "单", "双"}

g_fenfencaiDefine.game_lockstate = {}
g_fenfencaiDefine.game_lockstate.lock_no = 1
g_fenfencaiDefine.game_lockstate.lock_yes = 2

g_fenfencaiDefine.init_sit = 6    --初始化的6个座位

g_fenfencaiDefine.win_rate = {}    --这个是赔率
g_fenfencaiDefine.win_rate[1] = 2  --龙 赔率 1:1
g_fenfencaiDefine.win_rate[2] = 2  --虎 赔率 1:1
g_fenfencaiDefine.win_rate[3] = 10  --和 赔率 1:9
g_fenfencaiDefine.win_rate[4] = 2  --大 赔率 1:1
g_fenfencaiDefine.win_rate[5] = 2  --小 赔率 1:1
g_fenfencaiDefine.win_rate[6] = 2  --单 赔率 1:1
g_fenfencaiDefine.win_rate[7] = 2  --单 赔率 1:1


g_fenfencaiDefine.init_data = {}                 --初始化的数据，一开始初始化六个类型的牌桌，先初始化一个
g_fenfencaiDefine.init_data[1] = {}
--g_fenfencaiDefine.init_data[2] = {}

g_fenfencaiDefine.init_data[1]['tableid'] = {2271001}     --这里用table，防止后面有需要搭建多个同样类型的
g_fenfencaiDefine.init_data[1]['maxuser'] = 99
g_fenfencaiDefine.init_data[1]['tablename'] = "上庄场"
g_fenfencaiDefine.init_data[1]['bankerid'] = 1
g_fenfencaiDefine.init_data[1]['bankername'] = '追忆'
g_fenfencaiDefine.init_data[1]['bankernamelist'] = {'追忆','陈氏太极','枕头菌','KUKU','爱上好心情','那些年','男人本色','CoCol','陌路人','随遇而安','至尊宝','玩家101'}
g_fenfencaiDefine.init_data[1]['faceid'] = "90001_1001"
g_fenfencaiDefine.init_data[1]['banklimit'] = 500000
g_fenfencaiDefine.init_data[1]['enterlimit'] = 0
g_fenfencaiDefine.init_data[1]['entermax'] = -1
g_fenfencaiDefine.init_data[1]['tabletype'] = 1 	--上庄场
g_fenfencaiDefine.init_data[1]['bankjetton'] = 800000		

--[[
g_fenfencaiDefine.init_data[2]['tableid'] = {2271002}     --这里用table，防止后面有需要搭建多个同样类型的
g_fenfencaiDefine.init_data[2]['maxuser'] = 99
g_fenfencaiDefine.init_data[2]['tablename'] = "选球场"
g_fenfencaiDefine.init_data[2]['bankerid'] = 1
g_fenfencaiDefine.init_data[2]['bankername'] = '系统庄'
g_fenfencaiDefine.init_data[2]['bankernamelist'] = {'系统庄','系统庄2','系统庄3','系统庄4','系统庄5','系统庄6','系统庄7','系统庄8'}
g_fenfencaiDefine.init_data[2]['faceid'] = "90001_1001"
g_fenfencaiDefine.init_data[2]['banklimit'] = 300000
g_fenfencaiDefine.init_data[2]['enterlimit'] = 0
g_fenfencaiDefine.init_data[2]['entermax'] = -1
g_fenfencaiDefine.init_data[2]['tabletype'] = 2 	--2选球场
g_fenfencaiDefine.init_data[2]['bankjetton'] = 88888888		
]]
g_fenfencaiDefine.state_ready = 1    	--
g_fenfencaiDefine.state_start = 2		--开始状态
g_fenfencaiDefine.state_pour = 3      	--下注状态
g_fenfencaiDefine.state_getresult = 4 	--获取结果状态
g_fenfencaiDefine.state_count = 5     	--结算状态

g_fenfencaiDefine.start_time = 0   	--开始时间
g_fenfencaiDefine.pour_time = 37     	--下注时间
g_fenfencaiDefine.getresult_time = 5   	--获取结果时间
g_fenfencaiDefine.count_time = 10		--结算时间

g_fenfencaiDefine.chat_txt = 1
g_fenfencaiDefine.chat_img = 2
g_fenfencaiDefine.chat_voice = 3

g_fenfencaiDefine.robot_range = {}
g_fenfencaiDefine.robot_range[1] = {5000,1000000}
g_fenfencaiDefine.robot_range[2] = {5000,1000000}

g_fenfencaiDefine.area_rand_list = {
        {1,4,6},
        {1,4,7},
        {1,5,6},
        {1,5,7},

        {2,4,6},
        {2,4,7},
        {2,5,6},
        {2,5,7},

        {3,4,6},
        {3,4,7},
        {3,5,6},
        {3,5,7}
}

g_fenfencaiDefine.chance_list = 
{
     4706,
     4706,
     4706,
     4706,

     4706,
     4706,
     4706,
     4706,

     588,
     588,
     588,
     588
}