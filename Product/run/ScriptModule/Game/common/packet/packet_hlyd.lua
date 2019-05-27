


--2001~2100 是欢乐赢豆的协议号区间
PacketCode[2001] = {server = 2001, client = 2002, des = "cghlydenter", func = "HlydEnter"}
PacketCode[2002] = {server = 2001, client = 2002, des = "gchlydenter", func = "HlydEnter"}

PacketCode[2003] = {server = 2003, client = 2004, des = "cghlydleave", func = "HlydLeave"}
PacketCode[2004] = {server = 2003, client = 2004, des = "gchlydleave", func = "HlydLeave"}

PacketCode[2005] = {server = 2005, client = 2006, des = "cghlydsitdown", func = "HlydSitDown"}
PacketCode[2006] = {server = 2005, client = 2006, des = "gchlydsitdown", func = "HlydSitDown"}

PacketCode[2007] = {server = 2007, client = 2008, des = "cghlydpourjetton", func = "HlydPourJetton"}
PacketCode[2008] = {server = 2007, client = 2008, des = "gchlydpourjetton", func = "HlydPourJetton"}

PacketCode[2010] = {server = 2009, client = 2010, des = "gchlydcount", func = "HlydCount"}

PacketCode[2011] = {server = 2011, client = 2012, des = "cghlydtablelist", func = "HlydTableList"}
PacketCode[2012] = {server = 2011, client = 2012, des = "gchlydtablelist", func = "HlydTableList"}

PacketCode[2013] = {server = 2013, client = 2002, des = "cghlydcreatetable", func = "HlydCreateTable"}
PacketCode[2014] = {server = 2013, client = 2014, des = "gchlydcreatetable", func = "HlydCreateTable"}

PacketCode[2015] = {server = 2015, client = 2016, des = "cghlydbeingbanker", func = "HlydBeingBanker"}
PacketCode[2016] = {server = 2015, client = 2016, des = "gchlydbeingbanker", func = "HlydBeingBanker"}

PacketCode[2017] = {server = 2017, client = 2018, des = "cghlyddropbanker", func = "HlydDropBanker"}
PacketCode[2018] = {server = 2017, client = 2018, des = "gchlyddropbanker", func = "HlydDropBanker"}

PacketCode[2019] = {server = 2019, client = 2020, des = "cghlydstart", func = "HlydStart"}
PacketCode[2020] = {server = 2019, client = 2020, des = "gchlydstart", func = "HlydStart"}

PacketCode[2022] = {server = 2021, client = 2022, des = "gchlydupdate", func = "HlydUpdate"}

PacketCode[2024] = {server = 2023, client = 2024, des = "gchlydstandup", func = "HlydStandUp"}
PacketCode[2026] = {server = 2025, client = 2026, des = "gchlydgroupjetton", func = "HlydGroupJetton"}

PacketCode[2027] = {server = 2027, client = 2028, des = "cghlydhistory", func = "HlydHistory"}
PacketCode[2028] = {server = 2027, client = 2028, des = "gchyldhistory", func = "HlydHistory"}

PacketCode[2029] = {server = 2029, client = 2030, des = "cghlydchat", func = "HlydChat"}
PacketCode[2030] = {server = 2029, client = 2030, des = "gchlydchat", func = "HlydChat"}

PacketCode[2031] = {server = 2031, client = 2032, des = "cghlydstandlist", func = "HlydStandList"}
PacketCode[2032] = {server = 2031, client = 2032, des = "gchlydstandlist", func = "HlydStandList"}

PacketCode[2033] = {server = 2033, client = 2034, des = "cghlydbankerlist", func = "HlydBankerList"}
PacketCode[2034] = {server = 2033, client = 2034, des = "gchlydbankerlist", func = "HlydBankerList"}

PacketCode[2035] = {server = 2035, client = 2036, des = "cghlydstats", func = "HlydStats"}
PacketCode[2036] = {server = 2035, client = 2036, des = "gchlydstats", func = "HlydStats"}

