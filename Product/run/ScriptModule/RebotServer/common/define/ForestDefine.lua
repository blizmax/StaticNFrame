
g_forestDefine = {}

g_forestDefine.table_type = {}            --
g_forestDefine.table_type['junior'] = 1
g_forestDefine.table_type['medium'] = 2
g_forestDefine.table_type['serior'] = 3

g_forestDefine.game_type = 3500   --和协议号是同一个号段的

g_forestDefine.colour_list = {10,20,30}  --颜色的列表黄， 绿， 红
g_forestDefine.animal_list = {1,2,3,4}   --狮子，熊猫，兔子，猴子，兔子

--开奖的顺序
--11:黄狮子，21：绿狮子，31：红狮子
--12:黄熊猫，22：绿熊猫，32：红熊猫
--13:黄猴子，23：绿猴子，33：红猴子
--14:黄兔子，24：绿兔子，34：红兔子
--开奖的概率：106>105>104>103>102>101>31>21>11>32>22>12>33>23>13>34>24>14
--106(50) > 105(50)>104(100)>103(150)>102(150)>101(150)>31(200)> 21(200) > 11(400)>32(400)>22(600)>12(600)>33(800)>23(800)>13(1000)>34(1000)>24(1600)>14(1800)

g_forestDefine.index_list = {1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4,1,2,3,4}   --动物列表，狮子，熊猫，兔子，猴子，兔子

--开奖的顺序

g_forestDefine.rand_list = {{11,400},{12,600},{13,1000},{14,1800},{21,200},{22,600},{23,800},{24,1600},{31,200},
							{32,400},{33,800},{34,1000},{101,150},{102,150},{103,150},{104,100},{105,50}}  --


g_forestDefine.all_pour_list = {31,32,33,34,
							21,22,23,24,
							11,12,13,14,
							101,102,103,104   --先去掉彩金
							} --第一为是颜色，第二位是动物

g_forestDefine.pour_list = {31,32,33,34,
							21,22,23,24,
							11,12,13,14
							} --第一为是颜色，第二位是动物


g_forestDefine.pour_all = {11,12,13,14,
							21,22,23,24,
							31,32,33,34,
							11,12,13,14,
							21,22,23,24,
							31,32,33,34} --第一为是颜色，第二位是动物,这里有24个，是因为有24个座位，当需要送灯的时候，这里就会起效果

g_forestDefine.bao_list = {
101,  --翻倍：所中的灯翻2~3倍
102,  --送灯：随机送出2~11门灯
103,  --大三元
104,  --纵横四海
105,  --彩金
106   --鸿运当头
}

g_forestDefine.state_ready = 1
g_forestDefine.state_start = 2
g_forestDefine.state_play = 3
g_forestDefine.state_count = 4
g_forestDefine.state_end = 5

g_forestDefine.time_start = 10
g_forestDefine.time_play = 15
g_forestDefine.time_count = 30
g_forestDefine.time_end = 15

g_forestDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_forestDefine.init_data[1] = {}
g_forestDefine.init_data[2] = {}
g_forestDefine.init_data[3] = {}


g_forestDefine.init_data[1]['tableid'] = {2011001}     --这里用table，防止后面有需要搭建多个同样类型的
g_forestDefine.init_data[1]['maxuser'] = 999
g_forestDefine.init_data[1]['bankerid'] = 1
g_forestDefine.init_data[1]['bankername'] = '熊猫盼盼'
g_forestDefine.init_data[1]['bankernamelist'] = {'熊猫盼盼', '虾扯蛋','Mr陈','小李飞刀','夜狐','请稍后...','小百荷','Tenny','一笑人生','秋巧夏'}
g_forestDefine.init_data[1]['faceid'] = "90001_1001"
g_forestDefine.init_data[1]['banklimit'] = 1000000
g_forestDefine.init_data[1]['enterlimit'] = 0
g_forestDefine.init_data[1]['tabletype'] = 1


g_forestDefine.init_data[2]['tableid'] = {2012001}    --对应的tableid
g_forestDefine.init_data[2]['maxuser'] = 999
g_forestDefine.init_data[2]['bankerid'] = 1
g_forestDefine.init_data[2]['bankername'] = '萌萌兔'
g_forestDefine.init_data[2]['bankernamelist'] = {'萌萌兔','千度','森林小王子','九命猫','梦中蝴蝶','MISSYOU','幸运草','青豆君','回忆不哭','重新~~'}
g_forestDefine.init_data[2]['faceid'] = "90002_1001"
g_forestDefine.init_data[2]['banklimit'] = 1000000
g_forestDefine.init_data[2]['enterlimit'] = 0
g_forestDefine.init_data[2]['tabletype'] = 2

g_forestDefine.init_data[3]['tableid'] = {2013001}    --对应的tableid
g_forestDefine.init_data[3]['maxuser'] = 999
g_forestDefine.init_data[3]['bankerid'] = 1
g_forestDefine.init_data[3]['bankername'] = '森林王者'
g_forestDefine.init_data[3]['bankernamelist'] = {'森林王者','君儿','A+++','雨蝶','南方姑娘','牛达人','旧人往事','低调公主','Flower','xiao燕子','芭比兔'}
g_forestDefine.init_data[3]['faceid'] = "90003_1001"
g_forestDefine.init_data[3]['banklimit'] = 1000000
g_forestDefine.init_data[3]['enterlimit'] = 0
g_forestDefine.init_data[3]['tabletype'] = 3


g_forestDefine.robot_range = { {1000, 5000000}, {1000, 5000000},{1000,5000000},{1000, 5000000},{1000, 5000000},{1000, 5000000}}