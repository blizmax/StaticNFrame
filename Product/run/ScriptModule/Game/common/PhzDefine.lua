
g_phzDefine = {}

g_phzDefine.yes = 1
g_phzDefine.no = 0

--游戏类型
g_phzDefine.game_Paohz = 1		--跑胡子
g_phzDefine.game_Luods = 2		--落地扫
g_phzDefine.game_Liuhq = 3		--六胡抢
g_phzDefine.game_Shihk = 4		--十胡卡

--牌桌类型
g_phzDefine.table_gold = 1		--金币场
g_phzDefine.table_user = 2		--玩家使用开房卡自建的牌桌

--游戏状态
g_phzDefine.state_ready = 1		--准备状态，人数不足开局或一局结算后等待玩家确认继续
g_phzDefine.state_start = 2		--开始状态，人数足够且成功开局，此时发牌
g_phzDefine.state_play = 3		--玩牌状态，已经发完牌，各个玩家开始摸牌、打牌
g_phzDefine.state_end = 4		--结束状态，有玩家出现胡牌，此时进入结算展示，展示完成后不准备继续的玩家将被踢出房间
g_phzDefine.state_dismiss = 5	--解散状态，此状态的房间正等待所有的玩家退出，只能出不能进，并且不能开局
g_phzDefine.state_delete = 6	--删除状态，此状态的房间需要删除

--跑胡子玩家人数
g_phzDefine.init_sit_2 = 2		--2人房
g_phzDefine.init_sit_3 = 3		--3人房
g_phzDefine.init_sit_4 = 4		--4人房

--创建房间可选的局数
g_phzDefine.init_turns_8 = 8	--1个房卡开8局
g_phzDefine.init_turns_16 = 16	--2个房卡开16局

--翻醒与跟醒
g_phzDefine.wu_xing = 0			--不带醒
g_phzDefine.fan_xing = 1		--翻醒
g_phzDefine.gen_xing = 2		--跟醒

--多少胡息算作一囤
g_phzDefine.tun_per_1 = 1		--1息1囤
g_phzDefine.tun_per_3 = 3		--3息1囤

--点炮与胡牌
--g_phzDefine.hupai_self = 0		--无须必胡，自主选择
--g_phzDefine.hupai_youhu = 1		--有胡必胡，别人点炮必须胡，自摸也必须胡
--g_phzDefine.hupai_dianpao = 2	--点炮必胡，别人点炮必须胡，自摸无须必胡

--跑胡子底注与封顶
g_phzDefine.base_jetton = 1		--底注，在包房中基本底注是1
g_phzDefine.base_jetton_100 = 100	--在金币房的底注
g_phzDefine.max_jetton_300 = 300	--封顶300
g_phzDefine.max_jetton_600 = 600	--封顶600
g_phzDefine.max_jetton = 999999999	--无封顶
g_phzDefine.hz_jetton = 10		--黄庄的时候，一个王牌需要赔付的积分

--是否支持一五十
--g_phzDefine.yiWuShi_no = 0		--不支持一五十
--g_phzDefine.yiWuShi_yes = 1		--支持一五十

--数据列表分页后，单页的数据条数
g_phzDefine.per_page_num = 10	--单页数据条数

------------------------------------------------------------
-------begin---------跑胡子牌----------begin----------
------------------------------------------------------------
--跑胡子总牌数
g_phzCards = {}
g_phzCards.base_cards_num = 80		--不算癞子的基本牌数
g_phzCards.cards_paohz = 20			--十五胡手牌数
g_phzCards.cards_luods = 14			--落地扫手牌数
g_phzCards.cards_shihk = 20			--十胡卡手牌数
g_phzCards.cards_liuhq = 14			--六胡抢手牌数

