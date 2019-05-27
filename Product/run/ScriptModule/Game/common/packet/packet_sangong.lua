
--3100~3200 是三公

PacketCode[3101] = {server = 3101, client = 3102, des = "cgsangongenter", func = "SanGongEnter"}
PacketCode[3102] = {server = 3101, client = 3102, des = "gcsangongenter", func = "SanGongEnter"}

PacketCode[3103] = {server = 3103, client = 3104, des = "cgsangongsitdown", func = "SanGongSitDown"}
PacketCode[3104] = {server = 3103, client = 3104, des = "gcsangongsitdown", func = "SanGongSitDown"}

--PacketCode[3105] = {server = 3105, client = 3106, des = "cgsangongupdate", func = "SanGongEnter"}
PacketCode[3106] = {server = 3105, client = 3106, des = "gcsangongupdate", func = "SanGongUpdate"}   --通知玩家行动

--PacketCode[3107] = {server = 3107, client = 3108, des = "cgsangongstandup", func = "SanGongStandUp"}
PacketCode[3108] = {server = 3107, client = 3108, des = "gcsangongstandup", func = "SanGongStandUp"}

PacketCode[3109] = {server = 3109, client = 3110, des = "cgsangongleave", func = "SanGongLeave"}
PacketCode[3110] = {server = 3109, client = 3110, des = "gcsangongleave", func = "SanGongLeave"}

PacketCode[3111] = {server = 3111, client = 3112, des = "cgsangongpour", func = "SanGongPour"}
PacketCode[3112] = {server = 3111, client = 3112, des = "gcsangongpour", func = "SanGongPour"}

PacketCode[3113] = {server = 3113, client = 3114, des = "cgsangongstart", func = "SanGongStart"}
PacketCode[3114] = {server = 3113, client = 3114, des = "gcsangongstart", func = "SanGongStart"}

--PacketCode[3115] = {server = 3115, client = 3116, des = "cgsangongcount", func = "SanGongCount"}
PacketCode[3116] = {server = 3115, client = 3116, des = "gcsangongcount", func = "SanGongCount"}

PacketCode[3117] = {server = 3117, client = 3118, des = "cgsangongtablelist", func = "SanGongTableList"}
PacketCode[3118] = {server = 3117, client = 3118, des = "gcsangongtablelist", func = "SanGongTableList"}

PacketCode[3119] = {server = 3119, client = 3120, des = "cgsangongcreatetable", func = "SanGongCreateTable"}
PacketCode[3120] = {server = 3119, client = 3120, des = "gcsangongcreatetable", func = "SanGongCreateTable"}

PacketCode[3121] = {server = 3121, client = 3122, des = "cgsangongchat", func = "SanGongChat"}
PacketCode[3122] = {server = 3121, client = 3122, des = "gcsangongchat", func = "SanGongChat"}

PacketCode[3123] = {server = 3123, client = 3124, des = "cgsangongready", func = "SanGongReady"}
PacketCode[3124] = {server = 3123, client = 3124, des = "gcsangongready", func = "SanGongReady"}

PacketCode[3125] = {server = 3125, client = 3126, des = "cgsangongplay", func = "SanGongPlay"}
PacketCode[3126] = {server = 3125, client = 3126, des = "gcsangongplay", func = "SanGongPlay"}


PacketCode[3127] = {server = 3127, client = 3128, des = "cgsangongdissolve", func = "SanGongDissolve"}
PacketCode[3128] = {server = 3127, client = 3128, des = "gcsangongdissolve", func = "SanGongDissolve"}

PacketCode[3129] = {server = 3129, client = 3130, des = "cgsangongdissolveopt", func = "SanGongDissolveOpt"}
PacketCode[3130] = {server = 3129, client = 3130, des = "gcsangongdissolveopt", func = "SanGongDissolveOpt"}

PacketCode[3131] = {server = 3131, client = 3132, des = "cgsangongqiangzhuang", func = "SanGongQiangZhuang"}
PacketCode[3132] = {server = 3131, client = 3132, des = "gcsangongqiangzhuang", func = "SanGongQiangZhuang"}

