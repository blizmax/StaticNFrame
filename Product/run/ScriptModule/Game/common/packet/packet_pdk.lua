
--5100~5200 是跑得快的协议

--获取玩家的自己创建的房间列表
PacketCode[5101] = {server = 5101, client = 5102, des = "cgpdktablelist", func = "PdkTableList"}
PacketCode[5102] = {server = 5101, client = 5102, des = "gcpdktablelist", func = "PdkTableList"}

--创建房间
PacketCode[5103] = {server = 5103, client = 5104, des = "cgpdkcreate", func = "PdkCreate"}
PacketCode[5104] = {server = 5103, client = 5104, des = "gcpdkcreate", func = "PdkCreate"}

--加入房间
PacketCode[5105] = {server = 5105, client = 5106, des = "cgpdkenter", func = "PdkEnter"}
PacketCode[5106] = {server = 5105, client = 5106, des = "gcpdkenter", func = "PdkEnter"}

--坐下
PacketCode[5107] = {server = 5107, client = 5108, des = "cgpdksitdown", func = "PdkSitDown"}
PacketCode[5108] = {server = 5107, client = 5108, des = "gcpdksitdown", func = "PdkSitDown"}

--离开
PacketCode[5109] = {server = 5109, client = 5110, des = "cgpdkleave", func = "PdkLeave"}
PacketCode[5110] = {server = 5109, client = 5110, des = "gcpdkleave", func = "PdkLeave"}

--站起
PacketCode[5111] = {server = 5111, client = 5112, des = "cgpdkstandup", func = "PdkStandUp"}
PacketCode[5112] = {server = 5111, client = 5112, des = "gcpdkstandup", func = "PdkStandUp"}

--申请解散
PacketCode[5113] = {server = 5113, client = 5114, des = "cgpdkdissolve", func = "PdkDissolve"}
PacketCode[5114] = {server = 5113, client = 5114, des = "gcpdkdissolve", func = "PdkDissolve"}

--回应申请解散
PacketCode[5115] = {server = 5115, client = 5116, des = "cgpdkdissolveopt", func = "PdkDissolveOpt"}
PacketCode[5116] = {server = 5115, client = 5116, des = "gcpdkdissolveopt", func = "PdkDissolveOpt"}

--聊天
PacketCode[5117] = {server = 5117, client = 5118, des = "cgpdkchat", func = "PdkChat"}
PacketCode[5118] = {server = 5117, client = 5118, des = "gcpdkchat", func = "PdkChat"}

--开始
PacketCode[5119] = {server = 5119, client = 5120, des = "cgpdkstart", func = "PdkStart"}
PacketCode[5120] = {server = 5119, client = 5120, des = "gcpdkstart", func = "PdkStart"}

--通知行动玩家
PacketCode[5122] = {server = 5121, client = 5122, des = "gcpdkinformaction", func = "PdkInformAction"}

--玩家的行动
PacketCode[5123] = {server = 5123, client = 5124, des = "cgpdkaction", func = "PdkAction"}
PacketCode[5124] = {server = 5123, client = 5124, des = "gcpdkaction", func = "PdkAction"}

--通知客户端隐藏按钮
PacketCode[5126] = {server = 5125, client = 5126, des = "gcpdkconcealbutton", func = "PdkConcealButton"}

--出牌
PacketCode[5129] = {server = 5129, client = 5130, des = "cgpdkplay", func = "PdkPlay"}
PacketCode[5130] = {server = 5129, client = 5130, des = "gcpdkplay", func = "PdkPlay"}

--结算
PacketCode[5132] = {server = 5131, client = 5132, des = "gcpdkcount", func = "PdkCount"}

--准备
PacketCode[5133] = {server = 5133, client = 5134, des = "cgpdkready", func = "PdkReady"}
PacketCode[5134] = {server = 5133, client = 5134, des = "gcpdkready", func = "PdkReady"}

--战绩
PacketCode[5135] = {server = 5135, client = 5136, des = "cgpdkhistory", func = "PdkHistory"}
PacketCode[5136] = {server = 5135, client = 5136, des = "gcpdkhistory", func = "PdkHistory"}

--回放
PacketCode[5137] = {server = 5137, client = 5138, des = "cgpdkhistorystep", func = "PdkHistoryStep"}
PacketCode[5138] = {server = 5137, client = 5138, des = "gcpdkhistorystep", func = "PdkHistoryStep"}

--托管
PacketCode[5139] = {server = 5139, client = 5140, des = "cgpdkautoplayer", func = "PdkAutoplayer"}
PacketCode[5140] = {server = 5139, client = 5140, des = "gcpdkautoplayer", func = "PdkAutoplayer"}

--牌型检测
PacketCode[5144] = {server = 5143, client = 5144, des = "gcpdkplaycheckoutresult", func = "PdkPlayCheckoutResult"}

--等待下一局游戏开始
PacketCode[5146] = {server = 5145, client = 5146, des = "gcpdknext", func = "PdkNext"}

--客户端主动请求牌桌信息
PacketCode[5147] = {server = 5147, client = 5148, des = "cgpdkgettableinfo", func = "PdkGettableinfo"}
PacketCode[5148] = {server = 5147, client = 5148, des = "gcpdkgettableinfo", func = "PdkGettableinfo"}



ReturnCode["pdk_not_exist"] = 5101
ReturnCode["pdk_create_fail"] = 5102 
ReturnCode["pdk_full"] = 5103   
ReturnCode["pdk_sit_fail"] = 5105  
ReturnCode["pdk_leave_playing"] = 5106   
ReturnCode["pdk_leave_error"] = 5107 
ReturnCode["pdk_dissolve_fail"] = 5108   
ReturnCode["pdk_dissolve_fail2"] = 5109   
ReturnCode["pdk_dissolve_fail3"] = 5110  
ReturnCode["pdk_opt_error"] = 5111   
ReturnCode["pdk_action_no"] = 5112   
ReturnCode["pdk_poker_error"] = 5113     
ReturnCode["pdk_not_sit"] = 5114  
ReturnCode["pdk_sitdown_fail"] = 5115   
ReturnCode["pdk_stand_error"] = 5116   
ReturnCode["pdk_doNot_error"] = 5117   
ReturnCode["pdk_play_poker_error"] = 5118   
ReturnCode["pdk_poker_Type_error"] = 5119
ReturnCode["pdk_play_poker_error_2"] = 5120


ReturnCode[5101] = "房间已经解散"
ReturnCode[5102] = "创建房间失败"
ReturnCode[5103] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[5105] = "钻石不足"
ReturnCode[5106] = "牌局正在进行中，请稍后退出"
ReturnCode[5107] = "请在游戏结束后离开房间"
ReturnCode[5108] = "解散失败"
ReturnCode[5109] = "只有房主才能解散房间"
ReturnCode[5110] = "只有座位上的玩家才能申请解散房间"
ReturnCode[5111] = "操作错误"
ReturnCode[5112] = "还没有轮到你，请稍后再操作"
ReturnCode[5113] = "出牌失败，请重新出牌"
ReturnCode[5114] = "只有坐下的玩家才能准备"
ReturnCode[5115] = "坐下失败"
ReturnCode[5116] = "请等待牌局结束再站起"
ReturnCode[5117] = "上一家就是您出的牌，您不能过"
ReturnCode[5118] = "出的牌不在你的手牌列表中"
ReturnCode[5119] = "牌型错误"
ReturnCode[5120] = "你的牌打不过对方"