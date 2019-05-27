--3000~3100 是斗牛的玩法
--点击牛牛图标，获取玩家的自己创建的房间										
PacketCode[4001] = {server = 4001, client = 4002, des = "cgpaijiutablelist", func = "PaiJiuTableList"}
PacketCode[4002] = {server = 4001, client = 4002, des = "gcpaijiutablelist", func = "PaiJiuTableList"}

--创建房间
PacketCode[4003] = {server = 4003, client = 4004, des = "cgpaijiucreate", func = "PaiJiuCreate"}
PacketCode[4004] = {server = 4003, client = 4004, des = "gcpaijiucreate", func = "PaiJiuCreate"}

--发起解散房间
PacketCode[4005] = {server = 4005, client = 4006, des = "cgpaijiudissolve", func = "PaiJiuDissolve"}
PacketCode[4006] = {server = 4005, client = 4006, des = "gcpaijiudissolve", func = "PaiJiuDissolve"}

--回应解散房间请求  
PacketCode[4007] = {server = 4007, client = 4008, des = "cgpaijiudissolveopt", func = "PaiJiuDissolveOpt"}
PacketCode[4008] = {server = 4007, client = 4008, des = "gcpaijiudissolveopt", func = "PaiJiuDissolveOpt"}

--加入房间
PacketCode[4009] = {server = 4009, client = 4010, des = "cgpaijiujoinroom", func = "PaiJiuJoinRoom"}
PacketCode[4010] = {server = 4009, client = 4010, des = "gcpaijiujoinroom", func = "PaiJiuJoinRoom"}

--离开房间
PacketCode[4011] = {server = 4011, client = 4012, des = "cgpaijiuleave", func = "PaiJiuLeave"}
PacketCode[4012] = {server = 4011, client = 4012, des = "gcpaijiuleave", func = "PaiJiuLeave"}

--坐下
PacketCode[4013] = {server = 4013, client = 4014, des = "cgpaijiusitdown", func = "PaiJiuSitdown"}
PacketCode[4014] = {server = 4013, client = 4014, des = "gcpaijiusitdown", func = "PaiJiuSitdown"}

--站起
PacketCode[4015] = {server = 4015, client = 4016, des = "cgpaijiustandup", func = "PaiJiuStandup"}
PacketCode[4016] = {server = 4015, client = 4016, des = "gcpaijiustandup", func = "PaiJiuStandup"}

--开始游戏
PacketCode[4017] = {server = 4017, client = 4018, des = "cgpaijiustart", func = "PaiJiuStart"}
PacketCode[4018] = {server = 4017, client = 4018, des = "gcpaijiustart", func = "PaiJiuStart"}

--下注
PacketCode[4019] = {server = 4019, client = 4020, des = "cgpaijiupour", func = "PaiJiuPour"}
PacketCode[4020] = {server = 4019, client = 4020, des = "gcpaijiupour", func = "PaiJiuPour"}

--发牌
PacketCode[4022] = {server = 4021, client = 4022, des = "gcpaijiudeal", func = "PaiJiuDeal"}

--亮牌
PacketCode[4023] = {server = 4023, client = 4024, des = "cgpaijiushowpoker", func = "PaiJiuShowPoker"}
PacketCode[4024] = {server = 4023, client = 4024, des = "gcpaijiushowpoker", func = "PaiJiuShowPoker"}

--结算
PacketCode[4026] = {server = 4025, client = 4026, des = "gcpaijiucount", func = "PaiJiuCount"}

--聊天
PacketCode[4027] = {server = 4027, client = 4028, des = "cgpaijiuchat", func = "PaiJiuChat"}
PacketCode[4028] = {server = 4027, client = 4028, des = "gcpaijiuchat", func = "PaiJiuChat"}

--抢庄
PacketCode[4029] = {server = 4029, client = 4030, des = "cgpaijiubanket", func = "PaiJiuBanker"}
PacketCode[4030] = {server = 4029, client = 4030, des = "gcpaijiubanket", func = "PaiJiuBanker"}

--通知进入抢庄状态
PacketCode[4032] = {server = 4031, client = 4032, des = "gcpaijiubanketgo", func = "PaiJiuBanketgo"}

--准备
PacketCode[4033] = {server = 4033, client = 4034, des = "cgpaijiunext", func = "PaiJiuNext"}
PacketCode[4034] = {server = 4033, client = 4034, des = "gcpaijiunext", func = "PaiJiuNext"}

--开始下一局游戏
PacketCode[4036] = {server = 4035, client = 4036, des = "gcpaijiunextout", func = "PaiJiuNextopt"}

