


--8500~9000 是推筒子的玩法
PacketCode[8501] = {server = 8501, client = 8502, des = "cgttzenter", func = "TtzEnter"}
PacketCode[8502] = {server = 8501, client = 8502, des = "gcttzenter", func = "TtzEnter"}

PacketCode[8503] = {server = 8503, client = 8504, des = "cgttzleave", func = "TtzLeave"}
PacketCode[8504] = {server = 8503, client = 8504, des = "gcttzleave", func = "TtzLeave"}

PacketCode[8505] = {server = 8505, client = 8506, des = "cgttzsitdown", func = "TtzSitDown"}
PacketCode[8506] = {server = 8505, client = 8506, des = "gcttzsitdown", func = "TtzSitDown"}

PacketCode[8507] = {server = 8507, client = 8508, des = "cgttzpourjetton", func = "TtzPourJetton"}
PacketCode[8508] = {server = 8507, client = 8508, des = "gcttzpourjetton", func = "TtzPourJetton"}

PacketCode[8510] = {server = 8509, client = 8510, des = "gcttzcount", func = "TtzCount"}

PacketCode[8511] = {server = 8511, client = 8512, des = "cgttztablelist", func = "TtzTableList"}
PacketCode[8512] = {server = 8511, client = 8512, des = "gcttztablelist", func = "TtzTableList"}

PacketCode[8513] = {server = 8513, client = 8514, des = "cgttzcreatetable", func = "TtzCreateTable"}
PacketCode[8514] = {server = 8513, client = 9514, des = "gcttzcreatetable", func = "TtzCreateTable"}

PacketCode[8515] = {server = 8515, client = 8516, des = "cgttzbeingbanker", func = "TtzBeingBanker"}
PacketCode[8516] = {server = 8515, client = 8516, des = "gcttzbeingbanker", func = "TtzBeingBanker"}

PacketCode[8517] = {server = 8517, client = 8518, des = "cgttzdropbanker", func = "TtzDropBanker"}
PacketCode[8518] = {server = 8517, client = 8518, des = "gcttzdropbanker", func = "TtzDropBanker"}

PacketCode[8520] = {server = 8519, client = 8520, des = "gcttzgamestart", func = "TtzStart"}

PacketCode[8522] = {server = 8521, client = 8522, des = "gcttzbetstart", func = "TtzPour"}

PacketCode[8524] = {server = 8523, client = 8524, des = "gcttzstandup", func = "TtzStandUp"}
PacketCode[8526] = {server = 8525, client = 8526, des = "gcttzgroupjetton", func = "TtzGroupJetton"}

PacketCode[8527] = {server = 8527, client = 8528, des = "cgttzhistory", func = "TtzHistory"}
PacketCode[8528] = {server = 8527, client = 8528, des = "gcttzhistory", func = "TtzHistory"}

PacketCode[8529] = {server = 8529, client = 8530, des = "cgttzchat", func = "TtzChat"}
PacketCode[8530] = {server = 8529, client = 8530, des = "gcttzchat", func = "TtzChat"}

PacketCode[8531] = {server = 8531, client = 8532, des = "cgttzstandlist", func = "TtzStandList"}
PacketCode[8532] = {server = 8531, client = 8532, des = "gcttzstandlist", func = "TtzStandList"}

PacketCode[8533] = {server = 8533, client = 8534, des = "cgttzbankerlist", func = "TtzBankerList"}
PacketCode[8534] = {server = 8533, client = 8534, des = "gcttzbankerlist", func = "TtzBankerList"}

PacketCode[8535] = {server = 8535, client = 8536, des = "cgttzstats", func = "TtzStats"}
PacketCode[8536] = {server = 8535, client = 8536, des = "gcttzstats", func = "TtzStats"}

PacketCode[8537] = {server = 8537, client = 8538, des = "cgttzallgamehistory", func = "TtzAllGameHistory"}
PacketCode[8538] = {server = 8537, client = 8538, des = "gcttzallgamehistory", func = "TtzAllGameHistory"}

PacketCode[8539] = {server = 8539, client = 8540, des = "cgttzgamehistoryupdata", func = "TtzGameHistoryUpdata"}
PacketCode[8540] = {server = 8539, client = 8540, des = "gcttzgamehistoryupdata", func = "TtzGameHistoryUpdata"}

