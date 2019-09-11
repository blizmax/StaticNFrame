
g_sxyxDefine = {}

g_sxyxDefine.api_url = "http://api01.oriental-game.com:8085/"
g_sxyxDefine.data_api_url = "http://mucho.oriental-game.com:8057/"
g_sxyxDefine.XOperator = "mog136kd"
g_sxyxDefine.Xkey = "85AhTAkAA3nzhIvO"
g_sxyxDefine.user_prefix = "UserAAA"
g_sxyxDefine.providerId = 1 --游戏供应商 id : 1
g_sxyxDefine.gameCode = "oglive" --游戏代号 : oglive
g_sxyxDefine.GMT_index = 8  --时区
g_sxyxDefine.get_token_times = 25*60  --25分钟重新更新一下玩家token

g_sxyxDefine.test = 1
--牌桌状态
g_sxyxDefine.game_type = 5800
g_sxyxDefine.state_ready = 1
g_sxyxDefine.state_start = 2
g_sxyxDefine.state_pour = 3
g_sxyxDefine.state_sendcard = 4
g_sxyxDefine.state_count = 5
g_sxyxDefine.state_nxet = 6
g_sxyxDefine.state_delete = 7

--状态运行时间
g_sxyxDefine.time_pour = 25
g_sxyxDefine.time_sendcard  = 6
g_sxyxDefine.time_count = 3
g_sxyxDefine.time_nxet = 3


--下注区域赔率
g_sxyxDefine.lines = {}
g_sxyxDefine.lines[1] = 0.95    	--庄
g_sxyxDefine.lines[2] = 1    		--闲
g_sxyxDefine.lines[3] = 8    		--和
g_sxyxDefine.lines[4] = 11    		--庄对
g_sxyxDefine.lines[5] = 11    		--闲对

--押注区域 1 庄  2 闲  3 和  4 庄对 5 闲对
g_sxyxDefine.pour_type = {1,2,3,4,5}

--房间类型
g_sxyxDefine.table_type = {}           --
g_sxyxDefine.table_type['junior'] = 1
g_sxyxDefine.table_type['medium'] = 2
g_sxyxDefine.table_type['serior'] = 3
g_sxyxDefine.table_type['high'] = 4

 --初始化的8个座位
g_sxyxDefine.init_sit = 6     

g_sxyxDefine.min_pour_common = 100  --普通场最小下注
g_sxyxDefine.min_pour_grade = 10000  --高级场最小下注
g_sxyxDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_sxyxDefine.pour3_rate = 100        --第三个和第一个的倍数



--牌定义
g_sxyxDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}


--最小上庄分数
g_sxyxDefine.minBeingBankerJetton = {}
g_sxyxDefine.minBeingBankerJetton[1] = 100000000
g_sxyxDefine.minBeingBankerJetton[2] = 100000000
g_sxyxDefine.minBeingBankerJetton[3] = 100000000



--踢下庄家的分数
g_sxyxDefine.minXiaZhuanJetton = {}
g_sxyxDefine.minXiaZhuanJetton[1] = 1000000
g_sxyxDefine.minXiaZhuanJetton[2] = 1000000
g_sxyxDefine.minXiaZhuanJetton[3] = 1000000



g_sxyxDefine.table_common = 777777



g_sxyxDefine.page_item = 8   --每一页显示的牌桌的数量
g_sxyxDefine.min_bankjetton = 100000    --最小上庄的倍数
g_sxyxDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数




if g_servername == "run_tyyd" then
	g_sxyxDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_sxyxDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_sxyxDefine.pour2_rate = 10
	g_sxyxDefine.pour3_rate = 50
	
	g_sxyxDefine.min_pour_common = 100
	g_sxyxDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建

g_sxyxDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_sxyxDefine.init_data[1] = {}
--g_sxyxDefine.init_data[2] = {}
--g_sxyxDefine.init_data[3] = {}
--g_sxyxDefine.init_data[4] = {}
--g_sxyxDefine.init_data[5] = {}
--g_sxyxDefine.init_data[6] = {}