--跑胡子牌面定义
--color:1黑色，2红色，3万能色(王牌可以当做任意颜色)
g_phzCards.all_cards = {
    ['id']={id=1,name=2,color=3,value=4,},
    [1]={1,  '一',1,101,},
	[2]={2,  '一',1,101,},
	[3]={3,  '一',1,101,},
	[4]={4,  '一',1,101,},
    [5]={5,  '二',2,102,},
	[6]={6,  '二',2,102,},
	[7]={7,  '二',2,102,},
	[8]={8,  '二',2,102,},
	[9]={9,  '三',1,103,},
	[10]={10,'三',1,103,},
	[11]={11,'三',1,103,},
	[12]={12,'三',1,103,},
	[13]={13,'四',1,104,},
	[14]={14,'四',1,104,},
	[15]={15,'四',1,104,},
	[16]={16,'四',1,104,},
	[17]={17,'五',1,105,},
	[18]={18,'五',1,105,},
	[19]={19,'五',1,105,},
	[20]={20,'五',1,105,},
	[21]={21,'六',1,106,},
	[22]={22,'六',1,106,},
	[23]={23,'六',1,106,},
	[24]={24,'六',1,106,},
	[25]={25,'七',2,107,},
	[26]={26,'七',2,107,},
	[27]={27,'七',2,107,},
	[28]={28,'七',2,107,},
	[29]={29,'八',1,108,},
	[30]={30,'八',1,108,},
	[31]={31,'八',1,108,},
	[32]={32,'八',1,108,},
	[33]={33,'九',1,109,},
	[34]={34,'九',1,109,},
	[35]={35,'九',1,109,},
	[36]={36,'九',1,109,},
	[37]={37,'十',2,110,},
	[38]={38,'十',2,110,},
	[39]={39,'十',2,110,},
	[40]={40,'十',2,110,},
	[41]={41,'壹',1,201,},
	[42]={42,'壹',1,201,},
	[43]={43,'壹',1,201,},
	[44]={44,'壹',1,201,},
    [45]={45,'贰',2,202,},
	[46]={46,'贰',2,202,},
	[47]={47,'贰',2,202,},
	[48]={48,'贰',2,202,},
	[49]={49,'叁',1,203,},
	[50]={50,'叁',1,203,},
	[51]={51,'叁',1,203,},
	[52]={52,'叁',1,203,},
	[53]={53,'肆',1,204,},
	[54]={54,'肆',1,204,},
	[55]={55,'肆',1,204,},
	[56]={56,'肆',1,204,},
	[57]={57,'伍',1,205,},
	[58]={58,'伍',1,205,},
	[59]={59,'伍',1,205,},
	[60]={60,'伍',1,205,},
	[61]={61,'陸',1,206,},
	[62]={62,'陸',1,206,},
	[63]={63,'陸',1,206,},
	[64]={64,'陸',1,206,},
	[65]={65,'柒',2,207,},
	[66]={66,'柒',2,207,},
	[67]={67,'柒',2,207,},
	[68]={68,'柒',2,207,},
	[69]={69,'捌',1,208,},
	[70]={70,'捌',1,208,},
	[71]={71,'捌',1,208,},
	[72]={72,'捌',1,208,},
	[73]={73,'玖',1,209,},
	[74]={74,'玖',1,209,},
	[75]={75,'玖',1,209,},
	[76]={76,'玖',1,209,},
	[77]={77,'拾',2,210,},
	[78]={78,'拾',2,210,},
	[79]={79,'拾',2,210,},
	[80]={80,'拾',2,210,},
	[81]={81,'王',3,301,},
	[82]={82,'王',3,301,},
	[83]={83,'王',3,301,},
	[84]={84,'王',3,301,},
}

function g_phzCards.GetValue(id, index)
    local indexID = g_phzCards.all_cards["id"][index]
    if g_phzCards.all_cards[id] == nil then return nil else return g_phzCards.all_cards[id][indexID] end
end

function g_phzCards.GetItem(id)
    return g_phzCards.all_cards[id]
end

g_phzCards.cardIndex = g_phzCards.all_cards["id"]

--跑胡子支持的王牌数目
g_phzCards.king_0 = 0			--不支持癞子
g_phzCards.king_1 = 1			--支持1个癞子
g_phzCards.king_2 = 2			--支持2个癞子
g_phzCards.king_3 = 3			--支持3个癞子
g_phzCards.king_4 = 4			--支持4个癞子
------------------------------------------------------------
-------end---------跑胡子牌----------end--------------
------------------------------------------------------------





