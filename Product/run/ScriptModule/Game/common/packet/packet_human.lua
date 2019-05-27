
--注意，在packet 中，需要把这个文件放在第一位
--注意，所以的区域的分配写在这里
--2000~2100 欢乐赢豆
--2100~2200 欢乐竞技场
--2200~2300 广东麻将 
--2300~2400 跑胡子的代码段
--2400~2500 仿腾讯版欢乐牛牛
--2500~2600 是百人牛牛
--2600~2700 是普通老虎机
--2700~2800 是火锅英雄
--2800~2900 是湖北麻将
--2900~3000 是十点半
--3000~3100 是斗牛
--3100~3200 是三公
--3200~3300 是俱乐部
--3300~3400 是拼三张
--3400~3500 是竞赛

--1000~1100 是人物信息的ID区间
PacketCode[1001] = {server = 1001, client = 1002, des = "cgaccountlogin", func = "AccountLogin"}
PacketCode[1002] = {server = 1001, client = 1002, des = "gcaccountlogin", func = "AccountLogin"}

PacketCode[1003] = {server = 1003, client = 1004, des = "cggetplayerinfo", func = "GetPlayerInfo"}
PacketCode[1004] = {server = 1003, client = 1004, des = "gcgetplayerinfo", func = "GetPlayerInfo"}

PacketCode[1005] = {server = 1005, client = 1006, des = "cgreconnect", func = "ReConnect"}
PacketCode[1006] = {server = 1005, client = 1006, des = "gcreconnect", func = "ReConnect"}

--PacketCode[1007] = {server = 1007, client = 1008, des = "gcracecount", func = "RaceCount"}
PacketCode[1008] = {server = 1007, client = 1008, des = "gckitplayer", func = "KitPlayer"}

PacketCode[1009] = {server = 1009, client = 1010, des = "cgheartbeat", func = "Heartbeat"}
PacketCode[1010] = {server = 1009, client = 1010, des = "gcheartbeat", func = "Heartbeat"}

PacketCode[1011] = {server = 1011, client = 1012, des = "cggetinitinfo", func = "GetInitInfo"}
PacketCode[1012] = {server = 1011, client = 1012, des = "gcgetinitinfo", func = "GetInitInfo"}

PacketCode[1014] = {server = 1013, client = 1014, des = "gcbroadcast", func = "BroadCast"}

PacketCode[1015] = {server = 1015, client = 1016, des = "cggetmaillist", func = "GetMailList"}
PacketCode[1016] = {server = 1015, client = 1016, des = "gcgetmaillist", func = "GetMailList"}
PacketCode[1017] = {server = 1017, client = 1018, des = "cgreadmail", func = "ReadMail"}
PacketCode[1018] = {server = 1017, client = 1018, des = "gcreadmail", func = "ReadMail"}
PacketCode[1019] = {server = 1019, client = 1020, des = "cggetmailgoods", func = "GetMailGoods"}
PacketCode[1020] = {server = 1019, client = 1020, des = "gcgetmailgoods", func = "GetMailGoods"}

PacketCode[1021] = {server = 1021, client = 1022, des = "cgdeletemail", func = "DeleteMail"}
PacketCode[1022] = {server = 1021, client = 1022, des = "gcdeletemail", func = "DeleteMail"}

PacketCode[1023] = {server = 1023, client = 1024, des = "cgplayerstatus", func = "PlayerStatus"}
PacketCode[1024] = {server = 1023, client = 1024, des = "gcplayerstatus", func = "PlayerStatus"}

PacketCode[1025] = {server = 1025, client = 1026, des = "cgupdateplayerinfo", func = "UpdatePlayerInfo"}
PacketCode[1026] = {server = 1025, client = 1026, des = "gcupdateplayerinfo", func = "UpdatePlayerInfo"}

PacketCode[1028] = {server = 1027, client = 1028, des = "gcupdategoodslist", func = "UpdateGoodsList"}

PacketCode[1030] = {server = 1029, client = 1030, des = "gcnoticeinfo", func = "NoticeInfo"}