--PacketCode[3133] = {server = 3133, client = 3134, des = "gcsangongnoticeqiang", func = "SanGongNoticeQiang"}
PacketCode[3134] = {server = 3133, client = 3134, des = "gcsangongnoticeqiang", func = "SanGongNoticeQiang"}

PacketCode[3135] = {server = 3135, client = 3136, des = "cgsangonghistory", func = "SanGongHistory"}
PacketCode[3136] = {server = 3135, client = 3136, des = "gcsangonghistory", func = "SanGongHistory"}



ReturnCode["sangong_create_fail"] = 3101    --创建房间失败
ReturnCode["sangong_not_exist"] = 3102    --牌桌不存在
ReturnCode["sangong_is_full"] = 3103    --座位已经满
ReturnCode["sangong_not_enough"] = 3104    --人数不足，不能开始
ReturnCode["sangong_owner_start"] = 3105    --人数不足，不能开始
ReturnCode["sangong_start_error"] = 3106   --人数不足，不能开始
ReturnCode["sangong_leave_state"] = 3107   --请等待牌桌结束再离开
ReturnCode["sangong_play_error"] = 3108   --请等待牌桌结束再离开



ReturnCode["sangong_dissolve_fail"] = 3109   --解散失败
ReturnCode["sangong_dissolve_fail2"] = 3110   --房间还没开始前，只有房主才能解散房间
ReturnCode["sangong_dissolve_fail3"] = 3111   --只有在座玩家才能申请解散房间

ReturnCode["sangong_opt_error"] = 3112   --解散失败
ReturnCode["sangong_start_error2"] = 3113   --固定庄家，房主必须在座位上
ReturnCode["sangong_qiang_error"] = 3114   --类型错误，该牌桌不是抢庄玩发
ReturnCode["sangong_is_start"] = 3115   --游戏已经开始，请选择其他牌桌
ReturnCode["sangong_is_baodian"] = 3116   --你已经爆点，不能再要牌
ReturnCode["sangong_more_card"] = 3117   --手上已经有5张牌，不能再要牌
ReturnCode["sangong_sit_money"] = 3118   --钻石不够，坐下失败
ReturnCode["sangong_is_start"] = 3119   --牌局已经开始，请选取其他牌桌
ReturnCode["sangong_leave_zj"] = 3120   --你是庄家，请等待游戏结束再离开
ReturnCode["sangong_ready_error"] = 3121   --你是庄家，请等待游戏结束再离开
ReturnCode["sangong_ready_fail"] = 3122   --你是庄家，请等待游戏结束再离开

ReturnCode[3101] = "创建房间失败" 
ReturnCode[3102] = "该牌桌已经解散" 
ReturnCode[3103] = "该牌桌已经满座" 
ReturnCode[3104] = "人数不足，不能开始"
ReturnCode[3105] = "只有房主才能开始开始游戏" 
ReturnCode[3106] = "该游戏已经是开始状态" 
ReturnCode[3107] = "请等待牌局结束再离开" 
ReturnCode[3108] = "出牌操作错误" 
ReturnCode[3109] = "解散失败" 
ReturnCode[3110] = "只有房主才能解散房间" 
ReturnCode[3111] = "只有在座玩家才能申请解散房间" 
ReturnCode[3112] = "解散失败" 
ReturnCode[3113] = "固定庄家，房主需要在座位上" 
ReturnCode[3114] = "操作错误，该牌桌不是抢庄玩法" 
ReturnCode[3115] = "游戏已经开始，请选择其他牌桌" 
ReturnCode[3116] = "你已经爆点，不能在要牌" 
ReturnCode[3117] = "牌数已满，不能在要牌" 
ReturnCode[3118] = "该房间房费均摊，您钻石不够，坐下失败" 
ReturnCode[3119] = "牌局已经开始，请选择其他牌桌" 
ReturnCode[3120] = "您是庄家，请等待游戏结束再离开!"
ReturnCode[3121] = "准备失败"
ReturnCode[3122] = "请稍后在操作"


