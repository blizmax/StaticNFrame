
g_lhdDefine = {}
g_lhdDefine.game_type = 5300
g_lhdDefine.pour_type = {1,2,3} -- 龙、虎、和

g_lhdDefine.init_sit = 6    --初始化的6个座位

g_lhdDefine.table_common = 777777

g_lhdDefine.page_item = 8   --每一页显示的牌桌的数量

g_lhdcard_type = {}

g_lhdcard_type.teshu = 1    		--特殊牌
g_lhdcard_type.danzhang = 2     	--单张
g_lhdcard_type.duizi = 3     		--对子
g_lhdcard_type.shunzi = 4    		--顺子
g_lhdcard_type.tonghua = 5    		--同花
g_lhdcard_type.tonghuashun = 6   	--同花顺
g_lhdcard_type.baozi = 7     		--豹子

g_lhdArea_type = {}
g_lhdArea_type.long = 1             --龙
g_lhdArea_type.hu   = 2             --虎
g_lhdArea_type.he   = 3				--和

g_lhdArea_type.win_rate = {}
g_lhdArea_type.win_rate[g_lhdArea_type.long] = 1  --2 6.5
g_lhdArea_type.win_rate[g_lhdArea_type.hu]   = 1  --2 6.5
g_lhdArea_type.win_rate[g_lhdArea_type.he]   = 8  --9 1.45

g_lhdArea_type.rand_list = --14.45
{
	{g_lhdArea_type.long,4900},
	{g_lhdArea_type.hu,4900},
	{g_lhdArea_type.he,200}
}

g_lhdDefine.chance_list = {}
g_lhdDefine.chance_list[g_lhdArea_type.long] = 4900
g_lhdDefine.chance_list[g_lhdArea_type.hu] = 4900
g_lhdDefine.chance_list[g_lhdArea_type.he] = 200

g_lhdcard_type.win_rate = {}    --这个是赔率
g_lhdcard_type.win_rate[1] = 16  --和 赔率 1;16

g_lhdDefine.min_bankjetton = 100000    --最小上庄的倍数
g_lhdDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_lhdDefine.min_pour_common = 100  --普通场最小下注
g_lhdDefine.min_pour_grade = 10000  --高级场最小下注
g_lhdDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_lhdDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_lhdDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_lhdDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_lhdDefine.pour2_rate = 10
	g_lhdDefine.pour3_rate = 50
	
	g_lhdDefine.min_pour_common = 100
	g_lhdDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建
--所有牌桌都一样，暂时不区分类型

g_lhdDefine.init_data = {}                 --初始化的数据，一开始初始化六个类型的牌桌，先初始化一个
g_lhdDefine.init_data[1] = {}
g_lhdDefine.init_data[2] = {}
--g_lhdDefine.init_data[3] = {}
--g_lhdDefine.init_data[4] = {}
--g_lhdDefine.init_data[5] = {}
--g_lhdDefine.init_data[6] = {}

g_lhdDefine.init_data[1]['tableid'] = {2181001}     --这里用table，防止后面有需要搭建多个同样类型的
g_lhdDefine.init_data[1]['maxuser'] = 999
g_lhdDefine.init_data[1]['tablename'] = "荣耀厅 01"
g_lhdDefine.init_data[1]['bankerid'] = 1
g_lhdDefine.init_data[1]['bankername'] = '青醉'
g_lhdDefine.init_data[1]['bankernamelist'] =  {'青醉','狂想曲','wx100','江山美人','财神爷','赢他一个亿','发发发','百万梦想'}
g_lhdDefine.init_data[1]['faceid'] = "90001_1001"
g_lhdDefine.init_data[1]['banklimit'] = 200000
g_lhdDefine.init_data[1]['enterlimit'] = 0
g_lhdDefine.init_data[1]['entermax'] = -1
g_lhdDefine.init_data[1]['tabletype'] = 1
g_lhdDefine.init_data[1]['bankjetton'] = 500000
g_lhdDefine.init_data[1]['jiangchibalance'] = 1000 --奖池临界调控值
g_lhdDefine.init_data[1]['userleave'] = 5 --真人多少次没下注离开
g_lhdDefine.init_data[1]['limitred'] = 2000000 --限红


