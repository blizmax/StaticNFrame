

--3700~3800 是红黑大战的玩法
PacketCode[3701] = {server = 3701, client = 3702, des = "cghongheienter", func = "HongHeiEnter"}
PacketCode[3702] = {server = 3701, client = 3702, des = "gchongheienter", func = "HongHeiEnter"}

PacketCode[3703] = {server = 3703, client = 3704, des = "cghongheileave", func = "HongHeiLeave"}
PacketCode[3704] = {server = 3703, client = 3704, des = "gchongheileave", func = "HongHeiLeave"}

PacketCode[3705] = {server = 3705, client = 3706, des = "cghongheisitdown", func = "HongHeiSitDown"}
PacketCode[3706] = {server = 3705, client = 3706, des = "gchongheisitdown", func = "HongHeiSitDown"}

PacketCode[3707] = {server = 3707, client = 3708, des = "cghongheipourjetton", func = "HongHeiPourJetton"}
PacketCode[3708] = {server = 3707, client = 3708, des = "gchongheipourjetton", func = "HongHeiPourJetton"}

PacketCode[3710] = {server = 3709, client = 3710, des = "gchongheicount", func = "HongHeiCount"}

PacketCode[3711] = {server = 3711, client = 3712, des = "cghongheitablelist", func = "HongHeiTableList"}
PacketCode[3712] = {server = 3711, client = 3712, des = "gchongheitablelist", func = "HongHeiTableList"}

PacketCode[3713] = {server = 3713, client = 3713, des = "cghongheicreatetable", func = "HongHeiCreateTable"}
PacketCode[3714] = {server = 3713, client = 3714, des = "gchongheicreatetable", func = "HongHeiCreateTable"}

PacketCode[3715] = {server = 3715, client = 3716, des = "cghongheibeingbanker", func = "HongHeiBeingBanker"}
PacketCode[3716] = {server = 3715, client = 3716, des = "gchongheibeingbanker", func = "HongHeiBeingBanker"}

PacketCode[3717] = {server = 3717, client = 3718, des = "cghongheidropbanker", func = "HongHeiDropBanker"}
PacketCode[3718] = {server = 3717, client = 3718, des = "gchongheidropbanker", func = "HongHeiDropBanker"}

PacketCode[3720] = {server = 3719, client = 3720, des = "gchongheistart", func = "HongHeiStart"}

PacketCode[3722] = {server = 3721, client = 3722, des = "gchongheiupdate", func = "HongHeiUpdate"}

PacketCode[3724] = {server = 3723, client = 3724, des = "gchongheistandup", func = "HongHeiStandUp"}

PacketCode[3726] = {server = 3725, client = 3726, des = "gchongheigroupjetton", func = "HongHeiGroupJetton"}

PacketCode[3727] = {server = 3727, client = 3728, des = "cghongheihistory", func = "HongHeiHistory"}
PacketCode[3728] = {server = 3727, client = 3728, des = "gchongheihistory", func = "HongHeiHistory"}

PacketCode[3729] = {server = 3729, client = 3730, des = "cghongheichat", func = "HongHeiChat"}
PacketCode[3730] = {server = 3729, client = 3730, des = "gchongheichat", func = "HongHeiChat"}

PacketCode[3731] = {server = 3731, client = 3732, des = "cghongheistandlist", func = "HongHeiStandList"}
PacketCode[3732] = {server = 3731, client = 3732, des = "gchongheistandlist", func = "HongHeiStandList"}

PacketCode[3733] = {server = 3733, client = 3734, des = "cghongheibankerlist", func = "HongHeiBankerList"}
PacketCode[3734] = {server = 3733, client = 3734, des = "gchongheibankerlist", func = "HongHeiBankerList"}

PacketCode[3735] = {server = 3735, client = 3736, des = "cghongheistats", func = "HongHeiStats"}
PacketCode[3736] = {server = 3735, client = 3736, des = "gchongheistats", func = "HongHeiStats"}