PacketCode[1031] = {server = 1031, client = 1032, des = "cgagencyopt", func = "AgencyOpt"}
PacketCode[1032] = {server = 1031, client = 1032, des = "gcagencyopt", func = "AgencyOpt"}

PacketCode[1033] = {server = 1033, client = 1034, des = "cgentertable", func = "EnterTable"}
PacketCode[1034] = {server = 1033, client = 1034, des = "gcentertable", func = "EnterTable"}

PacketCode[1035] = {server = 1035, client = 1036, des = "cggetagencyinfo", func = "GetAgencyInfo"}
PacketCode[1036] = {server = 1035, client = 1036, des = "gcgetagencyinfo", func = "GetAgencyInfo"}

PacketCode[1037] = {server = 1037, client = 1038, des = "cginvitebind", func = "InviteBind"}
PacketCode[1038] = {server = 1037, client = 1038, des = "gcinvitebind", func = "InviteBind"}

PacketCode[1039] = {server = 1039, client = 1040, des = "cggivemoney", func = "AgencyGiveMoney"}
PacketCode[1040] = {server = 1039, client = 1040, des = "gcgivemoney", func = "AgencyGiveMoney"}

PacketCode[1041] = {server = 1041, client = 1042, des = "cgapplygroupmaster", func = "ApplyGroupMaster"}
PacketCode[1042] = {server = 1041, client = 1042, des = "gcapplygroupmaster", func = "ApplyGroupMaster"}

PacketCode[1043] = {server = 1043, client = 1044, des = "cggivemoneyrecord", func = "AgencyGiveMoneyRecord"}
PacketCode[1044] = {server = 1043, client = 1044, des = "gcgivemoneyrecord", func = "AgencyGiveMoneyRecord"}

PacketCode[1045] = {server = 1045, client = 1046, des = "cgregistration", func = "Registration"}
PacketCode[1046] = {server = 1045, client = 1046, des = "gcregistration", func = "Registration"}

PacketCode[1047] = {server = 1047, client = 1048, des = "cgluckturntable", func = "LuckTurnTable"}
PacketCode[1048] = {server = 1047, client = 1048, des = "gcluckturntable", func = "LuckTurnTable"}

PacketCode[1049] = {server = 1049, client = 1050, des = "cgupdateplayermoney", func = "UpdatePlayerMoney"}
PacketCode[1050] = {server = 1049, client = 1050, des = "gcupdateplayermoney", func = "UpdatePlayerMoney"}

PacketCode[1051] = {server = 1051, client = 1052, des = "cgaltersomeplayerinfo", func = "AlterSomePlayerInfo"}
PacketCode[1052] = {server = 1051, client = 1052, des = "gcaltersomeplayerinfo", func = "AlterSomePlayerInfo"}

ReturnCode["success"] = 0
ReturnCode["account_not_exist"] = 1001   --账号不存在
ReturnCode["password_not_match"] = 1002
ReturnCode["create_player_fail"] = 1003  --创建玩家失败
ReturnCode["award_itembase_not"] = 1004  --登录奖励的物品不存在
ReturnCode["award_redis_not"] = 1005     --缓存种不存在用户奖品信息
ReturnCode["login_award_finsh"] = 1006    --已经领取了
ReturnCode["login_award_expired"] = 1007    --签到已经过期了

