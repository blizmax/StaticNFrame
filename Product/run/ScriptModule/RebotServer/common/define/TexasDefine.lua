g_TexasDefine = {}


g_TexasDefine.game_type = 5701

--场次
g_TexasDefine.table_jbc = {}   
g_TexasDefine.table_jbc['free'] = 1        	--体验场
g_TexasDefine.table_jbc['junior'] = 2	   	--低级场
g_TexasDefine.table_jbc['medium'] = 3		--中级场
g_TexasDefine.table_jbc['serior'] = 4		--高级场	

g_TexasDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_TexasDefine.init_data[1] = {}
g_TexasDefine.init_data[2] = {}
g_TexasDefine.init_data[3] = {}
g_TexasDefine.init_data[4] = {}

g_TexasDefine.init_data[1].maxuser = 9					--最大人数
g_TexasDefine.init_data[1].enterlimit = 1000 			--最低入场限制
g_TexasDefine.init_data[1].entermax = -1				--最大入场限制 -1 为不限制
g_TexasDefine.init_data[1].littleblind = 10				--小盲注
g_TexasDefine.init_data[1].bigblind = 20				--大盲注
g_TexasDefine.init_data[1].ante = 10					--前注
g_TexasDefine.init_data[1].maxcarry = 20000				--最大携带
g_TexasDefine.init_data[1].robotjettonscope = {1000, 40000}--机器人金币范围
g_TexasDefine.init_data[1].maxrefuel = 1000				--最大加注
g_TexasDefine.init_data[1].boutcapp = 5					--最大加注回合
g_TexasDefine.init_data[1].maxuser = 9					--最大人数

g_TexasDefine.init_data[2].maxuser = 9					--最大人数
g_TexasDefine.init_data[2].enterlimit = 5000 			--最低入场限制
g_TexasDefine.init_data[2].entermax = -1				--最大入场限制 -1 为不限制
g_TexasDefine.init_data[2].littleblind = 100			--小盲注
g_TexasDefine.init_data[2].bigblind = 200				--大盲注
g_TexasDefine.init_data[2].ante = 100					--前注
g_TexasDefine.init_data[2].maxcarry = 100000			--最大携带
g_TexasDefine.init_data[2].robotjettonscope = {5000, 200000}--机器人金币范围
g_TexasDefine.init_data[2].maxrefuel = 10000			--最大加注
g_TexasDefine.init_data[2].boutcapp = 5					--最大加注回合
g_TexasDefine.init_data[2].maxuser = 9					--最大人数

g_TexasDefine.init_data[3].maxuser = 9					--最大人数
g_TexasDefine.init_data[3].enterlimit = 30000 			--最低入场限制
g_TexasDefine.init_data[3].entermax = -1				--最大入场限制 -1 为不限制
g_TexasDefine.init_data[3].littleblind = 500			--小盲注
g_TexasDefine.init_data[3].bigblind = 1000				--大盲注
g_TexasDefine.init_data[3].ante = 500					--前注
g_TexasDefine.init_data[3].maxcarry = 500000			--最大携带
g_TexasDefine.init_data[3].robotjettonscope = {30000, 1000000}--机器人金币范围
g_TexasDefine.init_data[3].maxrefuel = 50000			--最大加注
g_TexasDefine.init_data[3].boutcapp = 5					--最大加注回合
g_TexasDefine.init_data[3].maxuser = 9					--最大人数

g_TexasDefine.init_data[4].maxuser = 9					--最大人数
g_TexasDefine.init_data[4].enterlimit = 100000 			--最低入场限制
g_TexasDefine.init_data[4].entermax = -1				--最大入场限制 -1 为不限制
g_TexasDefine.init_data[4].littleblind = 2500			--小盲注
g_TexasDefine.init_data[4].bigblind = 5000				--大盲注
g_TexasDefine.init_data[4].ante = 2500					--前注
g_TexasDefine.init_data[4].maxcarry = -1				--最大携带
g_TexasDefine.init_data[4].robotjettonscope = {100000, 1000000}--机器人金币范围
g_TexasDefine.init_data[4].maxrefuel = 250000			--最大加注
g_TexasDefine.init_data[4].boutcapp = 5					--最大加注回合
g_TexasDefine.init_data[4].maxuser = 9					--最大人数

--游戏进行的状态
g_TexasDefine.state_ready = 1		--等待游戏开始
g_TexasDefine.state_start  = 2    	--游戏开始的状态
g_TexasDefine.state_play  = 3		--玩家行动的状态
g_TexasDefine.state_sendcard  = 4	--发牌的状态
g_TexasDefine.state_count  = 5		--结算的状态    
g_TexasDefine.state_next = 6		--等待开始下一局游戏的状态
g_TexasDefine.state_delete = 7		--等待删除

--状态运行的时间
g_TexasTime = {}
g_TexasTime.ready_time = 1800 		--时间结束后游戏还没开始自动解散房间
g_TexasTime.start_time = 3	      	--游戏开始及做动画的时间
g_TexasTime.play_time =  10		    --每位玩家每回的操作的时间
g_TexasTime.sendcard_time =  1		--每位玩家每回的操作的时间
g_TexasTime.count_time = 5			--结算及做动画的时间
g_TexasTime.next_time = 3			--等待下一局时间

--玩家状态
g_TexasPlayStatus = {}
g_TexasPlayStatus.status_ready = 101			
g_TexasPlayStatus.status_wait = 102   	--等待行动
g_TexasPlayStatus.status_play = 103		--正在行动

--玩家行动类型
g_TexasDefine.rang = 1					--让牌
g_TexasDefine.abandon = 2				--弃牌
g_TexasDefine.add = 3					--加注
g_TexasDefine.follow = 4				--跟注
g_TexasDefine.allon = 5					--全下

--牌圈
g_TexasDefine_cradcircle = {}
g_TexasDefine_cradcircle.PreFlop = 1	--前翻牌圈
g_TexasDefine_cradcircle.FlopRound = 2	--翻牌圈
g_TexasDefine_cradcircle.TurnRound = 3	--转牌圈
g_TexasDefine_cradcircle.RiverRound = 4	--合牌圈

--德州玩法
g_TexasType = {}				
g_TexasType.type_common = 101    	--通用玩法

--牌型
g_TexasCard_type = {}
g_TexasCard_type.danzhang = 1     	--高牌
g_TexasCard_type.yidui = 2     		--对子
g_TexasCard_type.liangdui = 3     	--两对
g_TexasCard_type.santiao = 4     	--三条
g_TexasCard_type.shunzi = 5    		--顺子
g_TexasCard_type.tonghua = 6     	--同花
g_TexasCard_type.hulu = 7     		--葫芦
g_TexasCard_type.sitiao = 8   		--四条
g_TexasCard_type.tonghuashun = 9   	--同花顺
g_TexasCard_type.datonghuashun = 10  --大同花顺

--聊天类型
g_TexasDefine.chat_txt = 1
g_TexasDefine.chat_img = 2
g_TexasDefine.chat_voice = 3

--牌定义
g_TexasDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413,
515,525
}													

g_TexasDefine.game_lockstate = {}
g_TexasDefine.game_lockstate.lock_no = 1
g_TexasDefine.game_lockstate.lock_yes = 2

g_TexasDefine.startPlayerNum = 2