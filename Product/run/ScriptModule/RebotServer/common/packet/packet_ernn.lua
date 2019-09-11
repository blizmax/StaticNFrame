--3000~3100 是斗牛的玩法
--点击牛牛图标，获取玩家的自己创建的房间										
PacketCode[3001] = {server = 3001, client = 3002, des = "cgernntablelist", func = "ErnnTableList"}
PacketCode[3002] = {server = 3001, client = 3002, des = "gcernntablelist", func = "ErnnTableList"}

--创建房间
PacketCode[3003] = {server = 3003, client = 3004, des = "cgernncreate", func = "ErnnCreate"}
PacketCode[3004] = {server = 3003, client = 3004, des = "gcernncreate", func = "ErnnCreate"}

--发起解散房间
PacketCode[3005] = {server = 3005, client = 3006, des = "cgernndissolve", func = "ErnnDissolve"}
PacketCode[3006] = {server = 3005, client = 3006, des = "gcernndissolve", func = "ErnnDissolve"}

--回应解散房间请求  
PacketCode[3007] = {server = 3007, client = 3008, des = "cgernndissolveopt", func = "ErnnDissolveOpt"}
PacketCode[3008] = {server = 3007, client = 3008, des = "gcernndissolveopt", func = "ErnnDissolveOpt"}

--加入房间
PacketCode[3009] = {server = 3009, client = 3010, des = "cgernnjoinroom", func = "ErnnJoinRoom"}
PacketCode[3010] = {server = 3009, client = 3010, des = "gcernnjoinroom", func = "ErnnJoinRoom"}

--离开房间
PacketCode[3011] = {server = 3011, client = 3012, des = "cgernnleave", func = "ErnnLeave"}
PacketCode[3012] = {server = 3011, client = 3012, des = "gcernnleave", func = "ErnnLeave"}

--坐下
PacketCode[3013] = {server = 3013, client = 3014, des = "cgernnsitdown", func = "ErnnSitdown"}
PacketCode[3014] = {server = 3013, client = 3014, des = "gcernnsitdown", func = "ErnnSitdown"}

--站起
PacketCode[3015] = {server = 3015, client = 3016, des = "cgernnstandup", func = "ErnnStandup"}
PacketCode[3016] = {server = 3015, client = 3016, des = "gcernnstandup", func = "ErnnStandup"}

--开始游戏
PacketCode[3017] = {server = 3017, client = 3018, des = "cgernnstart", func = "ErnnStart"}
PacketCode[3018] = {server = 3017, client = 3018, des = "gcernnstart", func = "ErnnStart"}

--下注
PacketCode[3019] = {server = 3019, client = 3020, des = "cgernnpour", func = "ErnnPour"}
PacketCode[3020] = {server = 3019, client = 3020, des = "gcernnpour", func = "ErnnPour"}

--发牌
PacketCode[3022] = {server = 3021, client = 3022, des = "gcernndeal", func = "ErnnDeal"}

--亮牌
PacketCode[3023] = {server = 3023, client = 3024, des = "cgernnshowpoker", func = "ErnnShowPoker"}
PacketCode[3024] = {server = 3023, client = 3024, des = "gcernnshowpoker", func = "ErnnShowPoker"}

--结算
PacketCode[3026] = {server = 3025, client = 3026, des = "gcernncount", func = "ErnnCount"}

--聊天
PacketCode[3027] = {server = 3027, client = 3028, des = "cgernnchat", func = "ErnnChat"}
PacketCode[3028] = {server = 3027, client = 3028, des = "gcernnchat", func = "ErnnChat"}

--抢庄
PacketCode[3029] = {server = 3029, client = 3030, des = "cgernnbanket", func = "ErnnBanker"}
PacketCode[3030] = {server = 3029, client = 3030, des = "gcernnbanket", func = "ErnnBanker"}

--通知进入抢庄状态
PacketCode[3032] = {server = 3031, client = 3032, des = "gcernnbanketgo", func = "ErnnBanketgo"}

--准备
PacketCode[3033] = {server = 3033, client = 3034, des = "cgernnnext", func = "ErnnNext"}
PacketCode[3034] = {server = 3033, client = 3034, des = "gcernnnext", func = "ErnnNext"}

--开始下一局游戏
PacketCode[3036] = {server = 3035, client = 3036, des = "gcernnnextout", func = "ErnnNextopt"}

--获得历史战绩
PacketCode[3037] = {server = 3037, client = 3038, des = "cgernnhistory", func = "ErnnHistory"}
PacketCode[3038] = {server = 3037, client = 3038, des = "gcernnhistory", func = "ErnnHistory"}

--确定庄家
PacketCode[3040] = {server = 3039, client = 3040, des = "gcernnbanketopt", func = "ErnnBanketopt"}

--通知进入下注状态
PacketCode[3042] = {server = 3041, client = 3042, des = "gcernnpourgo", func = "ErnnPourgo"}

--确定下注
PacketCode[3044] = {server = 3043, client = 3044, des = "gcernnpouropt", func = "ErnnPourout"}