g_lhdDefine.init_data[2]['tableid'] = {2182001}    --对应的tableid
g_lhdDefine.init_data[2]['maxuser'] = 999
g_lhdDefine.init_data[2]['tablename'] = "荣耀厅 02"
g_lhdDefine.init_data[2]['bankerid'] = 1
g_lhdDefine.init_data[2]['bankername'] = '王者、巅峰'
g_lhdDefine.init_data[2]['bankernamelist'] = {'echo','why me','o-o','千年三梦','后悔无期','长生不老','梦想家','来一发'}
g_lhdDefine.init_data[2]['faceid'] = "90002_1001"
g_lhdDefine.init_data[2]['banklimit'] = 200000
g_lhdDefine.init_data[2]['enterlimit'] = 0
g_lhdDefine.init_data[2]['entermax'] = -1
g_lhdDefine.init_data[2]['tabletype'] = 2
g_lhdDefine.init_data[2]['bankjetton'] = 500000
g_lhdDefine.init_data[2]['jiangchibalance'] = 1000 --奖池临界调控值
g_lhdDefine.init_data[2]['userleave'] = 5 --真人多少次没下注离开
g_lhdDefine.init_data[2]['limitred'] = 2000000 --限红

--g_lhdDefine.init_data[3]['tableid'] = {2183001}    --对应的tableid
--g_lhdDefine.init_data[3]['maxuser'] = 999
--g_lhdDefine.init_data[3]['tablename'] = "荣耀厅 03"
----g_lhdDefine.init_data[3]['bankerid'] = 1
--g_lhdDefine.init_data[3]['bankername'] = '心有阳光'
--g_lhdDefine.init_data[3]['bankernamelist'] = {'赌&神','pwd2000','TECH ME','出入江湖','三贱客','可儿','100cc','九姑娘'}
--g_lhdDefine.init_data[3]['faceid'] = "90003_1001"
--g_lhdDefine.init_data[3]['banklimit'] = 200000
--g_lhdDefine.init_data[3]['enterlimit'] = 0
--g_lhdDefine.init_data[3]['entermax'] = -1
--g_lhdDefine.init_data[3]['tabletype'] = 3
--g_lhdDefine.init_data[3]['bankjetton'] = 500000
--g_lhdDefine.init_data[3]['jiangchibalance'] = 1000 --奖池临界调控值
--g_lhdDefine.init_data[3]['userleave'] = 5 --真人多少次没下注离开
--g_lhdDefine.init_data[3]['limitred'] = 2000000 --限红

