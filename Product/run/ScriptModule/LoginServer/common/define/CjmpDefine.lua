
g_cjmpDefine = {}
g_cjmpDefine.game_type = 5400
g_cjmpDefine.pour_type = {1,2,3,4,5} -- 黑桃、红心、草花，方块，大小王

g_cjmpDefine.roundtimes = 60*60*24 --多久轮数清零 24小时

g_cjmpDefine.init_sit = 6    --初始化的6个座位

g_cjmpDefine.table_common = 777777

g_cjmpDefine.page_item = 8   --每一页显示的牌桌的数量

g_cjmpcard_type = {}

g_cjmpcard_type.teshu = 1    		--特殊牌
g_cjmpcard_type.danzhang = 2     	--单张
g_cjmpcard_type.duizi = 3     		--对子
g_cjmpcard_type.shunzi = 4    		--顺子
g_cjmpcard_type.tonghua = 5    		--同花
g_cjmpcard_type.tonghuashun = 6   	--同花顺
g_cjmpcard_type.baozi = 7     		--豹子


g_cjmpcard_type.win_rate = {}       --这个是赔率
g_cjmpcard_type.win_rate[1] = 4   --黑桃
g_cjmpcard_type.win_rate[2] = 4   --红心
g_cjmpcard_type.win_rate[3] = 3.8   --草花
g_cjmpcard_type.win_rate[4] = 3.8  --方块
g_cjmpcard_type.win_rate[5] = 20.0  --大小王   



g_cjmpDefine.min_bankjetton = 100000    --最小上庄的倍数
g_cjmpDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_cjmpDefine.min_pour_common = 100  --普通场最小下注
g_cjmpDefine.min_pour_grade = 10000  --高级场最小下注
g_cjmpDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_cjmpDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_cjmpDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_cjmpDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_cjmpDefine.pour2_rate = 10
	g_cjmpDefine.pour3_rate = 50
	
	g_cjmpDefine.min_pour_common = 100
	g_cjmpDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建
--所有牌桌都一样，暂时不区分类型

g_cjmpDefine.init_data = {}                 --初始化的数据，一开始初始化六个类型的牌桌，先初始化一个
g_cjmpDefine.init_data[1] = {}
g_cjmpDefine.init_data[2] = {}
--g_cjmpDefine.init_data[3] = {}
--g_cjmpDefine.init_data[4] = {}


g_cjmpDefine.init_data[1]['tableid'] = {2191001}     --这里用table，防止后面有需要搭建多个同样类型的
g_cjmpDefine.init_data[1]['maxuser'] = 999
g_cjmpDefine.init_data[1]['tablename'] = "荣耀厅 01"
g_cjmpDefine.init_data[1]['bankerid'] = 1
g_cjmpDefine.init_data[1]['bankername'] = '青醉'
g_cjmpDefine.init_data[1]['bankernamelist'] = {'青醉','狂想曲','wx100','江山美人','财神爷','赢他一个亿','发发发','百万梦想'}
g_cjmpDefine.init_data[1]['faceid'] = "90001_1001"
g_cjmpDefine.init_data[1]['banklimit'] = 500000
g_cjmpDefine.init_data[1]['enterlimit'] = 5000
g_cjmpDefine.init_data[1]['entermax'] = -1
g_cjmpDefine.init_data[1]['tabletype'] = 1
g_cjmpDefine.init_data[1]['bankjetton'] = 10000000
g_cjmpDefine.init_data[1]['jiangchibalance'] = 1000 --奖池临界调控值
g_cjmpDefine.init_data[1]['userleave'] = 30 --真人多少次没下注离开
g_cjmpDefine.init_data[1]['limitred'] = 2000000 --限红



g_cjmpDefine.init_data[2]['tableid'] = {2192001}    --对应的tableid
g_cjmpDefine.init_data[2]['maxuser'] = 999
g_cjmpDefine.init_data[2]['tablename'] = "荣耀厅 02"
g_cjmpDefine.init_data[2]['bankerid'] = 1
g_cjmpDefine.init_data[2]['bankername'] = '王者、巅峰'
g_cjmpDefine.init_data[2]['bankernamelist'] = {'echo','why me','o-o','千年三梦','后悔无期','长生不老','梦想家','来一发'}
g_cjmpDefine.init_data[2]['faceid'] = "90002_1001"
g_cjmpDefine.init_data[2]['banklimit'] = 500000
g_cjmpDefine.init_data[2]['enterlimit'] = 5000
g_cjmpDefine.init_data[2]['entermax'] = -1
g_cjmpDefine.init_data[2]['tabletype'] = 2
g_cjmpDefine.init_data[2]['bankjetton'] = 10000000
g_cjmpDefine.init_data[2]['jiangchibalance'] = 1000 --奖池临界调控值
g_cjmpDefine.init_data[2]['userleave'] = 30 --真人多少次没下注离开
g_cjmpDefine.init_data[2]['limitred'] = 2000000 --限红

