

--5300~5400 是龙虎斗的玩法
PacketCode[5301] = {server = 5301, client = 5302, des = "cglhdenter", func = "LhdEnter"}
PacketCode[5302] = {server = 5301, client = 5302, des = "gclhdenter", func = "LhdEnter"}

PacketCode[5303] = {server = 5303, client = 5304, des = "cglhdleave", func = "LhdLeave"}
PacketCode[5304] = {server = 5303, client = 5304, des = "gclhdleave", func = "LhdLeave"}

PacketCode[5305] = {server = 5305, client = 5306, des = "cglhdsitdown", func = "LhdSitDown"}
PacketCode[5306] = {server = 5305, client = 5306, des = "gclhdsitdown", func = "LhdSitDown"}

PacketCode[5307] = {server = 5307, client = 5308, des = "cglhdpourjetton", func = "LhdPourJetton"}
PacketCode[5308] = {server = 5307, client = 5308, des = "gclhdpourjetton", func = "LhdPourJetton"}

PacketCode[5310] = {server = 5309, client = 5310, des = "gclhdcount", func = "LhdCount"}

PacketCode[5311] = {server = 5311, client = 5312, des = "cglhdtablelist", func = "LhdTableList"}
PacketCode[5312] = {server = 5311, client = 5312, des = "gclhdtablelist", func = "LhdTableList"}

PacketCode[5313] = {server = 5313, client = 5313, des = "cglhdcreatetable", func = "LhdCreateTable"}
PacketCode[5314] = {server = 5313, client = 5314, des = "gclhdcreatetable", func = "LhdCreateTable"}

PacketCode[5315] = {server = 5315, client = 5316, des = "cglhdbeingbanker", func = "LhdBeingBanker"}
PacketCode[5316] = {server = 5315, client = 5316, des = "gclhdbeingbanker", func = "LhdBeingBanker"}

PacketCode[5317] = {server = 5317, client = 5318, des = "cglhddropbanker", func = "LhdDropBanker"}
PacketCode[5318] = {server = 5317, client = 5318, des = "gclhddropbanker", func = "LhdDropBanker"}

PacketCode[5320] = {server = 5319, client = 5320, des = "gclhdstart", func = "LhdStart"}

PacketCode[5322] = {server = 5321, client = 5322, des = "gclhdupdate", func = "LhdUpdate"}

PacketCode[5324] = {server = 5323, client = 5324, des = "gclhdstandup", func = "LhdStandUp"}

PacketCode[5326] = {server = 5325, client = 5326, des = "gclhdgroupjetton", func = "LhdGroupJetton"}

PacketCode[5327] = {server = 5327, client = 5328, des = "cglhdhistory", func = "LhdHistory"}
PacketCode[5328] = {server = 5327, client = 5328, des = "gclhdhistory", func = "LhdHistory"}

PacketCode[5329] = {server = 5329, client = 5330, des = "cglhdchat", func = "LhdChat"}
PacketCode[5330] = {server = 5329, client = 5330, des = "gclhdchat", func = "LhdChat"}

PacketCode[5331] = {server = 5331, client = 5332, des = "cglhdstandlist", func = "LhdStandList"}
PacketCode[5332] = {server = 5331, client = 5332, des = "gclhdstandlist", func = "LhdStandList"}

PacketCode[5333] = {server = 5333, client = 5334, des = "cglhdbankerlist", func = "LhdBankerList"}
PacketCode[5334] = {server = 5333, client = 5334, des = "gclhdbankerlist", func = "LhdBankerList"}

PacketCode[5335] = {server = 5335, client = 5336, des = "cglhdstats", func = "LhdStats"}
PacketCode[5336] = {server = 5335, client = 5336, des = "gclhdstats", func = "LhdStats"}

PacketCode[5353] = {server = 5353, client = 5338, des = "cglhdallgamehistory", func = "LhdAllGameHistory"}
PacketCode[5338] = {server = 5353, client = 5338, des = "gclhdallgamehistory", func = "LhdAllGameHistory"}

