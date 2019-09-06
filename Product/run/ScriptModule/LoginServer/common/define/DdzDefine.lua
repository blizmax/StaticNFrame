g_DdzDefine = {}


g_DdzDefine.game_type = 3900
--聊天类型
g_DdzDefine.chat_txt = 1
g_DdzDefine.chat_img = 2
g_DdzDefine.chat_voice = 3

--房费支付方式
g_DdzDefine.payment_owner = 1 		--房主支付
g_DdzDefine.payment_A = 2     		--AA支付

--支付类型
g_DdzDefine.pay_money = 1001 		--钻石支付
g_DdzDefine.pay_jetton = 1002		--金币支付
g_DdzDefine.pay_gold =  1003		--玉支付

g_DdzDefine.max_user = 3			--最大人数
g_DdzDefine.table_elementary = 101	--初级场
g_DdzDefine.table_middle = 102		--中级场
g_DdzDefine.table_jadvanced = 103	--高级场
g_DdzDefine.table_srf = 201 		--私人场

 --斗地主的类型
g_DdzjType = {}
g_DdzjType.general = 401 			--经典玩法
g_DdzjType.joy = 402				--欢乐玩法
g_DdzjType.wildCard = 403			--癞子玩法


--牌桌的状态
g_DdzStatus = {}
g_DdzStatus.state_ready = 1			--等待游戏开始，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_DdzStatus.status_start  = 2    	--游戏开始的状态
g_DdzStatus.status_qlandlord  = 3   --抢地主的状态
g_DdzStatus.status_wait  = 4   		--抢完地主做动画的状态
g_DdzStatus.state_play  = 5			--玩家行动的状态
g_DdzStatus.state_count  = 6		--结算的状态    
g_DdzStatus.state_dissolve = 8  	--申请解散的状态
g_DdzStatus.state_delete  = 9  		--等待删除状态的状态
g_DdzStatus.state_next  = 10  		--等待下一局游戏开始的状态
g_DdzStatus.state_double  = 11  	--等待下一局游戏开始的状态

--牌桌的状态运行的时间
g_DdzTime = {}
g_DdzTime.ready_time = 1800 		--1800游戏还没开始自动解散房间
g_DdzTime.start_time = 2			--游戏开始及做动画的时间
g_DdzTime.qlandlord_time  = 11  	--抢地主的时间
g_DdzTime.wait_time  = 4  			--抢完地主做动画的时间
g_DdzTime.play_time =  20			--每位玩家每回的操作的时间
g_DdzTime.count_time = 1			--结算及做动画的时间
g_DdzTime.dissolve_time = 300 		--申请解散的时候，最长的解散时间
g_DdzTime.next_time = 14 			--等待下一局游戏开始时间
g_DdzTime.double_time  = 10  		--加倍的时间

--玩家状态
g_DdzPlayerState = {}
g_DdzPlayerState.status_notReady = 100
g_DdzPlayerState.status_ready = 101	 --准备		
g_DdzPlayerState.status_wait = 102   --等待行动
g_DdzPlayerState.status_play = 103	 --正在行动


--玩家出牌类型
g_DdzPlay_poker = {}
g_DdzPlay_poker.doNot = 1				--过
g_DdzPlay_poker.play = 2				--出牌


--玩家行动类型
g_DdzAction = {}
g_DdzAction.type_jLandlord = 1 		--叫地主
g_DdzAction.type_qLandlord = 2 		--抢地主
g_DdzAction.type_play  = 3 			--出牌


