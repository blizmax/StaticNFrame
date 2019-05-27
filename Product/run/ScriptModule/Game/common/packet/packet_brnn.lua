


--250~2500 是百人牛牛的玩法
PacketCode[2501] = {server = 2501, client = 2502, des = "cgbrnnenter", func = "BrnnEnter"}
PacketCode[2502] = {server = 2501, client = 2502, des = "gcbrnnenter", func = "BrnnEnter"}

PacketCode[2503] = {server = 2503, client = 2504, des = "cgbrnnleave", func = "BrnnLeave"}
PacketCode[2504] = {server = 2503, client = 2504, des = "gcbrnnleave", func = "BrnnLeave"}

PacketCode[2505] = {server = 2505, client = 2506, des = "cgbrnnsitdown", func = "BrnnSitDown"}
PacketCode[2506] = {server = 2505, client = 2506, des = "gcbrnnsitdown", func = "BrnnSitDown"}

PacketCode[2507] = {server = 2507, client = 2508, des = "cgbrnnpourjetton", func = "BrnnPourJetton"}
PacketCode[2508] = {server = 2507, client = 2508, des = "gcbrnnpourjetton", func = "BrnnPourJetton"}

PacketCode[2510] = {server = 2509, client = 2510, des = "gcbrnncount", func = "BrnnCount"}

PacketCode[2511] = {server = 2511, client = 2512, des = "cgbrnntablelist", func = "BrnnTableList"}
PacketCode[2512] = {server = 2511, client = 2512, des = "gcbrnntablelist", func = "BrnnTableList"}

PacketCode[2513] = {server = 2513, client = 2502, des = "cgbrnncreatetable", func = "BrnnCreateTable"}
PacketCode[2514] = {server = 2513, client = 2514, des = "gcbrnncreatetable", func = "BrnnCreateTable"}

PacketCode[2515] = {server = 2515, client = 2516, des = "cgbrnnbeingbanker", func = "BrnnBeingBanker"}
PacketCode[2516] = {server = 2515, client = 2516, des = "gcbrnnbeingbanker", func = "BrnnBeingBanker"}

PacketCode[2517] = {server = 2517, client = 2518, des = "cgbrnndropbanker", func = "BrnnDropBanker"}
PacketCode[2518] = {server = 2517, client = 2518, des = "gcbrnndropbanker", func = "BrnnDropBanker"}

PacketCode[2520] = {server = 2519, client = 2520, des = "gcbrnnstart", func = "BrnnStart"}

PacketCode[2522] = {server = 2521, client = 2522, des = "gcbrnnupdate", func = "BrnnUpdate"}

PacketCode[2524] = {server = 2523, client = 2524, des = "gcbrnnstandup", func = "BrnnStandUp"}
PacketCode[2526] = {server = 2525, client = 2526, des = "gcbrnngroupjetton", func = "BrnnGroupJetton"}

PacketCode[2527] = {server = 2527, client = 2528, des = "cgbrnnhistory", func = "BrnnHistory"}
PacketCode[2528] = {server = 2527, client = 2528, des = "gcbrnnhistory", func = "BrnnHistory"}

PacketCode[2529] = {server = 2529, client = 2530, des = "cgbrnnchat", func = "BrnnChat"}
PacketCode[2530] = {server = 2529, client = 2530, des = "gcbrnnchat", func = "BrnnChat"}

PacketCode[2531] = {server = 2531, client = 2532, des = "cgbrnnstandlist", func = "BrnnStandList"}
PacketCode[2532] = {server = 2531, client = 2532, des = "gcbrnnstandlist", func = "BrnnStandList"}

PacketCode[2533] = {server = 2533, client = 2534, des = "cgbrnnbankerlist", func = "BrnnBankerList"}
PacketCode[2534] = {server = 2533, client = 2534, des = "gcbrnnbankerlist", func = "BrnnBankerList"}

PacketCode[2535] = {server = 2535, client = 2536, des = "cgbrnnstats", func = "BrnnStats"}
PacketCode[2536] = {server = 2535, client = 2536, des = "gcbrnnstats", func = "BrnnStats"}

--
ReturnCode["brnn_not_exist"] = 2501
ReturnCode["brnn_chair_notmatch"] = 2502
ReturnCode["brnn_chair_isplyaer"] = 2503
ReturnCode["brnn_is_sitdown"] = 2504
ReturnCode["brnn_pwd_notmatch"] = 2505
ReturnCode["brnn_banker_exist"] = 2506
ReturnCode["brnn_banker_exist"] = 2507
ReturnCode["brnn_state_notpour"] = 2508
ReturnCode["brnn_jetton_notenough"] = 2509
ReturnCode["brnn_pourtype_error"] = 2510
ReturnCode["brnn_sit_isbanker"] = 2511
ReturnCode["brnn_banker_leave"] = 2512
ReturnCode["brnn_behanker_list"] = 2513  --加入上庄列表
ReturnCode["brnn_behanker_drop"] = 2514  --申请下庄
ReturnCode["brnn_pour_banker"] = 2515  --庄家不能下注
ReturnCode["brnn_banker_notenough"] = 2516  --筹码不足，不能上庄
ReturnCode["brnn_pour_lose"] = 2517  --庄家不够赔钱了，不能下注
ReturnCode["brnn_is_pour"] = 2518  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["brnn_is_full"] = 2519  --人已经满了

ReturnCode[2501] = "该牌桌不存在"
ReturnCode[2502] = "坐下的位置不匹配"
ReturnCode[2503] = "该座位已经有玩家"
ReturnCode[2504] = "你已经坐下了，请勿重复操作"
ReturnCode[2505] = "牌桌密码不匹配"
ReturnCode[2506] = "庄家已经存在，上庄失败"
ReturnCode[2508] = "请稍后再下注"
ReturnCode[2509] = "您的筹码不足"
ReturnCode[2510] = "下注失败，请选择正确的下注区域"
ReturnCode[2511] = "坐下失败，你已经是庄家了！"
ReturnCode[2512] = "请先下庄再离开牌桌"
ReturnCode[2513] = "已加入上庄列表"
ReturnCode[2514] = "你已经申请下庄"
ReturnCode[2515] = "庄家不能下注"
ReturnCode[2516] = "筹码不足，不能上庄"
ReturnCode[2517] = "已达下注上限，下注失败"
ReturnCode[2518] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[2519] = "该牌桌人数已满，请选择其他牌桌"
----------------------