--通知进入看牌状态
PacketCode[3046] = {server = 3045, client = 3046, des = "gcernnshowpokergo", func = "ErnnShowpokergo"}

-- 自动亮牌
PacketCode[3047] = {server = 3047, client = 3048, des = "cgdouiuvolun", func = "ErnnVolun"}
PacketCode[3048] = {server = 3047, client = 3048, des = "gcdouiuvolun", func = "ErnnVolun"}

--动态加入
PacketCode[3049] = {server = 3049, client = 3050, des = "cgdouiudynamicjion", func = "ErnnDynamicJion"}
PacketCode[3050] = {server = 3049, client = 3050, des = "gcdouiudynamicjion", func = "ErnnDynamicJion"}

--获取房间信息
PacketCode[3051] = {server = 3051, client = 3052, des = "cgdouiuroominfo", func = "ErnnRoomInfo"}
PacketCode[3052] = {server = 3051, client = 3052, des = "gcdouiuroominfo", func = "ErnnRoomInfo"}

--托管
PacketCode[3053] = {server = 3053, client = 3054, des = "cgernntrusteeship", func = "ErnnTrusteeship"}
PacketCode[3054] = {server = 3053, client = 3054, des = "gcernntrusteeship", func = "ErnnTrusteeship"}


ReturnCode["ernn_not_exist"] = 3001
ReturnCode["ernn_chair_notmatch"] = 3002
ReturnCode["ernn_chair_isplyaer"] = 3003
ReturnCode["ernn_is_sitdown"] = 3004
ReturnCode["ernn_pwd_notmatch"] = 3005
ReturnCode["ernn_banker_exist"] = 3006
ReturnCode["ernn_banker_exist"] = 3007
ReturnCode["ernn_state_notpour"] = 3008
ReturnCode["ernn_jetton_notenough"] = 3009
ReturnCode["ernn_pourtype_error"] = 3010
ReturnCode["ernn_sit_isbanker"] = 3011
ReturnCode["ernn_banker_leave"] = 3012
ReturnCode["ernn_behanker_list"] = 3013  		--加入上庄列表
ReturnCode["ernn_behanker_drop"] = 3014  		--申请下庄
ReturnCode["ernn_pour_banker"] = 3015  		--庄家不能下注
ReturnCode["ernn_banker_notenough"] = 3016  	--筹码不足，不能上庄
ReturnCode["ernn_pour_lose"] = 3017  			--庄家不够赔钱了，不能下注
ReturnCode["ernn_is_pour"] = 3018  			--你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["ernn_full"] = 3019  				--人已经满了
ReturnCode["ernn_not_ownerid"] = 3020 		--不是房主不能开始游戏
ReturnCode["ernn_not_player"] = 3021 			--坐下玩家不足两人，不能开始游戏
ReturnCode["ernn_not_sit"]  = 3022  			--没在座位的的玩家不能进行对应操作
ReturnCode["ernn_not_state"] = 3023 			--与牌桌的状态不匹配
ReturnCode["ernn_not_pourjetton"] = 3024 		--底注类型不匹配
ReturnCode["ernn_opt_error"] = 3025     		--操作出错
ReturnCode["ernn_leave_error"] = 3026  		--离开房间出错
ReturnCode["ernn_bankermul_error"] = 3027 	--抢庄的倍数太高
ReturnCode["ernn_tbaleid_0"] = 3028 			--金币场还没开发
ReturnCode["ernn_not_enough"] = 3029 			--人数不够
ReturnCode["ernn_start_error"] = 3030 		-- 开始失败
ReturnCode["ernn_leave_state"] = 3031 		--请等待游戏结束再离开
ReturnCode["ernn_room_close"] = 3032 			--房间已经关闭，不能进入
ReturnCode["ernn_sit_ing"] = 3033				--您已经在座位上	
ReturnCode["ernn_dissolve_fail"] = 3034   	--申请解散房间失败
ReturnCode["ernn_dissolve_fail2"] = 3035 		--房间还没开始前，只有房主才能解散房间
ReturnCode["ernn_dissolve_fail3"] = 3036   	--只有在座玩家才能申请解散房间
ReturnCode["ernn_baker_pour"] = 3037   		--庄家不用下注
ReturnCode["ernn_pour_shibai"] = 3038   		--下注错误
ReturnCode["ernn_create_shibai"] = 3049       --创建房间失败
ReturnCode["ernn_money_short"] = 3050 		--钻石不足
ReturnCode["ernn_create_exceed"] = 3051 		--最多只能创建10房间哦
ReturnCode["ernn_isownerid_no"] = 3052 		--固定庄家：牌桌已经有5个人，还有一个位置是留个房主的
ReturnCode["ernn_noownerid_start"] = 3053		 --固定庄家：需要房主坐下才能开始
ReturnCode["ernn_tabletype_error"] = 3054		-- 桌子类型错误



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
ReturnCode[3054] = "桌子类型错误"
----------------------