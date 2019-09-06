
--1000~1100 是人物信息的ID区间
PacketCode[1301] = {server = 1301, client = 1302, des = "cggetagentinfo", func = "GetAgenctInfo"}
PacketCode[1302] = {server = 1301, client = 1302, des = "gcgetagentinfo", func = "GetAgenctInfo"}

PacketCode[1303] = {server = 1303, client = 1304, des = "cgagentopt", func = "AgenctOpt"}
PacketCode[1304] = {server = 1303, client = 1304, des = "gcagentopt", func = "AgenctOpt"}

PacketCode[1305] = {server = 1305, client = 1306, des = "cginvitebind", func = "InviteBind"}
PacketCode[1306] = {server = 1305, client = 1306, des = "gcinvitebind", func = "InviteBind"}


PacketCode[1307] = {server = 1307, client = 1308, des = "cggetagentincome", func = "GetAgentIncome"}
PacketCode[1308] = {server = 1307, client = 1308, des = "gcgetagentincome", func = "GetAgentIncome"}


PacketCode[1321] = {server = 1321, client = 1322, des = "cggetwiringinfo", func = "GetWiringInfo"}
PacketCode[1322] = {server = 1321, client = 1322, des = "gcgetwiringinfo", func = "GetWiringInfo"}

PacketCode[1323] = {server = 1323, client = 1324, des = "cggetwiringlist", func = "GetWiringList"}
PacketCode[1324] = {server = 1323, client = 1324, des = "gcgetwiringlist", func = "GetWiringList"}

PacketCode[1325] = {server = 1325, client = 1326, des = "cgwiringachievement", func = "WiringAchievement"}
PacketCode[1326] = {server = 1325, client = 1326, des = "gcwiringachievement", func = "WiringAchievement"}

PacketCode[1327] = {server = 1327, client = 1328, des = "cgwiringawardlist", func = "WiringAwardList"}
PacketCode[1328] = {server = 1327, client = 1328, des = "gcwiringawardlist", func = "WiringAwardList"}

PacketCode[1329] = {server = 1329, client = 1330, des = "cgwiringwithdraw", func = "WiringWithDraw"}
PacketCode[1330] = {server = 1329, client = 1330, des = "gcwiringwithdraw", func = "WiringWithDraw"}

PacketCode[1331] = {server = 1331, client = 1332, des = "cgwiringuserlist", func = "WiringUserList"}
PacketCode[1332] = {server = 1331, client = 1332, des = "gcwiringuserlist", func = "WiringUserList"}


PacketCode[1333] = {server = 1333, client = 1334, des = "cgcreatewiring", func = "CreateWiring"}
PacketCode[1334] = {server = 1333, client = 1334, des = "gccreatewiring", func = "CreateWiring"}

PacketCode[1335] = {server = 1335, client = 1336, des = "cgwiringdrawmoney", func = "WiringDrawMoney"}
PacketCode[1336] = {server = 1335, client = 1336, des = "gcwiringdrawmoney", func = "WiringDrawMoney"}

PacketCode[1337] = {server = 1337, client = 1338, des = "cgwiringappointmentwithdraw", func = "WiringAppointmentWithDraw"}
PacketCode[1338] = {server = 1337, client = 1338, des = "gcwiringappointmentwithdraw", func = "WiringAppointmentWithDraw"}

--下级查询
PacketCode[1339] = {server = 1339, client = 1340, des = "cgsubordinatecheck", func = "SubordinateCheck"}
PacketCode[1340] = {server = 1339, client = 1340, des = "gcsubordinatecheck", func = "SubordinateCheck"}

--设置下级保底
PacketCode[1341] = {server = 1341, client = 1342, des = "cgagentsetratenum", func = "AgentSetRateNum"}
PacketCode[1342] = {server = 1341, client = 1342, des = "gcagentsetratenum", func = "AgentSetRateNum"}

--获取分成的每日记录
PacketCode[1343] = {server = 1343, client = 1344, des = "cgyestdayachievement", func = "FenChengDayInfo"}
PacketCode[1344] = {server = 1343, client = 1344, des = "gcyestdayachievement", func = "FenChengDayInfo"}

--获取分成的每周记录
PacketCode[1345] = {server = 1345, client = 1346, des = "cglastweekachievement", func = "FenChengWeekList"}
PacketCode[1346] = {server = 1345, client = 1346, des = "gclastweekachievement", func = "FenChengWeekList"}

--获取今天的分红
PacketCode[1347] = {server = 1347, client = 1348, des = "cgtodaybonus", func = "GetTodayBonus"}
PacketCode[1348] = {server = 1347, client = 1348, des = "gctodaybonus", func = "GetTodayBonus"}

PacketCode[1349] = {server = 1349, client = 1350, des = "cgyesterdaybonus", func = "GetYesterdayBonus"}
PacketCode[1350] = {server = 1349, client = 1350, des = "gcyesterdaybonus", func = "GetYesterdayBonus"}

PacketCode[1351] = {server = 1351, client = 1352, des = "cguserbonus", func = "GetUserBonusInfo"}
PacketCode[1352] = {server = 1351, client = 1352, des = "gcuserbonus", func = "GetUserBonusInfo"}

PacketCode[1353] = {server = 1353, client = 1354, des = "cgusertakebonus", func = "UserTakeBonus"}
PacketCode[1354] = {server = 1353, client = 1354, des = "gcusertakebonus", func = "UserTakeBonus"}

PacketCode[1355] = {server = 1355, client = 1356, des = "cgquerytodaybonus", func = "QueryTodayBonus"}
PacketCode[1356] = {server = 1355, client = 1356, des = "gcquerytodaybonus", func = "QueryTodayBonus"}

PacketCode[1357] = {server = 1357, client = 1358, des = "cgqueryyesterdaybonus", func = "QueryYesterdayBonus"}
PacketCode[1358] = {server = 1357, client = 1358, des = "gcqueryyesterdaybonus", func = "QueryYesterdayBonus"}

PacketCode[1359] = {server = 1359, client = 1360, des = "cgbonuswithdraw", func = "BonusWithDraw"}
PacketCode[1360] = {server = 1359, client = 1360, des = "gcbonuswithdraw", func = "BonusWithDraw"}


ReturnCode["hlyd_share_fail"] = 1201
ReturnCode["rep_error"] = 1202
ReturnCode["draw_error"] = 1203
ReturnCode["rep_error_1"] = 1204
ReturnCode["draw_error_1"] = 1205
ReturnCode["check_error_1"] = 1206
ReturnCode["check_error_2"] = 1207


ReturnCode[1201] = "分享失败"
ReturnCode[1202] = "可预提金额不住！"
ReturnCode[1203] = "可提金额不住！"
ReturnCode[1204] = "提现失败！"
ReturnCode[1205] = "预提失败！"
ReturnCode[1206] = "查找不到该玩家！"
ReturnCode[1207] = "该玩家为你自己本身！"


ReturnCode["agent_set_max"] = 1208
ReturnCode[1208] = "返点超过最大限制"

ReturnCode["agent_pre_error"] = 1209
ReturnCode[1209] = "下级返点必须小于您的返点"

ReturnCode["agent_pre_error"] = 1210
ReturnCode[1210] = "返点不允许下降"

ReturnCode["agent_set_error"] = 1211
ReturnCode[1211] = "设置失败"

ReturnCode["bind_pre_not_agent"] = 1212
ReturnCode[1212] = "请绑定代理的邀请码"