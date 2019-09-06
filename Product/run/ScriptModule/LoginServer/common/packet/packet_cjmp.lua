

--5400~5500 是龙虎斗的玩法
PacketCode[5401] = {server = 5401, client = 5402, des = "cgcjmpenter", func = "CjmpEnter"}
PacketCode[5402] = {server = 5401, client = 5402, des = "gccjmpenter", func = "CjmpEnter"}

PacketCode[5403] = {server = 5403, client = 5404, des = "cgcjmpleave", func = "CjmpLeave"}
PacketCode[5404] = {server = 5403, client = 5404, des = "gccjmpleave", func = "CjmpLeave"}

PacketCode[5405] = {server = 5405, client = 5406, des = "cgcjmpsitdown", func = "CjmpSitDown"}
PacketCode[5406] = {server = 5405, client = 5406, des = "gccjmpsitdown", func = "CjmpSitDown"}

PacketCode[5407] = {server = 5407, client = 5408, des = "cgcjmppourjetton", func = "CjmpPourJetton"}
PacketCode[5408] = {server = 5407, client = 5408, des = "gccjmppourjetton", func = "CjmpPourJetton"}

PacketCode[5410] = {server = 5409, client = 5410, des = "gccjmpcount", func = "CjmpCount"}

PacketCode[5411] = {server = 5411, client = 5412, des = "cgcjmptablelist", func = "CjmpTableList"}
PacketCode[5412] = {server = 5411, client = 5412, des = "gccjmptablelist", func = "CjmpTableList"}

PacketCode[5413] = {server = 5413, client = 5413, des = "cgcjmpcreatetable", func = "CjmpCreateTable"}
PacketCode[5414] = {server = 5413, client = 5414, des = "gccjmpcreatetable", func = "CjmpCreateTable"}

PacketCode[5415] = {server = 5415, client = 5416, des = "cgcjmpbeingbanker", func = "CjmpBeingBanker"}
PacketCode[5416] = {server = 5415, client = 5416, des = "gccjmpbeingbanker", func = "CjmpBeingBanker"}

PacketCode[5417] = {server = 5417, client = 5418, des = "cgcjmpdropbanker", func = "CjmpDropBanker"}
PacketCode[5418] = {server = 5417, client = 5418, des = "gccjmpdropbanker", func = "CjmpDropBanker"}

PacketCode[5420] = {server = 5419, client = 5420, des = "gccjmpstart", func = "CjmpStart"}

PacketCode[5422] = {server = 5421, client = 5422, des = "gccjmpupdate", func = "CjmpUpdate"}

PacketCode[5424] = {server = 5423, client = 5424, des = "gccjmpstandup", func = "CjmpStandUp"}

PacketCode[5426] = {server = 5425, client = 5426, des = "gccjmpgroupjetton", func = "CjmpGroupJetton"}

PacketCode[5427] = {server = 5427, client = 5428, des = "cgcjmphistory", func = "CjmpHistory"}
PacketCode[5428] = {server = 5427, client = 5428, des = "gccjmphistory", func = "CjmpHistory"}

PacketCode[5429] = {server = 5429, client = 5430, des = "cgcjmpchat", func = "CjmpChat"}
PacketCode[5430] = {server = 5429, client = 5430, des = "gccjmpchat", func = "CjmpChat"}

PacketCode[5431] = {server = 5431, client = 5432, des = "cgcjmpstandlist", func = "CjmpStandList"}
PacketCode[5432] = {server = 5431, client = 5432, des = "gccjmpstandlist", func = "CjmpStandList"}

PacketCode[5433] = {server = 5433, client = 5434, des = "cgcjmpbankerlist", func = "CjmpBankerList"}
PacketCode[5434] = {server = 5433, client = 5434, des = "gccjmpbankerlist", func = "CjmpBankerList"}