ReturnCode["jetton_not_enough"] = 1008    --进入牌桌的时候，筹码不够
ReturnCode["jetton_exceed"] = 1009        --进入牌桌时，筹码超出了牌桌的上限
ReturnCode["enter_table_failed"] = 1010        --进入牌桌失败
ReturnCode["race_not_exist"] = 1011        --进入牌桌失败
ReturnCode["table_not_exist"] = 1012        --进入牌桌失败
ReturnCode["player_not_exist"] = 1013       --缓存中找不到对应的玩家的信息
ReturnCode["tableplayer_not_exist"] = 1014       --当前牌桌没有改玩家的信息
ReturnCode["sitdown_full"] = 1015       --桌子已经满人了
ReturnCode["carryjetton_not_enough"] = 1016       --携带的筹码不够
ReturnCode["jetton_overtop_limit"] = 1017       --筹码超出了限制
ReturnCode["mail_not_exist"] = 1018       --邮件不存在
ReturnCode["mail_goods_got"] = 1019       --邮件附近已经被领取
ReturnCode["mail_goods_unexist"] = 1020   --邮件附件不存在
ReturnCode["shop_item_unexist"] = 1021   --商店的物品不存在
ReturnCode["money_not_enough"] = 1022   --钱不够
ReturnCode["friends_full"] = 1023   --好友已达上限
ReturnCode["give_full"] = 1024   --赠送次数已经满了
ReturnCode["recv_full"] = 1025   --收到的赠送次数已达上限
ReturnCode["fruit_type_error"] = 1026   --客户端传过来的水果机类型有错误
ReturnCode["buy_diamond_error"] = 1027   --客户端传过来的水果机类型有错误
ReturnCode["mission_get_fail"] = 1028   --任务不存在
ReturnCode["mission_award_unfinish"] = 1029   --任务还没有完成
ReturnCode["mission_award_got"] = 1030   --任务奖励已经领取
ReturnCode["mission_not_exist"] = 1031   --任务奖励已经领取
ReturnCode["friends_call_failself"] = 1032   --自己不能添加自己为好友
ReturnCode["friends_gift_isgot"] = 1033   --礼物已经被领取了
ReturnCode["friends_isexist"] = 1034   --该玩家已经是你的好友
ReturnCode["horn_not_enough"] = 1035   --
ReturnCode["hpbull_not_exist"] = 1036   --
ReturnCode["hpbull_pourtype_error"] = 1037   --
ReturnCode["hpbull_enter_fail"] = 1038   --
ReturnCode["goods_equip_fail"] = 1039    --
ReturnCode["majiang_type_noexist"] = 1040   --麻将类型不存在
ReturnCode["majiang_enter_fail"] = 1041   --麻将类型不存在
ReturnCode["majiang_full"] = 1042   --麻将类型不存在
ReturnCode["majiang_table_notexist"] = 1043   --麻将类型不存在
ReturnCode["majiang_leave_playing"] = 1044   --正在玩牌，不能退出
ReturnCode["majiang_action_Wait"] = 1045   --请稍后再行动
ReturnCode["majiang_vipleave_one"] = 1046   --请稍后再行动
ReturnCode["chat_scene_not"] = 1047   --请稍后再行动
ReturnCode["majiang_vip_con_fail"] = 1048   --请稍后再行动
ReturnCode["majiang_enter_not_exist"] = 1049   --请稍后再行动
ReturnCode["majiang_leave_call"] = 1050   --申请离开牌桌
ReturnCode["wxbind_award_isgot"] = 1051   --
ReturnCode["wxbind_sign_not_exist"] = 1052   --
ReturnCode["wxbind_sign_canget_null"] = 1053   --
ReturnCode["popular_not_enough"] = 1054   --钱不够
ReturnCode["agency_is_band"] = 1055--已经绑定了玩家
ReturnCode["agency_not_exist"] = 1056 --代理不存在
ReturnCode["bind_not_self"] = 1057 --不能够绑定自己
ReturnCode["bind_is_apply"] = 1058 --已经申请了推广员
ReturnCode["apply_fail"] = 1059 --已经申请了推广员
ReturnCode["agency_bind_fail"] = 1060 --已经申请了推广员

ReturnCode["agency_bind_error"] = 1061 --绑定的推广码还没有绑定别人。

ReturnCode["not_agency"] = 1062		-- 不是代理
ReturnCode["givemoney_amount_error"] = 1063	-- 赠送钻石 钻石数量错误
ReturnCode["givemoney_not_self"] = 1064	-- 不能向自己赠送钻石
ReturnCode["inadequate_permissions"] = 1065 -- 权限不足

ReturnCode["already_have_access"] = 1066 --已经具备权限
ReturnCode["apply_group_master_error"] = 1067 -- 申请群主失败
ReturnCode["account_login_error"] = 1068 -- 申请群主失败

