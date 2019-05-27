--3000~3100 是斗牛的玩法
--点击牛牛图标，获取玩家的自己创建的房间										
PacketCode[3001] = {server = 3001, client = 3002, des = "cgdouniutablelist", func = "DouNiuTableList"}
PacketCode[3002] = {server = 3001, client = 3002, des = "gcdouniutablelist", func = "DouNiuTableList"}

--创建房间
PacketCode[3003] = {server = 3003, client = 3004, des = "cgdouniucreate", func = "DouNiuCreate"}
PacketCode[3004] = {server = 3003, client = 3004, des = "gcdouniucreate", func = "DouNiuCreate"}

--发起解散房间
PacketCode[3005] = {server = 3005, client = 3006, des = "cgdouniudissolve", func = "DouNiuDissolve"}
PacketCode[3006] = {server = 3005, client = 3006, des = "gcdouniudissolve", func = "DouNiuDissolve"}

--回应解散房间请求  
PacketCode[3007] = {server = 3007, client = 3008, des = "cgdouniudissolveopt", func = "DouNiuDissolveOpt"}
PacketCode[3008] = {server = 3007, client = 3008, des = "gcdouniudissolveopt", func = "DouNiuDissolveOpt"}

--加入房间
PacketCode[3009] = {server = 3009, client = 3010, des = "cgdouniujoinroom", func = "DouNiuJoinRoom"}
PacketCode[3010] = {server = 3009, client = 3010, des = "gcdouniujoinroom", func = "DouNiuJoinRoom"}

--离开房间
PacketCode[3011] = {server = 3011, client = 3012, des = "cgdouniuleave", func = "DouNiuLeave"}
PacketCode[3012] = {server = 3011, client = 3012, des = "gcdouniuleave", func = "DouNiuLeave"}

--坐下
PacketCode[3013] = {server = 3013, client = 3014, des = "cgdouniusitdown", func = "DouNiuSitdown"}
PacketCode[3014] = {server = 3013, client = 3014, des = "gcdouniusitdown", func = "DouNiuSitdown"}

--站起
PacketCode[3015] = {server = 3015, client = 3016, des = "cgdouniustandup", func = "DouNiuStandup"}
PacketCode[3016] = {server = 3015, client = 3016, des = "gcdouniustandup", func = "DouNiuStandup"}

--开始游戏
PacketCode[3017] = {server = 3017, client = 3018, des = "cgdouniustart", func = "DouNiuStart"}
PacketCode[3018] = {server = 3017, client = 3018, des = "gcdouniustart", func = "DouNiuStart"}

--下注
PacketCode[3019] = {server = 3019, client = 3020, des = "cgdouniupour", func = "DouNiuPour"}
PacketCode[3020] = {server = 3019, client = 3020, des = "gcdouniupour", func = "DouNiuPour"}

--发牌
PacketCode[3022] = {server = 3021, client = 3022, des = "gcdouniudeal", func = "DouNiuDeal"}

--亮牌
PacketCode[3023] = {server = 3023, client = 3024, des = "cgdouniushowpoker", func = "DouNiuShowPoker"}
PacketCode[3024] = {server = 3023, client = 3024, des = "gcdouniushowpoker", func = "DouNiuShowPoker"}

--结算
PacketCode[3026] = {server = 3025, client = 3026, des = "gcdouniucount", func = "DouNiuCount"}

--聊天
PacketCode[3027] = {server = 3027, client = 3028, des = "cgdouniuchat", func = "DouNiuChat"}
PacketCode[3028] = {server = 3027, client = 3028, des = "gcdouniuchat", func = "DouNiuChat"}

--抢庄
PacketCode[3029] = {server = 3029, client = 3030, des = "cgdouniubanket", func = "DouNiuBanker"}
PacketCode[3030] = {server = 3029, client = 3030, des = "gcdouniubanket", func = "DouNiuBanker"}

--通知进入抢庄状态
PacketCode[3032] = {server = 3031, client = 3032, des = "gcdouniubanketgo", func = "DouNiuBanketgo"}

--准备
PacketCode[3033] = {server = 3033, client = 3034, des = "cgdouniunext", func = "DouNiuNext"}
PacketCode[3034] = {server = 3033, client = 3034, des = "gcdouniunext", func = "DouNiuNext"}

--开始下一局游戏
PacketCode[3036] = {server = 3035, client = 3036, des = "gcdouniunextout", func = "DouNiuNextopt"}

--获得历史战绩
PacketCode[3037] = {server = 3037, client = 3038, des = "cgdouniuhistory", func = "DouNiuHistory"}
PacketCode[3038] = {server = 3037, client = 3038, des = "gcdouniuhistory", func = "DouNiuHistory"}