------------------------------------------------------------
-------begin---------跑胡子时间设置----------begin----------
------------------------------------------------------------
--注意这不是秒数，而是经过的定时器tick数，目前0.2秒一个tick
g_phzTimeSet= {}
g_phzTimeSet.opening_show_time = 0		--跑胡子开场秀时间
g_phzTimeSet.get_card_time = 6			--跑胡子给予每个玩家摸牌的时间
g_phzTimeSet.get_card_time_ex = 6		--跑胡子给予每个玩家摸牌的时间，协议驱动的摸牌，间隔时间不准，避免太急就增大一个时钟
g_phzTimeSet.send_card_time = 0			--跑胡子给全体玩家发牌时间
g_phzTimeSet.play_card_time = 10		--跑胡子给予每个玩家接牌的决策时间，这是发给客户端的秒数
g_phzTimeSet.play_card_time_ex = 50		--跑胡子给予每个玩家接牌的决策时间，相当于10s
g_phzTimeSet.show_result_time = 50		--跑胡子给予客户端展示结算数据的时间，相当于10s
g_phzTimeSet.hu_card_time = 20			--跑胡子胡牌时间
g_phzTimeSet.dismiss_time = 1800		--跑胡子房间解散时间，局头创建的房间最多等待的时间
g_phzTimeSet.req_waite_time = 120		--发起解散申请后，默认等待玩家的时间

--未初始化的时间戳赋值
g_phzTimeSet.default_timemark = -1
------------------------------------------------------------
-------end---------跑胡子时间设置----------end--------------
------------------------------------------------------------





------------------------------------------------------------
-------begin---------跑胡子操作----------begin--------------
------------------------------------------------------------
--跑胡子打牌操作类型
g_phzOperType = {}
g_phzOperType.none = 0					--无操作
g_phzOperType.chu_pai = 1				--打出牌
g_phzOperType.mo_pai = 2				--摸牌
g_phzOperType.pass = 3					--过
g_phzOperType.chi = 4					--吃
g_phzOperType.xia_huo = 5				--下伙
g_phzOperType.peng = 6					--碰
g_phzOperType.pao = 7					--跑
g_phzOperType.hu_pai = 8				--胡牌
g_phzOperType.chou_xiao = 9				--臭啸
g_phzOperType.xiao = 10					--啸
g_phzOperType.qing = 11					--倾
g_phzOperType.wang_diao = 12			--王钓
g_phzOperType.wang_chuang = 13			--王闯
g_phzOperType.wang_zha = 14				--王炸
g_phzOperType.wang_zha_wang = 15		--王炸王
g_phzOperType.huang_zhuang = 16			--黄庄
g_phzOperType.si_shou = 17				--死守
g_phzOperType.pre_qing = 18				--起手倾牌
g_phzOperType.pre_mo_pai = 19			--庄家起手摸牌
g_phzOperType.jin_pai = 20				--玩家进牌，进牌后需要出牌
g_phzOperType.chong_pao = 21			--重跑
g_phzOperType.mo_pai_confirm = 22		--摸牌确认
g_phzOperType.delay_handler = 23		--延迟处理若干操作

--跑胡子打牌操作状态
g_phzOperState = {}
g_phzOperState.begin = 1				--操作开始
g_phzOperState.waiting = 2				--操作等待，例如等待玩家出牌、是否吃/碰等
g_phzOperState.finish = 3				--操作完成

--跑胡子打牌操作通知类型
g_phzOperNotify = {}
g_phzOperNotify.select = 1				--通知玩家有吃、碰、跑、胡等选择
g_phzOperNotify.waiting = 2				--玩家选择后，由于还有高优先级的玩家没有决策，这时通知玩家等待
g_phzOperNotify.execute = 3				--操作确定后，通知各个玩家直接执行操作
g_phzOperNotify.error = 4				--玩家操作纸牌提交的数据出错
g_phzOperNotify.ignore = 5				--玩家操作纸牌提交的数据出错后，直接忽略该消息
------------------------------------------------------------
-------end---------跑胡子操作----------end------------------
------------------------------------------------------------


