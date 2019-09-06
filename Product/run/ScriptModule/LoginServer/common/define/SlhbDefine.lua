--扫雷红包
g_slhbDefine = {}

g_slhbDefine.game_type = 7100   --和协议号是同一个号段的

g_slhbDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_slhbDefine.init_data[1] = {}
g_slhbDefine.init_data[2] = {}
g_slhbDefine.init_data[3] = {}
g_slhbDefine.init_data[4] = {}

g_slhbDefine.init_data[1]['tableid'] = {2261001}     --这里用table，防止后面有需要搭建多个同样类型的
g_slhbDefine.init_data[1]['maxuser'] = 99
g_slhbDefine.init_data[1]['enterlimit'] = 0
g_slhbDefine.init_data[1]['entermax'] = -1
g_slhbDefine.init_data[1]['tabletype'] = 1
g_slhbDefine.init_data[1]['multiple'] = 1.5
g_slhbDefine.init_data[1]['hbnum'] = 7
g_slhbDefine.init_data[1]['hbtype'] = 2				--红包顺序： 1 大包优先 2 顺序埋雷
g_slhbDefine.init_data[1]['hbrange'] = {1000, 20000}--红包额度：10 - 500

g_slhbDefine.init_data[2]['tableid'] = {2262001}     --这里用table，防止后面有需要搭建多个同样类型的
g_slhbDefine.init_data[2]['maxuser'] = 99
g_slhbDefine.init_data[2]['enterlimit'] = 0
g_slhbDefine.init_data[2]['entermax'] = -1
g_slhbDefine.init_data[2]['tabletype'] = 2
g_slhbDefine.init_data[2]['multiple'] = 1
g_slhbDefine.init_data[2]['hbnum'] = 10
g_slhbDefine.init_data[2]['hbtype'] = 2				--红包顺序： 1 大包优先 2 顺序埋雷
g_slhbDefine.init_data[2]['hbrange'] = {1000, 20000}--红包额度：10 - 500

g_slhbDefine.init_data[3]['tableid'] = {2263001}     --这里用table，防止后面有需要搭建多个同样类型的
g_slhbDefine.init_data[3]['maxuser'] = 99
g_slhbDefine.init_data[3]['enterlimit'] = 0
g_slhbDefine.init_data[3]['entermax'] = -1
g_slhbDefine.init_data[3]['tabletype'] = 3
g_slhbDefine.init_data[3]['multiple'] = 1.5
g_slhbDefine.init_data[3]['hbnum'] = 7
g_slhbDefine.init_data[3]['hbtype'] = 1				--红包顺序： 1 大包优先 2 顺序埋雷
g_slhbDefine.init_data[3]['hbrange'] = {1000, 20000}--红包额度：10 - 500

g_slhbDefine.init_data[4]['tableid'] = {2264001}     --这里用table，防止后面有需要搭建多个同样类型的
g_slhbDefine.init_data[4]['maxuser'] = 99
g_slhbDefine.init_data[4]['enterlimit'] = 0
g_slhbDefine.init_data[4]['entermax'] = -1
g_slhbDefine.init_data[4]['tabletype'] = 4
g_slhbDefine.init_data[4]['multiple'] = 1
g_slhbDefine.init_data[4]['hbnum'] = 10
g_slhbDefine.init_data[4]['hbtype'] = 1				--红包顺序： 1 大包优先 2 顺序埋雷
g_slhbDefine.init_data[4]['hbrange'] = {1000, 20000}--红包额度：10 - 500

--游戏状态
g_slhbDefine.state_ready = 1    --在这个状态，检查是否有人埋雷没人埋雷就在这个状态等待
g_slhbDefine.state_start = 2	--开始状态
g_slhbDefine.state_rob = 3   	--抢红包状态
g_slhbDefine.state_count = 4    --结算时间

--状态时间
g_slhbDefine.ready_time = 20	--等待时间
g_slhbDefine.start_time = 1    	--开始时间
g_slhbDefine.rob_time = 7      	--抢红包
g_slhbDefine.count_time = 2		--结束时间

g_slhbDefine.chat_txt = 1
g_slhbDefine.chat_img = 2
g_slhbDefine.chat_voice = 3

g_slhbDefine.game_lockstate = {}
g_slhbDefine.game_lockstate.lock_no = 1
g_slhbDefine.game_lockstate.lock_yes = 2

--N局不行动踢出房间
g_slhbDefine.MaxNoAction = 20

g_slhbDefine.robot_range = { {20000, 800000}, {20000, 800000},{20000,800000},{20000, 800000},{20000, 800000},{20000, 800000}}