g_sxyxDefine.init_data[1]['tableid'] = {2210001}     --这里用table，防止后面有需要搭建多个同样类型的
g_sxyxDefine.init_data[1]['maxuser'] = 999
g_sxyxDefine.init_data[1]['bankerid'] = 1
g_sxyxDefine.init_data[1]['bankername'] = '有钱就要任性'
g_sxyxDefine.init_data[1]['bankernamelist'] = {'LV99','Ssss','FM栀子','伊人红妆','Hyun','自由飞翔','xiao明','Yeah','秋月风','秋雨梧桐','人生没有如果','吻武双全'}
g_sxyxDefine.init_data[1]['faceid'] = "900001_1001"
g_sxyxDefine.init_data[1]['banklimit'] = 1000000
g_sxyxDefine.init_data[1]['enterlimit'] = 0                               --进入的区间在
g_sxyxDefine.init_data[1]['entermax'] = -1
g_sxyxDefine.init_data[1]['tabletype'] = 1
g_sxyxDefine.init_data[1]['bankjetton'] = 1000000
g_sxyxDefine.init_data[1]['tablename'] = "荣耀厅01"
g_sxyxDefine.init_data[1]['multiple'] = 5
g_sxyxDefine.init_data[1]['limitred'] = 1000000

--g_sxyxDefine.init_data[2]['tableid'] = {2062001}    --对应的tableid
--g_sxyxDefine.init_data[2]['maxuser'] = 999
--g_sxyxDefine.init_data[2]['bankerid'] = 1
--g_sxyxDefine.init_data[2]['bankername'] = '熊孩儿'
--g_sxyxDefine.init_data[2]['bankernamelist'] = {'单炒饭','龙哥','Caiden','小新星','一生有你','清风徐来','那一年','Simon','-Miki-','南柯一梦','子非鱼','bilibili'}
--g_sxyxDefine.init_data[2]['faceid'] = "900002_1001"
--g_sxyxDefine.init_data[2]['banklimit'] = 1000000
--g_sxyxDefine.init_data[2]['enterlimit'] = 0
--g_sxyxDefine.init_data[2]['entermax'] = -1
--g_sxyxDefine.init_data[2]['tabletype'] = 2
--g_sxyxDefine.init_data[2]['bankjetton'] = 1000000
--g_sxyxDefine.init_data[2]['tablename'] = "荣耀厅01"
--g_sxyxDefine.init_data[2]['multiple'] = 5
--g_sxyxDefine.init_data[2]['limitred'] = 1000000

--g_sxyxDefine.init_data[3]['tableid'] = {2063001}    --对应的tableid
--g_sxyxDefine.init_data[3]['maxuser'] = 999
--g_sxyxDefine.init_data[3]['bankerid'] = 1
--g_sxyxDefine.init_data[3]['bankername'] = '无双'
--g_sxyxDefine.init_data[3]['bankernamelist'] = {'没有女盆友','地老天荒','Toro','胡汉三','栩蔓','香港小朱','刘哥','Silas','xiao太阳','Tony','花言少雨','追梦者'}
--g_sxyxDefine.init_data[3]['faceid'] = "900003_1001"
--g_sxyxDefine.init_data[3]['banklimit'] = 1000000
--g_sxyxDefine.init_data[3]['enterlimit'] = 0
--g_sxyxDefine.init_data[3]['entermax'] = 5000000
--g_sxyxDefine.init_data[3]['tabletype'] = 3
--g_sxyxDefine.init_data[3]['bankjetton'] = 1000000
--g_sxyxDefine.init_data[3]['tablename'] = "富贵厅01"
--g_sxyxDefine.init_data[3]['multiple'] = 5
--g_sxyxDefine.init_data[3]['limitred'] = 2000000