------------------------------------------------------------
-------begin---------跑胡子牌型----------begin--------------
------------------------------------------------------------
g_phzCardType = {}
g_phzCardType.qing = 1					--倾，4张相同的牌，类似：九九九九
g_phzCardType.pao = 2					--跑，4张相同的牌，类似：九九九九
g_phzCardType.xiao = 3					--啸，3张相同的牌，类似：玖玖玖
g_phzCardType.chou_xiao = 4				--臭啸，3张相同的牌，类似：玖玖玖
g_phzCardType.peng = 5					--碰，3张相同的牌，类似：玖玖玖
g_phzCardType.chi = 6					--吃，类似：一二三、二七十、六七八、壹贰叁、贰柒拾、肆伍陆
g_phzCardType.jiao = 7					--绞，类似：一一壹、壹壹一、八八捌、捌捌八
g_phzCardType.dui = 8					--对，2张相同的牌，类似：贰贰
--下面的不能称为一方门子
g_phzCardType.duan_shun = 9				--短顺，类似：一二、六八、贰叁、肆伍
g_phzCardType.duan_chi = 10				--短吃，类似：二十、柒拾
g_phzCardType.duan_jiao = 11			--短绞，类似：一壹、八捌
g_phzCardType.dan = 12					--单，1张牌
g_phzCardType.king = 13					--王牌，专门将所有的王牌放在一起，不是一个正常的牌型
g_phzCardType.kingEx = 14				--结算的时候不得已将王牌拆分为两列，这是新拆出的一列王牌，为2或3张王牌
------------------------------------------------------------
-------end---------跑胡子牌型----------end------------------
------------------------------------------------------------


------------------------------------------------------------
-------begin---------跑胡子胡息定义----------begin----------
------------------------------------------------------------
g_phzHuxi = {}
g_phzHuxi.hu_paohz = 15			--十五胡胡牌至少需要的胡息
g_phzHuxi.hu_luods = 6			--落地扫胡牌至少需要的胡息
g_phzHuxi.hu_shihk = 10			--十胡卡胡牌至少需要的胡息
g_phzHuxi.hu_liuhq = 6			--六胡抢胡牌至少需要的胡息

g_phzHuxi.b_qing = 12		--大字牌倾：例如：玖玖玖+玖(自摸的牌)
g_phzHuxi.s_qing = 9		--小字牌倾：例如：九九九+九(自摸的牌)
g_phzHuxi.b_pao = 9			--大字牌跑：例如：玖玖玖+玖(打出的牌)
g_phzHuxi.s_pao = 6			--小字牌跑：例如：九九九+九(打出的牌)
g_phzHuxi.b_xiao = 6		--大字牌啸：例如：玖玖+玖(自摸的牌)
g_phzHuxi.s_xiao = 3		--小字牌啸：例如：九九+九(自摸的牌)
g_phzHuxi.b_chi = 6			--大字牌吃：只有【壹贰叁】与【贰柒拾】
g_phzHuxi.s_chi = 3			--小字牌吃：只有【一二三】与【二七十】
g_phzHuxi.b_peng = 3		--大字牌碰：例如：玖玖+玖(打出的牌)
g_phzHuxi.s_peng = 1		--小字牌碰：例如：九九+九(打出的牌)
------------------------------------------------------------
-------end-----------跑胡子胡息定义----------end------------
------------------------------------------------------------