--获得历史战绩
PacketCode[4037] = {server = 4037, client = 4038, des = "cgpaijiuhistory", func = "PaiJiuHistory"}
PacketCode[4038] = {server = 4037, client = 4038, des = "gcpaijiuhistory", func = "PaiJiuHistory"}

--确定庄家
PacketCode[4040] = {server = 4039, client = 4040, des = "gcpaijiubanketopt", func = "PaiJiuBanketopt"}

--通知进入下注状态
PacketCode[4042] = {server = 4041, client = 4042, des = "gcpaijiupourgo", func = "PaiJiuPourgo"}

--确定下注
PacketCode[4044] = {server = 4043, client = 4044, des = "gcpaijiupouropt", func = "PaiJiuPourout"}

--通知进入看牌状态
PacketCode[4046] = {server = 4045, client = 4046, des = "gcpaijiushowpokergo", func = "PaiJiuShowpokergo"}

-- 自动亮牌
PacketCode[4047] = {server = 4047, client = 4048, des = "cgdouiuvolun", func = "PaiJiuVolun"}
PacketCode[4048] = {server = 4047, client = 4048, des = "gcdouiuvolun", func = "PaiJiuVolun"}

--动态加入
PacketCode[4049] = {server = 4049, client = 4050, des = "cgdouiudynamicjion", func = "PaiJiuDynamicJion"}
PacketCode[4050] = {server = 4049, client = 4050, des = "gcdouiudynamicjion", func = "PaiJiuDynamicJion"}

--获取房间信息
PacketCode[4051] = {server = 4051, client = 4052, des = "cgdouiuroominfo", func = "PaiJiuRoomInfo"}
PacketCode[4052] = {server = 4051, client = 4052, des = "gcdouiuroominfo", func = "PaiJiuRoomInfo"}

--托管
PacketCode[4053] = {server = 4053, client = 4054, des = "cgpaijiutrusteeship", func = "PaiJiuTrusteeship"}
PacketCode[4054] = {server = 4053, client = 4054, des = "gcpaijiutrusteeship", func = "PaiJiuTrusteeship"}

--隐藏按钮
PacketCode[4056] = {server = 4055, client = 4056, des = "gcpaijiuconcealbutton", func = "PaiJiuConcealButton"}

--买码
PacketCode[4057] = {server = 4057, client = 4058, des = "cgpaijiubuyjetton", func = "PaiJiuBuyJetton"}
PacketCode[4058] = {server = 4057, client = 4058, des = "gcpaijiubuyjetton", func = "PaiJiuBuyJetton"}

--提牌
PacketCode[4059] = {server = 4059, client = 4060, des = "cgpaijiuhintpoker", func = "PaiJiuHintPoker"}
PacketCode[4060] = {server = 4059, client = 4060, des = "gcpaijiuhintpoker", func = "PaiJiuHintPoker"}

--加倍
PacketCode[4061] = {server = 4061, client = 4062, des = "cgpaijiudouble", func = "PaiJiuDouble"}
PacketCode[4062] = {server = 4061, client = 4062, des = "gcpaijiudouble", func = "PaiJiuDouble"}

-- 牌型提示
PacketCode[4064] = {server = 4063, client = 4064, des = "gcpaijiupatterntips", func = "PaiJiuPatternTips"}

