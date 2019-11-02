--9000~8700 是斗牛的玩法
--点击牛牛图标，获取玩家的自己创建的房间										
PacketCode[9001] = {server = 9001, client = 9002, des = "cgdouniutztablelist", func = "DouNiuTzTableList"}
PacketCode[9002] = {server = 9001, client = 9002, des = "gcdouniutztablelist", func = "DouNiuTzTableList"}

--创建房间
PacketCode[9003] = {server = 9003, client = 9004, des = "cgdouniutzcreate", func = "DouNiuTzCreate"}
PacketCode[9004] = {server = 9003, client = 9004, des = "gcdouniutzcreate", func = "DouNiuTzCreate"}

--发起解散房间
PacketCode[9005] = {server = 9005, client = 9006, des = "cgdouniutzdissolve", func = "DouNiuTzDissolve"}
PacketCode[9006] = {server = 9005, client = 9006, des = "gcdouniutzdissolve", func = "DouNiuTzDissolve"}

--回应解散房间请求  
PacketCode[9007] = {server = 9007, client = 9008, des = "cgdouniutzdissolveopt", func = "DouNiuTzDissolveOpt"}
PacketCode[9008] = {server = 9007, client = 9008, des = "gcdouniutzdissolveopt", func = "DouNiuTzDissolveOpt"}

--加入房间
PacketCode[9009] = {server = 9009, client = 9010, des = "cgdouniutzjoinroom", func = "DouNiuTzJoinRoom"}
PacketCode[9010] = {server = 9009, client = 9010, des = "gcdouniutzjoinroom", func = "DouNiuTzJoinRoom"}

--离开房间
PacketCode[9011] = {server = 9011, client = 9012, des = "cgdouniutzleave", func = "DouNiuTzLeave"}
PacketCode[9012] = {server = 9011, client = 9012, des = "gcdouniutzleave", func = "DouNiuTzLeave"}

--坐下
PacketCode[9013] = {server = 9013, client = 9014, des = "cgdouniutzsitdown", func = "DouNiuTzSitdown"}
PacketCode[9014] = {server = 9013, client = 9014, des = "gcdouniutzsitdown", func = "DouNiuTzSitdown"}

--站起
PacketCode[9015] = {server = 9015, client = 9016, des = "cgdouniutzstandup", func = "DouNiuTzStandup"}
PacketCode[9016] = {server = 9015, client = 9016, des = "gcdouniutzstandup", func = "DouNiuTzStandup"}

--开始游戏
PacketCode[9017] = {server = 9017, client = 9018, des = "cgdouniutzstart", func = "DouNiuTzStart"}
PacketCode[9018] = {server = 9017, client = 9018, des = "gcdouniutzstart", func = "DouNiuTzStart"}

--下注
PacketCode[9019] = {server = 9019, client = 9020, des = "cgdouniutzpour", func = "DouNiuTzPour"}
PacketCode[9020] = {server = 9019, client = 9020, des = "gcdouniutzpour", func = "DouNiuTzPour"}

--发牌
PacketCode[9022] = {server = 9021, client = 9022, des = "gcdouniutzdeal", func = "DouNiuTzDeal"}

--亮牌
PacketCode[9023] = {server = 9023, client = 9024, des = "cgdouniutzshowpoker", func = "DouNiuTzShowPoker"}
PacketCode[9024] = {server = 9023, client = 9024, des = "gcdouniutzshowpoker", func = "DouNiuTzShowPoker"}

--结算
PacketCode[9026] = {server = 9025, client = 9026, des = "gcdouniutzcount", func = "DouNiuTzCount"}

--聊天
PacketCode[9027] = {server = 9027, client = 9028, des = "cgdouniutzchat", func = "DouNiuTzChat"}
PacketCode[9028] = {server = 9027, client = 9028, des = "gcdouniutzchat", func = "DouNiuTzChat"}

--抢庄
PacketCode[9029] = {server = 9029, client = 9030, des = "cgdouniutzbanket", func = "DouNiuTzBanker"}
PacketCode[9030] = {server = 9029, client = 9030, des = "gcdouniutzbanket", func = "DouNiuTzBanker"}

--通知进入抢庄状态
PacketCode[9032] = {server = 9031, client = 9032, des = "gcdouniutzbanketgo", func = "DouNiuTzBanketgo"}

--准备
PacketCode[9033] = {server = 9033, client = 9034, des = "cgdouniutznext", func = "DouNiuTzNext"}
PacketCode[9034] = {server = 9033, client = 9034, des = "gcdouniutznext", func = "DouNiuTzNext"}

--开始下一局游戏
PacketCode[9036] = {server = 9035, client = 9036, des = "gcdouniutznextout", func = "DouNiuTzNextopt"}

