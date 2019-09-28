
g_channel = {}


g_initData = {}  -- 初始化的数据
g_initData.jetton = 0
g_initData.money = 0   --初始化钻石

g_system = {}
g_system.sys_userid = 1
g_system.robot_pond_type = 101    --机器人的奖池的type


g_initData.jetton = 10000000
g_initData.money = 10000000   --初始化钻石	

if g_gamename == 'dwcgame' then
	g_initData.jetton = 10000000
	g_initData.money = 10000000--8   --初始化钻石
end

g_broadCast = {}
g_broadCast.cycle_login = 1   --玩家登陆进来的时候，系统发送的消息，只发送一次
g_broadCast.cycle_1 = 2    --每一分钟循环发送
g_broadCast.cycle_10 = 3   --每十分钟循环发送
g_broadCast.cycle_30 = 4   --每30分钟循环发送
g_broadCast.cycle_60 = 5   --每60分钟循环发送
g_broadCast.cycle_24 = 6   --每天系统时间重置时候发送


g_broadCast.type_login = 101  --系统广播,玩家一登录进来需要发送，只发送一次
g_broadCast.type_sys = 102    --系统广播
g_broadCast.type_horn = 103   --玩家使用小喇叭
g_broadCast.type_fruit = 104  --水果机系统广播
g_broadCast.type_roulette = 105  --幸运大转盘广播


g_mailState = {}
g_mailState.state_delete = 0
g_mailState.state_unread = 1     --未读
g_mailState.state_unreadadd = 2  --有附件未读
g_mailState.state_readadd = 3    --有附件已读
g_mailState.state_read = 4       --已读，附件也领取

g_mailType = {}
g_mailType.GM_SYS = 1       -- GM管理员发给系统的。
g_mailType.GM_Player = 2    -- GM管理员发给玩家的
g_mailType.SYS_Player = 3   --  
g_mailType.player_sys = 101 -- 玩家反馈的邮件




g_logintable = {}
g_logintable.type_hlyd = 2001
g_logintable.type_hljj = 2101
g_logintable.type_phz = 2301
g_logintable.type_gdmj = 2201   --广东麻将
g_logintable.type_brnn = 2501   --百人牛牛
g_logintable.type_hubmj = 2801   --百人牛牛
g_logintable.type_douniu = 3001 	--斗牛
g_logintable.type_douniutz = 9000 	--斗牛
g_logintable.type_ernn = 3010 	--三公
g_logintable.type_barccatat = 4401 	--百家乐
g_logintable.type_bcbm = 4201 	--奔驰宝马
g_logintable.type_psz = 3301 	--炸金花
g_logintable.type_ddz = 3901 	--斗地主
g_logintable.type_honghei = 3701 	--红黑大战
g_logintable.type_lhd = 5301 	--龙虎斗
g_logintable.type_cjmp = 5401 	--彩金明牌
g_logintable.type_sicbo = 5601 	--骰宝
 
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

BindType.bindType_None = 0     --未绑定，这个是游客登录的
BindType.bindType_TianYu = 1   --自己
BindType.bindType_QQ = 2       --QQ登入
BindType.bindType_WeiXin = 3   --微信登入
BindType.bindType_phone = 4    --手机号登入


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

g_userAward = {}
g_userAward.type_login = 101   --登录奖励，7天+1天
g_userAward.type_sign = 102    --签到奖励，31天


-- 货币类型
g_currencyType = {}
g_currencyType.money = 0
g_currencyType.jetton = 1
g_currencyType.bankjetton = 2
-- 货币操作类型
g_currencyOptType = {}
g_currencyOptType.giveMoneyTo = 0		-- 代理送出钻石
g_currencyOptType.giveMoneyFrom = 1		-- 玩家收到代理送出的钻石
g_currencyOptType.bankDeposit = 2		-- 银行-存款操作
g_currencyOptType.bankWithdraw = 3		-- 银行-取款操作
g_currencyOptType.bankTransferTo = 4	-- 银行-向其他玩家转账
g_currencyOptType.bankTransferFrom = 5	-- 银行-收到其他玩家的转账

g_bankruptSubsidy = {}
g_bankruptSubsidy.SubsidyNum = 3
g_bankruptSubsidy.SubsidyCount = 100

