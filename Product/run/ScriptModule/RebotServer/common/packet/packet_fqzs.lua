
PacketCode[4301] = {server = 4301, client = 4302, des = "cgfqzsenter", func = "FqzsEnter"}
PacketCode[4302] = {server = 4301, client = 4302, des = "gcfqzsenter", func = "FqzsEnter"}

PacketCode[4303] = {server = 4303, client = 4304, des = "cgfqzsleave", func = "FqzsLeave"}
PacketCode[4304] = {server = 4303, client = 4304, des = "gcfqzsleave", func = "FqzsLeave"}

PacketCode[4305] = {server = 4305, client = 4306, des = "cgfqzspour", func = "FqzsPour"}
PacketCode[4306] = {server = 4305, client = 4306, des = "gcfqzspour", func = "FqzsPour"}

PacketCode[4307] = {server = 4307, client = 4308, des = "cgfqzsupdatepour", func = "FqzsUpdatePour"}
PacketCode[4308] = {server = 4307, client = 4308, des = "gcfqzsupdatepour", func = "FqzsUpdatePour"}

--PacketCode[4310] = {server = 4309, client = 4310, des = "gcfqzscount", func = "FqzsCount"}
PacketCode[4310] = {server = 4309, client = 4310, des = "gcfqzscount", func = "FqzsCount"}

PacketCode[4311] = {server = 4311, client = 4312, des = "cgfqzsplayerlist", func = "FqzsPlayerList"}
PacketCode[4312] = {server = 4311, client = 4312, des = "gcfqzsplayerlist", func = "FqzsPlayerList"}

PacketCode[4315] = {server = 4315, client = 4316, des = "cgfqzsbebanker", func = "FqzsBeBanker"}
PacketCode[4316] = {server = 4315, client = 4316, des = "gcfqzsbebanker", func = "FqzsBeBanker"}

PacketCode[4317] = {server = 4317, client = 4318, des = "cgfqzsdropbanker", func = "FqzsDropBanker"}
PacketCode[4318] = {server = 4317, client = 4318, des = "gcfqzsdropbanker", func = "FqzsDropBanker"}

PacketCode[4320] = {server = 4319, client = 4320, des = "gcfqzsstart", func = "FqzsStart"}

PacketCode[4322] = {server = 4321, client = 4322, des = "gcfqzsupdate", func = "FqzsUpdate"}

PacketCode[4329] = {server = 4329, client = 4330, des = "cgfqzschat", func = "FqzsChat"}
PacketCode[4330] = {server = 4329, client = 4330, des = "gcfqzschat", func = "FqzsChat"}

PacketCode[4332] = {server = 4331, client = 4332, des = "gcfqzsend", func = "FqzsEnd"}

PacketCode[4333] = {server = 4333, client = 4334, des = "cgfqzsallgamehistory", func = "FqzsAllgameHistory"}
PacketCode[4334] = {server = 4333, client = 4334, des = "gcfqzsallgamehistory", func = "FqzsAllgameHistory"}

PacketCode[4335] = {server = 4335, client = 4336, des = "cgfqzsgamehistoryupdata", func = "FqzsGameHistoryUpdata"}
PacketCode[4336] = {server = 4335, client = 4336, des = "gcfqzsgamehistoryupdata", func = "FqzsGameHistoryUpdata"}

PacketCode[4337] = {server = 4337, client = 4338, des = "cgfqzsstandlist", func = "FqzsStandList"}
PacketCode[4338] = {server = 4337, client = 4338, des = "gcfqzsstandlist", func = "FqzsStandList"}

--
ReturnCode["fqzs_not_exist"] = 4301
ReturnCode["fqzs_chair_notmatch"] = 4302
ReturnCode["fqzs_chair_isplyaer"] = 4303
ReturnCode["fqzs_is_sitdown"] = 4304
ReturnCode["fqzs_pwd_notmatch"] = 4305
ReturnCode["fqzs_banker_exist"] = 4306
ReturnCode["fqzs_banker_exist"] = 4307
ReturnCode["fqzs_state_notpour"] = 4308
ReturnCode["fqzs_jetton_notenough"] = 4309
ReturnCode["fqzs_pourtype_error"] = 4310
ReturnCode["fqzs_sit_isbanker"] = 4311
ReturnCode["fqzs_banker_leave"] = 4312
ReturnCode["fqzs_behanker_list"] = 4313  --加入上庄列表
ReturnCode["fqzs_behanker_drop"] = 4314  --申请下庄
ReturnCode["fqzs_pour_banker"] = 4315  --庄家不能下注
ReturnCode["fqzs_banker_notenough"] = 4316  --筹码不足，不能上庄
ReturnCode["fqzs_pour_lose"] = 4317  --庄家不够赔钱了，不能下注
ReturnCode["fqzs_is_pour"] = 4318  --你已经在本局游戏中下注，请等待游戏结束再离开



ReturnCode["fqzs_is_full"] = 4319  --人已经满了
ReturnCode["fqzs_state_error"] = 4320  --人已经满了
ReturnCode["fqzs_not_banker"] = 4321  
ReturnCode["fqzs_is_drop"] = 4322  --已经申请下庄
ReturnCode["fqzs_drop_success"] = 4323  --申请下庄成功
ReturnCode["fqzs_banker_limit"] = 4324
ReturnCode["fqzs_table_type_error"] = 4325	-- 桌子类型错误


ReturnCode[4301] = "该牌桌不存在"
ReturnCode[4302] = "坐下的位置不匹配"
ReturnCode[4303] = "该座位已经有玩家"
ReturnCode[4304] = "你已经坐下了，请勿重复操作"
ReturnCode[4305] = "牌桌密码不匹配"
ReturnCode[4306] = "庄家已经存在，上庄失败"
ReturnCode[4308] = "请稍后再下注"
ReturnCode[4309] = "您的筹码不足"
ReturnCode[4310] = "下注失败，请选择正确的下注区域"
ReturnCode[4311] = "坐下失败，你已经是庄家了！"
ReturnCode[4312] = "请先下庄再退出房间吧"
ReturnCode[4313] = "已加入上庄列表"
ReturnCode[4314] = "你已经申请下庄"
ReturnCode[4315] = "庄家不能下注"
ReturnCode[4316] = "筹码不足，不能上庄"
ReturnCode[4317] = "已达下注上限，下注失败"
ReturnCode[4318] = "为了增加结算准确性，押注状态下不能退出哦"
ReturnCode[4319] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[4320] = "现在不是下注时间，请稍后再下注哦"
ReturnCode[4321] = "你还不是庄家呢"
ReturnCode[4322] = "你已经申请下庄了，请勿重复操作"
ReturnCode[4323] = "申请下庄成功"
ReturnCode[4324] = "金币不足，最低上庄需要3000金币"
ReturnCode[4325] = "桌子类型错误"

ReturnCode["fqzs_pour_max"] = 4326
ReturnCode[4326] = "已经超过下注限额，请选择其他区域下注"
----------------------