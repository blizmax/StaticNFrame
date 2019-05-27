g_PdkDefine = {}

--聊天类型
g_PdkDefine.chat_txt = 1
g_PdkDefine.chat_img = 2
g_PdkDefine.chat_voice = 3

--房费支付方式
g_PdkDefine.payment_owner = 1 		--房主支付
g_PdkDefine.payment_A = 2     		--AA支付

--支付类型
g_PdkDefine.pay_money = 1001 		--钻石支付
g_PdkDefine.pay_jetton = 1002		--金币支付
g_PdkDefine.pay_gold =  1003		--玉支付

--支付类型
g_PdkDefine.diamond_pay = 0 		--钻石支付
g_PdkDefine.jade_pay =  1		--玉支付

 --斗地主的类型
g_PdkjType = {}
g_PdkjType.general = 201 			--经典玩法

g_PdkDefine.max_user = 3			--最大人数
g_PdkDefine.table_elementary = 101	--初级场
g_PdkDefine.table_middle = 102		--中级场
g_PdkDefine.table_jadvanced = 103	--高级场
g_PdkDefine.table_srf = 201 		--私人场

--牌桌的状态
g_PdkStatus = {}
g_PdkStatus.state_ready = 1			--等待游戏开始
g_PdkStatus.status_start  = 2    	--游戏开始的状态
g_PdkStatus.state_play  = 3			--玩家行动的状态
g_PdkStatus.state_count  = 4		--结算的状态    
g_PdkStatus.state_dissolve = 5  	--申请解散的状态
g_PdkStatus.state_delete  = 6  		--等待删除状态的状态
g_PdkStatus.state_next  = 7  		--等待下一局游戏开始的状态


--牌桌的状态运行的时间
g_PdkTime = {}
g_PdkTime.ready_time = 1800 		--1800游戏还没开始自动解散房间
g_PdkTime.start_time = 1			--游戏开始及做动画的时间
g_PdkTime.play_time =  12			--每位玩家每回的操作的时间
g_PdkTime.count_time = 1			--结算及做动画的时间
g_PdkTime.dissolve_time = 300 		--申请解散的时候，最长的解散时间
g_PdkTime.next_time = 60 			--等待下一局游戏开始时间

--玩家状态
g_PdkPlayerState = {}
g_PdkPlayerState.status_notReady = 100
g_PdkPlayerState.status_ready = 101	 --准备		
g_PdkPlayerState.status_wait = 102   --等待行动
g_PdkPlayerState.status_play = 103	 --正在行动


--玩家出牌类型
g_PdkPlay_poker = {}
g_PdkPlay_poker.doNot = 1				--过
g_PdkPlay_poker.play = 2				--出牌


--玩家行动类型
g_PdkAction = {}
g_PdkAction.type_play  = 3 			--出牌


--牌型
g_PdkCard_type = {}
g_PdkCard_type.invalid = 0 				--错误牌型
g_PdkCard_type.danzhang = 1     		--单张
g_PdkCard_type.duizi = 2     			--对子
g_PdkCard_type.shunzi = 3    			--顺子
g_PdkCard_type.sanshunzi = 4			--三同牌顺子
g_PdkCard_type.liandui = 5    			--连对
g_PdkCard_type.feiji = 6    			--飞机
g_PdkCard_type.sanzhang = 7				--三同牌(只适用于最后一手牌)
g_PdkCard_type.sandaiyi = 8    			--三带一(只适用于最后一手牌)
g_PdkCard_type.sandaier = 9    			--三带二
g_PdkCard_type.sidaisan = 10				--四带三(只适用于勾选四带三)
g_PdkCard_type.stiffBomb = 11    		--硬炸弹()
g_PdkCard_type.threeABomb = 12    		--3A炸弹(只适用于勾选3A炸弹玩法)
g_PdkCard_type.threeAoneBomb = 13		--3A带一炸弹(只适用于勾选3A带一当炸弹玩法)


--牌定义												
g_PdkPokerList = {101,102,103,104,105,106,107,108,109,110,111,112,113,
				  201,202,203,204,205,206,207,208,209,210,211,212,213,
				  301,302,303,304,305,306,307,308,309,310,311,312,313,
				  401,402,403,404,405,406,407,408,409,410,411,412,413,
				  }   --不包括大小王的			1方块 2梅花 3红桃 4黑桃


--托管
g_PdkDefine.autoplayer_No = 0			--取消托管
g_PdkDefine.autoplayer_Yes = 1			--托管

--春天
g_PdkDefine.spring_No = 0				--不是春天
g_PdkDefine.spring_Yes = 1				--春天


g_PdkDefine.spc_win = 101
g_PdkDefine.spc_lose = 102
