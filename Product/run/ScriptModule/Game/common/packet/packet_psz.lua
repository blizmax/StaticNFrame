--3300~3400 是拼三张玩法的协议
--获取玩家的自己创建的房间列表										
PacketCode[3301] = {server = 3301, client = 3302, des = "cgpsztablelist", func = "PszTableList"}
PacketCode[3302] = {server = 3301, client = 3302, des = "gcpsztablelist", func = "PszTableList"}

--创建房间
PacketCode[3303] = {server = 3303, client = 3304, des = "cgpszcreate", func = "PszCreate"}
PacketCode[3304] = {server = 3303, client = 3304, des = "gcpszcreate", func = "PszCreate"}

--发起解散房间
PacketCode[3305] = {server = 3305, client = 3306, des = "cgpszdissolve", func = "PszDissolve"}
PacketCode[3306] = {server = 3305, client = 3306, des = "gcpszdissolve", func = "PszDissolve"}

--回应解散房间请求  
PacketCode[3307] = {server = 3307, client = 3308, des = "cgpszdissolveopt", func = "PszDissolveopt"}
PacketCode[3308] = {server = 3307, client = 3308, des = "gcpszdissolveopt", func = "PszDissolveopt"}

--加入房间
PacketCode[3309] = {server = 3309, client = 3310, des = "cgpszjoinroom", func = "PszJoinRoom"}
PacketCode[3310] = {server = 3309, client = 3310, des = "gcpszjoinroom", func = "PszJoinRoom"}

--离开房间
PacketCode[3311] = {server = 3311, client = 3312, des = "cgpszleave", func = "PszLeave"}
PacketCode[3312] = {server = 3311, client = 3312, des = "gcpszleave", func = "PszLeave"}

--坐下
PacketCode[3313] = {server = 3313, client = 3314, des = "cgpszsitdown", func = "PszSitdown"}
PacketCode[3314] = {server = 3313, client = 3314, des = "gcpszsitdown", func = "PszSitdown"}


--开始游戏
PacketCode[3315] = {server = 3315, client = 3316, des = "cgpszstart", func = "PszStart"}
PacketCode[3316] = {server = 3315, client = 3316, des = "gcpszstart", func = "PszStart"}

--通知行动玩家
PacketCode[3318] = {server = 3317, client = 3318, des = "gcpszinformaction", func = "PszInformAction"}

--玩家的行动
PacketCode[3319] = {server = 3319, client = 3320, des = "cgpszaction", func = "PszAction"}
PacketCode[3320] = {server = 3319, client = 3320, des = "gcpszaction", func = "PszAction"}

--结算
PacketCode[3322] = {server = 3321, client = 3322, des = "gcpszcount", func = "PszCount"}

--聊天
PacketCode[3323] = {server = 3323, client = 3324, des = "cgpszchat", func = "PszChat"}
PacketCode[3324] = {server = 3323, client = 3324, des = "gcpszchat", func = "PszChat"}

--准备
PacketCode[3325] = {server = 3325, client = 3326, des = "cgpsznext", func = "PszNext"}
PacketCode[3326] = {server = 3325, client = 3326, des = "gcpsznext", func = "PszNext"}

--等待下一局游戏开始
PacketCode[3328] = {server = 3327, client = 3328, des = "gcpsznextout", func = "PszNextopt"}

--获得历史战绩
PacketCode[3329] = {server = 3329, client = 3330, des = "cgpszhistory", func = "PszHistory"}
PacketCode[3330] = {server = 3329, client = 3330, des = "gcpszhistory", func = "PszHistory"}

--进人房间时，当游戏已经开始，主动推送给玩家
PacketCode[3331] = {server = 3331, client = 3332, des = "cgpszroominfo", func = "Pszroominfo"}
PacketCode[3332] = {server = 3331, client = 3332, des = "gcpszroominfo", func = "Pszroominfo"}

--站起
PacketCode[3334] = {server = 3333, client = 3334, des = "gcpszstandup", func = "PszStandup"}

--通知客户端隐藏看牌加注跟注等按钮
PacketCode[3336] = {server = 3335, client = 3336, des = "gcpszconcealbutton", func = "PszConcealButton"}

--自动跟注
PacketCode[3337] = {server = 3337, client = 3338, des = "cgpszaudocingl", func = "PszAudoCingl"}
PacketCode[3338] = {server = 3337, client = 3338, des = "gcpszaudocingl", func = "PszAudoCingl"}

--最后亮牌
PacketCode[3339] = {server = 3339, client = 3340, des = "cgpszendshow", func = "PszEndShow"}
PacketCode[3340] = {server = 3339, client = 3340, des = "gcpszendshow", func = "PszEndShow"}

--动态加入
PacketCode[3341] = {server = 3341, client = 3342, des = "cgpszdynamicjion", func = "PszDynamicJion"}
PacketCode[3342] = {server = 3341, client = 3342, des = "gcpszdynamicjion", func = "PszDynamicJion"}

--托管
PacketCode[3343] = {server = 3343, client = 3344, des = "cgpsztrusteeship", func = "PszTrusteeship"}
PacketCode[3344] = {server = 3343, client = 3344, des = "gcpsztrusteeship", func = "PszTrusteeship"}