--确定庄家
PacketCode[3040] = {server = 3039, client = 3040, des = "gcdouniubanketopt", func = "DouNiuBanketopt"}

--通知进入下注状态
PacketCode[3042] = {server = 3041, client = 3042, des = "gcdouniupourgo", func = "DouNiuPourgo"}

--确定下注
PacketCode[3044] = {server = 3043, client = 3044, des = "gcdouniupouropt", func = "DouNiuPourout"}

--通知进入看牌状态
PacketCode[3046] = {server = 3045, client = 3046, des = "gcdouniushowpokergo", func = "DouNiuShowpokergo"}

-- 自动亮牌
PacketCode[3047] = {server = 3047, client = 3048, des = "cgdouiuvolun", func = "DouNiuVolun"}
PacketCode[3048] = {server = 3047, client = 3048, des = "gcdouiuvolun", func = "DouNiuVolun"}

--动态加入
PacketCode[3049] = {server = 3049, client = 3050, des = "cgdouiudynamicjion", func = "DouNiuDynamicJion"}
PacketCode[3050] = {server = 3049, client = 3050, des = "gcdouiudynamicjion", func = "DouNiuDynamicJion"}

--获取房间信息
PacketCode[3051] = {server = 3051, client = 3052, des = "cgdouiuroominfo", func = "DouNiuRoomInfo"}
PacketCode[3052] = {server = 3051, client = 3052, des = "gcdouiuroominfo", func = "DouNiuRoomInfo"}

--托管
PacketCode[3053] = {server = 3053, client = 3054, des = "cgdouniutrusteeship", func = "DouNiuTrusteeship"}
PacketCode[3054] = {server = 3053, client = 3054, des = "gcdouniutrusteeship", func = "DouNiuTrusteeship"}

--隐藏按钮
PacketCode[3056] = {server = 3055, client = 3056, des = "gcdouniuconcealbutton", func = "DouNiuConcealButton"}

--买码
PacketCode[3057] = {server = 3057, client = 3058, des = "cgdouniubuyjetton", func = "DouNiuBuyJetton"}
PacketCode[3058] = {server = 3057, client = 3058, des = "gcdouniubuyjetton", func = "DouNiuBuyJetton"}

--提牌
PacketCode[3059] = {server = 3059, client = 3060, des = "cgdouniuhintpoker", func = "DouNiuHintPoker"}
PacketCode[3060] = {server = 3059, client = 3060, des = "gcdouniuhintpoker", func = "DouNiuHintPoker"}

--加倍
PacketCode[3061] = {server = 3061, client = 3062, des = "cgdouniudouble", func = "DouNiuDouble"}
PacketCode[3062] = {server = 3061, client = 3062, des = "gcdouniudouble", func = "DouNiuDouble"}

--队长更换通知
PacketCode[3064] = {server = 3063, client = 3064, des = "gcdouniucaptainchangeinform", func = "DouNiuCaptainChangeInform"}

