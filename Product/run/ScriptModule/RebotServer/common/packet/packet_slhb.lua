


--7100~7200 是红包扫雷
--进入
PacketCode[7101] = {server = 7101, client = 7102, des = "cgslhbenter", func = "SlhbEnter"}
PacketCode[7102] = {server = 7101, client = 7102, des = "gcslhbenter", func = "SlhbEnter"}
--离开
PacketCode[7103] = {server = 7103, client = 7104, des = "cgslhbleave", func = "SlhbLeave"}
PacketCode[7104] = {server = 7103, client = 7104, des = "gcslhbleave", func = "SlhbLeave"}
--获得红包列表
PacketCode[7105] = {server = 7105, client = 7106, des = "cgslhbgethblist", func = "SlhbGethbList"}
PacketCode[7106] = {server = 7105, client = 7106, des = "gcslhbgethblist", func = "SlhbGethbList"}
--埋雷
PacketCode[7107] = {server = 7107, client = 7108, des = "cgslhbburiedmine", func = "SlhbBuriedMine"}
PacketCode[7108] = {server = 7107, client = 7108, des = "gcslhbburiedmine", func = "SlhbBuriedMine"}
--取消雷
PacketCode[7109] = {server = 7109, client = 7110, des = "cgslhbcancelmine", func = "SlhbCancelMine"}
PacketCode[7110] = {server = 7109, client = 7110, des = "gcslhbcancelmine", func = "SlhbCancelMine"}
--开始
PacketCode[7112] = {server = 7111, client = 7112, des = "gcslhbstart", func = "SlhbStart"}
--抢红包
PacketCode[7113] = {server = 7113, client = 7114, des = "cgslhbrobhb", func = "SlhbRobHb"}
PacketCode[7114] = {server = 7113, client = 7114, des = "gcslhbrobhb", func = "SlhbRobHb"}
--结算
PacketCode[7116] = {server = 7115, client = 7116, des = "gcslhbcount", func = "SlhbCount"}
--玩家列表
PacketCode[7117] = {server = 7117, client = 7118, des = "cgslhbstandlist", func = "SlhbStandList"}
PacketCode[7118] = {server = 7117, client = 7118, des = "gcslhbstandlist", func = "SlhbStandList"}
--聊天
PacketCode[7119] = {server = 7119, client = 7120, des = "cgslhbchat", func = "SlhbChat"}
PacketCode[7120] = {server = 7119, client = 7120, des = "gcslhbchat", func = "SlhbChat"}

--
ReturnCode["slhb_not_exist"] = 7101			--该牌桌不存在
ReturnCode["slhb_is_full"] = 7102  			--该牌桌人数已满，请选择其他牌桌
ReturnCode["slhb__jetton_enter_1"] = 7103	--金币不足，无法进入牌桌
ReturnCode["slhb__jetton_BuriedMine_1"] = 7104	--金币不足，无法发红包
ReturnCode["slhb_bomb_error"] = 7105		--雷号不正确， 请设置在0~9的范围
ReturnCode["slhb_jetton_hb"] = 7106			--红包的金额错误，请设置在10~500的范围
ReturnCode["slhb_hb_exist"] = 7107			--您已经有红包在排队了，请稍后再来发红包
ReturnCode["slhb_hb_isyou"] = 7108			--您正在发红包，请稍后再退出游戏
ReturnCode["slhb_hb_rob"] = 7109			--您正在抢红包，请稍后再退出游戏
ReturnCode["slhb_hb_isyou_1"] = 7110		--这个红包是您发的哦！不要抢自己的红包嘛！
ReturnCode["jetton_enter_2"] = 7111			--金币不足，无法抢红包！
ReturnCode["slhb_banker_leave"] = 7112
ReturnCode["slhb_behanker_list"] = 7113  --加入上庄列表
ReturnCode["slhb_behanker_drop"] = 7114  --申请下庄
ReturnCode["slhb_pour_banker"] = 7115  --庄家不能下注
ReturnCode["slhb_banker_notenough"] = 7116  --筹码不足，不能上庄
ReturnCode["slhb_pour_lose"] = 7117  --庄家不够赔钱了，不能下注
ReturnCode["slhb_is_pour"] = 7118  --你已经在本局游戏中下注，请等待游戏结束再离

ReturnCode["slhb_is_bankerlist"] = 7120  --已经在上庄列表中，请耐心等候


ReturnCode["slhb_is_full"] = 7102  			--该牌桌人数已满，请选择其他牌桌

ReturnCode[7101] = "该牌桌不存在"
ReturnCode[7102] = "该牌桌人数已满，请选择其他牌桌!"
ReturnCode[7103] = "金币不足，无法进入牌桌!"
ReturnCode[7104] = "金币不足，无法发红包!"
ReturnCode[7105] = "雷号不正确，请设置在0~9的范围!"	
ReturnCode[7106] = "红包的金额错误，请设置在10~500的范围!"
ReturnCode[7108] = "您正在发红包，请稍后再退出游戏"
ReturnCode[7109] = "您正在抢红包，请稍后再退出游戏"
ReturnCode[7110] = "这个红包是您发的哦！不要抢自己的红包嘛！"
ReturnCode[7111] = "金币不足，无法抢红包！"