ReturnCode["paijiu_not_exist"] = 4001
ReturnCode["paijiu_chair_notmatch"] = 4002
ReturnCode["paijiu_chair_isplyaer"] = 4003
ReturnCode["paijiu_is_sitdown"] = 4004
ReturnCode["paijiu_pwd_notmatch"] = 4005
ReturnCode["paijiu_banker_exist"] = 4006
ReturnCode["paijiu_banker_exist"] = 4007
ReturnCode["paijiu_state_notpour"] = 4008
ReturnCode["paijiu_jetton_notenough"] = 4009
ReturnCode["paijiu_pourtype_error"] = 4010
ReturnCode["paijiu_sit_isbanker"] = 4011
ReturnCode["paijiu_banker_leave"] = 4012
ReturnCode["paijiu_behanker_list"] = 4013  		--加入上庄列表
ReturnCode["paijiu_behanker_drop"] = 4014  		--申请下庄
ReturnCode["paijiu_pour_banker"] = 4015  		--庄家不能下注
ReturnCode["paijiu_banker_notenough"] = 4016  	--筹码不足，不能上庄
ReturnCode["paijiu_pour_lose"] = 4017  			--庄家不够赔钱了，不能下注
ReturnCode["paijiu_is_pour"] = 4018  			--你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["paijiu_full"] = 4019  				--人已经满了
ReturnCode["paijiu_not_ownerid"] = 4020 		--不是房主不能开始游戏
ReturnCode["paijiu_not_player"] = 4021 			--坐下玩家不足两人，不能开始游戏
ReturnCode["paijiu_not_sit"]  = 4022  			--没在座位的的玩家不能进行对应操作
ReturnCode["paijiu_not_state"] = 4023 			--与牌桌的状态不匹配
ReturnCode["paijiu_not_pourjetton"] = 4024 		--底注类型不匹配
ReturnCode["paijiu_opt_error"] = 4025     		--操作出错
ReturnCode["paijiu_leave_error"] = 4026  		--离开房间出错
ReturnCode["paijiu_bankermul_error"] = 4027 	--抢庄的倍数太高
ReturnCode["paijiu_tbaleid_0"] = 4028 			--金币场还没开发
ReturnCode["paijiu_not_enough"] = 4029 			--人数不够
ReturnCode["paijiu_start_error"] = 4030 		-- 开始失败
ReturnCode["paijiu_leave_state"] = 4031 		--请等待游戏结束再离开
ReturnCode["paijiu_room_close"] = 4032 			--房间已经关闭，不能进入
ReturnCode["paijiu_sit_ing"] = 4033				--您已经在座位上	
ReturnCode["paijiu_dissolve_fail"] = 4034   	--申请解散房间失败
ReturnCode["paijiu_dissolve_fail2"] = 4035 		--房间还没开始前，只有房主才能解散房间
ReturnCode["paijiu_dissolve_fail3"] = 4036   	--只有在座玩家才能申请解散房间
ReturnCode["paijiu_baker_pour"] = 4037   		--庄家不用下注
ReturnCode["paijiu_pour_shibai"] = 4038   		--下注错误
ReturnCode["paijiu_create_shibai"] = 4049       --创建房间失败
ReturnCode["paijiu_money_short"] = 4050 		--钻石不足
ReturnCode["paijiu_create_exceed"] = 4051 		--最多只能创建10房间哦
ReturnCode["paijiu_isownerid_no"] = 4052 		--固定庄家：牌桌已经有5个人，还有一个位置是留个房主的
ReturnCode["paijiu_noownerid_start"] = 4053		--固定庄家：需要房主坐下才能开始
ReturnCode["paijiu_buy_banker"] = 4054		 	--买码不能买庄




ReturnCode[4001] = "该牌桌不存在"
ReturnCode[4002] = "坐下的位置不匹配"
ReturnCode[4003] = "该座位已经有玩家"
ReturnCode[4004] = "你已经坐下了，请勿重复操作"
ReturnCode[4005] = "牌桌密码不匹配"
ReturnCode[4006] = "庄家已经存在，上庄失败"
ReturnCode[4008] = "请稍后再下注"
ReturnCode[4009] = "您的筹码不足"
ReturnCode[4010] = "下注失败，请选择正确的下注区域"
ReturnCode[4011] = "坐下失败，你已经是庄家了！"
ReturnCode[4012] = "请先下庄再离开牌桌"
ReturnCode[4013] = "已加入上庄列表"
ReturnCode[4014] = "你已经申请下庄"
ReturnCode[4015] = "庄家不能下注"
ReturnCode[4016] = "筹码不足，不能上庄"
ReturnCode[4017] = "已达下注上限，下注失败"
ReturnCode[4018] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[4019] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[4020] = "你不是房主不能开始游戏"
ReturnCode[4021] = "坐下玩家不足两人，不能开始游戏"
ReturnCode[4022] = "没在座位的的玩家不能进行对应操作"
ReturnCode[4023] = "与牌桌的状态不匹配"
ReturnCode[4024] = "底注类型不匹配"
ReturnCode[4025] = "操作出错"
ReturnCode[4026] = "离开房间出错"
ReturnCode[4027] = "抢庄的倍数太高"
ReturnCode[4028] = "金币场还没开发"
ReturnCode[4029] = "人数不够,无法开始"
ReturnCode[4030] = "开始失败"
ReturnCode[4031] = "请等待游戏结束再离开"
ReturnCode[4032] = "房间已经关闭，不能进入"
ReturnCode[4033] = "您已经在座位上"
ReturnCode[4034] = "申请解散房间失败"
ReturnCode[4035] = "房间还没开始前，只有房主才能解散房间"
ReturnCode[4036] = "只有在座玩家才能申请解散房间"
ReturnCode[4037] = "庄家不用下注"
ReturnCode[4038] = "下注错误"
ReturnCode[4049] = "创建房间失败"
ReturnCode[4050] = "钻石不足"
ReturnCode[4051] = "最多只能创建10房间哦"
ReturnCode[4052] = "固定庄家：牌桌已经有5个人，还有一个位置是留个房主的"
ReturnCode[4053] = "固定庄家：需要房主坐下才能开始"
ReturnCode[4054] = "买码不能买庄"
----------------------