------------------------------------------------------------
-------begin---------跑胡子胡牌类型----------begin----------
------------------------------------------------------------
--胡牌的类型及对应的番数
--1~5，如果有，只能有一个
--6~11，肯定有，只能有一个
g_phzHuPaiType = {
	--		id	牌型		弃用	十五胡番数	落地扫番数	十胡卡番数	六胡抢番数
    ['id']={id=1,name=2,	fan=3,	paohzFan=4,	luodsFan=5,	shihkFan=6,	liuhqFan=7},
	[1]={1,  	'自摸',		2,		2,			2,			2,			2,},
	[2]={2,  	'王钓',		4,		4,			4,			4,			4,},
	[3]={3,  	'王闯',		8,		8,			8,			8,			8,},
	[4]={4,  	'王炸',		16,		16,			16,			16,			16,},
	[5]={5,  	'王炸王',	32,		32,			32,			32,			32,},
	[6]={6,  	'平胡',		1,		1,			1,			1,			1,},
	[7]={7,  	'黑胡',		4,		4,			4,			5,			5,},
	[8]={8,  	'红胡',		2,		2,			2,			3,			2,},
	[9]={9, 	'点朱',		3,		3,			3,			3,			3,},
	[10]={10,  	'红转朱',	3,		3,			3,			3,			3,},
	[11]={11,  	'全红胡',	4,		4,			4,			5,			4,},
}

function g_phzHuPaiType.GetValue(id, index)
    local indexID = g_phzHuPaiType["id"][index]
    if g_phzHuPaiType[id] == nil then return nil else return g_phzHuPaiType[id][indexID] end
end

function g_phzHuPaiType.GetItem(id)
    return g_phzHuPaiType[id]
end

g_phzHuPaiType.idx = g_phzHuPaiType["id"]
------------------------------------------------------------
-------end---------跑胡子胡牌类型----------end--------------
------------------------------------------------------------
--臭牌类型
g_phzBadCardType = {}
g_phzBadCardType.all = 1			--对吃、碰来说都是臭牌
g_phzBadCardType.chi = 2			--对吃来说是臭牌
g_phzBadCardType.peng = 3			--对碰来说是臭牌

--聊天类型
g_phzChatType = {}
g_phzChatType.txt = 1				--自主文字聊天
g_phzChatType.quick_txt = 2			--快速文字聊天
g_phzChatType.emoji = 3				--表情聊天
g_phzChatType.voice = 4				--自主语音聊天
g_phzChatType.quick_voice = 5		--快速语音聊天
g_phzChatType.tx_msg_voice = 6		--腾讯消息语音

--排行榜类型
g_phzRankType = {}
g_phzRankType.money = 1				--土豪榜，钻石、房卡之类的排行榜
g_phzRankType.jetton = 2			--富豪榜，筹码、金币之类的排行榜
g_phzRankType.play = 3				--战斗榜，玩牌局数、获取积分之类的排行榜

--跑胡子解散房间时，玩家的回应
g_phzDismissResp = {}
g_phzDismissResp.none = 0			--未表态
g_phzDismissResp.agree = 1			--同意解散
g_phzDismissResp.reject = 2			--拒绝解散

--跑胡子明招
g_phzMingZhaoType = {}
g_phzMingZhaoType.confirm = 0		--消息确认，无须跳出提示框及回应
g_phzMingZhaoType.notice = 1		--仅仅提示明招不能出牌，无须回应
g_phzMingZhaoType.select = 2		--提示明招，需要玩家回应自己的选择

--跑胡子通知信息类型
g_phzMsgNotify = {}
g_phzMsgNotify.common = 0			--一般的通知类型
g_phzMsgNotify.mingzhao = 1			--明招通知
g_phzMsgNotify.hupai = 2			--胡牌时选择过牌的提示

--跑胡子任务
g_phzTaskUpdate = {}
g_phzTaskUpdate.submit = 1			--提交任务，如完成某次分享后向服务器提交
g_phzTaskUpdate.award = 2			--申请任务奖励
g_phzTaskUpdate.update = 3			--提交或领取奖励后，服务器通知客户端更新任务数据

--跑胡子签到
g_phzSignIn = {}
g_phzSignIn.info = 1				--获取签到信息
g_phzSignIn.award = 2				--领取签到奖励

