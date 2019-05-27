
g_paiJiuDefine = {}


---------------------
-- 卡牌牌值定义
g_paiJiuCardDefine = {}
g_paiJiuCardDefine.tian = 66
g_paiJiuCardDefine.di = 11
g_paiJiuCardDefine.ren = 44
g_paiJiuCardDefine.e = 13					-- 鹅/和
g_paiJiuCardDefine.mei = 55
g_paiJiuCardDefine.changSan = 33
g_paiJiuCardDefine.banDeng = 22
g_paiJiuCardDefine.fuTou = 56
g_paiJiuCardDefine.hongTouShi = 46		-- 红头十/四六
g_paiJiuCardDefine.gaoJiaoQi = 16			-- 高脚七/么六
g_paiJiuCardDefine.tongChuiLiu = 15		-- 铜锤六/么六

g_paiJiuCardDefine.heiJiu = 36
g_paiJiuCardDefine.hongJiu = 45
g_paiJiuCardDefine.wanBa = 35
g_paiJiuCardDefine.pingBa = 26
g_paiJiuCardDefine.hongQi = 34
g_paiJiuCardDefine.heiQi = 25
g_paiJiuCardDefine.erSi = 24				-- 二四/红点六
g_paiJiuCardDefine.hongWu = 14
g_paiJiuCardDefine.heiWu = 23
g_paiJiuCardDefine.dingSan = 12			-- 丁三/红三点

for k, v in pairs(g_paiJiuCardDefine) do
	g_paiJiuCardDefine[v] = k
end

g_paiJiuCardDefine.wenList = {
	g_paiJiuCardDefine.tian,
	g_paiJiuCardDefine.di,
	g_paiJiuCardDefine.ren,
	g_paiJiuCardDefine.e,
	g_paiJiuCardDefine.mei,
	g_paiJiuCardDefine.changSan,
	g_paiJiuCardDefine.banDeng,
	g_paiJiuCardDefine.fuTou,
	g_paiJiuCardDefine.hongTouShi,
	g_paiJiuCardDefine.gaoJiaoQi,
	g_paiJiuCardDefine.tongChuiLiu
}

g_paiJiuCardDefine.wuList = {
	g_paiJiuCardDefine.heiJiu,
	g_paiJiuCardDefine.hongJiu,
	g_paiJiuCardDefine.wanBa,
	g_paiJiuCardDefine.pingBa,
	g_paiJiuCardDefine.hongQi,
	g_paiJiuCardDefine.heiQi,
	g_paiJiuCardDefine.erSi,
	g_paiJiuCardDefine.hongWu,
	g_paiJiuCardDefine.heiWu,
	g_paiJiuCardDefine.dingSan
}


-- 牌型定义
g_paiJiuPatternDefine = {}
g_paiJiuPatternDefine.invalid = -1
g_paiJiuPatternDefine.point0 = 0
g_paiJiuPatternDefine.point1 = 1
g_paiJiuPatternDefine.point2 = 2
g_paiJiuPatternDefine.point3 = 3
g_paiJiuPatternDefine.point4 = 4
g_paiJiuPatternDefine.point5 = 5
g_paiJiuPatternDefine.point6 = 6
g_paiJiuPatternDefine.point7 = 7
g_paiJiuPatternDefine.point8 = 8
g_paiJiuPatternDefine.point9 = 9
g_paiJiuPatternDefine.diGaoJiu = 11
g_paiJiuPatternDefine.tianGaoJiu = 12
g_paiJiuPatternDefine.diGang = 13
g_paiJiuPatternDefine.tianGang = 14
g_paiJiuPatternDefine.diWang = 15
g_paiJiuPatternDefine.tianWang = 16
g_paiJiuPatternDefine.zaWu = 17
g_paiJiuPatternDefine.zaQi = 18
g_paiJiuPatternDefine.zaBa = 19
g_paiJiuPatternDefine.zaJiu = 20
g_paiJiuPatternDefine.shuangLingLin = 21
g_paiJiuPatternDefine.shuangGaoJiao = 22
g_paiJiuPatternDefine.shuangHongTou = 23
g_paiJiuPatternDefine.shuangFuTou = 24
g_paiJiuPatternDefine.shuangBanDeng = 25
g_paiJiuPatternDefine.shuangChangSan = 26
g_paiJiuPatternDefine.shuangMei = 27
g_paiJiuPatternDefine.shuangE = 28
g_paiJiuPatternDefine.shuangRen = 29
g_paiJiuPatternDefine.shuangDi = 30
g_paiJiuPatternDefine.shuangTian = 31
g_paiJiuPatternDefine.zhiZun = 32

