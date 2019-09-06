
g_barccatatDefine = {}

--牌桌状态
g_barccatatDefine.game_type = 4400
g_barccatatDefine.state_ready = 1
g_barccatatDefine.state_start = 2
g_barccatatDefine.state_pour = 3
g_barccatatDefine.state_sendcard = 4
g_barccatatDefine.state_count = 5
g_barccatatDefine.state_nxet = 6
g_barccatatDefine.state_delete = 7

--状态运行时间
g_barccatatDefine.time_pour = 12
g_barccatatDefine.time_sendcard  = 6
g_barccatatDefine.time_count = 3
g_barccatatDefine.time_nxet = 3


--下注区域赔率
g_barccatatDefine.lines = {}
g_barccatatDefine.lines[1] = 1    	--庄
g_barccatatDefine.lines[2] = 1    		--闲
g_barccatatDefine.lines[3] = 8    		--和
g_barccatatDefine.lines[4] = 11    		--庄对
g_barccatatDefine.lines[5] = 11    		--闲对

--押注区域 1 庄  2 闲  3 和  4 庄对 5 闲对
g_barccatatDefine.pour_type = {1,2,3,4,5}

--房间类型
g_barccatatDefine.table_type = {}           --
g_barccatatDefine.table_type['junior'] = 1
g_barccatatDefine.table_type['medium'] = 2
g_barccatatDefine.table_type['serior'] = 3
g_barccatatDefine.table_type['high'] = 4

 --初始化的8个座位
g_barccatatDefine.init_sit = 6     

g_barccatatDefine.min_pour_common = 100  --普通场最小下注
g_barccatatDefine.min_pour_grade = 10000  --高级场最小下注
g_barccatatDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_barccatatDefine.pour3_rate = 100        --第三个和第一个的倍数



--牌定义
g_barccatatDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}


--最小上庄分数
g_barccatatDefine.minBeingBankerJetton = {}
g_barccatatDefine.minBeingBankerJetton[1] = 100000000
g_barccatatDefine.minBeingBankerJetton[2] = 100000000
g_barccatatDefine.minBeingBankerJetton[3] = 100000000



--踢下庄家的分数
g_barccatatDefine.minXiaZhuanJetton = {}
g_barccatatDefine.minXiaZhuanJetton[1] = 1000000
g_barccatatDefine.minXiaZhuanJetton[2] = 1000000
g_barccatatDefine.minXiaZhuanJetton[3] = 1000000



g_barccatatDefine.table_common = 777777



g_barccatatDefine.page_item = 8   --每一页显示的牌桌的数量
g_barccatatDefine.min_bankjetton = 100000    --最小上庄的倍数
g_barccatatDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数




if g_servername == "run_tyyd" then
	g_barccatatDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_barccatatDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_barccatatDefine.pour2_rate = 10
	g_barccatatDefine.pour3_rate = 50
	
	g_barccatatDefine.min_pour_common = 100
	g_barccatatDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建

g_barccatatDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_barccatatDefine.init_data[1] = {}
--g_barccatatDefine.init_data[2] = {}
--g_barccatatDefine.init_data[3] = {}
--g_barccatatDefine.init_data[4] = {}
--g_barccatatDefine.init_data[5] = {}
--g_barccatatDefine.init_data[6] = {}


g_barccatatDefine.init_data[1]['tableid'] = {2061001}     --这里用table，防止后面有需要搭建多个同样类型的
g_barccatatDefine.init_data[1]['maxuser'] = 999
g_barccatatDefine.init_data[1]['bankerid'] = 1
g_barccatatDefine.init_data[1]['bankername'] = '有钱就要任性'
g_barccatatDefine.init_data[1]['bankernamelist'] = {'LV99','Ssss','FM栀子','伊人红妆','Hyun','自由飞翔','xiao明','Yeah','秋月风','秋雨梧桐','人生没有如果','吻武双全'}
g_barccatatDefine.init_data[1]['faceid'] = "900001_1001"
g_barccatatDefine.init_data[1]['banklimit'] = 200000
g_barccatatDefine.init_data[1]['enterlimit'] = 0                               --进入的区间在
g_barccatatDefine.init_data[1]['entermax'] = -1
g_barccatatDefine.init_data[1]['tabletype'] = 1
g_barccatatDefine.init_data[1]['bankjetton'] = 500000
g_barccatatDefine.init_data[1]['tablename'] = "荣耀厅01"
g_barccatatDefine.init_data[1]['multiple'] = 5
g_barccatatDefine.init_data[1]['limitred'] = 999999999