--牌型
g_DdzCard_type = {}
g_DdzCard_type.invalid = 0 				--错误牌型
g_DdzCard_type.danzhang = 1     		--单张
g_DdzCard_type.duizi = 2     			--对子
g_DdzCard_type.shunzi = 3    			--顺子
g_DdzCard_type.szp = 4    				--三张牌
g_DdzCard_type.sanshunzi = 5			--三顺子
g_DdzCard_type.liandui = 6    			--连对
g_DdzCard_type.feiji = 7    			--飞机
g_DdzCard_type.sandaiyi = 8    			--三带一
g_DdzCard_type.sandaier = 9    			--三带二
g_DdzCard_type.sidaier = 10    			--四带二
g_DdzCard_type.softBomb = 11    		--软炸弹
g_DdzCard_type.stiffBomb = 12    		--硬炸弹
g_DdzCard_type.pureWildCardBomb = 13 	--纯癞子炸弹
g_DdzCard_type.kongBomb = 14 			--王炸


--牌定义												
g_DdzPokerList = {101,102,103,104,105,106,107,108,109,110,111,112,113,
				  201,202,203,204,205,206,207,208,209,210,211,212,213,
				  301,302,303,304,305,306,307,308,309,310,311,312,313,
				  401,402,403,404,405,406,407,408,409,410,411,412,413,
				  515,525}   --包括大小王的
				
--癞子牌的定义
g_DdzWildCardlist = {601, 602, 603, 604, 605 , 606, 607, 608, 609, 610, 611, 612, 613}				


--明牌
g_DdzDefine.mPoker_No = 0				--不明牌
g_DdzDefine.mPoker_Yes = 1				--明牌

--托管
g_DdzDefine.autoplayer_No = 0			--取消托管
g_DdzDefine.autoplayer_Yes = 1			--托管


--叫地主类型
g_DdzDefine.jLandlord_No = 101			--不叫
g_DdzDefine.jLandlord_Yes = 102			--叫地主

--抢地主类型
g_DdzDefine.qLandlord_No = 99			--不抢
g_DdzDefine.qLandlord_Yes = 1			--抢地主

--春天
g_DdzDefine.spring_No = 0				--不是春天
g_DdzDefine.spring_Yes = 1				--春天


g_DdzDefine.table_jbc = {}
g_DdzDefine.table_jbc['free']   = 1
g_DdzDefine.table_jbc['junior'] = 2
g_DdzDefine.table_jbc['medium'] = 3
g_DdzDefine.table_jbc['serior'] = 4

--进入金币车的最低分数
g_DdzDefine.minJettonJBC = {}
g_DdzDefine.minJettonJBC[g_DdzDefine.table_jbc['free']] = 60
g_DdzDefine.minJettonJBC[g_DdzDefine.table_jbc['junior']] = 1000
g_DdzDefine.minJettonJBC[g_DdzDefine.table_jbc['medium']] = 2000
g_DdzDefine.minJettonJBC[g_DdzDefine.table_jbc['serior']] = 10000

g_DdzDefine.maxJettonJBC = {}
g_DdzDefine.maxJettonJBC[g_DdzDefine.table_jbc['free']] = -1
g_DdzDefine.maxJettonJBC[g_DdzDefine.table_jbc['junior']] = -1
g_DdzDefine.maxJettonJBC[g_DdzDefine.table_jbc['medium']] = -1
g_DdzDefine.maxJettonJBC[g_DdzDefine.table_jbc['serior']] = -1

--机器人进入房间的金币区间
g_DdzDefine.RobotJettonSection = {}
g_DdzDefine.RobotJettonSection[g_DdzDefine.table_jbc['free']] = {100, 50000}
g_DdzDefine.RobotJettonSection[g_DdzDefine.table_jbc['junior']] = {1000, 100000}
g_DdzDefine.RobotJettonSection[g_DdzDefine.table_jbc['medium']] = {2000, 200000}
g_DdzDefine.RobotJettonSection[g_DdzDefine.table_jbc['serior']] = {10000, 500000}


--金币场底分
g_DdzDefine.tableDiFen = {}
g_DdzDefine.tableDiFen[g_DdzDefine.table_jbc['free']] = 1
g_DdzDefine.tableDiFen[g_DdzDefine.table_jbc['junior']] = 50
g_DdzDefine.tableDiFen[g_DdzDefine.table_jbc['medium']] = 100
g_DdzDefine.tableDiFen[g_DdzDefine.table_jbc['serior']] = 500


