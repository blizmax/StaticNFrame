--4100~4200 是二人梭哈玩法的协议
--获取玩家的自己创建的房间列表										
PacketCode[4101] = {server = 4101, client = 4102, des = "cgershtablelist", func = "ErshTableList"}
PacketCode[4102] = {server = 4101, client = 4102, des = "gcershtablelist", func = "ErshTableList"}

--创建房间
PacketCode[4103] = {server = 4103, client = 4104, des = "cgershcreate", func = "ErshCreate"}
PacketCode[4104] = {server = 4103, client = 4104, des = "gcershcreate", func = "ErshCreate"}

--发起解散房间
PacketCode[4105] = {server = 4105, client = 4106, des = "cgershdissolve", func = "ErshDissolve"}
PacketCode[4106] = {server = 4105, client = 4106, des = "gcershdissolve", func = "ErshDissolve"}

--回应解散房间请求  
PacketCode[4107] = {server = 4107, client = 4108, des = "cgershdissolveopt", func = "ErshDissolveopt"}
PacketCode[4108] = {server = 4107, client = 4108, des = "gcershdissolveopt", func = "ErshDissolveopt"}

--加入房间
PacketCode[4109] = {server = 4109, client = 4110, des = "cgershjoinroom", func = "ErshJoinRoom"}
PacketCode[4110] = {server = 4109, client = 4110, des = "gcershjoinroom", func = "ErshJoinRoom"}

--离开房间
PacketCode[4111] = {server = 4111, client = 4112, des = "cgershleave", func = "ErshLeave"}
PacketCode[4112] = {server = 4111, client = 4112, des = "gcershleave", func = "ErshLeave"}

--坐下
PacketCode[4113] = {server = 4113, client = 4114, des = "cgershsitdown", func = "ErshSitdown"}
PacketCode[4114] = {server = 4113, client = 4114, des = "gcershsitdown", func = "ErshSitdown"}


--开始游戏
PacketCode[4115] = {server = 4115, client = 4116, des = "cgershstart", func = "ErshStart"}
PacketCode[4116] = {server = 4115, client = 4116, des = "gcershstart", func = "ErshStart"}

--通知行动玩家
PacketCode[4118] = {server = 4117, client = 4118, des = "gcershinformaction", func = "ErshInformAction"}

--玩家的行动
PacketCode[4119] = {server = 4119, client = 4120, des = "cgershaction", func = "ErshAction"}
PacketCode[4120] = {server = 4119, client = 4120, des = "gcershaction", func = "ErshAction"}

--结算
PacketCode[4122] = {server = 4121, client = 4122, des = "gcershcount", func = "ErshCount"}

--聊天
PacketCode[4123] = {server = 4123, client = 4124, des = "cgershchat", func = "ErshChat"}
PacketCode[4124] = {server = 4123, client = 4124, des = "gcershchat", func = "ErshChat"}

--准备
PacketCode[4125] = {server = 4125, client = 4126, des = "cgershnext", func = "ErshNext"}
PacketCode[4126] = {server = 4125, client = 4126, des = "gcershnext", func = "ErshNext"}

--等待下一局游戏开始
PacketCode[4128] = {server = 4127, client = 4128, des = "gcershnextout", func = "ErshNextopt"}

--获得历史战绩
PacketCode[4129] = {server = 4129, client = 4130, des = "cgershhistory", func = "ErshHistory"}
PacketCode[4130] = {server = 4129, client = 4130, des = "gcershhistory", func = "ErshHistory"}

--进人房间时，当游戏已经开始，主动推送给玩家
PacketCode[4131] = {server = 4131, client = 4132, des = "cgershroominfo", func = "Ershroominfo"}
PacketCode[4132] = {server = 4131, client = 4132, des = "gcershroominfo", func = "Ershroominfo"}

--站起
PacketCode[4134] = {server = 4133, client = 4134, des = "gcershstandup", func = "ErshStandup"}

--通知客户端隐藏看牌加注跟注等按钮
PacketCode[4136] = {server = 4135, client = 4136, des = "gcershconcealbutton", func = "ErshConcealButton"}

--动态加入
PacketCode[4141] = {server = 4141, client = 4142, des = "cgershdynamicjion", func = "ErshDynamicJion"}
PacketCode[4142] = {server = 4141, client = 4142, des = "gcershdynamicjion", func = "ErshDynamicJion"}

-- 发牌
--PacketCode[4143] = {server = 4143, client = 4144, des = "cgershdeal", func = "ErshDeal"}
PacketCode[4144] = {server = 4143, client = 4144, des = "gcershdeal", func = "ErshDeal"}


