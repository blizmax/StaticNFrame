

--2100~2200 是欢乐竞技场的协议号区间
PacketCode[2101] = {server = 2101, client = 2102, des = "cghljjenter", func = "HljjEnter"}
PacketCode[2102] = {server = 2101, client = 2102, des = "gchljjenter", func = "HljjEnter"}

PacketCode[2103] = {server = 2103, client = 2104, des = "cghljjleave", func = "HljjLeave"}
PacketCode[2104] = {server = 2103, client = 2104, des = "gchljjleave", func = "HljjLeave"}

PacketCode[2105] = {server = 2105, client = 2106, des = "cghljjsitdown", func = "HljjSitDown"}
PacketCode[2106] = {server = 2105, client = 2106, des = "gchljjsitdown", func = "HljjSitDown"}

PacketCode[2107] = {server = 2107, client = 2108, des = "cghljjpourjetton", func = "HljjPourJetton"}
PacketCode[2108] = {server = 2107, client = 2108, des = "gchljjpourjetton", func = "HljjPourJetton"}

PacketCode[2110] = {server = 2109, client = 2110, des = "gchljjcount", func = "HljjCount"}

PacketCode[2111] = {server = 2111, client = 2112, des = "cghljjtablelist", func = "HljjTableList"}
PacketCode[2112] = {server = 2111, client = 2112, des = "gchljjtablelist", func = "HljjTableList"}

PacketCode[2113] = {server = 2113, client = 2102, des = "cghljjcreatetable", func = "HljjCreateTable"}
PacketCode[2114] = {server = 2113, client = 2114, des = "gchljjcreatetable", func = "HljjCreateTable"}

PacketCode[2115] = {server = 2115, client = 2116, des = "cghljjbeingbanker", func = "HljjBeingBanker"}
PacketCode[2116] = {server = 2115, client = 2116, des = "gchljjbeingbanker", func = "HljjBeingBanker"}

PacketCode[2117] = {server = 2117, client = 2118, des = "cghljjdropbanker", func = "HljjDropBanker"}
PacketCode[2118] = {server = 2117, client = 2118, des = "gchljjdropbanker", func = "HljjDropBanker"}

PacketCode[2120] = {server = 2119, client = 2120, des = "gchljjstart", func = "HljjStart"}

PacketCode[2122] = {server = 2121, client = 2122, des = "gchljjupdate", func = "HljjUpdate"}

PacketCode[2124] = {server = 2123, client = 2124, des = "gchljjstandup", func = "HljjStandUp"}
PacketCode[2126] = {server = 2125, client = 2126, des = "gchljjgroupjetton", func = "HljjGroupJetton"}

PacketCode[2127] = {server = 2127, client = 2128, des = "cghljjhistory", func = "HljjHistory"}
PacketCode[2128] = {server = 2127, client = 2128, des = "gchljjhistory", func = "HljjHistory"}

PacketCode[2129] = {server = 2129, client = 2130, des = "cghljjchat", func = "HljjChat"}
PacketCode[2130] = {server = 2129, client = 2130, des = "gchljjchat", func = "HljjChat"}

PacketCode[2131] = {server = 2131, client = 2132, des = "cghljjstandlist", func = "HljjStandList"}
PacketCode[2132] = {server = 2131, client = 2132, des = "gchljjstandlist", func = "HljjStandList"}

PacketCode[2133] = {server = 2133, client = 2134, des = "cghljjselectwinnum", func = "HljjSelectWinNum"}
PacketCode[2134] = {server = 2133, client = 2134, des = "gchljjselectwinnun", func = "HljjSelectWinNum"}

PacketCode[2136] = {server = 2135, client = 2136, des = "gchljjwinnum", func = "HljjWinNum"}

PacketCode[2137] = {server = 2137, client = 2138, des = "cghljjbankerlist", func = "HljjBankerList"}
PacketCode[2138] = {server = 2137, client = 2138, des = "gchljjbankerlist", func = "HljjBankerList"}



----------------------这里是欢乐赢豆的错误码
ReturnCode["hljj_not_exist"] = 2101
ReturnCode["hljj_chair_notmatch"] = 2102
ReturnCode["hljj_chair_isplyaer"] = 2103
ReturnCode["hljj_is_sitdown"] = 2104
ReturnCode["hljj_pwd_notmatch"] = 2105
ReturnCode["hljj_banker_exist"] = 2106
ReturnCode["hljj_banker_exist"] = 2107
ReturnCode["hljj_state_notpour"] = 2108
ReturnCode["hljj_jetton_notenough"] = 2109
ReturnCode["hljj_pourtype_error"] = 2110
ReturnCode["hljj_sit_isbanker"] = 2111
ReturnCode["hljj_banker_leave"] = 2112
ReturnCode["hljj_behanker_list"] = 2113  --加入上庄列表
ReturnCode["hljj_behanker_drop"] = 2114  --申请下庄
ReturnCode["hljj_pour_banker"] = 2115  --庄家不能下注
ReturnCode["hljj_banker_notenough"] = 2116  --筹码不足，不能上庄
ReturnCode["hljj_pour_lose"] = 2117  --筹码不足，不能上庄
ReturnCode["hljj_select_fail"] = 2118  --状态不对，不能选择杯子
ReturnCode["hljj_select_error"] = 2119  --请选择正确的下注区域
ReturnCode["hljj_is_pour"] = 2120  --请选择正确的下注区域
ReturnCode["hljj_is_full"] = 2121  --该牌桌人数已满

ReturnCode[2101] = "该牌桌不存在"
ReturnCode[2102] = "坐下的位置不匹配"
ReturnCode[2103] = "该座位已经有玩家"
ReturnCode[2104] = "你已经坐下了，请勿重复操作"
ReturnCode[2105] = "牌桌密码不匹配"
ReturnCode[2106] = "庄家已经存在，上庄失败"
ReturnCode[2108] = "请稍后再下注"
ReturnCode[2109] = "您的筹码不足"
ReturnCode[2110] = "下注失败，请选择正确的下注区域"
ReturnCode[2111] = "坐下失败，你已经是庄家了！"
ReturnCode[2112] = "请先下庄再离开牌桌"
ReturnCode[2113] = "已加入上庄列表"
ReturnCode[2114] = "你已经申请下庄"
ReturnCode[2115] = "庄家不能下注"
ReturnCode[2116] = "筹码不足，不能上庄"
ReturnCode[2117] = "已达下注上限，下注失败"
ReturnCode[2118] = "请在开始前选择开奖区域"
ReturnCode[2119] = "请选择正确的下注区域"
ReturnCode[2120] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[2121] = "该牌桌人数已满，请选择其他牌桌"
----------------------