
g_ernnDefine = {}

--斗牛玩法


g_ernnDefine.game_type = 3000
g_ernnType = {}				
g_ernnType.type_nnsz = 101    --牛牛上庄
g_ernnType.type_gdzj = 102    --固定庄家
g_ernnType.type_zyqz = 103    --自由抢庄
g_ernnType.type_mpqz = 104    --看牌抢庄
g_ernnType.type_tbnn = 105    --通比牛牛

--低分
g_ernnPour_type = {}
g_ernnPour_type.pour_1 = 1
g_ernnPour_type.pour_2 = 2
g_ernnPour_type.pour_4 = 4
g_ernnPour_type.pour_1_2 = 12    --低分1/2
g_ernnPour_type.pour_2_4 = 24    --低分2/4
g_ernnPour_type.pour_4_8 = 48    --低分4/8
g_ernnPour_type.pour_1_2_3 = 123    --低分4/8
g_ernnPour_type.pour_3_4_5 = 345    --低分4/8
--低分分数
g_ernnPour_fenshu = {}
g_ernnPour_fenshu[g_ernnPour_type.pour_1] = {1}
g_ernnPour_fenshu[g_ernnPour_type.pour_2] = {2}
g_ernnPour_fenshu[g_ernnPour_type.pour_4] = {4}
g_ernnPour_fenshu[g_ernnPour_type.pour_1_2] = {1,2}
g_ernnPour_fenshu[g_ernnPour_type.pour_2_4] = {2,4}
g_ernnPour_fenshu[g_ernnPour_type.pour_4_8] = {4,8}
g_ernnPour_fenshu[g_ernnPour_type.pour_1_2_3] = {1,2,3}
g_ernnPour_fenshu[g_ernnPour_type.pour_3_4_5] = {3,4,5}


--局数
g_ernnDefine.game_num1 = 10    --10局
g_ernnDefine.game_num2 = 20 	 --20局

--房费支付方式
g_ernnDefine.payment_owner = 1 --房主支付
g_ernnDefine.payment_A = 2     --AA支付
g_ernnDefine.payment_free = 3     --免付费

--上庄分数
g_ernnDefine.banker_min_pour1 = 0    --无
g_ernnDefine.banker_min_pour2 = 100  --100
g_ernnDefine.banker_min_pour3 = 150  --150
g_ernnDefine.banker_min_pour4 = 200  --150

--游戏进行的状态
g_ernnDefine.state_ready = 1		--在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_ernnDefine.state_banker = 2     --抢庄状态
g_ernnDefine.state_start  = 3     --游戏开始
g_ernnDefine.state_wait  = 4    	--等待下注
g_ernnDefine.state_play  = 15		--等待看牌 
g_ernnDefine.state_count  = 6		--结算    
g_ernnDefine.state_next = 7		--开始下一局游戏
g_ernnDefine.state_dissolve = 8   --申请解散
g_ernnDefine.state_delete  = 9    --等待删除状态


--
--状态运行的时间
g_ernnTime = {}
g_ernnTime.ready_time = 1800 --1800
g_ernnTime.pour_time = 10      	--下注的时间(也下面的等待下注时间，修改时一同修改)
g_ernnTime.showpoket_time = 13	--发牌 看牌的时间(也下面的发牌与等待亮牌，修改时一同修改)
g_ernnTime.dissolve_time = 60 --120 	--申请解散的时候，最长的解散时间
g_ernnTime.user_play = 15

g_ernnTime.wait_time =  9       		--等待下注时间
g_ernnTime.play_time =  13      		--发牌与等待亮牌
g_ernnTime.next_time =  15		--等待下一局时间
g_ernnTime.banker_time = 10 		--抢庄时间
g_ernnTime.count_time = 3		--结算时间


g_ernnDefine.max_user = 2		--最大人数
g_ernnDefine.table_jbc = 101	--金币场
g_ernnDefine.table_srf = 201 	--私人场

--牌点大小
g_ernnCard_type = {}
g_ernnCard_type.wu_niu = 0     	--无牛
g_ernnCard_type.niu_yi = 1     	--牛一
g_ernnCard_type.niu_er = 2     	--牛二
g_ernnCard_type.niu_san = 3    	--牛三
g_ernnCard_type.niu_si = 4     	--牛四
g_ernnCard_type.niu_wu = 5     	--牛五
g_ernnCard_type.niu_liu = 6    	--牛六
g_ernnCard_type.niu_qi = 7     	--牛七
g_ernnCard_type.niu_ba = 8     	--牛八
g_ernnCard_type.niu_jiu = 9   	--牛九
g_ernnCard_type.niu_niu = 10  	--牛牛
g_ernnCard_type.yin_niu = 11   	--银牛 四张花牌 一张10
g_ernnCard_type.jin_niu = 12 		--金牛/无花牛