PacketCode[3737] = {server = 3737, client = 3738, des = "cghongheiallgamehistory", func = "HongHeiAllGameHistory"}
PacketCode[3738] = {server = 3737, client = 3738, des = "gchongheiallgamehistory", func = "HongHeiAllGameHistory"}

PacketCode[3739] = {server = 3739, client = 3740, des = "cghongheigamehistoryupdata", func = "HongHeiGameHistoryUpdata"}
PacketCode[3740] = {server = 3739, client = 3740, des = "gchongheigamehistoryupdata", func = "HongHeiGameHistoryUpdata"}

PacketCode[3741] = {server = 3741, client = 3742, des = "cghongheipourjettonxt", func = "HongHeiPourJettonXt"}
PacketCode[3742] = {server = 3741, client = 3742, des = "gchongheipourjettonxt", func = "HongHeiPourJettonXt"}
--
ReturnCode["honghei_not_exist"] = 3701
ReturnCode["honghei_chair_notmatch"] = 3702
ReturnCode["honghei_chair_isplyaer"] = 3703
ReturnCode["honghei_is_sitdown"] = 3704
ReturnCode["honghei_pwd_notmatch"] = 3705
ReturnCode["honghei_banker_exist"] = 3706
ReturnCode["honghei_banker_exist"] = 3707
ReturnCode["honghei_state_notpour"] = 3708
ReturnCode["honghei_jetton_notenough"] = 3709
ReturnCode["honghei_pourtype_error"] = 3710
ReturnCode["honghei_sit_isbanker"] = 3711
ReturnCode["honghei_banker_leave"] = 3712
ReturnCode["honghei_behanker_list"] = 3713  --加入上庄列表
ReturnCode["honghei_behanker_drop"] = 3714  --申请下庄
ReturnCode["honghei_pour_banker"] = 3715  --庄家不能下注
ReturnCode["honghei_banker_notenough"] = 3716  --筹码不足，不能上庄
ReturnCode["honghei_pour_lose"] = 3717  --庄家不够赔钱了，不能下注
ReturnCode["honghei_is_pour"] = 3718  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["honghei_is_full"] = 3719  --人已经满了
ReturnCode["honghei_pourlimit"] = 3720 --限红200000

ReturnCode["honghei_xt_success"] = 3721  --续投成功
ReturnCode["honghei_xt_fail"] = 3722  --续投失败

ReturnCode["honghei_is_drop"] = 3723  -- 您已经申请下庄了
ReturnCode["honghei_drop_success"] = 3724  --申请下庄成功
ReturnCode["honghei_not_banker"] = 3725  --你还不是庄家呢

ReturnCode[3701] = "该牌桌不存在"
ReturnCode[3702] = "坐下的位置不匹配"
ReturnCode[3703] = "该座位已经有玩家"
ReturnCode[3704] = "你已经坐下了，请勿重复操作"
ReturnCode[3705] = "牌桌密码不匹配"
ReturnCode[3706] = "庄家已经存在，上庄失败"
ReturnCode[3708] = "请稍后再下注"
ReturnCode[3709] = "您的筹码不足"
ReturnCode[3710] = "下注失败，请选择正确的下注区域"
ReturnCode[3711] = "坐下失败，你已经是庄家了！"
ReturnCode[3712] = "请先下庄再离开牌桌"
ReturnCode[3713] = "已加入上庄列表"
ReturnCode[3714] = "你已经申请下庄"
ReturnCode[3715] = "庄家不能下注"
ReturnCode[3716] = "筹码不足，不能上庄"
ReturnCode[3717] = "已达下注上限，下注失败"
ReturnCode[3718] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[3719] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3720] = "该玩家在该区域的下注额度大于最大限制"
ReturnCode[3721] = "续投成功"  
ReturnCode[3722] = "续投失败"
ReturnCode[3723] = "您已经申请下庄了"  
ReturnCode[3724] = "申请下庄成功" 
ReturnCode[3725] = "你还不是庄家呢"
----------------------