PacketCode[5339] = {server = 5339, client = 5340, des = "cglhdgamehistoryupdata", func = "LhdGameHistoryUpdata"}
PacketCode[5340] = {server = 5339, client = 5340, des = "gclhdgamehistoryupdata", func = "LhdGameHistoryUpdata"}

PacketCode[5341] = {server = 5341, client = 5342, des = "cglhdpourjettonxt", func = "LhdPourJettonXt"}
PacketCode[5342] = {server = 5341, client = 5342, des = "gclhdpourjettonxt", func = "LhdPourJettonXt"}

--
ReturnCode["lhd_not_exist"] = 5301
ReturnCode["lhd_chair_notmatch"] = 5302
ReturnCode["lhd_chair_isplyaer"] = 5303
ReturnCode["lhd_is_sitdown"] = 5304
ReturnCode["lhd_pwd_notmatch"] = 5305
ReturnCode["lhd_banker_exist"] = 5306
ReturnCode["lhd_banker_exist"] = 5307
ReturnCode["lhd_state_notpour"] = 5308
ReturnCode["lhd_jetton_notenough"] = 5309
ReturnCode["lhd_pourtype_error"] = 5310
ReturnCode["lhd_sit_isbanker"] = 5311
ReturnCode["lhd_banker_leave"] = 5312
ReturnCode["lhd_behanker_list"] = 5313  --加入上庄列表
ReturnCode["lhd_behanker_drop"] = 5314  --申请下庄
ReturnCode["lhd_pour_banker"] = 5315  --庄家不能下注
ReturnCode["lhd_banker_notenough"] = 5316  --筹码不足，不能上庄
ReturnCode["lhd_pour_lose"] = 5317  --庄家不够赔钱了，不能下注
ReturnCode["lhd_is_pour"] = 5318  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["lhd_is_full"] = 5319  --人已经满了
ReturnCode["lhd_pourlimit"] = 5320 --限红200000
ReturnCode["lhd_is_bankerlist"] = 5321  --已经在上庄列表中，请耐心等候
ReturnCode["lhd_is_drop"] = 5322  -- 您已经申请下庄了
ReturnCode["lhd_drop_success"] = 5323  --申请下庄成功
ReturnCode["lhd_not_banker"] = 5324  --你还不是庄家呢
ReturnCode["lhd_xt_success"] = 5325  --续投成功
ReturnCode["lhd_xt_fail"] = 5326  --续投失败

ReturnCode["lhd_not_open"] = 5327 -- 未开放

ReturnCode["lhd_give_repeated_win_repeated_fail"] = 5328  --获得连胜奖励失败


ReturnCode[5301] = "该牌桌不存在"
ReturnCode[5302] = "坐下的位置不匹配"
ReturnCode[5303] = "该座位已经有玩家"
ReturnCode[5304] = "你已经坐下了，请勿重复操作"
ReturnCode[5305] = "牌桌密码不匹配"
ReturnCode[5306] = "庄家已经存在，上庄失败"
ReturnCode[5308] = "请稍后再下注"
ReturnCode[5309] = "您的筹码不足"
ReturnCode[5310] = "下注失败，请选择正确的下注区域"
ReturnCode[5311] = "坐下失败，你已经是庄家了！"
ReturnCode[5312] = "请先下庄再离开牌桌"
ReturnCode[5313] = "已加入上庄列表"
ReturnCode[5314] = "你已经申请下庄"
ReturnCode[5315] = "庄家不能下注"
ReturnCode[5316] = "金币不足，最低上庄需要3000金币"
ReturnCode[5317] = "已达下注上限，下注失败"
ReturnCode[5318] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[5319] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[5320] = "该玩家在该区域的下注额度大于最大限制"
ReturnCode[5321] = "已经在上庄列表中，请耐心等候"
ReturnCode[5322] = "您已经申请下庄了"  
ReturnCode[5323] = "申请下庄成功" 
ReturnCode[5324] = "你还不是庄家呢"

ReturnCode[5325] = "续投成功"
ReturnCode[5326] = "续投失败"

ReturnCode[5327] = "未开放"
----------------------