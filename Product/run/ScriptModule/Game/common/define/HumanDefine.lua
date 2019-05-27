
g_channel = {}


g_initData = {}  -- 初始化的数据
g_initData.jetton = 3000
g_initData.money = 0   --初始化钻石


if g_isDebug == 1 then
	g_initData.jetton = 100000
	g_initData.money = 200   --初始化钻石	
end

if g_servername == "run_hubmj" or g_servername == "run_zsmj" or g_servername == "run_xwpk" then
	g_initData.jetton = 20000
	g_initData.money = 8   --初始化钻石	
elseif g_servername == "run_gdmj" then
	g_initData.jetton = 20000
	g_initData.money = 20   --初始化钻石
elseif g_servername == "run_ylmj" then
	g_initData.jetton = 20000
	g_initData.money = 8   --初始化钻石	
end

g_broadCast = {}
g_broadCast.cycle_login = 1   --玩家登陆进来的时候，系统发送的消息，只发送一次
g_broadCast.cycle_1 = 2    --每一分钟循环发送
g_broadCast.cycle_10 = 3   --每十分钟循环发送
g_broadCast.cycle_30 = 4   --每30分钟循环发送
g_broadCast.cycle_60 = 5   --每60分钟循环发送
g_broadCast.cycle_24 = 6   --每天系统时间重置时候发送



g_broadCast.type_login = 101  --系统广播,玩家一登录进来需要发送，只发送一次
g_broadCast.type_sys = 102  --系统广播
g_broadCast.type_horn = 103  --玩家使用小喇叭
g_broadCast.type_fruit = 104  --水果机系统广播
g_broadCast.type_roulette = 105  --幸运大转盘广播


g_mailState = {}
g_mailState.state_delete = 0
g_mailState.state_unread = 1     --未读
g_mailState.state_unreadadd = 2  --有附件未读
g_mailState.state_readadd = 3    --有附件已读
g_mailState.state_read = 4       --已读，附件也领取

g_mailType = {}
g_mailType.GM_SYS = 1 -- GM管理员发给系统的。
g_mailType.GM_Player = 2 -- GM管理员发给玩家的
g_mailType.SYS_Player = 3 -- 系统主动发给玩家的
g_mailType.player_sys = 101 -- 玩家反馈的邮件




g_logintable = {}
g_logintable.type_hlyd = 2001
g_logintable.type_hljj = 2101
g_logintable.type_phz = 2301
g_logintable.type_gdmj = 2201   --广东麻将
g_logintable.type_brnn = 2501   --百人牛牛
g_logintable.type_hubmj = 2801   --百人牛牛
g_logintable.type_douniu = 3009 	--斗牛
g_logintable.type_sangong = 3101 	--三公
g_logintable.type_psz = 3309 	--
g_logintable.type_paijiu = 4010		-- 牌九
g_logintable.type_ddz = 3905 	--斗地主


g_logintable.majiang_win = 99999   


g_weekSeconds = 604800
g_daySeconds = 86400
g_hourSeconds = 3600
g_minuteSeconds = 60


g_playerState = {}
g_playerState.state_leave = 1    --离开
g_playerState.state_resume = 2   --恢复


g_goodsCategory = {}
g_goodsCategory.currency = 1  --货币
g_goodsCategory.vip = 2        --vip
g_goodsCategory.vehicle = 4    --交通工具
g_goodsCategory.gift = 5       --礼品
g_goodsCategory.prop_each = 6       --互动道具
g_goodsCategory.prop_card = 7         --周卡，月卡
g_goodsCategory.prop_use = 8         --使用的道具
g_goodsCategory.gift_bag = 9         --表情包，新手礼包，首冲礼包，节日礼包

g_goodsID = {}
g_goodsID.money = 1001
g_goodsID.jetton = 1002
g_goodsID.gold = 1003     

BindType = {}

BindType.bindType_None = 0     --未绑定
BindType.bindType_TianYu = 1   --自己
BindType.bindType_QQ = 2       --QQ登入
BindType.bindType_WeiXin = 3   --微信登入


g_noticeType = {}
g_noticeType.mail_unread = 101   --未读邮件
g_noticeType.mail_unget = 102    --未领取附件邮件
g_noticeType.horn_amount = 201   --小喇叭数量
g_noticeType.friends_call = 301  --好友请求列表
g_noticeType.friends_give = 302  --好友赠送
g_noticeType.mission_11 = 411    --天天返利可以领取
g_noticeType.mission_12 = 412    --每日任务可以领取
g_noticeType.mission_13 = 413    --周任务可以领取
g_noticeType.mission_21 = 421    --成长足迹可以领取
g_noticeType.mission_22 = 422    --时光碎片可以领取
g_noticeType.Roulette_free = 501 --幸运大转盘免费

g_gamePayWay = {}
g_gamePayWay.pay_owner = 1   --房主付费
g_gamePayWay.pay_AA = 2   --AA付费
g_gamePayWay.pay_free = 3   --免付费
g_gamePayWay.pay_baoming = 4   --报名费

g_agencyLevel = {}
g_agencyLevel.groupMaster = 91