--g_lhdDefine.init_data[4]['tableid'] = {2184001}    --对应的tableid
--g_lhdDefine.init_data[4]['maxuser'] = 999
--g_lhdDefine.init_data[4]['tablename'] = "荣耀厅 04"
--g_lhdDefine.init_data[4]['bankerid'] = 1
--g_lhdDefine.init_data[4]['bankername'] = '初夏'
--g_lhdDefine.init_data[4]['bankernamelist'] = {'mygod','北京男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_lhdDefine.init_data[4]['faceid'] = "90004_1001"
--g_lhdDefine.init_data[4]['banklimit'] = 200000
--g_lhdDefine.init_data[4]['enterlimit'] = 0
--g_lhdDefine.init_data[4]['entermax'] = -1
--g_lhdDefine.init_data[4]['tabletype'] = 4
--g_lhdDefine.init_data[4]['bankjetton'] = 500000
--g_lhdDefine.init_data[4]['jiangchibalance'] = 1000 --奖池临界调控值
--g_lhdDefine.init_data[4]['userleave'] = 5 --真人多少次没下注离开
--g_lhdDefine.init_data[4]['limitred'] = 2000000 --限红

--g_lhdDefine.init_data[5]['tableid'] = {2185001}    --对应的tableid
--g_lhdDefine.init_data[5]['maxuser'] = 999
--g_lhdDefine.init_data[5]['tablename'] = "荣耀厅 05"
--g_lhdDefine.init_data[5]['bankerid'] = 1
--g_lhdDefine.init_data[5]['bankername'] = '冷锋'
--g_lhdDefine.init_data[5]['bankernamelist'] = {'mygod','广东男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_lhdDefine.init_data[5]['faceid'] = "90005_1001"
--g_lhdDefine.init_data[5]['banklimit'] = 200000
--g_lhdDefine.init_data[5]['enterlimit'] = 0
--g_lhdDefine.init_data[5]['entermax'] = -1
--g_lhdDefine.init_data[5]['tabletype'] = 5
--g_lhdDefine.init_data[5]['bankjetton'] = 5000000
--g_lhdDefine.init_data[5]['jiangchibalance'] = 1000 --奖池临界调控值
--g_lhdDefine.init_data[5]['userleave'] = 5 --真人多少次没下注离开
--g_lhdDefine.init_data[5]['limitred'] = 2000000 --限红

--g_lhdDefine.init_data[6]['tableid'] = {2186001}    --对应的tableid
--g_lhdDefine.init_data[6]['maxuser'] = 999
--g_lhdDefine.init_data[6]['tablename'] = "荣耀厅 06"
--g_lhdDefine.init_data[6]['bankerid'] = 1
--g_lhdDefine.init_data[6]['bankername'] = '热血'
--g_lhdDefine.init_data[6]['bankernamelist'] = {'mygod','广西男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_lhdDefine.init_data[6]['faceid'] = "90006_1001"
--g_lhdDefine.init_data[6]['banklimit'] = 200000
--g_lhdDefine.init_data[6]['enterlimit'] = 5000
--g_lhdDefine.init_data[6]['entermax'] = -1
--g_lhdDefine.init_data[6]['tabletype'] = 6
--g_lhdDefine.init_data[6]['bankjetton'] = 5000000
--g_lhdDefine.init_data[6]['jiangchibalance'] = 1000 --奖池临界调控值
--g_lhdDefine.init_data[6]['userleave'] = 5 --真人多少次没下注离开
--g_lhdDefine.init_data[6]['limitred'] = 2000000 --限红


g_lhdDefine.table_type = {}    --
g_lhdDefine.table_type['junior'] = 1
g_lhdDefine.table_type['medium'] = 2
g_lhdDefine.table_type['serior'] = 3
g_lhdDefine.table_type['high'] = 4
g_lhdDefine.table_type['higher'] = 5
g_lhdDefine.table_type['highest'] = 6


g_lhdDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_lhdDefine.state_start = 2
g_lhdDefine.state_perflop = 3   --摇豆子时间
g_lhdDefine.state_pour = 4      --下注的时间
g_lhdDefine.state_count = 5      --结算时间
g_lhdDefine.state_end = 6      --结束的状态


g_lhdDefine.perflop_time = 3   --开始的时候，摇杯子的时间
g_lhdDefine.pour_time = 15      --下注的时间
g_lhdDefine.count_time = 10

g_lhdDefine.chat_txt = 1
g_lhdDefine.chat_img = 2
g_lhdDefine.chat_voice = 3


g_lhdDefine.type_ydc = 1       --欢乐赢
g_lhdDefine.type_lxc = 2

g_lhdDefine.leave_dismiss = 1   --房主解散房间
g_lhdDefine.leave_timeout = 2   --五轮没下注被踢出

g_lhdDefine.quit_count  = 10     --几轮没下注后就被踢出

g_lhdDefine.time_public = 8  --如果是大众长，就有20秒的掉线时间
g_lhdDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_lhdDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励





g_lhdDefine.robot_range = {}
g_lhdDefine.robot_range[1] = {5000,1000000}
g_lhdDefine.robot_range[2] = {5000,1000000}
g_lhdDefine.robot_range[3] = {5000,1000000}
g_lhdDefine.robot_range[4] = {5000,1000000}
g_lhdDefine.robot_range[5] = {5000,1000000}
g_lhdDefine.robot_range[6] = {5000,1000000}

g_lhdDefine.five_repeated_win_reward = 8800
g_lhdDefine.ten_repeated_win_reward = 88800
g_lhdDefine.twenty_repeated_win_reward = 88800