-- 用于反向查询
for k, v in pairs(g_paiJiuPatternDefine) do
	g_paiJiuPatternDefine[v] = k
end
---------------------

--斗牛玩法
g_paiJiuType = {}				
g_paiJiuType.type_nnsz = 501    --牛牛上庄
g_paiJiuType.type_gdzj = 502    --固定庄家
g_paiJiuType.type_zyqz = 503    --自由抢庄
g_paiJiuType.type_mpqz = 504    --看牌抢庄
g_paiJiuType.type_tbnn = 505    --通比牛牛

-- 牌九牌组数量
g_paiJiuDefine.xiao_pai_jiu = 1
g_paiJiuDefine.da_pai_jiu = 2

--低分
g_paiJiuPour_type = {}
g_paiJiuPour_type.pour_1 = 1
g_paiJiuPour_type.pour_2 = 2
g_paiJiuPour_type.pour_4 = 4
g_paiJiuPour_type.pour_1_2 = 12    --低分1/2
g_paiJiuPour_type.pour_2_4 = 24    --低分2/4
g_paiJiuPour_type.pour_4_8 = 48    --低分4/8
g_paiJiuPour_type.pour_1_2_3 = 123    --低分4/8
g_paiJiuPour_type.pour_3_4_5 = 345    --低分4/8
--低分分数
g_paiJiuPour_fenshu = {}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_1] = {1}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_2] = {2}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_4] = {4}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_1_2] = {1,2}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_2_4] = {2,4}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_4_8] = {4,8}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_1_2_3] = {1,2,3}
g_paiJiuPour_fenshu[g_paiJiuPour_type.pour_3_4_5] = {3,4,5}


--局数
g_paiJiuDefine.game_num1 = 10    --10局
g_paiJiuDefine.game_num2 = 20 	 --20局

--房费支付方式
g_paiJiuDefine.payment_owner = 1 --房主支付
g_paiJiuDefine.payment_A = 2     --AA支付
g_paiJiuDefine.payment_free = 3     --免付费

--上庄分数
g_paiJiuDefine.banker_min_pour1 = 0    --无
g_paiJiuDefine.banker_min_pour2 = 100  --100
g_paiJiuDefine.banker_min_pour3 = 150  --150
g_paiJiuDefine.banker_min_pour4 = 200  --150

--游戏进行的状态
g_paiJiuDefine.state_ready = 1		--在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_paiJiuDefine.state_banker = 2     --抢庄状态
g_paiJiuDefine.state_start  = 3     --游戏开始
g_paiJiuDefine.state_wait  = 4    	--等待下注
g_paiJiuDefine.state_play  = 5		--等待看牌 
g_paiJiuDefine.state_count  = 6		--结算    
g_paiJiuDefine.state_next = 7		--开始下一局游戏
g_paiJiuDefine.state_dissolve = 8   --申请解散
g_paiJiuDefine.state_delete  = 9    --等待删除状态


--
--状态运行的时间
g_paiJiuTime = {}
g_paiJiuTime.ready_time = 1800 --1800
g_paiJiuTime.pour_time = 10      	--下注的时间(也下面的等待下注时间，修改时一同修改)
g_paiJiuTime.showpoket_time = 13	--发牌 看牌的时间(也下面的发牌与等待亮牌，修改时一同修改)
g_paiJiuTime.dissolve_time = 60 --120 	--申请解散的时候，最长的解散时间
g_paiJiuTime.user_play = 15

g_paiJiuTime.wait_time = 13		--13  	       		--等待下注时间
g_paiJiuTime.play_time = 15 	--13      		--发牌与等待亮牌
g_paiJiuTime.next_time = 10 	--15		--等待下一局时间
g_paiJiuTime.banker_time = 12	--10 		--抢庄时间
g_paiJiuTime.count_time = 5  	--5		--结算时间


g_paiJiuDefine.max_user = 6		--最大人数
g_paiJiuDefine.table_jbc = 101	--金币场
g_paiJiuDefine.table_srf = 201 	--私人场