PacketCode[8541] = {server = 8541, client = 8542, des = "gcttzopencard", func = "TtzWorkOpenCard"}
PacketCode[8542] = {server = 8541, client = 8542, des = "gcttzopencard", func = "TtzWorkOpenCard"}


PacketCode[8543] = {server = 8543, client = 8544, des = "ccttreconnect", func = "TtzReconnect"}
PacketCode[8544] = {server = 8543, client = 8544, des = "gcttreconnect", func = "TtzReconnect"}
--
ReturnCode["Ttz_not_exist"] = 8501
ReturnCode["Ttz_chair_notmatch"] = 8502
ReturnCode["Ttz_chair_isplyaer"] = 8503
ReturnCode["Ttz_is_sitdown"] = 8504
ReturnCode["Ttz_pwd_notmatch"] = 8505
ReturnCode["Ttz_banker_exist"] = 8507
ReturnCode["Ttz_state_notpour"] = 8508
ReturnCode["Ttz_jetton_notenough"] = 8509
ReturnCode["Ttz_pourtype_error"] = 8510
ReturnCode["Ttz_sit_isbanker"] = 8511
ReturnCode["Ttz_banker_leave"] = 8512
ReturnCode["Ttz_behanker_list"] = 8513  --加入上庄列表
ReturnCode["Ttz_behanker_drop"] = 8514  --申请下庄
ReturnCode["Ttz_pour_banker"] = 8515  --庄家不能下注
ReturnCode["Ttz_banker_notenough"] = 8516  --筹码不足，不能上庄
ReturnCode["Ttz_pour_lose"] = 8517  --庄家不够赔钱了，不能下注
ReturnCode["Ttz_is_pour"] = 8518  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["Ttz_is_full"] = 8519  --人已经满了
ReturnCode["Ttz_is_bankerlist"] = 8520  --已经在上庄列表中，请耐心等候
ReturnCode["Ttz_jetton_error_1"] = 8526  --金币不足，无法进人房间
ReturnCode["Ttz_jetton_error_2"] = 8527  --金币太多，无法进人房间
ReturnCode["Ttz_banker_notenough_2"] = 8528  --筹码不足，不能上庄


ReturnCode[8501] = "该牌桌不存在"
ReturnCode[8502] = "坐下的位置不匹配"
ReturnCode[8503] = "该座位已经有玩家"
ReturnCode[8504] = "你已经坐下了，请勿重复操作"
ReturnCode[8505] = "牌桌密码不匹配"
ReturnCode[8506] = "庄家已经存在，上庄失败"
ReturnCode[8508] = "请稍后再下注"
ReturnCode[8509] = "您的筹码不足"
ReturnCode[8510] = "下注失败，请选择正确的下注区域"
ReturnCode[8511] = "坐下失败，你已经是庄家了！"
ReturnCode[8512] = "请先下庄再离开牌桌"
ReturnCode[8513] = "已加入上庄列表"
ReturnCode[8514] = "你已经申请下庄"
ReturnCode[8515] = "庄家不能下注"
ReturnCode[8516] = "金币不足，最低上庄需要3000金币"
ReturnCode[8517] = "已达下注上限，下注失败"
ReturnCode[8518] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[8519] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[8520] = "您已经加入上庄列表中，请耐心等着"
ReturnCode[8526] = "金币不足，无法进入房间"
ReturnCode[8527] = "金币太多，无法进入房间"
ReturnCode[8528] = "金币不足，最低上庄需要3000金币"


ReturnCode["Ttz_not_banker"] = 8521  
ReturnCode[8521] = "您还不是庄家呢"  

ReturnCode["Ttz_is_drop"] = 8522  
ReturnCode[8522] = "您已经申请下庄了"  

ReturnCode["Ttz_drop_success"] = 8523  
ReturnCode[8523] = "申请下庄成功"  


ReturnCode["Ttz_bebankeer_success"] = 8524  
ReturnCode[8524] = "申请上庄成功"  


ReturnCode["Ttz_pour_max"] = 8525  
ReturnCode[8525] = "已经超过下注限额，请选择更小的面值"  
----------------------