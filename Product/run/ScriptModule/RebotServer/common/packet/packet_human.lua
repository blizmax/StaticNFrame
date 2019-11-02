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

PacketInfo = {}
PacketInfo.msgBegin = "head"
PacketInfo.headSize = 12
PacketInfo.beginSsize = 4
PacketInfo.buffSize = 4
PacketInfo.packetIDSize = 2
PacketInfo.operatorIDSize = 2
PacketInfo.buffMaxSize = 10240


PacketCode = PacketCode or { }
ReturnCode = ReturnCode or { }

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

PacketCode[1039] = {server = 1039, client = 1040, des = "cguserawardinfo", func = "UserAwardInfo"}
PacketCode[1040] = {server = 1039, client = 1040, des = "gcuserawardinfo", func = "UserAwardInfo"}

PacketCode[1041] = {server = 1041, client = 1042, des = "cggetuseraward", func = "GetUserAward"}
PacketCode[1042] = {server = 1041, client = 1042, des = "gcgetuseraward", func = "GetUserAward"}

PacketCode[1043] = {server = 1043, client = 1044, des = "cgbankdepositwithdraw", func = "BankDepositWithdraw"}
PacketCode[1044] = {server = 1043, client = 1044, des = "gcbankdepositwithdraw", func = "BankDepositWithdraw"}

PacketCode[1045] = {server = 1045, client = 1046, des = "cgbanktransfer", func = "BankTransfer"}
PacketCode[1046] = {server = 1045, client = 1046, des = "gcbanktransfer", func = "BankTransfer"}

PacketCode[1047] = {server = 1047, client = 1048, des = "cgbanktransferhistory", func = "BankTransferHistory"}
PacketCode[1048] = {server = 1047, client = 1048, des = "gcbanktransferhistory", func = "BankTransferHistory"}

PacketCode[1049] = {server = 1049, client = 1050, des = "cgbanksetpassword", func = "BankSetPassword"}
PacketCode[1050] = {server = 1049, client = 1050, des = "gcbanksetpassword", func = "BankSetPassword"}

PacketCode[1051] = {server = 1051, client = 1052, des = "cgbanklogin", func = "BankLogin"}
PacketCode[1052] = {server = 1051, client = 1052, des = "gcbanklogin", func = "BankLogin"}

PacketCode[1053] = {server = 1053, client = 1054, des = "cgexchangegifts", func = "ExchangeGifts"}
PacketCode[1054] = {server = 1053, client = 1054, des = "gcexchangegifts", func = "ExchangeGifts"}

PacketCode[1055] = {server = 1055, client = 1056, des = "cgcheckbindcode", func = "CheckBindCode"}
PacketCode[1056] = {server = 1055, client = 1056, des = "gccheckbindcode", func = "CheckBindCode"}

PacketCode[1057] = {server = 1057, client = 1058, des = "cgbankruptsubsidy", func = "BankruptSubsidy"}
PacketCode[1058] = {server = 1057, client = 1058, des = "gcbankruptsubsidy", func = "BankruptSubsidy"}

PacketCode[1059] = {server = 1059, client = 1060, des = "cgcommissionget", func = "CommissionGet"}
PacketCode[1060] = {server = 1059, client = 1060, des = "gccommissionget", func = "CommissionGet"}

PacketCode[1061] = {server = 1061, client = 1062, des = "cgtxinfogetorbind", func = "TxInfoGetOrBind"}
PacketCode[1062] = {server = 1061, client = 1062, des = "gctxinfogetorbind", func = "TxInfoGetOrBind"}

PacketCode[1063] = {server = 1063, client = 1064, des = "cgwithdrawdeposit", func = "WithdrawDeposit"}
PacketCode[1064] = {server = 1063, client = 1064, des = "gcwithdrawdeposit", func = "WithdrawDeposit"}

PacketCode[1065] = {server = 1065, client = 1066, des = "cgcounthistory", func = "CountHistory"}
PacketCode[1066] = {server = 1065, client = 1066, des = "gccounthistory", func = "CountHistory"}

PacketCode[1067] = {server = 1067, client = 1068, des = "cgbindphone", func = "BindPhone"}
PacketCode[1068] = {server = 1067, client = 1068, des = "gcbindphone", func = "BindPhone"}

PacketCode[1069] = {server = 1069, client = 1070, des = "cgluckwheel", func = "LuckWheel"}
PacketCode[1070] = {server = 1069, client = 1070, des = "gcluckwheel", func = "LuckWheel"}