--获得历史战绩
PacketCode[9037] = {server = 9037, client = 9038, des = "cgdouniutzhistory", func = "DouNiuTzHistory"}
PacketCode[9038] = {server = 9037, client = 9038, des = "gcdouniutzhistory", func = "DouNiuTzHistory"}

--确定庄家
PacketCode[9040] = {server = 9039, client = 9040, des = "gcdouniutzbanketopt", func = "DouNiuTzBanketopt"}

--通知进入下注状态
PacketCode[9042] = {server = 9041, client = 9042, des = "gcdouniutzpourgo", func = "DouNiuTzPourgo"}

--确定下注
PacketCode[9044] = {server = 9043, client = 9044, des = "gcdouniutzpouropt", func = "DouNiuTzPourout"}

--通知进入看牌状态
PacketCode[9046] = {server = 9045, client = 9046, des = "gcdouniutzshowpokergo", func = "DouNiuTzShowpokergo"}

-- 自动亮牌
PacketCode[9047] = {server = 9047, client = 9048, des = "cgdouiuvolun", func = "DouNiuTzVolun"}
PacketCode[9048] = {server = 9047, client = 9048, des = "gcdouiuvolun", func = "DouNiuTzVolun"}

--动态加入
PacketCode[9049] = {server = 9049, client = 9050, des = "cgdouiudynamicjion", func = "DouNiuTzDynamicJion"}
PacketCode[9050] = {server = 9049, client = 9050, des = "gcdouiudynamicjion", func = "DouNiuTzDynamicJion"}

--获取房间信息
PacketCode[9051] = {server = 9051, client = 9052, des = "cgdouiuroominfo", func = "DouNiuTzRoomInfo"}
PacketCode[9052] = {server = 9051, client = 9052, des = "gcdouiuroominfo", func = "DouNiuTzRoomInfo"}

--托管
PacketCode[9053] = {server = 9053, client = 9054, des = "cgdouniutztrusteeship", func = "DouNiuTzTrusteeship"}
PacketCode[9054] = {server = 9053, client = 9054, des = "gcdouniutztrusteeship", func = "DouNiuTzTrusteeship"}

--换桌
PacketCode[9055] = {server = 9055, client = 9056, des = "cgdouniutzvarytable", func = "DouNiuTzVaryTable"}
PacketCode[9056] = {server = 9055, client = 9056, des = "gcdouniutzvarytable", func = "DouNiuTzVaryTable"}

--旁观进入
PacketCode[9057] = {server = 9057, client = 9058, des = "cgdouniutzupjoinroom", func = "DouNiuTzupJoinRoom"}
PacketCode[9058] = {server = 9057, client = 9058, des = "gcdouniutzupjoinroom", func = "DouNiuTzupJoinRoom"}

--旁观进入
--PacketCode[9059] = {server = 9059, client = 9060, des = "cgdouniutzupjoinroom", func = "DouNiuTzupJoinRoom"}
PacketCode[9060] = {server = 9059, client = 9060, des = "gcdouniutzsecondfapai", func = "DouNiuTzsecondfapai"}