PacketCode[5435] = {server = 5435, client = 5436, des = "cgcjmpstats", func = "CjmpStats"}
PacketCode[5436] = {server = 5435, client = 5436, des = "gccjmpstats", func = "CjmpStats"}

PacketCode[5453] = {server = 5453, client = 5438, des = "cgcjmpallgamehistory", func = "CjmpAllGameHistory"}
PacketCode[5438] = {server = 5453, client = 5438, des = "gccjmpallgamehistory", func = "CjmpAllGameHistory"}

PacketCode[5439] = {server = 5439, client = 5440, des = "cgcjmpgamehistoryupdata", func = "CjmpGameHistoryUpdata"}
PacketCode[5440] = {server = 5439, client = 5440, des = "gccjmpgamehistoryupdata", func = "CjmpGameHistoryUpdata"}
--
ReturnCode["cjmp_not_exist"] = 5401
ReturnCode["cjmp_chair_notmatch"] = 5402
ReturnCode["cjmp_chair_isplyaer"] = 5403
ReturnCode["cjmp_is_sitdown"] = 5404
ReturnCode["cjmp_pwd_notmatch"] = 5405
ReturnCode["cjmp_banker_exist"] = 5406
ReturnCode["cjmp_banker_exist"] = 5407
ReturnCode["cjmp_state_notpour"] = 5408
ReturnCode["cjmp_jetton_notenough"] = 5409
ReturnCode["cjmp_pourtype_error"] = 5410
ReturnCode["cjmp_sit_isbanker"] = 5411
ReturnCode["cjmp_banker_leave"] = 5412
ReturnCode["cjmp_behanker_list"] = 5413  --加入上庄列表
ReturnCode["cjmp_behanker_drop"] = 5414  --申请下庄
ReturnCode["cjmp_pour_banker"] = 5415  --庄家不能下注
ReturnCode["cjmp_banker_notenough"] = 5416  --筹码不足，不能上庄
ReturnCode["cjmp_pour_lose"] = 5417  --庄家不够赔钱了，不能下注
ReturnCode["cjmp_is_pour"] = 5418  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["cjmp_is_full"] = 5419  --人已经满了
ReturnCode["cjmp_pourlimit"] = 5420 --限红200000
ReturnCode["cjmp_is_bankerlist"] = 5421  --已经在上庄列表中，请耐心等候
ReturnCode["cjmp_is_drop"] = 5422  -- 您已经申请下庄了
ReturnCode["cjmp_drop_success"] = 5423  --申请下庄成功
ReturnCode["cjmp_lastlimit"] = 5424 -- 下注超过上把下注值


ReturnCode[5401] = "该牌桌不存在"
ReturnCode[5402] = "坐下的位置不匹配"
ReturnCode[5403] = "该座位已经有玩家"
ReturnCode[5404] = "你已经坐下了，请勿重复操作"
ReturnCode[5405] = "牌桌密码不匹配"
ReturnCode[5406] = "庄家已经存在，上庄失败"
ReturnCode[5408] = "请稍后再下注"
ReturnCode[5409] = "您的筹码不足"
ReturnCode[5410] = "下注失败，请选择正确的下注区域"
ReturnCode[5411] = "坐下失败，你已经是庄家了！"
ReturnCode[5412] = "请先下庄再离开牌桌"
ReturnCode[5413] = "已加入上庄列表"
ReturnCode[5414] = "你已经申请下庄"
ReturnCode[5415] = "庄家不能下注"
ReturnCode[5416] = "筹码不足，不能上庄"
ReturnCode[5417] = "已达下注上限，下注失败"
ReturnCode[5418] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[5419] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[5420] = "该区域下注额度已达上限，请选择其他区域下注"
ReturnCode[5421] = "已经在上庄列表中，请耐心等候"
ReturnCode[5422] = "您已经申请下庄了"  
ReturnCode[5423] = "申请下庄成功" 
ReturnCode[5424] = "下注超过上把下注值"

----------------------