
g_douNiuDefine = {}

--斗牛玩法
g_douNiuType = {}				
g_douNiuType.type_nnsz = 101    --牛牛上庄
g_douNiuType.type_gdzj = 102    --固定庄家
g_douNiuType.type_zyqz = 103    --自由抢庄
g_douNiuType.type_mpqz = 104    --看牌抢庄
g_douNiuType.type_tbnn = 105    --通比牛牛

--低分
g_douNiuPour_type = {}
g_douNiuPour_type.pour_1 = 1
g_douNiuPour_type.pour_2 = 2
g_douNiuPour_type.pour_4 = 4
g_douNiuPour_type.pour_5 = 5
g_douNiuPour_type.pour_8 = 8
g_douNiuPour_type.pour_10 = 10
g_douNiuPour_type.pour_1_2 = 12    --低分1/2
g_douNiuPour_type.pour_2_4 = 24    --低分2/4
g_douNiuPour_type.pour_3_6 = 36    --低分2/4
g_douNiuPour_type.pour_4_8 = 48    --低分4/8
g_douNiuPour_type.pour_5_10 = 510    --低分4/8
g_douNiuPour_type.pour_1_2_3 = 123    --低分4/8
g_douNiuPour_type.pour_3_4_5 = 345    --低分4/8
g_douNiuPour_type.pour_10_20 = 1020    --低分4/8


--低分分数
g_douNiuPour_fenshu = {}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_1] = {1}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_2] = {2}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_4] = {4}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_5] = {5}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_8] = {8}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_10] = {10}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_1_2] = {1,2}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_2_4] = {2,4}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_3_6] = {3,6}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_4_8] = {4,8}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_5_10] = {5,10}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_1_2_3] = {1,2,3}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_3_4_5] = {3,4,5}
g_douNiuPour_fenshu[g_douNiuPour_type.pour_10_20] = {10,20}


--局数
g_douNiuDefine.game_num1 = 10    --10局
g_douNiuDefine.game_num2 = 20 	 --20局

--房费支付方式
g_douNiuDefine.payment_owner = 1 --房主支付
g_douNiuDefine.payment_A = 2     --AA支付
g_douNiuDefine.payment_free = 3     --免付费

--上庄分数
g_douNiuDefine.banker_min_pour1 = 0    --无
g_douNiuDefine.banker_min_pour2 = 100  --100
g_douNiuDefine.banker_min_pour3 = 150  --150
g_douNiuDefine.banker_min_pour4 = 200  --150

--游戏进行的状态
g_douNiuDefine.state_ready = 1		--在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_douNiuDefine.state_banker = 2     --抢庄状态
g_douNiuDefine.state_start  = 3     --游戏开始
g_douNiuDefine.state_wait  = 4    	--等待下注
g_douNiuDefine.state_play  = 5		--等待看牌 
g_douNiuDefine.state_count  = 6		--结算    
g_douNiuDefine.state_next = 7		--开始下一局游戏
g_douNiuDefine.state_dissolve = 8   --申请解散
g_douNiuDefine.state_delete  = 9    --等待删除状态


--
--状态运行的时间
g_douNiuTime = {}
g_douNiuTime.ready_time = 43200 --1800
g_douNiuTime.pour_time = 5      	--下注的时间(也下面的等待下注时间，修改时一同修改)
g_douNiuTime.showpoket_time = 8	--发牌 看牌的时间(也下面的发牌与等待亮牌，修改时一同修改)
g_douNiuTime.dissolve_time = 60 --120 	--申请解散的时候，最长的解散时间
g_douNiuTime.user_play = 15

g_douNiuTime.wait_time = 8		--13  	       		--等待下注时间
g_douNiuTime.play_time = 8 	--13      		--发牌与等待亮牌
g_douNiuTime.next_time = 5 	--15		--等待下一局时间
g_douNiuTime.banker_time = 4	--10 		--抢庄时间
g_douNiuTime.count_time = 3  	--3		--结算时间


g_douNiuDefine.max_user = 6		--最大人数
g_douNiuDefine.table_jbc = 101	--金币场
g_douNiuDefine.table_srf = 201 	--私人场

