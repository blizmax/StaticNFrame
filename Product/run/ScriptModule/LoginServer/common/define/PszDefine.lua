g_PszDefine = {}


g_PszDefine.game_type = 3300
--房费支付方式
g_PszDefine.payment_owner = 1 		--房主支付
g_PszDefine.payment_A = 2     		--AA支付
g_PszDefine.payment_free = 3     --免付费
	
--游戏进行的状态
g_PszDefine.state_ready = 1			--等待游戏开始，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_PszDefine.state_start  = 2    	--游戏开始的状态
g_PszDefine.state_play  = 3			--玩家行动的状态
g_PszDefine.state_count  = 4		--结算的状态    
g_PszDefine.state_next = 5			--等待开始下一局游戏的状态
g_PszDefine.state_dissolve = 6  	--申请解散的状态
g_PszDefine.state_delete  = 7  		--等待删除状态的状态

--状态运行的时间
g_PszTime = {}
g_PszTime.ready_time = 1800 		--1800游戏还没开始自动解散房间
g_PszTime.start_time = 4	--1      		--游戏开始及做动画的时间
g_PszTime.play_time =  10		--10	    	--每位玩家每回的操作的时间
g_PszTime.count_time = 5		--5  			--结算及做动画的时间
g_PszTime.next_time = 5			--等待下一局时间
g_PszTime.dissolve_time = 60 		--申请解散的时候，最长的解散时间

--牌型
g_PszCard_type = {}
g_PszCard_type.teshu = -1    		--特殊牌
g_PszCard_type.danzhang = 0     	--单张
g_PszCard_type.duizi = 1     		--对子
g_PszCard_type.xiaoshun = 2    		--小顺
g_PszCard_type.shunzi = 3    		--顺子
g_PszCard_type.tonghua = 4     		--同花
g_PszCard_type.tonghuashun = 5   	--同花顺
g_PszCard_type.baozi = 6     		--豹子


--底分
g_PszDefine_difen = {}
g_PszDefine_difen[123] = 1
g_PszDefine_difen[1234] = 1


--底分转换
g_PszDefine_zhuan = {}
g_PszDefine_zhuan[1] = 1
g_PszDefine_zhuan[2] = 100
g_PszDefine_zhuan[3] = 500
g_PszDefine_zhuan[4] = 1000


--金币场的底分
g_PszDefine_difen[1] = 1
g_PszDefine_difen[100] = 100
g_PszDefine_difen[500] = 500
g_PszDefine_difen[1000] = 1000

g_PszDefine_difenlist = {}
g_PszDefine_difenlist[123] = {1,2,3}
g_PszDefine_difenlist[1234] = {1,2,3,4}
--金币场的底分
g_PszDefine_difenlist[1] = {1, 2, 5, 10}
g_PszDefine_difenlist[100] = {100, 200, 500, 1000}
g_PszDefine_difenlist[500] = {500, 1000, 2000, 4000}
g_PszDefine_difenlist[1000] = {1000, 2000, 5000, 10000}


--聊天类型
g_PszDefine.chat_txt = 1
g_PszDefine.chat_img = 2
g_PszDefine.chat_voice = 3

--牌定义
g_PszDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}													


--玩家状态
g_PszPlayStatus = {}
g_PszPlayStatus.status_ready = 101			
g_PszPlayStatus.status_wait = 102   	--等待行动
g_PszPlayStatus.status_play = 103		--正在行动
g_PszPlayStatus.status_next = 104    	--准备等待


--玩家行动类型
g_PszDefine.abandonpoker = 0			--弃牌
g_PszDefine.lookpoker = 1				--看牌
g_PszDefine.comparepoker = 2			--比牌
g_PszDefine.upjetton = 3				--加注
g_PszDefine.followjetton = 4			--跟注


--牌的状态
g_PszPokerStatus = {} 
g_PszPokerStatus.poker_lookno = 0    	--还没看牌
g_PszPokerStatus.poker_looked = 1    	--看了牌
g_PszPokerStatus.poker_abandon = 2    --弃牌

g_PszDefine.max_user = 6			--最大人数
g_PszDefine.table_srf = 101          --这个是私人房
g_PszDefine.table_jbc = {}   
g_PszDefine.table_jbc['free'] = 1        --
g_PszDefine.table_jbc['junior'] = 2
g_PszDefine.table_jbc['medium'] = 3
g_PszDefine.table_jbc['serior'] = 4

