g_ErshDefine = {}



g_ErshDefine.game_type = 4100   --和协议号是同一个号段的
--房费支付方式
g_ErshDefine.payment_owner = 1 		--房主支付
g_ErshDefine.payment_A = 2     		--AA支付
	
--游戏进行的状态
g_ErshDefine.state_ready = 1		--等待游戏开始，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_ErshDefine.state_start  = 2    	--游戏开始的状态
g_ErshDefine.state_deal = 3			--发牌
g_ErshDefine.state_play = 4			--玩家行动的状态
g_ErshDefine.state_count = 5		--结算的状态    
g_ErshDefine.state_next = 6			--等待开始下一局游戏的状态
g_ErshDefine.state_dissolve = 7  	--申请解散的状态
g_ErshDefine.state_delete  = 8  	--等待删除状态的状态

--状态运行的时间
g_ErshTime = {}
g_ErshTime.ready_time = 1800 		--1800游戏还没开始自动解散房间
g_ErshTime.start_time = 1	--1      	--游戏开始及做动画的时间
g_ErshTime.deal_time = 1				--发牌时间
g_ErshTime.play_time =  30		--10	    	--每位玩家每回的操作的时间
g_ErshTime.count_time = 4		--5  			--结算及做动画的时间
g_ErshTime.next_time = 3			--等待下一局时间
g_ErshTime.dissolve_time = 60 		--申请解散的时候，最长的解散时间


g_ErshDefine.max_user = 6			--最大人数
g_ErshDefine.table_elementary = 1	--初级场
g_ErshDefine.table_middle = 2		--中级场
g_ErshDefine.table_jadvanced = 3	--高级场
g_ErshDefine.table_high = 4		-- 
g_ErshDefine.table_jbc_list = {		-- 用于做循环
	g_ErshDefine.table_elementary,
	g_ErshDefine.table_middle,
	g_ErshDefine.table_jadvanced,
	g_ErshDefine.table_high,
}

g_ErshDefine.table_srf = 201 		--私人场

-- 金币场金币最低要求
g_ErshDefine.min_jetton = {}
g_ErshDefine.min_jetton[g_ErshDefine.table_elementary] = 10000
g_ErshDefine.min_jetton[g_ErshDefine.table_middle] = 100000
g_ErshDefine.min_jetton[g_ErshDefine.table_jadvanced] = 1000000
g_ErshDefine.min_jetton[g_ErshDefine.table_high] = 10000000

--牌型
g_ErshCard_type = {}
g_ErshCard_type.teshu = -1    		--特殊牌
g_ErshCard_type.danzhang = 0     	--单张
g_ErshCard_type.duizi = 1     		--对子
g_ErshCard_type.xiaoshun = 2    		--小顺
g_ErshCard_type.shunzi = 3    		--顺子
g_ErshCard_type.tonghua = 4     		--同花
g_ErshCard_type.tonghuashun = 5   	--同花顺
g_ErshCard_type.baozi = 6     		--豹子


--底分
g_ErshDefine_difen = {}
g_ErshDefine_difen[1] = 1
g_ErshDefine_difen[123] = 1
g_ErshDefine_difen[1234] = 1

g_ErshDefine_difenlist = {}
g_ErshDefine_difenlist[123] = {1,2,3}
g_ErshDefine_difenlist[1234] = {1,2,3,4}


--聊天类型
g_ErshDefine.chat_txt = 1
g_ErshDefine.chat_img = 2
g_ErshDefine.chat_voice = 3

--牌定义
g_ErshDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}													


--玩家状态
g_ErshPlayStatus = {}
g_ErshPlayStatus.status_ready = 101			
g_ErshPlayStatus.status_wait = 102   	--等待行动
g_ErshPlayStatus.status_play = 103		--正在行动
g_ErshPlayStatus.status_next = 104    	--准备等待


--玩家行动类型
g_ErshDefine.abandonpoker = 0			--弃牌
g_ErshDefine.lookpoker = 1				--看牌
g_ErshDefine.comparepoker = 2			--比牌
g_ErshDefine.upjetton = 3				--加注
g_ErshDefine.followjetton = 4			--跟注


--牌的状态
g_ErshPokerStatus = {} 
g_ErshPokerStatus.poker_lookno = 0    	--还没看牌
g_ErshPokerStatus.poker_looked = 1    	--看了牌
g_ErshPokerStatus.poker_abandon = 2    --弃牌

