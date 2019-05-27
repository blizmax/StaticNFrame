
--2200~2300 是广东麻将的协议区间
PacketCode[2201] = {server = 2201, client = 2202, des = "cggdmjenter", func = "GdmjEnter"}
PacketCode[2202] = {server = 2201, client = 2202, des = "gcgdmjenter", func = "GdmjEnter"}
--
PacketCode[2203] = {server = 2203, client = 2204, des = "cggdmjaction", func = "GdmjAction"}
PacketCode[2204] = {server = 2203, client = 2204, des = "gcgdmjaction", func = "GdmjAction"}

PacketCode[2205] = {server = 2205, client = 2206, des = "cggdmjcreate", func = "GdmjCreate"}
PacketCode[2206] = {server = 2205, client = 2206, des = "gcgdmjcreate", func = "GdmjCreate"}

PacketCode[2208] = {server = 2207, client = 2208, des = "gcgdmjstart", func = "GdmjStart"}

PacketCode[2210] = {server = 2209, client = 2210, des = "gcgdmjupdate", func = "GdmjUpdate"}

PacketCode[2211] = {server = 2211, client = 2212, des = "cggdmjleave", func = "GdmjLeave"}
PacketCode[2212] = {server = 2211, client = 2212, des = "gcgdmjleave", func = "GdmjLeave"}

PacketCode[2213] = {server = 2213, client = 2214, des = "cggdmjready", func = "GdmjReady"}
PacketCode[2214] = {server = 2213, client = 2214, des = "gcgdmjready", func = "GdmjReady"}

PacketCode[2215] = {server = 2215, client = 2216, des = "cggdmjsitdown", func = "GdmjSitDown"}
PacketCode[2216] = {server = 2215, client = 2216, des = "gcgdmjsitdown", func = "GdmjSitDown"}

PacketCode[2217] = {server = 2217, client = 2218, des = "cggdmjstandup", func = "GdmjStandUp"}
PacketCode[2218] = {server = 2217, client = 2218, des = "gcgdmjstandup", func = "GdmjStandUp"}

PacketCode[2220] = {server = 2219, client = 2220, des = "gcgdmjaddplayer", func = "GdmjAddPlayer"}

PacketCode[2222] = {server = 2221, client = 2222, des = "gcgdmjaccount", func = "GdmjAccount"}

PacketCode[2223] = {server = 2223, client = 2224, des = "cggdmjdissolve", func = "GdmjDissolve"}
PacketCode[2224] = {server = 2223, client = 2224, des = "gcgdmjdissolve", func = "GdmjDissolve"}

PacketCode[2225] = {server = 2225, client = 2226, des = "cggdmjdissolveopt", func = "GdmjDissolveOpt"}
PacketCode[2226] = {server = 2225, client = 2226, des = "gcgdmjdissolveopt", func = "GdmjDissolveOpt"}

PacketCode[2227] = {server = 2227, client = 2228, des = "cggdmjtablelist", func = "GdmjTableList"}
PacketCode[2228] = {server = 2227, client = 2228, des = "gcgdmjtablelist", func = "GdmjTableList"}

PacketCode[2229] = {server = 2229, client = 2230, des = "cggdmjhistory", func = "GdmjHistory"}
PacketCode[2230] = {server = 2229, client = 2230, des = "gcgdmjhistory", func = "GdmjHistory"}

PacketCode[2231] = {server = 2231, client = 2232, des = "cggdmjhistorystep", func = "GdmjHistoryStep"}
PacketCode[2232] = {server = 2231, client = 2232, des = "gcgdmjhistorystep", func = "GdmjHistoryStep"}

PacketCode[2233] = {server = 2233, client = 2234, des = "cggdmjcancelauto", func = "GdmjCancelAuto"}
PacketCode[2234] = {server = 2233, client = 2234, des = "gcgdmjcancelauto", func = "GdmjCancelAuto"}

PacketCode[2235] = {server = 2235, client = 2236, des = "cggdmjchat", func = "GdmjChat"}
PacketCode[2236] = {server = 2235, client = 2236, des = "gcgdmjchat", func = "GdmjChat"}


ReturnCode["gdmj_not_exist"] = 2201
ReturnCode["gdmj_full"] = 2202    --牌桌已满
ReturnCode["gdmj_opt_error"] = 2203    --操作出错
ReturnCode["gdmj_player_not"] = 2204   --该玩家不在牌桌中
ReturnCode["gdmj_leave_playing"] = 2205   --牌桌正在进行中
ReturnCode["gdmj_dissolve_fail"] = 2206   --解散失败
ReturnCode["gdmj_dissolve_fail2"] = 2207   --只有房主才能解散房间
ReturnCode["gdmj_dissolve_fail3"] = 2208   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_opt_error"] = 2209   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_leave_error"] = 2210   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_sitdown_error"] = 2211   --该座位已经有人了
ReturnCode["gdmj_sitdown_fail"] = 2212   --该座位已经有人了
ReturnCode["gdmj_stand_error"] = 2213   --请等待牌局结束再站起

ReturnCode["gdmj_create_fail"] = 2214   --请等待牌局结束再站起

ReturnCode["gdmj_sit_fail"] = 2215   --坐下失败，钻石不足

ReturnCode[2201] = "房间已经解散"
ReturnCode[2202] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[2203] = "操作出错"
ReturnCode[2204] = "操作出错"
ReturnCode[2205] = "牌局正在进行中，请稍后退出"
ReturnCode[2206] = "解散失败"
ReturnCode[2207] = "只有房主才能解散房间"
ReturnCode[2208] = "只有座位上的玩家才能申请解散房间"
ReturnCode[2209] = "操作错误"
ReturnCode[2210] = "请在游戏结束后离开房间"
ReturnCode[2211] = "该座位已经有玩家，请选择其他座位"
ReturnCode[2212] = "坐下失败"
ReturnCode[2213] = "请等待牌局结束再站起"
ReturnCode[2214] = "创建房间失败"
ReturnCode[2215] = "钻石不足,坐下失败"