ReturnCode["douniutz_not_exist"] = 9001
ReturnCode["douniutz_chair_notmatch"] = 9002
ReturnCode["douniutz_chair_isplyaer"] = 9003
ReturnCode["douniutz_is_sitdown"] = 9004
ReturnCode["douniutz_pwd_notmatch"] = 9005
ReturnCode["douniutz_banker_exist"] = 9006
ReturnCode["douniutz_banker_exist"] = 9007
ReturnCode["douniutz_state_notpour"] = 9008
ReturnCode["douniutz_jetton_notenough"] = 9009
ReturnCode["douniutz_pourtype_error"] = 9010
ReturnCode["douniutz_sit_isbanker"] = 9011
ReturnCode["douniutz_banker_leave"] = 9012
ReturnCode["douniutz_behanker_list"] = 9013  		--加入上庄列表
ReturnCode["douniutz_behanker_drop"] = 9014  		--申请下庄
ReturnCode["douniutz_pour_banker"] = 9015  		--庄家不能下注
ReturnCode["douniutz_banker_notenough"] = 9016  	--筹码不足，不能上庄
ReturnCode["douniutz_pour_lose"] = 9017  			--庄家不够赔钱了，不能下注
ReturnCode["douniutz_is_pour"] = 9018  			--你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["douniutz_full"] = 9019  				--人已经满了
ReturnCode["douniutz_not_ownerid"] = 9020 		--不是房主不能开始游戏
ReturnCode["douniutz_not_player"] = 9021 			--坐下玩家不足两人，不能开始游戏
ReturnCode["douniutz_not_sit"]  = 9022  			--没在座位的的玩家不能进行对应操作
ReturnCode["douniutz_not_state"] = 9023 			--与牌桌的状态不匹配
ReturnCode["douniutz_not_pourjetton"] = 9024 		--底注类型不匹配
ReturnCode["douniutz_opt_error"] = 9025     		--操作出错
ReturnCode["douniutz_leave_error"] = 9026  		--离开房间出错
ReturnCode["douniutz_bankermul_error"] = 9027 	--抢庄的倍数太高
ReturnCode["douniutz_tbaleid_0"] = 9028 			--金币场还没开发
ReturnCode["douniutz_not_enough"] = 9029 			--人数不够
ReturnCode["douniutz_start_error"] = 9030 		-- 开始失败
ReturnCode["douniutz_leave_state"] = 9031 		--请等待游戏结束再离开
ReturnCode["douniutz_room_close"] = 9032 			--房间已经关闭，不能进入
ReturnCode["douniutz_sit_ing"] = 9033				--您已经在座位上	
ReturnCode["douniutz_dissolve_fail"] = 9034   	--申请解散房间失败
ReturnCode["douniutz_dissolve_fail2"] = 9035 		--房间还没开始前，只有房主才能解散房间
ReturnCode["douniutz_dissolve_fail3"] = 9036   	--只有在座玩家才能申请解散房间
ReturnCode["douniutz_baker_pour"] = 9037   		--庄家不用下注
ReturnCode["douniutz_pour_shibai"] = 9038   		--下注错误
ReturnCode["douniutz_create_shibai"] = 9049       --创建房间失败
ReturnCode["douniutz_money_short"] = 9050 		--钻石不足
ReturnCode["douniutz_create_exceed"] = 9051 		--最多只能创建10房间哦
ReturnCode["douniutz_isownerid_no"] = 9052 		--固定庄家：牌桌已经有5个人，还有一个位置是留个房主的
ReturnCode["douniutz_noownerid_start"] = 9053		 --固定庄家：需要房主坐下才能开始
ReturnCode["douniutz_put_error"] = 9054		 	--拼牛需要有10倍数哦
ReturnCode["douniutz_Jetton_overbrim"] = 9055		--您的金币太多了， 请到更高级的场去玩！！！！
ReturnCode["douniutz_reward_not_exist"] = 9056		--奖励不存在



 
ReturnCode[9001] = "该牌桌不存在"
ReturnCode[9002] = "坐下的位置不匹配"
ReturnCode[9003] = "该座位已经有玩家"
ReturnCode[9004] = "你已经坐下了，请勿重复操作"
ReturnCode[9005] = "牌桌密码不匹配"
ReturnCode[9006] = "庄家已经存在，上庄失败"
ReturnCode[9008] = "请稍后再下注"
ReturnCode[9009] = "您的筹码不足"
ReturnCode[9010] = "下注失败，请选择正确的下注区域"
ReturnCode[9011] = "坐下失败，你已经是庄家了！"
ReturnCode[9012] = "请先下庄再离开牌桌"
ReturnCode[9013] = "已加入上庄列表"
ReturnCode[9014] = "你已经申请下庄"
ReturnCode[9015] = "庄家不能下注"
ReturnCode[9016] = "筹码不足，不能上庄"
ReturnCode[9017] = "已达下注上限，下注失败"
ReturnCode[9018] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[9019] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[9020] = "你不是房主不能开始游戏"
ReturnCode[9021] = "坐下玩家不足两人，不能开始游戏"
ReturnCode[9022] = "没在座位的的玩家不能进行对应操作"
ReturnCode[9023] = "与牌桌的状态不匹配"
ReturnCode[9024] = "底注类型不匹配"
ReturnCode[9025] = "操作出错"
ReturnCode[9026] = "离开房间出错"
ReturnCode[9027] = "抢庄的倍数太高"
ReturnCode[9028] = "金币场还没开发"
ReturnCode[9029] = "人数不够,无法开始"
ReturnCode[9030] = "开始失败"
ReturnCode[9031] = "请等待游戏结束再离开"
ReturnCode[9032] = "房间已经关闭，不能进入"
ReturnCode[9033] = "您已经在座位上"
ReturnCode[9034] = "申请解散房间失败"
ReturnCode[9035] = "房间还没开始前，只有房主才能解散房间"
ReturnCode[9036] = "只有在座玩家才能申请解散房间"
ReturnCode[9037] = "庄家不用下注"
ReturnCode[9038] = "下注错误"
ReturnCode[9049] = "创建房间失败"
ReturnCode[9050] = "钻石不足"
ReturnCode[9051] = "最多只能创建10房间哦"
ReturnCode[9052] = "固定庄家：牌桌已经有5个人，还有一个位置是留个房主的"
ReturnCode[9053] = "固定庄家：需要房主坐下才能开始"
ReturnCode[9054] = "拼牛需要有10倍数哦"
ReturnCode[9055] = "您的金币太多了， 请到更高级的场去玩！！！！"
ReturnCode[9056] = "奖励不存在"

----------------------