g_withdrawdepositminjetton = 10000 

g_faceIDList = {'90001_1001','90002_1001','90003_1001','90004_1001','90005_1001','90006_1001','90007_1001','90008_1001','90009_1001','90010_1001','90011_1001','90012_1001',
				'90013_1001','90014_1001','90015_1001','90016_1001','90017_1001','90018_1001'}


g_rosterType = {}
g_rosterType.type_heimingdan = 1
g_rosterType.type_baimingdan = 2


g_brokerageType = {}
g_brokerageType.mode = 1      --1时为不加本金的抽水， 2时为加上本金的抽水

g_robotSwitch = {}
g_robotSwitch.state = 2      --斗地主、炸金花和抢庄牛牛的机器人开关 1为不进机器人 2 为进机器人

g_giveRetainJetton = 500	--赠送金币至少要保留的数值		

g_gameUserNum = 100

g_RobotBanker = 2           --要不要机器人排队上庄：1 不用， 2 要

g_RobotBeBankerJetton = {}
g_RobotBeBankerJetton[1] = 800000
g_RobotBeBankerJetton[2] = 1200000

g_moneyOptType = {}

g_moneyOptType.opt_pay = 101            --支付入金
g_moneyOptType.opt_withdraw = 102      --提现出金
g_moneyOptType.opt_withdraw_refuse = 103      --驳回
g_moneyOptType.opt_vip_pay = 104
g_moneyOptType.opt_withdraw_fee = 105

g_moneyOptType.opt_feeget = 111        --佣金提取到账户
g_moneyOptType.opt_dayget = 112        --每日领取
g_moneyOptType.opt_register = 113        --注册赠送
g_moneyOptType.opt_transfer_in = 114
g_moneyOptType.opt_transfer_out = 115

g_moneyOptType.opt_bank_in = 116
g_moneyOptType.opt_bank_out = 117

g_moneyOptType.opt_commission_count = 118    --每周佣金结算，这个结算是结算到提现账户，
g_moneyOptType.opt_mail_get = 119   --邮件领取附件


g_moneyOptType.opt_bjl = 4400   --百家乐
g_moneyOptType.opt_bcbm = 4200   --奔驰宝马

g_moneyOptType.opt_brnn = 2500   --百人牛牛
g_moneyOptType.opt_cjmp = 5400   --彩金明牌
g_moneyOptType.opt_ddz = 3900   --斗地主
g_moneyOptType.opt_qznn = 3000   --抢庄牛牛
g_moneyOptType.opt_ersh = 4100   --二人梭哈

g_moneyOptType.opt_brnn = 3500   --狮子王国
g_moneyOptType.opt_cjmp = 4300   --飞禽走兽
g_moneyOptType.opt_ddz = 3600   --水果机
g_moneyOptType.opt_qznn = 3700   --红黑大战
g_moneyOptType.opt_ersh = 5300   --龙虎斗
g_moneyOptType.opt_ersh = 3300   --拼三张
g_moneyOptType.opt_ersh = 5600   --骰宝
g_moneyOptType.opt_ersh = 5701   --德州扑克


g_gamestate = {}
g_gamestate.normal = 1          --表示游戏正常运行
g_gamestate.close_enter = 2     --表示游戏不能进入
g_gamestate.close_run = 3       --表示游戏不能进入且不运行

g_winBroadcount = 100000		--玩家赢钱发广播的最小值

g_SpecialChannel = "adminTest"

g_CommissionFwSwitch = 1  		--1: 抽水算个流水， 2抽水不算流水

--分红 算法
g_bonusLevel = {}
g_bonusLevel[1] = {ach=5000000,rate=1}
g_bonusLevel[2] = {ach=10000000,rate=2}
g_bonusLevel[3] = {ach=50000000,rate=4}
g_bonusLevel[4] = {ach=100000000,rate=6}
g_bonusLevel[5] = {ach=200000000,rate=7}
g_bonusLevel[6] = {ach=300000000,rate=8}
g_bonusLevel[7] = {ach=500000000,rate=9}
g_bonusLevel[8] = {ach=700000000,rate=10}
g_bonusLevel[9] = {ach=1000000000,rate=11}
g_bonusLevel[10] = {ach=1000000000,rate=12}