ReturnCode["ersh_not_exist"] = 4101			--该牌桌不存在
ReturnCode["ersh_chair_isplyaer"] = 4102		--该座位已经有玩家
ReturnCode["ersh_is_sitdown"] = 4103			--你已经坐下了，请勿重复操作
ReturnCode["ersh_state_notpour"] = 4104		--请稍后再下注
ReturnCode["ersh_full"] = 4105  				--人已经满了
ReturnCode["ersh_not_ownerid"] = 4106 		--不是房主不能开始游戏
ReturnCode["ersh_not_player"] = 4107 		--坐下玩家不足两人，不能开始游戏
ReturnCode["ersh_not_state"] = 4108 			--还没有轮到你操作
ReturnCode["ersh_opt_error"] = 4109     		--操作出错
ReturnCode["ersh_leave_error"] = 4110  		--离开房间出错
ReturnCode["ersh_tbaleid_0"] = 4111 			--金币场还没开发
ReturnCode["ersh_not_enough"] = 4112 		--人数不够
ReturnCode["ersh_start_error"] = 4113 		--开始失败
ReturnCode["ersh_leave_state"] = 4114 		--请等待游戏结束再离开
ReturnCode["ersh_room_close"] = 4115			--房间已经关闭，不能进入
ReturnCode["ersh_sit_ing"] = 41016 			--您已经在座位上	
ReturnCode["ersh_dissolve_fail"] = 4117   	--申请解散房间失败
ReturnCode["ersh_dissolve_fail2"] = 4118 	--房间还没开始前，只有房主才能解散房间
ReturnCode["ersh_dissolve_fail3"] = 4119   	--只有在座玩家才能申请解散房间
ReturnCode["ersh_pour_shibai"] = 4120   		--下注错误
ReturnCode["ersh_create_shibai"] = 4121       --创建房间失败
ReturnCode["ersh_money_short"] = 4122 		--钻石不足
ReturnCode["ersh_create_exceed"] = 4123 		--最多只能创建10房间哦
ReturnCode["ersh_action_no"] = 4124 			--还没轮到你操作，请等待
ReturnCode["ersh_mincomp_no"] = 4125 		--现在还不可以比牌
ReturnCode["ersh_menpai_no"] = 4126 			--现在还不可以看牌
ReturnCode["ersh_not_sit"] = 4127 			--你不在座位，不能操作
ReturnCode["ersh_owner_start"] = 4128		--只有房主才能开始房间
ReturnCode["ersh_jeeton_short"] = 4129		--金币不足下注失败
ReturnCode["ersh_upjetton_no"] = 4130		--加注失败
ReturnCode["ersh_bipai_no"] = 4131			--比牌失败
ReturnCode["ersh_not_read"] = 4133			--还有人没准备
ReturnCode["ersh_endshow_no"] = 4133			--牌局还没结束不能亮牌
ReturnCode["ersh_tbale_end"] = 4134			
ReturnCode["ersh_action_error"] = 4135		-- 操作错误
ReturnCode["ersh_jetton_notenough"] = 4136		-- 您的筹码不足
ReturnCode["ersh_table_type_error"] = 4137		-- 牌桌类型错误



ReturnCode[4101] = "该牌桌不存在"
ReturnCode[4102] = "该座位已经有玩家"
ReturnCode[4103] = "你已经坐下了，请勿重复操作"
ReturnCode[4104] = "请稍后再下注"
ReturnCode[4105] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[4106] = "你不是房主不能开始游戏"
ReturnCode[4107] = "坐下玩家不足两人，不能开始游戏"
ReturnCode[4108] = "还没有轮到你操作"
ReturnCode[4109] = "操作出错"
ReturnCode[4110] = "离开房间出错"
ReturnCode[4111] = "金币场还没开发"
ReturnCode[4112] = "人数不够,无法开始"
ReturnCode[4113] = "开始失败"
ReturnCode[4114] = "请等待游戏结束再离开"
ReturnCode[4115] = "房间已经关闭，不能进入"
ReturnCode[4116] = "您已经在座位上"
ReturnCode[4117] = "申请解散房间失败"
ReturnCode[4118] = "房间还没开始前，只有房主才能解散房间"
ReturnCode[4119] = "只有在座玩家才能申请解散房间"
ReturnCode[4120] = "下注错误"
ReturnCode[4121] = "创建房间失败"
ReturnCode[4122] = "钻石不足"
ReturnCode[4123] = "最多只能创建10房间哦"
ReturnCode[4124] = "还没轮到你操作，请等待"
ReturnCode[4125] = "现在还不可以比牌"
ReturnCode[4126] = "现在还不可以看牌"
ReturnCode[4127] = "你不在座位，不能操作"
ReturnCode[4128] = "只有房主才能开始房间"
ReturnCode[4129] = "金币不足下注失败"
ReturnCode[4130] = "加注失败"
ReturnCode[4131] = "比牌失败"
ReturnCode[4133] = "还有人没准备"
ReturnCode[4133] = "牌局还没结束不能亮牌"
ReturnCode[4134] = "最后一局已经开了，您就别进去了"
ReturnCode[4135] = "操作错误"
ReturnCode[4136] = "您的筹码不足"
ReturnCode[4137] = "牌桌类型错误"
----------------------