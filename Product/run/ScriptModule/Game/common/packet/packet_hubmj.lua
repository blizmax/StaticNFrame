
--2200~2300 是广东麻将的协议区间
PacketCode[2801] = {server = 2801, client = 2802, des = "cghubmjenter", func = "HubmjEnter"}
PacketCode[2802] = {server = 2801, client = 2802, des = "gchubmjenter", func = "HubmjEnter"}
--
PacketCode[2803] = {server = 2803, client = 2804, des = "cghubmjaction", func = "HubmjAction"}
PacketCode[2804] = {server = 2803, client = 2804, des = "gchubmjaction", func = "HubmjAction"}

PacketCode[2805] = {server = 2805, client = 2806, des = "cghubmjcreate", func = "HubmjCreate"}
PacketCode[2806] = {server = 2805, client = 2806, des = "gchubmjcreate", func = "HubmjCreate"}

PacketCode[2808] = {server = 2807, client = 2808, des = "gchubmjstart", func = "HubmjStart"}

PacketCode[2810] = {server = 2809, client = 2810, des = "gchubmjupdate", func = "HubmjUpdate"}

PacketCode[2811] = {server = 2811, client = 2812, des = "cghubmjleave", func = "HubmjLeave"}
PacketCode[2812] = {server = 2811, client = 2812, des = "gchubmjleave", func = "HubmjLeave"}

PacketCode[2813] = {server = 2813, client = 2814, des = "cghubmjready", func = "HubmjReady"}
PacketCode[2814] = {server = 2813, client = 2814, des = "gchubmjready", func = "HubmjReady"}

PacketCode[2815] = {server = 2815, client = 2816, des = "cghubmjsitdown", func = "HubmjSitDown"}
PacketCode[2816] = {server = 2815, client = 2816, des = "gchubmjsitdown", func = "HubmjSitDown"}

PacketCode[2817] = {server = 2817, client = 2818, des = "cghubmjstandup", func = "HubmjStandUp"}
PacketCode[2818] = {server = 2817, client = 2818, des = "gchubmjstandup", func = "HubmjStandUp"}

PacketCode[2820] = {server = 2819, client = 2820, des = "gchubmjaddplayer", func = "HubmjAddPlayer"}

PacketCode[2822] = {server = 2821, client = 2822, des = "gchubmjaccount", func = "HubmjAccount"}

PacketCode[2823] = {server = 2823, client = 2824, des = "cghubmjdissolve", func = "HubmjDissolve"}
PacketCode[2824] = {server = 2823, client = 2824, des = "gchubmjdissolve", func = "HubmjDissolve"}

PacketCode[2825] = {server = 2825, client = 2826, des = "cghubmjdissolveopt", func = "HubmjDissolveOpt"}
PacketCode[2826] = {server = 2825, client = 2826, des = "gchubmjdissolveopt", func = "HubmjDissolveOpt"}

PacketCode[2827] = {server = 2827, client = 2828, des = "cghubmjtablelist", func = "HubmjTableList"}
PacketCode[2828] = {server = 2827, client = 2828, des = "gchubmjtablelist", func = "HubmjTableList"}

PacketCode[2829] = {server = 2829, client = 2830, des = "cghubmjhistory", func = "HubmjHistory"}
PacketCode[2830] = {server = 2829, client = 2830, des = "gchubmjhistory", func = "HubmjHistory"}

PacketCode[2831] = {server = 2831, client = 2832, des = "cghubmjhistorystep", func = "HubmjHistoryStep"}
PacketCode[2832] = {server = 2831, client = 2832, des = "gchubmjhistorystep", func = "HubmjHistoryStep"}

PacketCode[2833] = {server = 2833, client = 2834, des = "cghubmjcancelauto", func = "HubmjCancelAuto"}
PacketCode[2834] = {server = 2833, client = 2834, des = "gchubmjcancelauto", func = "HubmjCancelAuto"}

PacketCode[2835] = {server = 2835, client = 2836, des = "cghubmjchat", func = "HubmjChat"}
PacketCode[2836] = {server = 2835, client = 2836, des = "gchubmjchat", func = "HubmjChat"}


ReturnCode["hubmj_not_exist"] = 2801
ReturnCode["hubmj_full"] = 2802    --牌桌已满
ReturnCode["hubmj_opt_error"] = 2803    --操作出错
ReturnCode["hubmj_player_not"] = 2804   --该玩家不在牌桌中
ReturnCode["hubmj_leave_playing"] = 2805   --牌桌正在进行中
ReturnCode["hubmj_dissolve_fail"] = 2806   --解散失败
ReturnCode["hubmj_dissolve_fail2"] = 2807   --只有房主才能解散房间
ReturnCode["hubmj_dissolve_fail3"] = 2808   --只有座位上的玩家才能申请解散房间
ReturnCode["hubmj_opt_error"] = 2809   --只有座位上的玩家才能申请解散房间
ReturnCode["hubmj_leave_error"] = 2810   --只有座位上的玩家才能申请解散房间
ReturnCode["hubmj_sitdown_error"] = 2811   --该座位已经有人了
ReturnCode["hubmj_sitdown_fail"] = 2812   --该座位已经有人了
ReturnCode["hubmj_stand_error"] = 2813   --请等待牌局结束再站起

ReturnCode[2801] = "房间已经解散"
ReturnCode[2802] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[2803] = "操作出错"
ReturnCode[2804] = "操作出错"
ReturnCode[2805] = "牌局正在进行中，请稍后退出"
ReturnCode[2806] = "解散失败"
ReturnCode[2807] = "只有房主才能解散房间"
ReturnCode[2808] = "只有座位上的玩家才能申请解散房间"
ReturnCode[2809] = "操作错误"
ReturnCode[2810] = "请在游戏结束后离开房间"
ReturnCode[2811] = "该座位已经有玩家，请选择其他座位"
ReturnCode[2812] = "坐下失败"
ReturnCode[2813] = "请等待牌局结束再站起"