--跑胡子申请代理状态
g_phzAgentStatus = {}
g_phzAgentStatus.none = 0					--未申请
g_phzAgentStatus.applied = 1				--已申请，正在处理中
g_phzAgentStatus.reject = 2					--拒绝申请，不详细说明原因
g_phzAgentStatus.reject_repeat = 3			--重复申请被拒绝
g_phzAgentStatus.reject_unqualified = 4		--不够资格，所以申请被拒绝
g_phzAgentStatus.reject_data = 5			--资料未填完整或资料不真实，所以申请被拒绝
g_phzAgentStatus.failed = 6					--非人工审核原因造成的申请失败
g_phzAgentStatus.succ = 7					--申请成功

--跑胡子代理相关的奖励类型
g_phzAgentAward = {}
g_phzAgentAward.bingding = 1				--首次绑定其他代理人的奖励
g_phzAgentAward.daily = 2					--绑定其他代理人，每日登录奖励
g_phzAgentAward.agent = 3					--作为代理人，根据绑定自己的人数领取的奖励

--跑胡子玩家在牌桌上的状态
g_phzPlayerStatus = {}
g_phzPlayerStatus.normal = 1				--正常，正常在线打牌
g_phzPlayerStatus.escrow = 2				--托管，系统托管打牌(包括：离线托管、长时间不操作造成的托管、主动申请托管)

--跑胡子玩家临时存储的数据类型
g_phzUserDataType = {}
g_phzUserDataType.ming_zhao = 1				--明招
g_phzUserDataType.gcmsg_pao = 2				--跑牌协议包
g_phzUserDataType.gcmsg_hu = 3				--胡牌协议包
g_phzUserDataType.gcmsg_result = 4			--牌桌的单局结算协议包
g_phzUserDataType.gcmsg_huangzhuang = 5		--牌桌的黄庄结算协议包
g_phzUserDataType.cgmsg_pass = 6			--玩家过牌
g_phzUserDataType.gcmsg_total_result = 7	--总结算协议包

--跑胡子功能模块类型
g_phzModuleType = {}
g_phzModuleType.none = 0					--未指定的类型
g_phzModuleType.redpack = 1					--红包
g_phzModuleType.activity = 2				--活动
g_phzModuleType.notice = 3					--登录公告
g_phzModuleType.guessredpacket = 4			--天天猜红包

--最大的有效类型，记得扩展增加类型后要同步更新
g_phzModuleType.min = g_phzModuleType.redpack
g_phzModuleType.max = g_phzModuleType.guessredpacket

--跑胡子金币场玩法设置
g_phzGoldGame = 
{
	--游戏类型				  游戏类型					支持玩家数		支持王牌数	跟醒或翻醒				封顶					x息=1囤		是否支持一五十	是否支持点炮	有胡必胡	自摸翻倍	红黑胡
    ['gametype']			={gametype=1,				playercnt=2,	kingcnt=3,	genxing=4,				maxjetton=5,			xitun=6,	yiwushi=7,		dianpao=8,		hupai=9,	double=10,	honghei=11,},
	[g_phzDefine.game_Paohz]={g_phzDefine.game_Paohz,  	3,				3,			g_phzDefine.gen_xing,	g_phzDefine.max_jetton,	3,			0,				0,				0,			0,			0,},
	[g_phzDefine.game_Luods]={g_phzDefine.game_Luods,  	3,				2,			g_phzDefine.gen_xing,	g_phzDefine.max_jetton,	1,			0,				1,				0,			0,			0,},
	[g_phzDefine.game_Liuhq]={g_phzDefine.game_Liuhq,  	3,				0,			g_phzDefine.gen_xing,	g_phzDefine.max_jetton,	3,			0,				1,				1,			1,			1,},
	[g_phzDefine.game_Shihk]={g_phzDefine.game_Shihk,  	3,				0,			g_phzDefine.gen_xing,	g_phzDefine.max_jetton,	3,			0,				1,				1,			1,			1,},
}

function g_phzGoldGame.GetValue(gametype, index)
	local indexID = g_phzGoldGame["gametype"][index]
	if g_phzGoldGame[gametype] == nil then return nil else return g_phzGoldGame[gametype][indexID] end