ReturnCode[1001] = "账号不存在"
ReturnCode[1002] = "密码不匹配"
ReturnCode[1003] = "创建玩家失败"
ReturnCode[1004] = "领取礼品失败"
ReturnCode[1005] = "领取礼品失败"
ReturnCode[1006] = "你今天已经签到了"
ReturnCode[1007] = "签到已经过期了"
ReturnCode[1008] = "你的筹码不够"      --筹码不够，提示进入充值
ReturnCode[1010] = "进入牌桌失败"      --筹码不够，提示进入充值
ReturnCode[1011] = "操作失败"   --该类型比赛不存在
ReturnCode[1012] = "进入牌桌失败"   --该类型比赛不存在
ReturnCode[1013] = "该玩家不存在"   --该类型比赛不存在
ReturnCode[1014] = "你不在改牌桌中"   --该类型比赛不存在
ReturnCode[1015] = "该牌桌已经满人"   --该类型比赛不存在
ReturnCode[1016] = "筹码不够"   --携带的筹码不够了
ReturnCode[1017] = "筹码超出了该类型的限制"
ReturnCode[1018] = "邮件不存在"
ReturnCode[1019] = "邮件附件已经领取"
ReturnCode[1020] = "邮件附件不存在"
ReturnCode[1021] = "商店的物品不存在"
ReturnCode[1022] = "钻石不够"
ReturnCode[1023] = "好友已达上限"
ReturnCode[1024] = "赠送已达上限"
ReturnCode[1025] = "收到的礼物已达上限"
ReturnCode[1026] = "水果机类型出错"   --客户端传过来的水果机类型有错误
ReturnCode[1027] = "购买钻石失败"   --客户端传过来的水果机类型有错误
ReturnCode[1028] = "获取任务列表失败"   --客户端传过来的水果机类型有错误
ReturnCode[1029] = "任务还没完成"   --客户端传过来的水果机类型有错误
ReturnCode[1030] = "奖励已经领取"   --客户端传过来的水果机类型有错误
ReturnCode[1031] = "任务不存在"   --客户端传过来的水果机类型有错误
ReturnCode[1032] = "自己不能添加自己为好友"
ReturnCode[1033] = "你已经领取该好友的礼物了"
ReturnCode[1034] = "该玩家已经是你的好友"
ReturnCode[1035] = "小喇叭数量不够"
ReturnCode[1036] = "牌桌不存在"
ReturnCode[1037] = "下注失败"
ReturnCode[1038] = "进入斗牛失败"
ReturnCode[1039] = "佩戴物品失败"
ReturnCode[1040] = "请选择合适的牌桌"
ReturnCode[1041] = "进入牌桌失败"
ReturnCode[1042] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[1043] = "该牌桌不存在"
ReturnCode[1044] = "请在游戏结束后退出房间"
ReturnCode[1045] = "请稍后再行动"
ReturnCode[1046] = "每个牌局只允许申请一次退出房间"
ReturnCode[1047] = "牌桌类型不存在"
ReturnCode[1048] = "房卡不足，续费失败"
ReturnCode[1049] = "请输入正确的牌桌号"
ReturnCode[1050] = "您已成功申请解散房间"
ReturnCode[1051] = "你已经绑定了好友"
ReturnCode[1052] = "该邀请码不存在"
ReturnCode[1053] = "你已经领完房卡了"
ReturnCode[1054] = "魅力值不足"
ReturnCode[1055] = "你已经绑定了代理"
ReturnCode[1056] = "绑定的邀请码不存在"
ReturnCode[1057] = "不能绑定自己的邀请码"
ReturnCode[1058] = "你已经申请了代理资格，请联系官方微信好咨询"
ReturnCode[1059] = "申请代理操作失败"
ReturnCode[1060] = "绑定邀请码失败"
ReturnCode[1061] = "绑定邀请码失败"
ReturnCode[1062] = "您不具备代理资格，请联系官方微信号咨询"
ReturnCode[1063] = "钻石数量填写错误"-- 赠送钻石 钻石数量错误
ReturnCode[1064] = "不能向自己赠送钻石"
ReturnCode[1065] = "权限不足"
ReturnCode[1066] = "您已具备该权限"
ReturnCode[1067] = "申请群主失败"
ReturnCode[1068] = "该账号登录异常，请联系系统管理员"