PacketCode[1071] = {server = 1071, client = 1072, des = "cgluckwheelstart", func = "LuckWheelStart"}
PacketCode[1072] = {server = 1071, client = 1072, des = "gcluckwheelstart", func = "LuckWheelStart"}

PacketCode[1073] = {server = 1073, client = 1074, des = "cgmodifypassword", func = "ModifyPassword"}
PacketCode[1074] = {server = 1073, client = 1074, des = "gcmodyfipassword", func = "ModifyPassword"}

PacketCode[1075] = {server = 1075, client = 1076, des = "cgactivityconnections", func = "ActivityConnections"}
PacketCode[1076] = {server = 1075, client = 1076, des = "gcactivityconnections", func = "ActivityConnections"}

PacketCode[1077] = {server = 1077, client = 1078, des = "cgactivityhero", func = "ActivityHero"}
PacketCode[1078] = {server = 1077, client = 1078, des = "gcactivityhero", func = "ActivityHero"}

PacketCode[1080] = {server = 1079, client = 1080, des = "gcpeiwanginroom", func = "Getpeiwang"}

PacketCode[1081] = {server = 1081, client = 1082, des = "cgregisterplayernum", func = "GetRegisterPlayerNum"}
PacketCode[1082] = {server = 1081, client = 1082, des = "gcregisterplayernum", func = "GetRegisterPlayerNum"}

--龙虎斗连胜活动
PacketCode[1083] = {server = 1083, client = 1084, des = "cglhdrepeatedwininfo", func = "LhdRepeatedWinInfo"}
PacketCode[1084] = {server = 1083, client = 1084, des = "gclhdrepeatedwininfo", func = "LhdRepeatedWinInfo"}

PacketCode[1085] = {server = 1085, client = 1086, des = "cglhdrepeatedwinreward", func = "LhdRepeatedWinReward"}
PacketCode[1086] = {server = 1085, client = 1086, des = "gclhdrepeatedwinreward", func = "LhdRepeatedWinReward"}

--
PacketCode[1087] = {server = 1087, client = 1088, des = "cgdouniutzspecailrewardinfo", func = "DouNiuTzSpecialRewardInfo"}
PacketCode[1088] = {server = 1087, client = 1088, des = "gcdouniutzspecailrewardinfo", func = "DouNiuTzSpecialRewardInfo"}


PacketCode[1089] = {server = 1089, client = 1090, des = "cgdouniutzgivespecailrewardinfo", func = "DouNiuTzGiveSpecialRewardInfo"}
PacketCode[1090] = {server = 1089, client = 1090, des = "gcdouniutzgivespecailrewardinfo", func = "DouNiuTzGiveSpecialRewardInfo"}

PacketCode[1091] = {server = 1091, client = 1092, des = "cggetbandphoneactivityinfo", func = "activityBandPhone"}
PacketCode[1092] = {server = 1091, client = 1092, des = "gcgetbandphoneactivityinfo", func = "activityBandPhone"}

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

ReturnCode["bank_pwd_never_set"] = 1064	-- 尚未设置银行密码
ReturnCode["arg_amount_error"] = 1065	-- 参数数量错误
ReturnCode["bank_jetton_not_enough"] = 1065 -- 银行筹码存款不足
ReturnCode["bank_pwd_already_set"] = 1066 -- 已经设置银行密码
ReturnCode["bank_pwd_error"] = 1067		-- 银行密码错误
ReturnCode["user_not_exist"] = 1068		-- 用户不存在


ReturnCode["modify_not_match"] = 1069		-- 请输入正确的验证码
ReturnCode["modify_error"] = 1070		    -- 修改密码失败

ReturnCode["bind_phone_isexist"] = 1071		-- 该手机已经绑定其他账号
ReturnCode["bind_code_error"] = 1072		    -- 请输入正确的验证码
ReturnCode["bind_error"] = 1073		    -- 绑定失败



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
ReturnCode[1041] = "进入牌桌失败, 您已经在别的牌桌"
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


ReturnCode["award_is_got"] = 1062 --绑定的推广码还没有绑定别人。
ReturnCode[1062] = "今天的奖励已经领取，明天有更丰厚的奖励呢"


ReturnCode["award_get_error"] = 1063 --绑定的推广码还没有绑定别人。
ReturnCode[1062] = "领取失败"



ReturnCode[1064] = "尚未设置银行密码"
ReturnCode[1065] = "参数数量错误"
ReturnCode[1065] = "银行筹码存款不足"
ReturnCode[1066] = "已经设置银行密码"
ReturnCode[1067] = "银行密码错误"
ReturnCode[1068] = "用户不存在"


