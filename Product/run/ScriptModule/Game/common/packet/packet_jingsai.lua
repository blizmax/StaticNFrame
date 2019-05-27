


--3400~3500 是竞赛
PacketCode[3401] = {server = 3401, client = 3402, des = "cgjingsaienter", func = "JingSaiEnter"}
PacketCode[3402] = {server = 3401, client = 3402, des = "gcjingsaienter", func = "JingSaiEnter"}

PacketCode[3403] = {server = 3403, client = 3404, des = "cgjingsaicreate", func = "JingSaiCreate"}
PacketCode[3404] = {server = 3403, client = 3404, des = "gcjingsaicreate", func = "JingSaiCreate"}

PacketCode[3405] = {server = 3405, client = 3406, des = "cgjingsaidel", func = "JingSaiDel"}
PacketCode[3406] = {server = 3405, client = 3406, des = "gcjingsaidel", func = "JingSaiDel"}

PacketCode[3407] = {server = 3407, client = 3408, des = "cgjingsailist", func = "JingSaiList"}
PacketCode[3408] = {server = 3407, client = 3408, des = "gcjingsailist", func = "JingSaiList"}

PacketCode[3409] = {server = 3409, client = 3410, des = "cgjingsaimemberlist", func = "JingSaiMemberList"}
PacketCode[3410] = {server = 3409, client = 3410, des = "gcjingsaimemberlist", func = "JingSaiMemberList"}

PacketCode[3411] = {server = 3411, client = 3412, des = "cgjingsaitablelist", func = "JingSaiTableList"}
PacketCode[3412] = {server = 3411, client = 3412, des = "gcjingsaitablelist", func = "JingSaiTableList"}

PacketCode[3413] = {server = 3413, client = 3414, des = "cgjingsaisetdes", func = "JingSaiSetDes"}
PacketCode[3414] = {server = 3413, client = 3414, des = "gcjingsaisetdes", func = "JingSaiSetDes"}

PacketCode[3415] = {server = 3415, client = 3416, des = "cgjingsaicreatetable", func = "JingSaiCreateTable"}
PacketCode[3416] = {server = 3415, client = 3416, des = "gcjingsaicreatetable", func = "JingSaiCreateTable"}

PacketCode[3417] = {server = 3417, client = 3418, des = "cgjingsaihistory", func = "JingSaiHistory"}
PacketCode[3418] = {server = 3417, client = 3418, des = "gcjingsaihistory", func = "JingSaiHistory"}

PacketCode[3419] = {server = 3419, client = 3420, des = "cgjingsaiupdate", func = "JingSaiUpdate"}
PacketCode[3420] = {server = 3419, client = 3420, des = "gcjingsaiupdate", func = "JingSaiUpdate"}

PacketCode[3421] = {server = 3421, client = 3422, des = "cgjingsaipay", func = "JingSaiPay"}
PacketCode[3422] = {server = 3421, client = 3422, des = "gcjingsaipay", func = "JingSaiPay"}

--

----------------------

ReturnCode["jingsai_error_jlb"] = 3401   --名称不能为空
ReturnCode["jingsai_error_admin"] = 3402   --
ReturnCode["jingsai_name_nil"] = 3403   --
ReturnCode["jingsai_not_exist"] = 3404   --
ReturnCode["jingsai_del_fail"] = 3405   --
ReturnCode["jingsai_del_state"] = 3406   --
ReturnCode["jingsai_is_end"] = 3407   --
ReturnCode["jingsai_is_ready"] = 3408   --
ReturnCode["jingsai_is_counting"] = 3409   --
ReturnCode["jingsai_table_max"] = 3410   --

ReturnCode[3401] = "只有在俱乐部中才能创建竞赛"
ReturnCode[3402] = "只有管理员才能创建竞赛"
ReturnCode[3403] = "竞赛名字不能为空" 
ReturnCode[3404] = "该竞赛不存在" 
ReturnCode[3405] = "删除失败"
ReturnCode[3406] = "竞赛已经开始，请勿删除" 
ReturnCode[3407] = "比赛已经结束，请选择其他比赛" 
ReturnCode[3408] = "比赛还没开始，请稍后" 
ReturnCode[3409] = "比赛正在结算中，请勿创建房间" 
ReturnCode[3410] = "请加入其他空余房间" 