g_DdzDefine.DdzPokerListTo_cards = {}

g_DdzDefine.DdzPokerListTo_cards[103] = 10
g_DdzDefine.DdzPokerListTo_cards[203] = 11
g_DdzDefine.DdzPokerListTo_cards[303] = 12
g_DdzDefine.DdzPokerListTo_cards[403] = 13

g_DdzDefine.DdzPokerListTo_cards[104] = 20
g_DdzDefine.DdzPokerListTo_cards[204] = 21
g_DdzDefine.DdzPokerListTo_cards[304] = 22
g_DdzDefine.DdzPokerListTo_cards[404] = 23

g_DdzDefine.DdzPokerListTo_cards[105] = 30
g_DdzDefine.DdzPokerListTo_cards[205] = 31
g_DdzDefine.DdzPokerListTo_cards[305] = 32
g_DdzDefine.DdzPokerListTo_cards[405] = 33

g_DdzDefine.DdzPokerListTo_cards[106] = 40
g_DdzDefine.DdzPokerListTo_cards[206] = 41
g_DdzDefine.DdzPokerListTo_cards[306] = 42
g_DdzDefine.DdzPokerListTo_cards[406] = 43

g_DdzDefine.DdzPokerListTo_cards[107] = 50
g_DdzDefine.DdzPokerListTo_cards[207] = 51
g_DdzDefine.DdzPokerListTo_cards[307] = 52
g_DdzDefine.DdzPokerListTo_cards[407] = 53

g_DdzDefine.DdzPokerListTo_cards[108] = 60
g_DdzDefine.DdzPokerListTo_cards[208] = 61
g_DdzDefine.DdzPokerListTo_cards[308] = 62
g_DdzDefine.DdzPokerListTo_cards[408] = 63

g_DdzDefine.DdzPokerListTo_cards[109] = 70
g_DdzDefine.DdzPokerListTo_cards[209] = 71
g_DdzDefine.DdzPokerListTo_cards[309] = 72
g_DdzDefine.DdzPokerListTo_cards[409] = 73

g_DdzDefine.DdzPokerListTo_cards[110] = 80
g_DdzDefine.DdzPokerListTo_cards[210] = 81
g_DdzDefine.DdzPokerListTo_cards[310] = 82
g_DdzDefine.DdzPokerListTo_cards[410] = 83

g_DdzDefine.DdzPokerListTo_cards[111] = 90
g_DdzDefine.DdzPokerListTo_cards[211] = 91
g_DdzDefine.DdzPokerListTo_cards[311] = 92
g_DdzDefine.DdzPokerListTo_cards[411] = 93

g_DdzDefine.DdzPokerListTo_cards[112] = 100
g_DdzDefine.DdzPokerListTo_cards[212] = 101
g_DdzDefine.DdzPokerListTo_cards[312] = 102
g_DdzDefine.DdzPokerListTo_cards[412] = 103

g_DdzDefine.DdzPokerListTo_cards[113] = 110
g_DdzDefine.DdzPokerListTo_cards[213] = 111
g_DdzDefine.DdzPokerListTo_cards[313] = 112
g_DdzDefine.DdzPokerListTo_cards[413] = 113

g_DdzDefine.DdzPokerListTo_cards[101] = 120
g_DdzDefine.DdzPokerListTo_cards[201] = 121
g_DdzDefine.DdzPokerListTo_cards[301] = 122
g_DdzDefine.DdzPokerListTo_cards[401] = 123

g_DdzDefine.DdzPokerListTo_cards[102] = 130
g_DdzDefine.DdzPokerListTo_cards[202] = 131
g_DdzDefine.DdzPokerListTo_cards[302] = 132
g_DdzDefine.DdzPokerListTo_cards[402] = 133

g_DdzDefine.DdzPokerListTo_cards[515] = 140
g_DdzDefine.DdzPokerListTo_cards[525] = 150