ReturnCode["phone_error"] = 1069 --请稍后再申请验证码
ReturnCode[1069] = "请稍后再申请验证码" --

ReturnCode["phone_code_notexit"] = 1070 --请稍后再申请验证码
ReturnCode[1070] = "请输入正确的手机号和验证码" --

ReturnCode["phone_code_pastdue"] = 1071 --请稍后再申请验证码
ReturnCode[1071] = "您的验证码已过期" --

ReturnCode["phone_code_pastdue"] = 1072 --请稍后再申请验证码
ReturnCode[1072] = "您的验证码有误，请输入正确的验证码" --

ReturnCode["exchange_code_error"] = 1073 --请稍后再申请验证码
ReturnCode[1073] = "您的兑换码有误，请输入正确的验证码" --


ReturnCode["exchange_user_got"] = 1074 --请稍后再申请验证码
ReturnCode[1074] = "您已经领取兑换物品了，请勿重复领取" --

ReturnCode["exchange_excode_got"] = 1075 --请稍后再申请验证码
ReturnCode[1075] = "该兑换码已经失效，请输入正确的兑换码" --


ReturnCode["bind_code_nil"] = 1076 --请稍后再申请验证码
ReturnCode[1076] = "邀请码不能为空" --

ReturnCode["bind_code_error"] = 1077 --请稍后再申请验证码
ReturnCode[1077] = "邀请码不存在" --

ReturnCode["Subsidy_error"] = 1078	--今天的金币已经领取完了
ReturnCode[1078] = "今天的金币已经领取完了"

ReturnCode["Subsidy_error_2"] = 1079	--今天的金币已经领取完了
ReturnCode[1079] = "只有不足1金币时，才能领取！"

ReturnCode["get_commission_error_1"] = 1080	--今天的金币已经领取完了
ReturnCode[1080] = "暂无可领佣金，快去推广赚去佣金去吧"

ReturnCode["bin_error_1"] = 1081	
ReturnCode[1081] = "绑定失败，该玩家是你的下属"

ReturnCode["bin_error_2"] = 1082	
ReturnCode[1082] = "绑定失败，该玩家不是代理"

ReturnCode["bin_count_error_2"] = 1083	
ReturnCode[1083] = "你已经绑定过，请不要重复绑定"

ReturnCode["bin_count_error_3"] = 1084	
ReturnCode[1084] = "请输入正确的信息"

ReturnCode["Tx_error_1"] = 1085	
ReturnCode[1085] = "金币不足无法结算"

ReturnCode["Tx_error_2"] = 1086	
ReturnCode[1086] = "你还不是代理，只有代理才能结算"

ReturnCode["Tx_error_3"] = 1087
ReturnCode[1087] = "结算失败"

ReturnCode["Tx_error_4"] = 1088
ReturnCode[1088] = "请绑定后，再来结算"

ReturnCode["bin_count_error_1"] = 1089
ReturnCode[1089] = "你还不是代理，请成为代理再来结算提现通道"

ReturnCode["phone_bind_error_1"] = 1090
ReturnCode[1090] = "该号码已经绑定了别的玩家"

ReturnCode["luckwheel_error_1"] = 1091
ReturnCode[1091] = "由于您的业绩流水未达标，所以无法参与此次活动！"

ReturnCode["luckwheel_error_2"] = 1092
ReturnCode[1092] = "您已经参与过抽奖了，无法重复参与！"

ReturnCode["game_Maintain"] = 1093
ReturnCode[1093] = "该游戏正在维护中！"

ReturnCode["function_limit"] = 1094
ReturnCode[1094] = "该功能已被限制"

ReturnCode["vericode_not_match"] = 1095		-- 请输入正确的验证码
ReturnCode[1095] = "请输入正确的验证码"

ReturnCode["modify_error"] = 1096		    -- 修改密码失败
ReturnCode[1096] = "修改密码失败"

ReturnCode["bind_phone_isexist"] = 1097		-- 该手机已经绑定其他账号
ReturnCode[1097] = "该手机号码已经绑定其他账号"

ReturnCode["bind_error"] = 1098		    -- 绑定失败
ReturnCode[1098] = "绑定失败"

ReturnCode["modify_must_bind"] = 1099	    -- 绑定失败
ReturnCode[1099] = "请先绑定手机号码"

--下一个阶级的从1501开始