g_PszDefine.table_jbc_list = {1,2,3,4}

--限制 
g_PszDefine.restrict_OK = 1				--游戏开始后可以进入房间
g_PszDefine.restrict_NO = 0				--游戏开始后不能进入房间


--游戏开始后坐下
g_PszDefine.newsit_yes = 0				--游戏开始后坐下的
g_PszDefine.newsit_no = 1				--游戏开始前坐下的


--支付类型
g_PszDefine.diamond_pay = 0 			--钻石支付
g_PszDefine.jade_pay =  1				--玉支付
g_PszDefine.jetton_pay =  2				--金币支付

--比牌倍数
g_PszDefine.doublecomp_two = 0 			--双倍比牌
g_PszDefine.doublecomp_one = 1 			--单倍比牌

--最大投注回合
g_PszDefine.boutcapp_5 = 5				--五轮封顶	
g_PszDefine.boutcapp_10 = 10			--十轮封顶
g_PszDefine.boutcapp_15 = 15			--十五轮封顶

--比牌起始回合
g_PszDefine.mincomp_1 = 1				--一轮可比
g_PszDefine.mincomp_2 = 2				--二轮可比
g_PszDefine.mincomp_3 = 3				--三轮可比

--闷牌
g_PszDefine.menpai_0 = 0				--不闷牌
g_PszDefine.menpai_1 = 1				--闷一轮
g_PszDefine.menpai_2 = 2				--闷二轮
g_PszDefine.menpai_3 = 3				--闷三轮

--金币场最大空闲房间
g_PszDefine.jettontable = 10            --金币场同时最大有空位房间数

--自动准备
g_PszDefine.autoready_OK = 1			--自动准备
g_PszDefine.autoready_NO = 0			--不自动准备

--自动跟注
g_PszDefine.autoCingl_OK = 1			--自动跟注
g_PszDefine.autoCingl_NO = 0			--不自动跟注


--底分 
g_PszPour_type = {}
g_PszPour_type.pour_123 = 123
g_PszPour_type.pour_1234 = 1234

g_PszPour_fenshu = {}
g_PszPour_fenshu[g_PszPour_type.pour_123] = {1, 2, 3}
g_PszPour_fenshu[g_PszPour_type.pour_1234] = {1, 2, 3, 4}

--拼三张玩法

g_PszType = {}				
g_PszType.type_psz = 301    --普通玩法



--进入金币车的最低分数
g_PszDefine.minJettonJBC = {}
g_PszDefine.minJettonJBC[g_PszDefine.table_jbc['free']] = 50
g_PszDefine.minJettonJBC[g_PszDefine.table_jbc['junior']] = 5000
g_PszDefine.minJettonJBC[g_PszDefine.table_jbc['medium']] = 30000
g_PszDefine.minJettonJBC[g_PszDefine.table_jbc['serior']] = 60000

g_PszDefine.maxJettonJBC = {}
g_PszDefine.maxJettonJBC[g_PszDefine.table_jbc['free']] = -1
g_PszDefine.maxJettonJBC[g_PszDefine.table_jbc['junior']] = -1
g_PszDefine.maxJettonJBC[g_PszDefine.table_jbc['medium']] = -1
g_PszDefine.maxJettonJBC[g_PszDefine.table_jbc['serior']] = -1

--机器人进入房间的金币区间
g_PszDefine.RobotJettonSection = {}
g_PszDefine.RobotJettonSection[g_PszDefine.table_jbc['free']] = {50, 5000}
g_PszDefine.RobotJettonSection[g_PszDefine.table_jbc['junior']] = {5000, 30000}
g_PszDefine.RobotJettonSection[g_PszDefine.table_jbc['medium']] = {30000, 60000}
g_PszDefine.RobotJettonSection[g_PszDefine.table_jbc['serior']] = {60000, 100000}



--金币场满N人开
g_PszDefine.numPlayerStart = 2

g_PszDefine.game_lockstate = {}
g_PszDefine.game_lockstate.lock_no = 1
g_PszDefine.game_lockstate.lock_yes = 2