g_DdzDefine.cardsToDdzPokerList = {}
g_DdzDefine.cardsToDdzPokerList[10] = 103 
g_DdzDefine.cardsToDdzPokerList[11] = 203 
g_DdzDefine.cardsToDdzPokerList[12] = 303 
g_DdzDefine.cardsToDdzPokerList[13] = 403 

g_DdzDefine.cardsToDdzPokerList[20] = 104 
g_DdzDefine.cardsToDdzPokerList[21] = 204 
g_DdzDefine.cardsToDdzPokerList[22] = 304 
g_DdzDefine.cardsToDdzPokerList[23] = 404 

g_DdzDefine.cardsToDdzPokerList[30] = 105 
g_DdzDefine.cardsToDdzPokerList[31] = 205 
g_DdzDefine.cardsToDdzPokerList[32] = 305 
g_DdzDefine.cardsToDdzPokerList[33] = 405 

g_DdzDefine.cardsToDdzPokerList[40] = 106 
g_DdzDefine.cardsToDdzPokerList[41] = 206 
g_DdzDefine.cardsToDdzPokerList[42] = 306 
g_DdzDefine.cardsToDdzPokerList[43] = 406 

g_DdzDefine.cardsToDdzPokerList[50] = 107 
g_DdzDefine.cardsToDdzPokerList[51] = 207 
g_DdzDefine.cardsToDdzPokerList[52] = 307 
g_DdzDefine.cardsToDdzPokerList[53] = 407 

g_DdzDefine.cardsToDdzPokerList[60] = 108 
g_DdzDefine.cardsToDdzPokerList[61] = 208 
g_DdzDefine.cardsToDdzPokerList[62] = 308 
g_DdzDefine.cardsToDdzPokerList[63] = 408 

g_DdzDefine.cardsToDdzPokerList[70] = 109 
g_DdzDefine.cardsToDdzPokerList[71] = 209 
g_DdzDefine.cardsToDdzPokerList[72] = 309
g_DdzDefine.cardsToDdzPokerList[73] = 409

g_DdzDefine.cardsToDdzPokerList[80] = 110
g_DdzDefine.cardsToDdzPokerList[81] = 210
g_DdzDefine.cardsToDdzPokerList[82] = 310
g_DdzDefine.cardsToDdzPokerList[83] = 410

g_DdzDefine.cardsToDdzPokerList[90] = 111
g_DdzDefine.cardsToDdzPokerList[91] = 211
g_DdzDefine.cardsToDdzPokerList[92] = 311
g_DdzDefine.cardsToDdzPokerList[93] = 411

g_DdzDefine.cardsToDdzPokerList[100] = 112
g_DdzDefine.cardsToDdzPokerList[101] = 212
g_DdzDefine.cardsToDdzPokerList[102] = 312
g_DdzDefine.cardsToDdzPokerList[103] = 412

g_DdzDefine.cardsToDdzPokerList[110] = 113 
g_DdzDefine.cardsToDdzPokerList[111] = 213
g_DdzDefine.cardsToDdzPokerList[112] = 313 
g_DdzDefine.cardsToDdzPokerList[113] = 413 

g_DdzDefine.cardsToDdzPokerList[120] = 101
g_DdzDefine.cardsToDdzPokerList[121] = 201 
g_DdzDefine.cardsToDdzPokerList[122] = 301 
g_DdzDefine.cardsToDdzPokerList[123] = 401 

g_DdzDefine.cardsToDdzPokerList[130] = 102 
g_DdzDefine.cardsToDdzPokerList[131] = 202 
g_DdzDefine.cardsToDdzPokerList[132] = 302 
g_DdzDefine.cardsToDdzPokerList[133] = 402 

g_DdzDefine.cardsToDdzPokerList[140] = 515
g_DdzDefine.cardsToDdzPokerList[150] = 525

g_DdzDefine.game_lockstate = {}
g_DdzDefine.game_lockstate.lock_no = 1
g_DdzDefine.game_lockstate.lock_yes = 2



