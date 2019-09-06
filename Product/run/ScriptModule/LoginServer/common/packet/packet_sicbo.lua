

--5600~5700 是骰宝的玩法
PacketCode[5601] = {server = 5601, client = 5602, des = "cgsicboenter", func = "SicboEnter"}
PacketCode[5602] = {server = 5601, client = 5602, des = "gcsicboenter", func = "SicboEnter"}

PacketCode[5603] = {server = 5603, client = 5604, des = "cgsicboleave", func = "SicboLeave"}
PacketCode[5604] = {server = 5603, client = 5604, des = "gcsicboleave", func = "SicboLeave"}

PacketCode[5605] = {server = 5605, client = 5606, des = "cgsicbositdown", func = "SicboSitDown"}
PacketCode[5606] = {server = 5605, client = 5606, des = "gcsicbositdown", func = "SicboSitDown"}

PacketCode[5607] = {server = 5607, client = 5608, des = "cgsicbopourjetton", func = "SicboPourJetton"}
PacketCode[5608] = {server = 5607, client = 5608, des = "gcsicbopourjetton", func = "SicboPourJetton"}

PacketCode[5610] = {server = 5609, client = 5610, des = "gcsicbocount", func = "SicboCount"}

PacketCode[5611] = {server = 5611, client = 5612, des = "cgsicbotablelist", func = "SicboTableList"}
PacketCode[5612] = {server = 5611, client = 5612, des = "gcsicbotablelist", func = "SicboTableList"}

PacketCode[5613] = {server = 5613, client = 5613, des = "cgsicbocreatetable", func = "SicboCreateTable"}
PacketCode[5614] = {server = 5613, client = 5614, des = "gcsicbocreatetable", func = "SicboCreateTable"}

PacketCode[5615] = {server = 5615, client = 5616, des = "cgsicbobeingbanker", func = "SicboBeingBanker"}
PacketCode[5616] = {server = 5615, client = 5616, des = "gcsicbobeingbanker", func = "SicboBeingBanker"}

PacketCode[5617] = {server = 5617, client = 5618, des = "cgsicbodropbanker", func = "SicboDropBanker"}
PacketCode[5618] = {server = 5617, client = 5618, des = "gcsicbodropbanker", func = "SicboDropBanker"}

PacketCode[5620] = {server = 5619, client = 5620, des = "gcsicbostart", func = "SicboStart"}

PacketCode[5622] = {server = 5621, client = 5622, des = "gcsicboupdate", func = "SicboUpdate"}

PacketCode[5624] = {server = 5623, client = 5624, des = "gcsicbostandup", func = "SicboStandUp"}

PacketCode[5626] = {server = 5625, client = 5626, des = "gcsicbogroupjetton", func = "SicboGroupJetton"}

PacketCode[5627] = {server = 5627, client = 5628, des = "cgsicbohistory", func = "SicboHistory"}
PacketCode[5628] = {server = 5627, client = 5628, des = "gcsicbohistory", func = "SicboHistory"}

PacketCode[5629] = {server = 5629, client = 5630, des = "cgsicbochat", func = "SicboChat"}
PacketCode[5630] = {server = 5629, client = 5630, des = "gcsicbochat", func = "SicboChat"}

PacketCode[5631] = {server = 5631, client = 5632, des = "cgsicbostandlist", func = "SicboStandList"}
PacketCode[5632] = {server = 5631, client = 5632, des = "gcsicbostandlist", func = "SicboStandList"}

PacketCode[5633] = {server = 5633, client = 5634, des = "cgsicbobankerlist", func = "SicboBankerList"}
PacketCode[5634] = {server = 5633, client = 5634, des = "gcsicbobankerlist", func = "SicboBankerList"}

PacketCode[5635] = {server = 5635, client = 5636, des = "cgsicbostats", func = "SicboStats"}
PacketCode[5636] = {server = 5635, client = 5636, des = "gcsicbostats", func = "SicboStats"}

PacketCode[5653] = {server = 5653, client = 5638, des = "cgsicboallgamehistory", func = "SicboAllGameHistory"}
PacketCode[5638] = {server = 5653, client = 5638, des = "gcsicboallgamehistory", func = "SicboAllGameHistory"}

PacketCode[5639] = {server = 5639, client = 5640, des = "cgsicbogamehistoryupdata", func = "SicboGameHistoryUpdata"}
PacketCode[5640] = {server = 5639, client = 5640, des = "gcsicbogamehistoryupdata", func = "SicboGameHistoryUpdata"}
--
ReturnCode["sicbo_not_exist"] = 5601
ReturnCode["sicbo_chair_notmatch"] = 5602
ReturnCode["sicbo_chair_isplyaer"] = 5603
ReturnCode["sicbo_is_sitdown"] = 5604
ReturnCode["sicbo_pwd_notmatch"] = 5605
ReturnCode["sicbo_banker_exist"] = 5606
ReturnCode["sicbo_banker_exist"] = 5607
ReturnCode["sicbo_state_notpour"] = 5608
ReturnCode["sicbo_jetton_notenough"] = 5609
ReturnCode["sicbo_pourtype_error"] = 5610
ReturnCode["sicbo_sit_isbanker"] = 5611
ReturnCode["sicbo_banker_leave"] = 5612
ReturnCode["sicbo_behanker_list"] = 5613  --加入上庄列表
ReturnCode["sicbo_behanker_drop"] = 5614  --申请下庄
ReturnCode["sicbo_pour_banker"] = 5615  --庄家不能下注
ReturnCode["sicbo_banker_notenough"] = 5616  --筹码不足，不能上庄
ReturnCode["sicbo_pour_lose"] = 5617  --庄家不够赔钱了，不能下注
ReturnCode["sicbo_is_pour"] = 5618  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["sicbo_is_full"] = 5619  --人已经满了
ReturnCode["sicbo_pourlimit"] = 5620 --限红200000
ReturnCode["sicbo_is_bankerlist"] = 5621  --已经在上庄列表中，请耐心等候
ReturnCode["sicbo_is_drop"] = 5622  -- 您已经申请下庄了
ReturnCode["sicbo_drop_success"] = 5623  --申请下庄成功
ReturnCode["sicbo_lastlimit"] = 5624 -- 下注超过上把下注值


ReturnCode[5601] = "该牌桌不存在"
ReturnCode[5602] = "坐下的位置不匹配"
ReturnCode[5603] = "该座位已经有玩家"
ReturnCode[5604] = "你已经坐下了，请勿重复操作"
ReturnCode[5605] = "牌桌密码不匹配"
ReturnCode[5606] = "庄家已经存在，上庄失败"
ReturnCode[5608] = "请稍后再下注"
ReturnCode[5609] = "您的筹码不足"
ReturnCode[5610] = "下注失败，请选择正确的下注区域"
ReturnCode[5611] = "坐下失败，你已经是庄家了！"
ReturnCode[5612] = "请先下庄再离开牌桌"
ReturnCode[5613] = "已加入上庄列表"
ReturnCode[5614] = "你已经申请下庄"
ReturnCode[5615] = "庄家不能下注"
ReturnCode[5616] = "筹码不足，不能上庄"
ReturnCode[5617] = "已达下注上限，下注失败"
ReturnCode[5618] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[5619] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[5620] = "该区域下注额度已达上限，请选择其他区域下注"
ReturnCode[5621] = "已经在上庄列表中，请耐心等候"
ReturnCode[5622] = "您已经申请下庄了"  
ReturnCode[5623] = "申请下庄成功" 
ReturnCode[5624] = "下注超过上把下注值"

----------------------