--牌点赔率
g_ernnRate_type = {}
g_ernnRate_type[0] = 1
g_ernnRate_type[1] = 1
g_ernnRate_type[2] = 1
g_ernnRate_type[3] = 1
g_ernnRate_type[4] = 1
g_ernnRate_type[5] = 1
g_ernnRate_type[6] = 1
g_ernnRate_type[7] = 2
g_ernnRate_type[8] = 3
g_ernnRate_type[9] = 4
g_ernnRate_type[10] = 5
g_ernnRate_type[11] = 5
g_ernnRate_type[12] = 5

--聊天类型
g_ernnDefine.chat_txt = 1
g_ernnDefine.chat_img = 2
g_ernnDefine.chat_voice = 3

--牌点赔率类型
g_ernnRate_typeone = 1
g_ernnRate_typetwo = 2

--牌定义
g_ernnDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}

--玩家状态
g_ernnPlayStatus = {}
g_ernnPlayStatus.status_ready = 101   --没次开局前初始化成这个数值
g_ernnPlayStatus.status_banker = 102
g_ernnPlayStatus.status_wait = 103   --准备了之后就等待。等待玩家选择倍数
g_ernnPlayStatus.status_play = 104
g_ernnPlayStatus.status_next = 105    --每一轮结束后，点击准备，进入到这个状态
g_ernnPlayStatus.status_buqiang = 106 --106	



--最高抢庄倍数
g_ernnDefine.mul_one = 1  	--一倍
g_ernnDefine.mul_two = 2		--两倍
g_ernnDefine.mul_three = 3	--三倍
g_ernnDefine.mul_four = 4		--四倍

--限制 
g_ernnDefine.restrict_OK = 1		--游戏开始后可以进入房间
g_ernnDefine.restrict_NO = 0		--游戏开始后不能进入房间

--自动准备
g_ernnDefine.autoready_OK = 1		--自动准备
g_ernnDefine.autoready_NO = 0		--不自动准备

--游戏开始后坐下
g_ernnDefine.newsit_yes = 0		--游戏开始后坐下的
g_ernnDefine.newsit_no = 1		--游戏开始前坐下的

--自动亮牌
g_ernnDefine.volun_yes = 1 		--自动亮牌
g_ernnDefine.volun_no = 0 		--不自动亮牌


--支付类型
g_ernnDefine.diamond_pay = 0 		--钻石支付
g_ernnDefine.jade_pay =  1		--玉支付

--托管 
g_ernnDefine.trusteeship_no = 0	--不托管
g_ernnDefine.trusteeship_yes = 1	--托管

--需要托管的超时次数
g_ernnDefine.maxovertime = 2		--超时大于等于2是自动托管

-- 金币场桌子类型
g_ernnDefine.grade_jbc = {}           --
g_ernnDefine.grade_jbc['junior'] = 1
g_ernnDefine.grade_jbc['medium'] = 2
g_ernnDefine.grade_jbc['serior'] = 3
g_ernnDefine.grade_jbc['high'] = 4

g_ernnDefine.table_type = {1,2,3,4}


-- 金币场金币最低要求
g_ernnDefine.min_jetton = {}
g_ernnDefine.min_jetton[1] = 10000
g_ernnDefine.min_jetton[2] = 100000
g_ernnDefine.min_jetton[3] = 1000000
g_ernnDefine.min_jetton[4] = 10000000

g_ernnDefine.pourList = {}
g_ernnDefine.pourList[1] = 400
g_ernnDefine.pourList[2] = 1200
g_ernnDefine.pourList[3] = 3000

g_ernnDefine.max_qiang = 4    --最大的抢庄倍数是4倍

g_ernnDefine.jbc_pour_mul = {
	0.2,
	0.1,
	0.05,
	0.025
}


--机器人进入房间的金币区间
g_ernnDefine.RobotJettonSection = {}
g_ernnDefine.RobotJettonSection[1] = {20000, 100000}
g_ernnDefine.RobotJettonSection[2] = {80000, 500000}
g_ernnDefine.RobotJettonSection[3] = {500000, 10000000}
g_ernnDefine.RobotJettonSection[4] = {500000, 20000000}