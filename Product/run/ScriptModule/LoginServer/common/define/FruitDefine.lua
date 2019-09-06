
g_fruitDefine = {}

g_fruitDefine.table_type = {}           --
g_fruitDefine.table_type['junior'] = 1
g_fruitDefine.table_type['medium'] = 2
g_fruitDefine.table_type['serior'] = 3
g_fruitDefine.table_type['high'] = 4

g_fruitDefine.game_type = 3600   --和协议号是同一个号段的

g_fruitDefine.colour_list = {10,20,30}  --  颜色的列表黄， 绿，红
g_fruitDefine.animal_list = {1,2,3,4}   --动物列表，兔子，猴子，熊猫，狮子

--开奖的顺序

g_fruitDefine.rand_list = {{11,1000},{12,720},{21,840},{22,450},{31,840},{32,450},{41,840},{42,400},{51,850},{52,220},
							{61,850},{62,220},{71,850},{81,100},{72,220},{82,50},
						   {101,200},{102,150},{103,100},{104,200},{105,50},{106,100},{107,100},{108,100},{109,100}} --{110,0}}  --110的闪电先不用,把这个50的概率加到前面

g_fruitDefine.fruit_list = {1,2,3,4,5,6,7,8,9}   --苹果，橙子，哈密瓜，铃铛，西瓜，星星，77，bar,goodluck

g_fruitDefine.pour_real = {1,2,3,4,5,6,7,8}   --这8个是有倍数的，9是没有倍数的



--g_fruitDefine.all_pour_list = {11,12,21,22,31,32,41,42,51,52,61,62,71,72,81,82,91,92, 101,102,103,104,105,106,107,108,109,110}
g_fruitDefine.all_pour_list = {11,12,21,22,31,32,41,42,51,52,61,62,71,72}
g_fruitDefine.bao_list = {
101,  --小四喜
102,  --小三元
103,  --大三元
104,  --小满贯
105,  --大满贯
106,  --开火车
107,  --小猫变身
108,  --送灯
109,  --庄家通杀
110}  --闪电

g_fruitDefine.bei_list = {{2,2,2,2,2,2,2,50},{5,10,10,10,20,20,20,100}}
	
g_fruitDefine.pour_list = {41,22, 42,82,81,12,11,32,
							52,51,91,12,
							21,22,42,71,72,12,31,32,
							62,61,92,12}   --第一为是类别，第二位是倍数

g_fruitDefine.animal_list = {1,2,3,4}   --动物列表，兔子，猴子，熊猫，狮子

g_fruitDefine.state_ready = 1
g_fruitDefine.state_start = 2
g_fruitDefine.state_play = 3
g_fruitDefine.state_count = 4
g_fruitDefine.state_end = 5


g_fruitDefine.time_start = 10
g_fruitDefine.time_play = 15
g_fruitDefine.time_count = 30
g_fruitDefine.time_end = 15   --设为12秒看看


g_fruitDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_fruitDefine.init_data[1] = {}
g_fruitDefine.init_data[2] = {}
g_fruitDefine.init_data[3] = {}
g_fruitDefine.init_data[4] = {}

g_fruitDefine.init_data[1]['tableid'] = {2021001}     --这里用table，防止后面有需要搭建多个同样类型的
g_fruitDefine.init_data[1]['maxuser'] = 999
g_fruitDefine.init_data[1]['bankerid'] = 1
g_fruitDefine.init_data[1]['bankername'] = '放学别走'
g_fruitDefine.init_data[1]['bankernamelist'] = {'放学别走','泡沫之夏','no matter','陌上花开','①生只爱你①人','酒醉三分醒','资本家','以往情深','天天男神','美人如画','Hickey','Hollow '}
g_fruitDefine.init_data[1]['faceid'] = "90001_1001"
g_fruitDefine.init_data[1]['banklimit'] = 1000000
g_fruitDefine.init_data[1]['enterlimit'] = 1000
g_fruitDefine.init_data[1]['entermax'] = 2000000
g_fruitDefine.init_data[1]['tabletype'] = 1
g_fruitDefine.init_data[1]['bankjetton'] = 1000000

g_fruitDefine.init_data[2]['tableid'] = {2022001}    --对应的tableid
g_fruitDefine.init_data[2]['maxuser'] = 999
g_fruitDefine.init_data[2]['bankerid'] = 1
g_fruitDefine.init_data[2]['bankername'] = '乖乖兔'
g_fruitDefine.init_data[2]['bankernamelist'] = {'乖乖兔','瘋子范','岁月如初','The Only','完美N+1','痴心少年','彩虹的天堂','步非+烟花','朝花又夕拾','小时光','红酒配美人','性别㈠朵花','理想大于天'}
g_fruitDefine.init_data[2]['faceid'] = "90002_1001"
g_fruitDefine.init_data[2]['banklimit'] = 10000000
g_fruitDefine.init_data[2]['enterlimit'] = 10000
g_fruitDefine.init_data[2]['entermax'] = 20000000
g_fruitDefine.init_data[2]['tabletype'] = 2
g_fruitDefine.init_data[2]['bankjetton'] = 10000000

g_fruitDefine.init_data[3]['tableid'] = {2023001}    --对应的tableid
g_fruitDefine.init_data[3]['maxuser'] = 999
g_fruitDefine.init_data[3]['bankerid'] = 1
g_fruitDefine.init_data[3]['bankername'] = '念心'
g_fruitDefine.init_data[3]['bankernamelist'] = {'念心','相忘于江湖','没本事惹我','空手沾香','Love forever','王者之路','Love yesterday','岁月无声','删除回忆录','雨中的彩虹','知足常乐','劳斯莱斯'}
g_fruitDefine.init_data[3]['faceid'] = "90003_1001"
g_fruitDefine.init_data[3]['banklimit'] = 50000000
g_fruitDefine.init_data[3]['enterlimit'] = 100000
g_fruitDefine.init_data[3]['entermax'] = 100000000
g_fruitDefine.init_data[3]['tabletype'] = 3
g_fruitDefine.init_data[3]['bankjetton'] = 50000000

g_fruitDefine.init_data[4]['tableid'] = {2024001}    --对应的tableid
g_fruitDefine.init_data[4]['maxuser'] = 999
g_fruitDefine.init_data[4]['bankerid'] = 1
g_fruitDefine.init_data[4]['bankername'] = '取名字很难'
g_fruitDefine.init_data[4]['bankernamelist'] = {'空心者','取名字很难','天黑黑','似花非花','一生的唯一','爱wo别走','XO-Thunder','一無所有','删除回忆录','眉心画','Rain Sounds','雪舞兮'}
g_fruitDefine.init_data[4]['faceid'] = "90004_1001"
g_fruitDefine.init_data[4]['banklimit'] = 100000000
g_fruitDefine.init_data[4]['enterlimit'] = 1000000
g_fruitDefine.init_data[4]['entermax'] = -1
g_fruitDefine.init_data[4]['tabletype'] = 4
g_fruitDefine.init_data[4]['bankjetton'] = 100000000


g_fruitDefine.remainpourjetton = 10     	--最大下注与庄家身上筹码的比例

g_fruitDefine.robot_range = {}
g_fruitDefine.robot_range[1] = {10000,5000000}
g_fruitDefine.robot_range[2] = {10000,5000000}
g_fruitDefine.robot_range[3] = {10000,5000000}
g_fruitDefine.robot_range[4] = {10000,5000000}
g_fruitDefine.robot_range[5] = {10000,5000000}
g_fruitDefine.robot_range[6] = {10000,5000000}

