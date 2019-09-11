


--3500~3700 是森林舞会
PacketCode[3501] = {server = 3501, client = 3502, des = "cgforestenter", func = "ForestEnter"}
PacketCode[3502] = {server = 3501, client = 3502, des = "gcforestenter", func = "ForestEnter"}

PacketCode[3503] = {server = 3503, client = 3504, des = "cgforestleave", func = "ForestLeave"}
PacketCode[3504] = {server = 3503, client = 3504, des = "gcforestleave", func = "ForestLeave"}

PacketCode[3505] = {server = 3505, client = 3506, des = "cgforestpour", func = "ForestPour"}
PacketCode[3506] = {server = 3505, client = 3506, des = "gcforestpour", func = "ForestPour"}

PacketCode[3507] = {server = 3507, client = 3508, des = "cgforestupdatepour", func = "ForestUpdatePour"}
PacketCode[3508] = {server = 3507, client = 3508, des = "gcforestupdatepour", func = "ForestUpdatePour"}

--PacketCode[3510] = {server = 3509, client = 3510, des = "gcforestcount", func = "ForestCount"}
PacketCode[3510] = {server = 3509, client = 3510, des = "gcforestcount", func = "ForestCount"}

PacketCode[3511] = {server = 3511, client = 3512, des = "cgforestplayerlist", func = "ForestPlayerList"}
PacketCode[3512] = {server = 3511, client = 3512, des = "gcforestplayerlist", func = "ForestPlayerList"}

PacketCode[3515] = {server = 3515, client = 3516, des = "cgforestbebanker", func = "ForestBeBanker"}
PacketCode[3516] = {server = 3515, client = 3516, des = "gcforestbebanker", func = "ForestBeBanker"}

PacketCode[3517] = {server = 3517, client = 3518, des = "cgforestdropbanker", func = "ForestDropBanker"}
PacketCode[3518] = {server = 3517, client = 3518, des = "gcforestdropbanker", func = "ForestDropBanker"}

PacketCode[3520] = {server = 3519, client = 3520, des = "gcforeststart", func = "ForestStart"}

PacketCode[3522] = {server = 3521, client = 3522, des = "gcforestupdate", func = "ForestUpdate"}

PacketCode[3529] = {server = 3529, client = 3530, des = "cgforestchat", func = "ForestChat"}
PacketCode[3530] = {server = 3529, client = 3530, des = "gcforestchat", func = "ForestChat"}

--
ReturnCode["forest_not_exist"] = 3501
ReturnCode["forest_chair_notmatch"] = 3502
ReturnCode["forest_chair_isplyaer"] = 3503
ReturnCode["forest_is_sitdown"] = 3504
ReturnCode["forest_pwd_notmatch"] = 3505
ReturnCode["forest_banker_exist"] = 3506
ReturnCode["forest_banker_exist"] = 3507
ReturnCode["forest_state_notpour"] = 3508
ReturnCode["forest_jetton_notenough"] = 3509
ReturnCode["forest_pourtype_error"] = 3510
ReturnCode["forest_sit_isbanker"] = 3511
ReturnCode["forest_banker_leave"] = 3512
ReturnCode["forest_behanker_list"] = 3513  --加入上庄列表
ReturnCode["forest_behanker_drop"] = 3514  --申请下庄
ReturnCode["forest_pour_banker"] = 3515  --庄家不能下注
ReturnCode["forest_banker_notenough"] = 3516  --筹码不足，不能上庄
ReturnCode["forest_pour_lose"] = 3517  --庄家不够赔钱了，不能下注
ReturnCode["forest_is_pour"] = 3518  --你已经在本局游戏中下注，请等待游戏结束再离开



ReturnCode["forest_is_full"] = 3519  --人已经满了
ReturnCode["forest_state_error"] = 3520  --人已经满了
ReturnCode["forest_not_banker"] = 3521  
ReturnCode["forest_is_drop"] = 3522  --已经申请下庄
ReturnCode["forest_drop_success"] = 3523  --申请下庄成功
ReturnCode["forest_banker_limit"] = 3524


ReturnCode[3501] = "该牌桌不存在"
ReturnCode[3502] = "坐下的位置不匹配"
ReturnCode[3503] = "该座位已经有玩家"
ReturnCode[3504] = "你已经坐下了，请勿重复操作"
ReturnCode[3505] = "牌桌密码不匹配"
ReturnCode[3506] = "庄家已经存在，上庄失败"
ReturnCode[3508] = "请稍后再下注"
ReturnCode[3509] = "您的筹码不足"
ReturnCode[3510] = "下注失败，请选择正确的下注区域"
ReturnCode[3511] = "坐下失败，你已经是庄家了！"
ReturnCode[3512] = "请先下庄再退出房间吧"
ReturnCode[3513] = "已加入上庄列表"
ReturnCode[3514] = "你已经申请下庄"
ReturnCode[3515] = "庄家不能下注"
ReturnCode[3516] = "筹码不足，不能上庄"
ReturnCode[3517] = "已达下注上限，下注失败"
ReturnCode[3518] = "为了增加结算准确性，压注状态下不能退出哦(记得取消自动续压)"
ReturnCode[3519] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3520] = "现在不是下注时间，请稍后在下注哦"
ReturnCode[3521] = "你还不是庄家呢"
ReturnCode[3522] = "你已经申请下庄了，请勿重复操作"
ReturnCode[3523] = "申请下庄成功"
ReturnCode[3524] = "金币还差一点，请先到商城中购买然后再过来吧"
----------------------