--g_cjmpDefine.init_data[3]['tableid'] = {2193001}    --对应的tableid
--g_cjmpDefine.init_data[3]['maxuser'] = 999
--g_cjmpDefine.init_data[3]['tablename'] = "荣耀厅 03"
--g_cjmpDefine.init_data[3]['bankerid'] = 1
--g_cjmpDefine.init_data[3]['bankername'] = '心有阳光'
--g_cjmpDefine.init_data[3]['bankernamelist'] = {'赌&神','pwd2000','TECH ME','出入江湖','三贱客','可儿','100cc','九姑娘'}
--g_cjmpDefine.init_data[3]['faceid'] = "90003_1001"
--g_cjmpDefine.init_data[3]['banklimit'] = 500000
--g_cjmpDefine.init_data[3]['enterlimit'] = 5000
--g_cjmpDefine.init_data[3]['entermax'] = -1
--g_cjmpDefine.init_data[3]['tabletype'] = 3
--g_cjmpDefine.init_data[3]['bankjetton'] = 10000000
--g_cjmpDefine.init_data[3]['jiangchibalance'] = 1000 --奖池临界调控值
--g_cjmpDefine.init_data[3]['userleave'] = 30 --真人多少次没下注离开
--g_cjmpDefine.init_data[3]['limitred'] = 2000000 --限红

--g_cjmpDefine.init_data[4]['tableid'] = {2194001}    --对应的tableid
--g_cjmpDefine.init_data[4]['maxuser'] = 999
--g_cjmpDefine.init_data[4]['tablename'] = "荣耀厅 04"
--g_cjmpDefine.init_data[4]['bankerid'] = 1
--g_cjmpDefine.init_data[4]['bankername'] = '初夏'
--g_cjmpDefine.init_data[4]['bankernamelist'] = {'mygod','北京男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_cjmpDefine.init_data[4]['faceid'] = "90004_1001"
--g_cjmpDefine.init_data[4]['banklimit'] = 500000
--g_cjmpDefine.init_data[4]['enterlimit'] = 5000
--g_cjmpDefine.init_data[4]['entermax'] = -1
--g_cjmpDefine.init_data[4]['tabletype'] = 4
--g_cjmpDefine.init_data[4]['bankjetton'] = 10000000
--g_cjmpDefine.init_data[4]['jiangchibalance'] = 1000 --奖池临界调控值
--g_cjmpDefine.init_data[4]['userleave'] = 30 --真人多少次没下注离开
--g_cjmpDefine.init_data[4]['limitred'] = 2000000 --限红




g_cjmpDefine.table_type = {}    --
g_cjmpDefine.table_type['junior'] = 1
g_cjmpDefine.table_type['medium'] = 2
g_cjmpDefine.table_type['serior'] = 3
g_cjmpDefine.table_type['high'] = 4
g_cjmpDefine.table_type['higher'] = 5
g_cjmpDefine.table_type['highest'] = 6


g_cjmpDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_cjmpDefine.state_start = 2
g_cjmpDefine.state_perflop = 3   --摇豆子时间
g_cjmpDefine.state_pour = 4      --下注的时间
g_cjmpDefine.state_count = 5      --结算时间
g_cjmpDefine.state_end = 6      --结束的状态


g_cjmpDefine.perflop_time = 3   --开始的时候，摇杯子的时间
g_cjmpDefine.pour_time = 15      --下注的时间
g_cjmpDefine.count_time = 12

g_cjmpDefine.chat_txt = 1
g_cjmpDefine.chat_img = 2
g_cjmpDefine.chat_voice = 3


g_cjmpDefine.type_ydc = 1       --欢乐赢
g_cjmpDefine.type_lxc = 2

g_cjmpDefine.leave_dismiss = 1   --房主解散房间
g_cjmpDefine.leave_timeout = 2   --五轮没下注被踢出

g_cjmpDefine.quit_count  = 10     --几轮没下注后就被踢出

g_cjmpDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_cjmpDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_cjmpDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励





g_cjmpDefine.robot_range = {}
g_cjmpDefine.robot_range[1] = {10000,1000000}
g_cjmpDefine.robot_range[2] = {100000,5000000}
g_cjmpDefine.robot_range[3] = {300000,10000000}
g_cjmpDefine.robot_range[4] = {500000,50000000}