PacketCode[2039] = {server = 2039, client = 2040, des = "cghlydsetjifen", func = "HlydSetJifen"}
PacketCode[2040] = {server = 2039, client = 2040, des = "gchlydsetjifen", func = "HlydSetJifen"}

PacketCode[2041] = {server = 2041, client = 2042, des = "cghlyddissolve", func = "HlydDissolve"}
PacketCode[2042] = {server = 2041, client = 2042, des = "gchlyddissolve", func = "HlydDissolve"}

PacketCode[2043] = {server = 2043, client = 2044, des = "cghlydzhanji", func = "HlydZhanJi"}
PacketCode[2044] = {server = 2043, client = 2044, des = "gchlydzhanji", func = "HlydZhanJi"}




--
ReturnCode["hlyd_not_exist"] = 2001
ReturnCode["hlyd_chair_notmatch"] = 2002
ReturnCode["hlyd_chair_isplyaer"] = 2003
ReturnCode["hlyd_is_sitdown"] = 2004
ReturnCode["hlyd_pwd_notmatch"] = 2005
ReturnCode["hlyd_banker_exist"] = 2006
ReturnCode["hlyd_banker_exist"] = 2007
ReturnCode["hlyd_state_notpour"] = 2008
ReturnCode["hlyd_jetton_notenough"] = 2009
ReturnCode["hlyd_pourtype_error"] = 2010
ReturnCode["hlyd_sit_isbanker"] = 2011
ReturnCode["hlyd_banker_leave"] = 2012
ReturnCode["hlyd_behanker_list"] = 2013  --加入上庄列表
ReturnCode["hlyd_behanker_drop"] = 2014  --申请下庄
ReturnCode["hlyd_pour_banker"] = 2015  --庄家不能下注

ReturnCode["hlyd_banker_notenough"] = 2016  --筹码不足，不能上庄
ReturnCode["hlyd_pour_lose"] = 2017  --庄家不够赔钱了，不能下注
ReturnCode["hlyd_is_pour"] = 2018  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["hlyd_is_full"] = 2019  --人已经满了
ReturnCode["hlyd_jifen_error"] = 2020  --只有房主才能给玩家设置积分
ReturnCode["hlyd_no_player"] = 2021  --玩家不在牌桌中
ReturnCode["hlyd_start_error"] = 2022  --玩家不在牌桌中
ReturnCode["hlyd_dissolve_owner"] = 2023  --房主才能解散房间
ReturnCode["hlyd_dissolve_error"] = 2024  --有庄家时不能解散房间
ReturnCode["hlyd_create_fail"] = 2025  --创建房间失败

ReturnCode[2001] = "该牌桌不存在"
ReturnCode[2002] = "坐下的位置不匹配"
ReturnCode[2003] = "该座位已经有玩家"
ReturnCode[2004] = "你已经坐下了，请勿重复操作"
ReturnCode[2005] = "牌桌密码不匹配"
ReturnCode[2006] = "庄家已经存在，上庄失败"
ReturnCode[2008] = "请稍后再下注"
ReturnCode[2009] = "您的筹码不足"
ReturnCode[2010] = "下注失败，请选择正确的下注区域"
ReturnCode[2011] = "坐下失败，你已经是庄家了！"
ReturnCode[2012] = "请先下庄再离开牌桌"
ReturnCode[2013] = "已加入上庄列表"
ReturnCode[2014] = "你已经申请下庄"
ReturnCode[2015] = "庄家不能下注"
ReturnCode[2016] = "筹码不足，不能上庄"
ReturnCode[2017] = "已达下注上限，下注失败"
ReturnCode[2018] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[2019] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[2020] = "只有房主才能给玩家设置积分"
ReturnCode[2021] = "该玩家不在牌桌中"
ReturnCode[2022] = "只有庄家才能开始牌局"
ReturnCode[2023] = "只有房主才能解散房间"
ReturnCode[2024] = "请在庄家下庄后解散房间"
ReturnCode[2025] = "创建房间失败"
----------------------