--限制 
g_ErshDefine.restrict_OK = 1				--游戏开始后可以进入房间
g_ErshDefine.restrict_NO = 0				--游戏开始后不能进入房间


--游戏开始后坐下
g_ErshDefine.newsit_yes = 0				--游戏开始后坐下的
g_ErshDefine.newsit_no = 1				--游戏开始前坐下的


--支付类型
g_ErshDefine.diamond_pay = 0 			--钻石支付
g_ErshDefine.jade_pay =  1				--玉支付
g_ErshDefine.jetton_pay =  2				--金币支付

--比牌倍数
g_ErshDefine.doublecomp_two = 0 			--双倍比牌
g_ErshDefine.doublecomp_one = 1 			--单倍比牌

--最大投注回合
g_ErshDefine.boutcapp_5 = 5				--五轮封顶	
g_ErshDefine.boutcapp_10 = 10			--十轮封顶
g_ErshDefine.boutcapp_15 = 15			--十五轮封顶

--比牌起始回合
g_ErshDefine.mincomp_1 = 1				--一轮可比
g_ErshDefine.mincomp_2 = 2				--二轮可比
g_ErshDefine.mincomp_3 = 3				--三轮可比

--闷牌
g_ErshDefine.menpai_0 = 0				--不闷牌
g_ErshDefine.menpai_1 = 1				--闷一轮
g_ErshDefine.menpai_2 = 2				--闷二轮
g_ErshDefine.menpai_3 = 3				--闷三轮

--金币场最大空闲房间
g_ErshDefine.jettontable = 10            --金币场同时最大有空位房间数

--自动准备
g_ErshDefine.autoready_OK = 1			--自动准备
g_ErshDefine.autoready_NO = 0			--不自动准备


--底分 
g_ErshPour_type = {}
g_ErshPour_type.pour_123 = 123
g_ErshPour_type.pour_1234 = 1234

g_ErshPour_fenshu = {}
g_ErshPour_fenshu[g_ErshPour_type.pour_123] = {1, 2, 3}
g_ErshPour_fenshu[g_ErshPour_type.pour_1234] = {1, 2, 3, 4}

--拼三张玩法

g_ErshType = {}				
g_ErshType.type_ersh = 301    --普通玩法

-- Ante:在每个起手牌之前要求所有玩家必须投入的强制性赌注（仅用于使用 "ante" 的游戏，如七张牌梭哈）。
-- 二人梭哈
g_ErshDefine.opt_invalid = 0		-- 无效
g_ErshDefine.opt_fold = 1			-- 弃牌
g_ErshDefine.opt_no_raise = 2		-- 不加
g_ErshDefine.opt_call = 3			-- 跟注
g_ErshDefine.opt_raise = 4			-- 加注
g_ErshDefine.opt_show_hand = 5		-- 梭哈

-- 加注选项相对于底注的倍数
g_ErshDefine.raise_mul = {1, 2, 3, 4}	
-- 手牌数量
g_ErshDefine.hand_card_count = 5

-- 牌型
g_ErshPattern = {}
g_ErshPattern.invalid = 0
g_ErshPattern.shanPai = 1				-- 散排
g_ErshPattern.duiZi = 2					-- 对子
g_ErshPattern.liangDui = 3				-- 两对
g_ErshPattern.sanTiao = 4				-- 三条
g_ErshPattern.xiaoShunZi = 5			-- 小顺子, Ace,8,9,10,Jack 组成的顺子
g_ErshPattern.shunZi = 6				-- 顺子
g_ErshPattern.tongHua = 7				-- 同花
g_ErshPattern.huLu = 8					-- 葫芦
g_ErshPattern.tieZhi = 9				-- 铁支
g_ErshPattern.xiaoTongHuaShun = 10		-- 小同花顺, Ace,8,9,10,Jack 组成的顺子
g_ErshPattern.tongHuaShun = 11			-- 同花顺
g_ErshPattern.huangJiaTongHuaShun = 12	-- 皇家同花顺

g_ErshDefine.robot_range = {}
g_ErshDefine.robot_range[g_ErshDefine.table_elementary] = {10000,100000}
g_ErshDefine.robot_range[g_ErshDefine.table_middle] = {100000, 1000000}
g_ErshDefine.robot_range[g_ErshDefine.table_jadvanced] = {300000,10000000}
g_ErshDefine.robot_range[g_ErshDefine.table_high] = {500000, 50000000}