--[[g_barccatatDefine.init_data[2]['tableid'] = {2062001}    --对应的tableid
g_barccatatDefine.init_data[2]['maxuser'] = 999
g_barccatatDefine.init_data[2]['bankerid'] = 1
g_barccatatDefine.init_data[2]['bankername'] = '熊孩儿'
g_barccatatDefine.init_data[2]['bankernamelist'] = {'单炒饭','龙哥','Caiden','小新星','一生有你','清风徐来','那一年','Simon','-Miki-','南柯一梦','子非鱼','bilibili'}
g_barccatatDefine.init_data[2]['faceid'] = "900002_1001"
g_barccatatDefine.init_data[2]['banklimit'] = 200000
g_barccatatDefine.init_data[2]['enterlimit'] = 0
g_barccatatDefine.init_data[2]['entermax'] = -1
g_barccatatDefine.init_data[2]['tabletype'] = 2
g_barccatatDefine.init_data[2]['bankjetton'] = 1000000
g_barccatatDefine.init_data[2]['tablename'] = "荣耀厅01"
g_barccatatDefine.init_data[2]['multiple'] = 5
g_barccatatDefine.init_data[2]['limitred'] = 999999999
]]
--[[
g_barccatatDefine.init_data[3]['tableid'] = {2063001}    --对应的tableid
g_barccatatDefine.init_data[3]['maxuser'] = 999
g_barccatatDefine.init_data[3]['bankerid'] = 1
g_barccatatDefine.init_data[3]['bankername'] = '无双'
g_barccatatDefine.init_data[3]['bankernamelist'] = {'没有女盆友','地老天荒','Toro','胡汉三','栩蔓','香港小朱','刘哥','Silas','xiao太阳','Tony','花言少雨','追梦者'}
g_barccatatDefine.init_data[3]['faceid'] = "900003_1001"
g_barccatatDefine.init_data[3]['banklimit'] = 200000
g_barccatatDefine.init_data[3]['enterlimit'] = 0
g_barccatatDefine.init_data[3]['entermax'] = 5000000
g_barccatatDefine.init_data[3]['tabletype'] = 3
g_barccatatDefine.init_data[3]['bankjetton'] = 1000000
g_barccatatDefine.init_data[3]['tablename'] = "富贵厅01"
g_barccatatDefine.init_data[3]['multiple'] = 5
g_barccatatDefine.init_data[3]['limitred'] = 999999999



g_barccatatDefine.init_data[4]['tableid'] = {2064001}    --对应的tableid
g_barccatatDefine.init_data[4]['maxuser'] = 999
g_barccatatDefine.init_data[4]['bankerid'] = 1
g_barccatatDefine.init_data[4]['bankername'] = '天仙妹妹'
g_barccatatDefine.init_data[4]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
g_barccatatDefine.init_data[4]['faceid'] = "900004_1001"
g_barccatatDefine.init_data[4]['banklimit'] = 200000
g_barccatatDefine.init_data[4]['enterlimit'] = 0
g_barccatatDefine.init_data[4]['entermax'] = -1
g_barccatatDefine.init_data[4]['tabletype'] = 4
g_barccatatDefine.init_data[4]['bankjetton'] = 1000000
g_barccatatDefine.init_data[4]['tablename'] = "富贵厅01"
g_barccatatDefine.init_data[4]['multiple'] = 5
g_barccatatDefine.init_data[4]['limitred'] = 999999999

g_barccatatDefine.init_data[5]['tableid'] = {2065001}    --对应的tableid
g_barccatatDefine.init_data[5]['maxuser'] = 999
g_barccatatDefine.init_data[5]['bankerid'] = 1
g_barccatatDefine.init_data[5]['bankername'] = '天仙妹妹'
g_barccatatDefine.init_data[5]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
g_barccatatDefine.init_data[5]['faceid'] = "900004_1001"
g_barccatatDefine.init_data[5]['banklimit'] = 200000
g_barccatatDefine.init_data[5]['enterlimit'] = 0
g_barccatatDefine.init_data[5]['entermax'] = -1
g_barccatatDefine.init_data[5]['tabletype'] = 5
g_barccatatDefine.init_data[5]['bankjetton'] = 1000000
g_barccatatDefine.init_data[5]['tablename'] = "尊享厅01"
g_barccatatDefine.init_data[5]['multiple'] = 5
g_barccatatDefine.init_data[5]['limitred'] = 999999999

g_barccatatDefine.init_data[6]['tableid'] = {2066001}    --对应的tableid
g_barccatatDefine.init_data[6]['maxuser'] = 999
g_barccatatDefine.init_data[6]['bankerid'] = 1
g_barccatatDefine.init_data[6]['bankername'] = '天仙妹妹'
g_barccatatDefine.init_data[6]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
g_barccatatDefine.init_data[6]['faceid'] = "900004_1001"
g_barccatatDefine.init_data[6]['banklimit'] = 200000
g_barccatatDefine.init_data[6]['enterlimit'] = 0
g_barccatatDefine.init_data[6]['entermax'] = -1
g_barccatatDefine.init_data[6]['tabletype'] = 6
g_barccatatDefine.init_data[6]['bankjetton'] = 1000000
g_barccatatDefine.init_data[6]['tablename'] = "尊享厅02"
g_barccatatDefine.init_data[6]['multiple'] = 5
g_barccatatDefine.init_data[6]['limitred'] = 999999999
]]

g_barccatatDefine.chat_txt = 1
g_barccatatDefine.chat_img = 2
g_barccatatDefine.chat_voice = 3


g_barccatatDefine.type_ydc = 1       --欢乐赢
g_barccatatDefine.type_lxc = 2

g_barccatatDefine.leave_dismiss = 1   --房主解散房间
g_barccatatDefine.leave_timeout = 2   --五轮没下注被踢出

g_barccatatDefine.quit_count  = 10     --几轮没下注后就被踢出

g_barccatatDefine.time_public = 6  --如果是大众长，就有20秒的掉线时间
g_barccatatDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_barccatatDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励


g_barccatatDefine.remainpourjetton  = 10  --最大下注与庄家身上筹码的比例

g_barccatatDefine.robot_range = { {1000, 1000000}, {1000, 1000000},{1000,1000000},{1000, 1000000},{1000, 1000000},{1000, 1000000}}

--当庄N局后下庄
g_barccatatDefine.DropBankerNum = 10

g_barccatatDefine.BeingBanker = {}
g_barccatatDefine.BeingBanker[1] = 200000 --大厅
g_barccatatDefine.BeingBanker[2] = 200000 --俱乐部

g_barccatatDefine.DropBanker = {}
g_barccatatDefine.DropBanker[1] = 200000 --大厅
g_barccatatDefine.DropBanker[2] = 200000 --俱乐部