ReturnCode["douniu_not_exist"] = 3001
ReturnCode["douniu_chair_notmatch"] = 3002
ReturnCode["douniu_chair_isplyaer"] = 3003
ReturnCode["douniu_is_sitdown"] = 3004
ReturnCode["douniu_pwd_notmatch"] = 3005
ReturnCode["douniu_banker_exist"] = 3006
ReturnCode["douniu_banker_exist"] = 3007
ReturnCode["douniu_state_notpour"] = 3008
ReturnCode["douniu_jetton_notenough"] = 3009
ReturnCode["douniu_pourtype_error"] = 3010
ReturnCode["douniu_sit_isbanker"] = 3011
ReturnCode["douniu_banker_leave"] = 3012
ReturnCode["douniu_behanker_list"] = 3013  		--加入上庄列表
ReturnCode["douniu_behanker_drop"] = 3014  		--申请下庄
ReturnCode["douniu_pour_banker"] = 3015  		--庄家不能下注
ReturnCode["douniu_banker_notenough"] = 3016  	--筹码不足，不能上庄
ReturnCode["douniu_pour_lose"] = 3017  			--庄家不够赔钱了，不能下注
ReturnCode["douniu_is_pour"] = 3018  			--你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["douniu_full"] = 3019  				--人已经满了
ReturnCode["douniu_not_ownerid"] = 3020 		--不是房主不能开始游戏
ReturnCode["douniu_not_player"] = 3021 			--坐下玩家不足两人，不能开始游戏
ReturnCode["douniu_not_sit"]  = 3022  			--没在座位的的玩家不能进行对应操作
ReturnCode["douniu_not_state"] = 3023 			--与牌桌的状态不匹配
ReturnCode["douniu_not_pourjetton"] = 3024 		--底注类型不匹配
ReturnCode["douniu_opt_error"] = 3025     		--操作出错
ReturnCode["douniu_leave_error"] = 3026  		--离开房间出错
ReturnCode["douniu_bankermul_error"] = 3027 	--抢庄的倍数太高
ReturnCode["douniu_tbaleid_0"] = 3028 			--金币场还没开发
ReturnCode["douniu_not_enough"] = 3029 			--人数不够
ReturnCode["douniu_start_error"] = 3030 		-- 开始失败
ReturnCode["douniu_leave_state"] = 3031 		--请等待游戏结束再离开
ReturnCode["douniu_room_close"] = 3032 			--房间已经关闭，不能进入
ReturnCode["douniu_sit_ing"] = 3033				--您已经在座位上	
ReturnCode["douniu_dissolve_fail"] = 3034   	--申请解散房间失败
ReturnCode["douniu_dissolve_fail2"] = 3035 		--房间还没开始前，只有房主才能解散房间
ReturnCode["douniu_dissolve_fail3"] = 3036   	--只有在座玩家才能申请解散房间
ReturnCode["douniu_baker_pour"] = 3037   		--庄家不用下注
ReturnCode["douniu_pour_shibai"] = 3038   		--下注错误
ReturnCode["douniu_create_shibai"] = 3049       --创建房间失败
ReturnCode["douniu_money_short"] = 3050 		--钻石不足
ReturnCode["douniu_create_exceed"] = 3051 		--最多只能创建10房间哦
ReturnCode["douniu_isownerid_no"] = 3052 		--固定庄家：牌桌已经有5个人，还有一个位置是留个房主的
ReturnCode["douniu_noownerid_start"] = 3053		--固定庄家：需要房主坐下才能开始
ReturnCode["douniu_buy_banker"] = 3054		 	--买码不能买庄




ReturnCode[3001] = "该牌桌不存在"
ReturnCode[3002] = "坐下的位置不匹配"
ReturnCode[3003] = "该座位已经有玩家"
ReturnCode[3004] = "你已经坐下了，请勿重复操作"
ReturnCode[3005] = "牌桌密码不匹配"
ReturnCode[3006] = "庄家已经存在，上庄失败"
ReturnCode[3008] = "请稍后再下注"
ReturnCode[3009] = "您的筹码不足"
ReturnCode[3010] = "下注失败，请选择正确的下注区域"
ReturnCode[3011] = "坐下失败，你已经是庄家了！"
ReturnCode[3012] = "请先下庄再离开牌桌"
ReturnCode[3013] = "已加入上庄列表"
ReturnCode[3014] = "你已经申请下庄"
ReturnCode[3015] = "庄家不能下注"
ReturnCode[3016] = "筹码不足，不能上庄"
ReturnCode[3017] = "已达下注上限，下注失败"
ReturnCode[3018] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[3019] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3020] = "你不是房主不能开始游戏"
ReturnCode[3021] = "坐下玩家不足两人，不能开始游戏"
ReturnCode[3022] = "没在座位的的玩家不能进行对应操作"
ReturnCode[3023] = "与牌桌的状态不匹配"
ReturnCode[3024] = "底注类型不匹配"
ReturnCode[3025] = "操作出错"
ReturnCode[3026] = "离开房间出错"
ReturnCode[3027] = "抢庄的倍数太高"
ReturnCode[3028] = "金币场还没开发"
ReturnCode[3029] = "人数不够,无法开始"
ReturnCode[3030] = "开始失败"
ReturnCode[3031] = "请等待游戏结束再离开"
ReturnCode[3032] = "房间已经关闭，不能进入"
ReturnCode[3033] = "您已经在座位上"
ReturnCode[3034] = "申请解散房间失败"
ReturnCode[3035] = "房间还没开始前，只有房主才能解散房间"
ReturnCode[3036] = "只有在座玩家才能申请解散房间"
ReturnCode[3037] = "庄家不用下注"
ReturnCode[3038] = "下注错误"
ReturnCode[3049] = "创建房间失败"
ReturnCode[3050] = "钻石不足"
ReturnCode[3051] = "最多只能创建10房间哦"
ReturnCode[3052] = "固定庄家：牌桌已经有5个人，还有一个位置是留个房主的"
ReturnCode[3053] = "固定庄家：需要房主坐下才能开始"
ReturnCode[3054] = "买码不能买庄"
----------------------