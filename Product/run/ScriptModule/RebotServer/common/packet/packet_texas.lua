--5701~5799 是德州扑克的玩法

--进入
PacketCode[5701] = {server = 5701, client = 5702, des = "cgtexasenter", func = "TexasEnter"}
PacketCode[5702] = {server = 5701, client = 5702, des = "gctexasenter", func = "TexasEnter"}

--离开
PacketCode[5703] = {server = 5703, client = 5704, des = "cgtexasleave", func = "TexasLeave"}
PacketCode[5704] = {server = 5703, client = 5704, des = "gctexasleave", func = "TexasLeave"}

--坐下
PacketCode[5705] = {server = 5705, client = 5706, des = "cgtexassitdown", func = "TexassitDown"}
PacketCode[5706] = {server = 5705, client = 5706, des = "gctexassitdown", func = "TexassitDown"}

--站起
PacketCode[5708] = {server = 5707, client = 5708, des = "gctexasstandup", func = "TexasStandup"}

--开始
PacketCode[5710] = {server = 5709, client = 5710, des = "gctexasstart", func = "TexasStart"}

--玩家行动
PacketCode[5711] = {server = 5711, client = 5712, des = "cgtexasaction", func = "TexasAction"}
PacketCode[5712] = {server = 5711, client = 5712, des = "gctexasaction", func = "TexasAction"}

--发公共牌
PacketCode[5714] = {server = 5713, client = 5714, des = "gctexassendcard", func = "TexasSendCard"}

--结算
PacketCode[5716] = {server = 5715, client = 5716, des = "gctexascount", func = "Texascount"}

--等待下一局游戏开始
PacketCode[5718] = {server = 5717, client = 5718, des = "gctexasnextout", func = "TexasNextout"}

--发牌
PacketCode[5719] = {server = 5719, client = 5720, des = "cgtexaschat", func = "TexasChat"}
PacketCode[5720] = {server = 5719, client = 5720, des = "gctexaschat", func = "TexasChat"}

--兑换金币
PacketCode[5721] = {server = 5721, client = 5722, des = "cgtexaspickjetton", func = "TexasPickJetton"}
PacketCode[5722] = {server = 5721, client = 5722, des = "gctexaspickjetton", func = "TexasPickJetton"}


ReturnCode["texas_not_exist"] = 5701			--该牌桌不存在
ReturnCode["texas_is_sitdown"] = 5702			--你已经坐下了，请勿重复操作
ReturnCode["texas_sit_full"] = 5703  			--座位已经满了
ReturnCode["texas_not_state"] = 5704 			--还没有轮到你操作
ReturnCode["texas_leave_error"] = 5705  		--离开房间出错
ReturnCode["texas_leave_state"] = 5706 			--请等待游戏结束再离开	
ReturnCode["texas_not_sit"] = 5707 				--你不在座位，不能操作				
ReturnCode["texas_jetton_error_1"] = 5708 		--金币太多， 请到更高级的场次			
ReturnCode["texas_jetton_error_2"] = 5709 		--金币不足		
ReturnCode["texas_abandon_error"] = 5710 		--让牌失败		

ReturnCode[5701] = "该牌桌不存在！"
ReturnCode[5702] = "你已经坐下了，请勿重复操作！"
ReturnCode[5703] = "座位已经满了"
ReturnCode[5704] = "还没有轮到你操作, 请稍等！"
ReturnCode[5705] = "离开房间出错！"
ReturnCode[5706] = "请等待游戏结束再离开！"	
ReturnCode[5707] = "你不在座位，不能操作！"				
ReturnCode[5708] = "金币太多， 请到更高级的场次!"			
ReturnCode[5709] = "金币不足!"	
ReturnCode[5710] = "让牌失败!"		