ReturnCode["psz_not_exist"] = 3301			--该牌桌不存在
ReturnCode["psz_chair_isplyaer"] = 3302		--该座位已经有玩家
ReturnCode["psz_is_sitdown"] = 3303			--你已经坐下了，请勿重复操作
ReturnCode["psz_state_notpour"] = 3304		--请稍后再下注
ReturnCode["psz_full"] = 3305  				--人已经满了
ReturnCode["psz_not_ownerid"] = 3306 		--不是房主不能开始游戏
ReturnCode["psz_not_player"] = 3307 		--坐下玩家不足两人，不能开始游戏
ReturnCode["psz_not_state"] = 3308 			--还没有轮到你操作
ReturnCode["psz_opt_error"] = 3309     		--操作出错
ReturnCode["psz_leave_error"] = 3310  		--离开房间出错
ReturnCode["psz_tbaleid_0"] = 3311 			--金币场还没开发
ReturnCode["psz_not_enough"] = 3312 		--人数不够
ReturnCode["psz_start_error"] = 3313 		--开始失败
ReturnCode["psz_leave_state"] = 3314 		--请等待游戏结束再离开
ReturnCode["psz_room_close"] = 3315			--房间已经关闭，不能进入
ReturnCode["psz_sit_ing"] = 33016 			--您已经在座位上	
ReturnCode["psz_dissolve_fail"] = 3317   	--申请解散房间失败
ReturnCode["psz_dissolve_fail2"] = 3318 	--房间还没开始前，只有房主才能解散房间
ReturnCode["psz_dissolve_fail3"] = 3319   	--只有在座玩家才能申请解散房间
ReturnCode["psz_pour_shibai"] = 3320   		--下注错误
ReturnCode["psz_create_shibai"] = 3321       --创建房间失败
ReturnCode["psz_money_short"] = 3322 		--钻石不足
ReturnCode["psz_create_exceed"] = 3323 		--最多只能创建10房间哦
ReturnCode["psz_action_no"] = 3324 			--还没轮到你操作，请等待
ReturnCode["psz_mincomp_no"] = 3325 		--现在还不可以比牌
ReturnCode["psz_menpai_no"] = 3326 			--现在还不可以看牌
ReturnCode["psz_not_sit"] = 3327 			--你不在座位，不能操作
ReturnCode["psz_owner_start"] = 3328		--只有房主才能开始房间
ReturnCode["psz_jeeton_short"] = 3329		--金币不足下注失败
ReturnCode["psz_upjetton_no"] = 3330		--加注失败
ReturnCode["psz_bipai_no"] = 3331			--比牌失败
ReturnCode["psz_not_read"] = 3333			--还有人没准备
ReturnCode["psz_endshow_no"] = 3333			--牌局还没结束不能亮牌
ReturnCode["psz_tbale_end"] = 3334			
ReturnCode["psz_jetton_error_1"] = 3335		
ReturnCode["psz_jetton_error_2"] = 3336




ReturnCode[3301] = "该牌桌不存在"
ReturnCode[3302] = "该座位已经有玩家"
ReturnCode[3303] = "你已经坐下了，请勿重复操作"
ReturnCode[3304] = "请稍后再下注"
ReturnCode[3305] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3306] = "你不是房主不能开始游戏"
ReturnCode[3307] = "坐下玩家不足两人，不能开始游戏"
ReturnCode[3308] = "还没有轮到你操作"
ReturnCode[3309] = "操作出错"
ReturnCode[3310] = "离开房间出错"
ReturnCode[3311] = "金币场还没开发"
ReturnCode[3312] = "人数不够,无法开始"
ReturnCode[3313] = "开始失败"
ReturnCode[3314] = "请等待游戏结束再离开"
ReturnCode[3315] = "房间已经关闭，不能进入"
ReturnCode[3316] = "您已经在座位上"
ReturnCode[3317] = "申请解散房间失败"
ReturnCode[3318] = "房间还没开始前，只有房主才能解散房间"
ReturnCode[3319] = "只有在座玩家才能申请解散房间"
ReturnCode[3320] = "下注错误"
ReturnCode[3321] = "创建房间失败"
ReturnCode[3322] = "钻石不足"
ReturnCode[3323] = "最多只能创建10房间哦"
ReturnCode[3324] = "还没轮到你操作，请等待"
ReturnCode[3325] = "现在还不可以比牌"
ReturnCode[3326] = "现在还不可以看牌"
ReturnCode[3327] = "你不在座位，不能操作"
ReturnCode[3328] = "只有房主才能开始房间"
ReturnCode[3329] = "金币不足下注失败"
ReturnCode[3330] = "加注失败"
ReturnCode[3331] = "比牌失败"
ReturnCode[3333] = "还有人没准备"
ReturnCode[3333] = "牌局还没结束不能亮牌"
ReturnCode[3334] = "最后一局已经开了，您就别进去了"
ReturnCode[3335] = "积分不足，请选择弃牌，或者跟注！"
ReturnCode[3336] = "积分不足，操作失败。系统已为加多2分钟的操作时间，及时找群主充值！"

----------------------