--牌点大小
g_douNiuCard_type = {}
g_douNiuCard_type.wu_niu = 0     	--无牛
g_douNiuCard_type.niu_yi = 1     	--牛一
g_douNiuCard_type.niu_er = 2     	--牛二
g_douNiuCard_type.niu_san = 3    	--牛三
g_douNiuCard_type.niu_si = 4     	--牛四
g_douNiuCard_type.niu_wu = 5     	--牛五
g_douNiuCard_type.niu_liu = 6    	--牛六
g_douNiuCard_type.niu_qi = 7     	--牛七
g_douNiuCard_type.niu_ba = 8     	--牛八
g_douNiuCard_type.niu_jiu = 9   	--牛九
g_douNiuCard_type.niu_niu = 10  	--牛牛
g_douNiuCard_type.shunzi_niu = 11  	--顺子牛
g_douNiuCard_type.tonghua_niu = 12  --同花牛
g_douNiuCard_type.wu_hua = 13 		--五花牛
g_douNiuCard_type.hulu_niu = 14  	--葫芦牛
g_douNiuCard_type.zha_dan = 15   	--炸弹牛
g_douNiuCard_type.wu_xiao = 16   	--五小牛
g_douNiuCard_type.chongtian_niu = 17   	--冲天牛

--聊天类型
g_douNiuDefine.chat_txt = 1
g_douNiuDefine.chat_img = 2
g_douNiuDefine.chat_voice = 3

--牌点赔率类型
g_douNiuRate_typeone = 1
g_douNiuRate_typetwo = 2

--牌定义
g_douNiuDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413,
515,525				--515小王   525 大王
}

--玩家状态
g_douniuPlayStatus = {}
g_douniuPlayStatus.status_ready = 101   --没次开局前初始化成这个数值
g_douniuPlayStatus.status_banker = 102
g_douniuPlayStatus.status_wait = 103   --准备了之后就等待。等待玩家选择倍数
g_douniuPlayStatus.status_play = 104
g_douniuPlayStatus.status_next = 105    --105
g_douniuPlayStatus.status_buqiang = 106 --106	


--最高抢庄倍数
g_douNiuDefine.mul_one = 1  	--一倍
g_douNiuDefine.mul_two = 2		--两倍
g_douNiuDefine.mul_three = 3	--三倍
g_douNiuDefine.mul_four = 4		--四倍
g_douNiuDefine.mul_five = 5		--五倍

--限制 
g_douNiuDefine.restrict_OK = 1		--游戏开始后可以进入房间
g_douNiuDefine.restrict_NO = 0		--游戏开始后不能进入房间

--自动准备
g_douNiuDefine.autoready_OK = 1		--自动准备
g_douNiuDefine.autoready_NO = 0		--不自动准备

--游戏开始后坐下
g_douNiuDefine.newsit_yes = 0		--游戏开始后坐下的
g_douNiuDefine.newsit_no = 1		--游戏开始前坐下的

--自动亮牌
g_douNiuDefine.volun_yes = 1 		--自动亮牌
g_douNiuDefine.volun_no = 0 		--不自动亮牌


--支付类型
g_douNiuDefine.diamond_pay = 0 		--钻石支付
g_douNiuDefine.jade_pay =  1		--玉支付

--托管 
g_douNiuDefine.trusteeship_no = 0	--不托管
g_douNiuDefine.trusteeship_yes = 1	--托管

--需要托管的超时次数
g_douNiuDefine.maxovertime = 2		--超时大于等于2是自动托管

--自动开桌
g_douNiuAutoStart = {}
g_douNiuAutoStart_Manual = 99		--手动开桌
g_douNiuAutoStart_Auto_4 = 4		--满4人开
g_douNiuAutoStart_Auto_5 = 5		--满5人开
g_douNiuAutoStart_Auto_6 = 6		--满6人开
g_douNiuAutoStart_Auto_7 = 7		--满7人开
g_douNiuAutoStart_Auto_8 = 8		--满8人开

--最大推注：
g_douNiuTZ = {}
g_douNiuTZ_no = 0					--无
g_douNiuTZ_10 = 10					--10倍
g_douNiuTZ_20 = 20					--20倍
g_douNiuTZ_30 = 30					--30倍
g_douNiuTZ_40 = 40					--40倍

g_douNiuDefine.spc_win = 101
g_douNiuDefine.spc_lose = 102