end

function g_phzGoldGame.GetItem(gametype)
	return g_phzGoldGame[gametype]
end

--金币场场次分级
g_phzGoldTableLevel = {}
g_phzGoldTableLevel.none = 0		--未确定
g_phzGoldTableLevel.junior = 1		--初级场
g_phzGoldTableLevel.medium = 2		--中级场
g_phzGoldTableLevel.senior = 3		--高级场

--跑胡子金币场数据设置
g_phzGoldTable_Paohz = 
{
	--场次级别						倍率				最小携带金币量	最大携带金币量	进入门槛描述
	['tablelevel']					={basejetton=1,		minjetton=2,	maxjetton=3,	description=4,},
	[g_phzGoldTableLevel.junior]	={10,  				1000,			100000,			"1千至10万金币"},
	[g_phzGoldTableLevel.medium]	={500,  			50000,			1000000,		"5万至100万金币"},
	[g_phzGoldTableLevel.senior]	={1000,  			500000,			-1,				"50万金币以上"},
}

--落地扫金币场数据设置
g_phzGoldTable_Luods = 
{
	--场次级别						倍率				最小携带金币量	最大携带金币量	进入门槛描述
	['tablelevel']					={basejetton=1,		minjetton=2,	maxjetton=3,	description=4,},
	[g_phzGoldTableLevel.junior]	={10,  				1000,			3000000,		"1千至300万金币"},
	[g_phzGoldTableLevel.medium]	={2000,  			500000,			10000000,		"50万至1000万金币"},
	[g_phzGoldTableLevel.senior]	={5000,  			3000000,		-1,				"300万金币以上"},
}

--六胡抢金币场数据设置
g_phzGoldTable_Liuhq = 
{
	--场次级别						倍率				最小携带金币量	最大携带金币量	进入门槛描述
	['tablelevel']					={basejetton=1,		minjetton=2,	maxjetton=3,	description=4,},
	[g_phzGoldTableLevel.junior]	={100,  			5000,			100000,			"5千至10万金币"},
	[g_phzGoldTableLevel.medium]	={500,  			50000,			1000000,		"5万至100万金币"},
	[g_phzGoldTableLevel.senior]	={1000,  			500000,			-1,				"50万金币以上"},
}

--十胡卡金币场数据设置
g_phzGoldTable_Shihk = 
{
	--场次级别						倍率				最小携带金币量	最大携带金币量	进入门槛描述
	['tablelevel']					={basejetton=1,		minjetton=2,	maxjetton=3,	description=4,},
	[g_phzGoldTableLevel.junior]	={100,  			5000,			100000,			"5千至10万金币"},
	[g_phzGoldTableLevel.medium]	={500,  			50000,			1000000,		"5万至100万金币"},
	[g_phzGoldTableLevel.senior]	={1000,  			500000,			-1,				"50万金币以上"},
}

g_phzGoldTable = {}
function g_phzGoldTable.GetValue(gametype, tablelevel, index)
	local goldTable = g_phzGoldTable_Paohz
	if gametype == g_phzDefine.game_Luods then
		goldTable = g_phzGoldTable_Luods
	elseif gametype == g_phzDefine.game_Liuhq then
		goldTable = g_phzGoldTable_Liuhq
	elseif gametype == g_phzDefine.game_Shihk then
		goldTable = g_phzGoldTable_Shihk
	end
	
	local indexID = goldTable["tablelevel"][index]
	if goldTable[tablelevel] == nil then return nil else return goldTable[tablelevel][indexID] end
end

function g_phzGoldTable.GetItem(gametype, tablelevel)
	local goldTable = g_phzGoldTable_Paohz
	if gametype == g_phzDefine.game_Luods then
		goldTable = g_phzGoldTable_Luods
	elseif gametype == g_phzDefine.game_Liuhq then
		goldTable = g_phzGoldTable_Liuhq
	elseif gametype == g_phzDefine.game_Shihk then
		goldTable = g_phzGoldTable_Shihk
	end
	
	return goldTable[tablelevel]
end

