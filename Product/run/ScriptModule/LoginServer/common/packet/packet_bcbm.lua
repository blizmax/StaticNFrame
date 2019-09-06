


--3600~3700 是水果机
PacketCode[4201] = {server = 4201, client = 4202, des = "cgbcbmenter", func = "BcbmEnter"}
PacketCode[4202] = {server = 4201, client = 4202, des = "gcbcbmenter", func = "BcbmEnter"}

PacketCode[4203] = {server = 4203, client = 4204, des = "cgbcbmleave", func = "BcbmLeave"}
PacketCode[4204] = {server = 4203, client = 4204, des = "gcbcbmleave", func = "BcbmLeave"}

PacketCode[4205] = {server = 4205, client = 4206, des = "cgbcbmpour", func = "BcbmPour"}
PacketCode[4206] = {server = 4205, client = 4206, des = "gcbcbmpour", func = "BcbmPour"}

PacketCode[4207] = {server = 4207, client = 4208, des = "cgbcbmupdatepour", func = "BcbmUpdatePour"}
PacketCode[4208] = {server = 4207, client = 4208, des = "gcbcbmupdatepour", func = "BcbmUpdatePour"}

--PacketCode[4210] = {server = 4209, client = 4210, des = "gcbcbmcount", func = "BcbmCount"}
PacketCode[4210] = {server = 4209, client = 4210, des = "gcbcbmcount", func = "BcbmCount"}

PacketCode[4211] = {server = 4211, client = 4212, des = "cgbcbmplayerlist", func = "BcbmPlayerList"}
PacketCode[4212] = {server = 4211, client = 4212, des = "gcbcbmplayerlist", func = "BcbmPlayerList"}

PacketCode[4215] = {server = 4215, client = 4216, des = "cgbcbmbebanker", func = "BcbmBeBanker"}
PacketCode[4216] = {server = 4215, client = 4216, des = "gcbcbmbebanker", func = "BcbmBeBanker"}

PacketCode[4217] = {server = 4217, client = 4218, des = "cgbcbmdropbanker", func = "BcbmDropBanker"}
PacketCode[4218] = {server = 4217, client = 4218, des = "gcbcbmdropbanker", func = "BcbmDropBanker"}

PacketCode[4220] = {server = 4219, client = 4220, des = "gcbcbmstart", func = "BcbmStart"}

PacketCode[4222] = {server = 4221, client = 4222, des = "gcbcbmupdate", func = "BcbmUpdate"}

PacketCode[4229] = {server = 4229, client = 4230, des = "cgbcbmchat", func = "BcbmChat"}
PacketCode[4230] = {server = 4229, client = 4230, des = "gcbcbmchat", func = "BcbmChat"}

PacketCode[4232] = {server = 4231, client = 4232, des = "gcbcbmend", func = "BcbmEnd"}

PacketCode[4233] = {server = 4233, client = 4234, des = "cgbcbmallgamehistory", func = "BcbmAllgameHistory"}
PacketCode[4234] = {server = 4233, client = 4234, des = "gcbcbmallgamehistory", func = "BcbmAllgameHistory"}

PacketCode[4235] = {server = 4235, client = 4236, des = "cgbcbmgamehistoryupdata", func = "BcbmGameHistoryUpdata"}
PacketCode[4236] = {server = 4235, client = 4236, des = "gcbcbmgamehistoryupdata", func = "BcbmGameHistoryUpdata"}

PacketCode[4237] = {server = 4237, client = 4238, des = "cgbcbmstandlist", func = "BcbmStandList"}
PacketCode[4238] = {server = 4237, client = 4238, des = "gcbcbmstandlist", func = "BcbmStandList"}

--
ReturnCode["bcbm_not_exist"] = 4201
ReturnCode["bcbm_chair_notmatch"] = 4202
ReturnCode["bcbm_chair_isplyaer"] = 4203
ReturnCode["bcbm_is_sitdown"] = 4204
ReturnCode["bcbm_pwd_notmatch"] = 4205
ReturnCode["bcbm_banker_exist"] = 4206
ReturnCode["bcbm_banker_exist"] = 4207
ReturnCode["bcbm_state_notpour"] = 4208
ReturnCode["bcbm_jetton_notenough"] = 4209
ReturnCode["bcbm_pourtype_error"] = 4210
ReturnCode["bcbm_sit_isbanker"] = 4211
ReturnCode["bcbm_banker_leave"] = 4212
ReturnCode["bcbm_behanker_list"] = 4213  --加入上庄列表
ReturnCode["bcbm_behanker_drop"] = 4214  --申请下庄
ReturnCode["bcbm_pour_banker"] = 4215  --庄家不能下注
ReturnCode["bcbm_banker_notenough"] = 4216  --筹码不足，不能上庄
ReturnCode["bcbm_pour_lose"] = 4217  --庄家不够赔钱了，不能下注
ReturnCode["bcbm_is_pour"] = 4218  --你已经在本局游戏中下注，请等待游戏结束再离开



ReturnCode["bcbm_is_full"] = 4219  --人已经满了
ReturnCode["bcbm_state_error"] = 4220  --人已经满了
ReturnCode["bcbm_not_banker"] = 4221  
ReturnCode["bcbm_is_drop"] = 4222  --已经申请下庄
ReturnCode["bcbm_drop_success"] = 4223  --申请下庄成功
ReturnCode["bcbm_banker_limit"] = 4224
ReturnCode["bcbm_table_type_error"] = 4225	-- 桌子类型错误


ReturnCode[4201] = "该牌桌不存在"
ReturnCode[4202] = "坐下的位置不匹配"
ReturnCode[4203] = "该座位已经有玩家"
ReturnCode[4204] = "你已经坐下了，请勿重复操作"
ReturnCode[4205] = "牌桌密码不匹配"
ReturnCode[4206] = "庄家已经存在，上庄失败"
ReturnCode[4208] = "请稍后再下注"
ReturnCode[4209] = "您的筹码不足"
ReturnCode[4210] = "下注失败，请选择正确的下注区域"
ReturnCode[4211] = "坐下失败，你已经是庄家了！"
ReturnCode[4212] = "请先下庄再退出房间吧"
ReturnCode[4213] = "已加入上庄列表"
ReturnCode[4214] = "你已经申请下庄"
ReturnCode[4215] = "庄家不能下注"
ReturnCode[4216] = "筹码不足，不能上庄"
ReturnCode[4217] = "已达下注上限，下注失败"
ReturnCode[4218] = "为了增加结算准确性，押注状态下不能退出哦"
ReturnCode[4219] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[4220] = "现在不是下注时间，请稍后再下注哦"
ReturnCode[4221] = "你还不是庄家呢"
ReturnCode[4222] = "你已经申请下庄了，请勿重复操作"
ReturnCode[4223] = "申请下庄成功"
ReturnCode[4224] = "金币不足，最低上庄需要3000金币"
ReturnCode[4225] = "桌子类型错误"


ReturnCode["bcbm_pour_max"] = 4226
ReturnCode[4226] = "已经超过下注限额，请选择其他区域下注"
----------------------