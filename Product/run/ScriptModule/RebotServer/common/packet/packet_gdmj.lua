
--8600~8700 是广东麻将的协议区间
PacketCode[8601] = {server = 8601, client = 8602, des = "cggdmjenter", func = "GdmjEnter"}
PacketCode[8602] = {server = 8601, client = 8602, des = "gcgdmjenter", func = "GdmjEnter"}
--
PacketCode[8603] = {server = 8603, client = 8604, des = "cggdmjaction", func = "GdmjAction"}
PacketCode[8604] = {server = 8603, client = 8604, des = "gcgdmjaction", func = "GdmjAction"}

PacketCode[8605] = {server = 8605, client = 8606, des = "cggdmjcreate", func = "GdmjCreate"}
PacketCode[8606] = {server = 8605, client = 8606, des = "gcgdmjcreate", func = "GdmjCreate"}

PacketCode[8608] = {server = 8607, client = 8608, des = "gcgdmjstart", func = "GdmjStart"}

PacketCode[8610] = {server = 8609, client = 8610, des = "gcgdmjupdate", func = "GdmjUpdate"}

PacketCode[8611] = {server = 8611, client = 8612, des = "cggdmjleave", func = "GdmjLeave"}
PacketCode[8612] = {server = 8611, client = 8612, des = "gcgdmjleave", func = "GdmjLeave"}

PacketCode[8613] = {server = 8613, client = 8614, des = "cggdmjready", func = "GdmjReady"}
PacketCode[8614] = {server = 8613, client = 8614, des = "gcgdmjready", func = "GdmjReady"}

PacketCode[8615] = {server = 8615, client = 8616, des = "cggdmjsitdown", func = "GdmjSitDown"}
PacketCode[8616] = {server = 8615, client = 8616, des = "gcgdmjsitdown", func = "GdmjSitDown"}

PacketCode[8617] = {server = 8617, client = 8618, des = "cggdmjstandup", func = "GdmjStandUp"}
PacketCode[8618] = {server = 8617, client = 8618, des = "gcgdmjstandup", func = "GdmjStandUp"}

PacketCode[8620] = {server = 8619, client = 8620, des = "gcgdmjaddplayer", func = "GdmjAddPlayer"}

PacketCode[8622] = {server = 8621, client = 8622, des = "gcgdmjaccount", func = "GdmjAccount"}

PacketCode[8623] = {server = 8623, client = 8624, des = "cggdmjdissolve", func = "GdmjDissolve"}
PacketCode[8624] = {server = 8623, client = 8624, des = "gcgdmjdissolve", func = "GdmjDissolve"}

PacketCode[8625] = {server = 8625, client = 8626, des = "cggdmjdissolveopt", func = "GdmjDissolveOpt"}
PacketCode[8626] = {server = 8625, client = 8626, des = "gcgdmjdissolveopt", func = "GdmjDissolveOpt"}

PacketCode[8627] = {server = 8627, client = 8628, des = "cggdmjtablelist", func = "GdmjTableList"}
PacketCode[8628] = {server = 8627, client = 8628, des = "gcgdmjtablelist", func = "GdmjTableList"}

PacketCode[8629] = {server = 8629, client = 8630, des = "cggdmjhistory", func = "GdmjHistory"}
PacketCode[8630] = {server = 8629, client = 8630, des = "gcgdmjhistory", func = "GdmjHistory"}

PacketCode[8631] = {server = 8631, client = 8632, des = "cggdmjhistorystep", func = "GdmjHistoryStep"}
PacketCode[8632] = {server = 8631, client = 8632, des = "gcgdmjhistorystep", func = "GdmjHistoryStep"}

PacketCode[8633] = {server = 8633, client = 8634, des = "cggdmjcancelauto", func = "GdmjCancelAuto"}
PacketCode[8634] = {server = 8633, client = 8634, des = "gcgdmjcancelauto", func = "GdmjCancelAuto"}

PacketCode[8635] = {server = 8635, client = 8636, des = "cggdmjchat", func = "GdmjChat"}
PacketCode[8636] = {server = 8635, client = 8636, des = "gcgdmjchat", func = "GdmjChat"}

--换桌
PacketCode[8637] = {server = 8637, client = 8638, des = "cggdmjvarytable", func = "GdmjVaryTable"}
PacketCode[8638] = {server = 8637, client = 8638, des = "gcgdmjvarytable", func = "GdmjVaryTable"}

ReturnCode["gdmj_not_exist"] = 8601
ReturnCode["gdmj_full"] = 8602    --牌桌已满
ReturnCode["gdmj_opt_error"] = 8603    --操作出错
ReturnCode["gdmj_player_not"] = 8604   --该玩家不在牌桌中
ReturnCode["gdmj_leave_playing"] = 8605   --牌桌正在进行中
ReturnCode["gdmj_dissolve_fail"] = 8606   --解散失败
ReturnCode["gdmj_dissolve_fail2"] = 8607   --只有房主才能解散房间
ReturnCode["gdmj_dissolve_fail3"] = 8608   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_opt_error"] = 8609   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_leave_error"] = 8610   --只有座位上的玩家才能申请解散房间
ReturnCode["gdmj_sitdown_error"] = 8611   --该座位已经有人了
ReturnCode["gdmj_sitdown_fail"] = 8612   --该座位已经有人了
ReturnCode["gdmj_stand_error"] = 8613   --请等待牌局结束再站起

ReturnCode["gdmj_create_fail"] = 8614   --请等待牌局结束再站起

ReturnCode["gdmj_sit_fail"] = 8615   --坐下失败，钻石不足

ReturnCode[8601] = "房间已经解散"
ReturnCode[8602] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[8603] = "操作出错"
ReturnCode[8604] = "操作出错"
ReturnCode[8605] = "牌局正在进行中，请稍后退出"
ReturnCode[8606] = "解散失败"
ReturnCode[8607] = "只有房主才能解散房间"
ReturnCode[8608] = "只有座位上的玩家才能申请解散房间"
ReturnCode[8609] = "操作错误"
ReturnCode[8610] = "请在游戏结束后离开房间"
ReturnCode[8611] = "该座位已经有玩家，请选择其他座位"
ReturnCode[8612] = "坐下失败"
ReturnCode[8613] = "请等待牌局结束再站起"
ReturnCode[8614] = "创建房间失败"
ReturnCode[8615] = "金币不足,坐下失败"