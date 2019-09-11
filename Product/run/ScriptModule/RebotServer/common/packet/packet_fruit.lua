


--3600~3700 是水果机
PacketCode[3601] = {server = 3601, client = 3602, des = "cgfruitenter", func = "FruitEnter"}
PacketCode[3602] = {server = 3601, client = 3602, des = "gcfruitenter", func = "FruitEnter"}

PacketCode[3603] = {server = 3603, client = 3604, des = "cgfruitleave", func = "FruitLeave"}
PacketCode[3604] = {server = 3603, client = 3604, des = "gcfruitleave", func = "FruitLeave"}

PacketCode[3605] = {server = 3605, client = 3606, des = "cgfruitpour", func = "FruitPour"}
PacketCode[3606] = {server = 3605, client = 3606, des = "gcfruitpour", func = "FruitPour"}

PacketCode[3607] = {server = 3607, client = 3608, des = "cgfruitupdatepour", func = "FruitUpdatePour"}
PacketCode[3608] = {server = 3607, client = 3608, des = "gcfruitupdatepour", func = "FruitUpdatePour"}

--PacketCode[3610] = {server = 3609, client = 3610, des = "gcfruitcount", func = "FruitCount"}
PacketCode[3610] = {server = 3609, client = 3610, des = "gcfruitcount", func = "FruitCount"}

PacketCode[3611] = {server = 3611, client = 3612, des = "cgfruitplayerlist", func = "FruitPlayerList"}
PacketCode[3612] = {server = 3611, client = 3612, des = "gcfruitplayerlist", func = "FruitPlayerList"}

PacketCode[3615] = {server = 3615, client = 3616, des = "cgfruitbebanker", func = "FruitBeBanker"}
PacketCode[3616] = {server = 3615, client = 3616, des = "gcfruitbebanker", func = "FruitBeBanker"}

PacketCode[3617] = {server = 3617, client = 3618, des = "cgfruitdropbanker", func = "FruitDropBanker"}
PacketCode[3618] = {server = 3617, client = 3618, des = "gcfruitdropbanker", func = "FruitDropBanker"}

PacketCode[3620] = {server = 3619, client = 3620, des = "gcfruitstart", func = "FruitStart"}

PacketCode[3622] = {server = 3621, client = 3622, des = "gcfruitupdate", func = "FruitUpdate"}

PacketCode[3629] = {server = 3629, client = 3630, des = "cgfruitchat", func = "FruitChat"}
PacketCode[3630] = {server = 3629, client = 3630, des = "gcfruitchat", func = "FruitChat"}

--
ReturnCode["fruit_not_exist"] = 3601
ReturnCode["fruit_chair_notmatch"] = 3602
ReturnCode["fruit_chair_isplyaer"] = 3603
ReturnCode["fruit_is_sitdown"] = 3604
ReturnCode["fruit_pwd_notmatch"] = 3605
ReturnCode["fruit_banker_exist"] = 3606
ReturnCode["fruit_banker_exist"] = 3607
ReturnCode["fruit_state_notpour"] = 3608
ReturnCode["fruit_jetton_notenough"] = 3609
ReturnCode["fruit_pourtype_error"] = 3610
ReturnCode["fruit_sit_isbanker"] = 3611
ReturnCode["fruit_banker_leave"] = 3612
ReturnCode["fruit_behanker_list"] = 3613  --加入上庄列表
ReturnCode["fruit_behanker_drop"] = 3614  --申请下庄
ReturnCode["fruit_pour_banker"] = 3615  --庄家不能下注
ReturnCode["fruit_banker_notenough"] = 3616  --筹码不足，不能上庄
ReturnCode["fruit_pour_lose"] = 3617  --庄家不够赔钱了，不能下注
ReturnCode["fruit_is_pour"] = 3618  --你已经在本局游戏中下注，请等待游戏结束再离开



ReturnCode["fruit_is_full"] = 3619  --人已经满了
ReturnCode["fruit_state_error"] = 3620  --人已经满了
ReturnCode["fruit_not_banker"] = 3621  
ReturnCode["fruit_is_drop"] = 3622  --已经申请下庄
ReturnCode["fruit_drop_success"] = 3623  --申请下庄成功
ReturnCode["fruit_banker_limit"] = 3624

ReturnCode["fruit_jetton_error_1"] = 3626  --金币不足，无法进人房间
ReturnCode["fruit_jetton_error_2"] = 3627  --金币太多，无法进人房间


ReturnCode[3601] = "该牌桌不存在"
ReturnCode[3602] = "坐下的位置不匹配"
ReturnCode[3603] = "该座位已经有玩家"
ReturnCode[3604] = "你已经坐下了，请勿重复操作"
ReturnCode[3605] = "牌桌密码不匹配"
ReturnCode[3606] = "庄家已经存在，上庄失败"
ReturnCode[3608] = "请稍后再下注"
ReturnCode[3609] = "您的筹码不足"
ReturnCode[3610] = "下注失败，请选择正确的下注区域"
ReturnCode[3611] = "坐下失败，你已经是庄家了！"
ReturnCode[3612] = "请先下庄再退出房间吧"
ReturnCode[3613] = "已加入上庄列表"
ReturnCode[3614] = "你已经申请下庄"
ReturnCode[3615] = "庄家不能下注"
ReturnCode[3616] = "筹码不足，不能上庄"
ReturnCode[3617] = "已达下注上限，下注失败"
ReturnCode[3618] = "为了增加结算准确性，压注状态下不能退出哦(记得取消自动续压)"
ReturnCode[3619] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3620] = "现在不是下注时间，请稍后在下注哦"
ReturnCode[3621] = "你还不是庄家呢"
ReturnCode[3622] = "你已经申请下庄了，请勿重复操作"
ReturnCode[3623] = "申请下庄成功"
ReturnCode[3624] = "金币还差一点，请先到商城中购买然后再过来吧"

ReturnCode["fruit_pour_max"] = 3625
ReturnCode[3625] = "已经超过下注限额，请选择其他区域下注"
ReturnCode[3626] = "金币不足，无法进入房间"
ReturnCode[3627] = "金币太多，无法进入房间"
----------------------