--g_sxyxDefine.init_data[4]['tableid'] = {2064001}    --对应的tableid
--g_sxyxDefine.init_data[4]['maxuser'] = 999
--g_sxyxDefine.init_data[4]['bankerid'] = 1
--g_sxyxDefine.init_data[4]['bankername'] = '天仙妹妹'
--g_sxyxDefine.init_data[4]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
--g_sxyxDefine.init_data[4]['faceid'] = "900004_1001"
--g_sxyxDefine.init_data[4]['banklimit'] = 1000000
--g_sxyxDefine.init_data[4]['enterlimit'] = 0
--g_sxyxDefine.init_data[4]['entermax'] = -1
--g_sxyxDefine.init_data[4]['tabletype'] = 4
--g_sxyxDefine.init_data[4]['bankjetton'] = 1000000
--g_sxyxDefine.init_data[4]['tablename'] = "富贵厅01"
--g_sxyxDefine.init_data[4]['multiple'] = 5
--g_sxyxDefine.init_data[4]['limitred'] = 2000000

--g_sxyxDefine.init_data[5]['tableid'] = {2065001}    --对应的tableid
--g_sxyxDefine.init_data[5]['maxuser'] = 999
--g_sxyxDefine.init_data[5]['bankerid'] = 1
--g_sxyxDefine.init_data[5]['bankername'] = '天仙妹妹'
--g_sxyxDefine.init_data[5]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
--g_sxyxDefine.init_data[5]['faceid'] = "900004_1001"
--g_sxyxDefine.init_data[5]['banklimit'] = 1000000
--g_sxyxDefine.init_data[5]['enterlimit'] = 0
--g_sxyxDefine.init_data[5]['entermax'] = -1
--g_sxyxDefine.init_data[5]['tabletype'] = 5
--g_sxyxDefine.init_data[5]['bankjetton'] = 1000000
--g_sxyxDefine.init_data[5]['tablename'] = "尊享厅01"
--g_sxyxDefine.init_data[5]['multiple'] = 5
--g_sxyxDefine.init_data[5]['limitred'] = 5000000

--g_sxyxDefine.init_data[6]['tableid'] = {2066001}    --对应的tableid
--g_sxyxDefine.init_data[6]['maxuser'] = 999
--g_sxyxDefine.init_data[6]['bankerid'] = 1
--g_sxyxDefine.init_data[6]['bankername'] = '天仙妹妹'
--g_sxyxDefine.init_data[6]['bankernamelist'] = {'寂寞的夜','咖啡时光','烟几包','BOBO','头等大事','光辉岁月','Alex','子余','阳光笑脸','骚年不赖','欣余','请叫我***'}
--g_sxyxDefine.init_data[6]['faceid'] = "900004_1001"
--g_sxyxDefine.init_data[6]['banklimit'] = 1000000
--g_sxyxDefine.init_data[6]['enterlimit'] = 0
--g_sxyxDefine.init_data[6]['entermax'] = -1
--g_sxyxDefine.init_data[6]['tabletype'] = 6
--g_sxyxDefine.init_data[6]['bankjetton'] = 1000000
--g_sxyxDefine.init_data[6]['tablename'] = "尊享厅02"
--g_sxyxDefine.init_data[6]['multiple'] = 5
--g_sxyxDefine.init_data[6]['limitred'] = 5000000


g_sxyxDefine.chat_txt = 1
g_sxyxDefine.chat_img = 2
g_sxyxDefine.chat_voice = 3


g_sxyxDefine.type_ydc = 1       --欢乐赢
g_sxyxDefine.type_lxc = 2

g_sxyxDefine.leave_dismiss = 1   --房主解散房间
g_sxyxDefine.leave_timeout = 2   --五轮没下注被踢出

g_sxyxDefine.quit_count  = 10     --几轮没下注后就被踢出

g_sxyxDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_sxyxDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_sxyxDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励


g_sxyxDefine.remainpourjetton  = 10  --最大下注与庄家身上筹码的比例

g_sxyxDefine.robot_range = { {1000, 5000000}, {1000, 5000000},{1000,5000000},{1000, 5000000},{1000, 5000000},{1000, 5000000}}

--当庄N局后下庄
g_sxyxDefine.DropBankerNum = 10

g_sxyxDefine.BeingBanker = {}
g_sxyxDefine.BeingBanker[1] = 200000 --大厅
g_sxyxDefine.BeingBanker[2] = 200000 --俱乐部

g_sxyxDefine.DropBanker = {}
g_sxyxDefine.DropBanker[1] = 100000 --大厅
g_sxyxDefine.DropBanker[2] = 100000 --俱乐部