--牌点大小
g_paiJiuCard_type = {}
g_paiJiuCard_type.wu_niu = 0     	--无牛
g_paiJiuCard_type.niu_yi = 1     	--牛一
g_paiJiuCard_type.niu_er = 2     	--牛二
g_paiJiuCard_type.niu_san = 3    	--牛三
g_paiJiuCard_type.niu_si = 4     	--牛四
g_paiJiuCard_type.niu_wu = 5     	--牛五
g_paiJiuCard_type.niu_liu = 6    	--牛六
g_paiJiuCard_type.niu_qi = 7     	--牛七
g_paiJiuCard_type.niu_ba = 8     	--牛八
g_paiJiuCard_type.niu_jiu = 9   	--牛九
g_paiJiuCard_type.niu_niu = 10  	--牛牛
g_paiJiuCard_type.zha_dan = 11   	--炸弹牛
g_paiJiuCard_type.wu_hua = 12 		--五花牛
g_paiJiuCard_type.wu_xiao = 13   	--五小牛

g_paiJiuCard_type.shunzi_niu = 14  	--顺子牛
g_paiJiuCard_type.tonghua_niu = 15  --同花牛
g_paiJiuCard_type.hulu_niu = 16  	--葫芦牛


--聊天类型
g_paiJiuDefine.chat_txt = 1
g_paiJiuDefine.chat_img = 2
g_paiJiuDefine.chat_voice = 3

--牌点赔率类型
g_paiJiuRate_typeone = 1
g_paiJiuRate_typetwo = 2

--牌定义
g_paiJiuDefine.pokerList = {}
for i = 1, 2 do	
	for _, v in ipairs(g_paiJiuCardDefine.wenList) do
		table.insert(g_paiJiuDefine.pokerList, v)
	end
end
for _, v in ipairs(g_paiJiuCardDefine.wuList) do
	table.insert(g_paiJiuDefine.pokerList, v)
end

--玩家状态
g_paijiuPlayStatus = {}
g_paijiuPlayStatus.status_ready = 101   --没次开局前初始化成这个数值
g_paijiuPlayStatus.status_banker = 102
g_paijiuPlayStatus.status_wait = 103   --准备了之后就等待。等待玩家选择倍数
g_paijiuPlayStatus.status_play = 104
g_paijiuPlayStatus.status_next = 105    --105
g_paijiuPlayStatus.status_buqiang = 106 --106	


--最高抢庄倍数
g_paiJiuDefine.mul_one = 1  	--一倍
g_paiJiuDefine.mul_two = 2		--两倍
g_paiJiuDefine.mul_three = 3	--三倍
g_paiJiuDefine.mul_four = 4		--四倍
g_paiJiuDefine.mul_five = 5		--五倍

--限制 
g_paiJiuDefine.restrict_OK = 1		--游戏开始后可以进入房间
g_paiJiuDefine.restrict_NO = 0		--游戏开始后不能进入房间

--自动准备
g_paiJiuDefine.autoready_OK = 1		--自动准备
g_paiJiuDefine.autoready_NO = 0		--不自动准备

--游戏开始后坐下
g_paiJiuDefine.newsit_yes = 0		--游戏开始后坐下的
g_paiJiuDefine.newsit_no = 1		--游戏开始前坐下的

--自动亮牌
g_paiJiuDefine.volun_yes = 1 		--自动亮牌
g_paiJiuDefine.volun_no = 0 		--不自动亮牌


--支付类型
g_paiJiuDefine.diamond_pay = 0 		--钻石支付
g_paiJiuDefine.jade_pay =  1		--玉支付

--托管 
g_paiJiuDefine.trusteeship_no = 0	--不托管
g_paiJiuDefine.trusteeship_yes = 1	--托管

--需要托管的超时次数
g_paiJiuDefine.maxovertime = 2		--超时大于等于2是自动托管

--自动开桌
g_paiJiuAutoStart = {}
g_paiJiuAutoStart_Manual = 99		--手动开桌
g_paiJiuAutoStart_Auto_4 = 4		--满4人开
g_paiJiuAutoStart_Auto_5 = 5		--满5人开
g_paiJiuAutoStart_Auto_6 = 6		--满6人开
g_paiJiuAutoStart_Auto_7 = 7		--满7人开
g_paiJiuAutoStart_Auto_8 = 8		--满8人开

--最大推注：
g_paiJiuTZ = {}
g_paiJiuTZ_no = 0					--无
g_paiJiuTZ_10 = 10					--10倍
g_paiJiuTZ_20 = 20					--20倍
g_paiJiuTZ_30 = 30					--30倍
g_paiJiuTZ_40 = 40					--40倍

	