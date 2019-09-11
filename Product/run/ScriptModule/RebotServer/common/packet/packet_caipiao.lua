--协议 :6000~7000 是彩票的玩法

--6001 - 6100   公共协议
--押注记录
PacketCode[6001] = {server = 6001, client = 6002, des = "cgcaipiaopourrecord", func = "CaiPiaoPourRecord"}
PacketCode[6002] = {server = 6001, client = 6002, des = "gccaipiaopourrecord", func = "CaiPiaoPourRecord"}
--开奖记录 路单
PacketCode[6003] = {server = 6003, client = 6004, des = "cgcaipiaohistory", func = "CaiPiaoOpenResult"}
PacketCode[6004] = {server = 6003, client = 6004, des = "gccaipiaohistory", func = "CaiPiaoOpenResult"}

--撤单
PacketCode[6005] = {server = 6005, client = 6006, des = "cgcaipiaorevocation", func = "CaiPiaoRevocation"}
PacketCode[6006] = {server = 6005, client = 6006, des = "gccaipiaorevocation", func = "CaiPiaoRevocation"}

--续投
PacketCode[6007] = {server = 6007, client = 6008, des = "cgcaipiaorxutou", func = "CaiPiaoXuTou"}
PacketCode[6008] = {server = 6007, client = 6008, des = "gccaipiaorxutou", func = "CaiPiaoXuTou"}

-------------------------------------------------------------------------------------------------------
                                        --6101 - 6200   幸运飞艇
--------------------------------------------------------------------------------------------------------
--进入
PacketCode[6101] = {server = 6101, client = 6102, des = "cgcaipiaoenter", func = "CaiPiaoXyftEnter"}
PacketCode[6102] = {server = 6101, client = 6102, des = "gccaipiaoenter", func = "CaiPiaoXyftEnter"}

--离开
PacketCode[6103] = {server = 6103, client = 6104, des = "cgcaipiaoleave", func = "CaiPiaoXyftLeave"}
PacketCode[6104] = {server = 6103, client = 6104, des = "gccaipiaoleave", func = "CaiPiaoXyftLeave"}

--下注
PacketCode[6105] = {server = 6105, client = 6106, des = "cgcaipiaopourjetton", func = "CaiPiaoXyftPourJetton"}
PacketCode[6106] = {server = 6105, client = 6106, des = "gccaipiaopourjetton", func = "CaiPiaoXyftPourJetton"}

----撤回
--PacketCode[6107] = {server = 6107, client = 6108, des = "cgcaipiaorevocation", func = "CaiPiaoXyftRevocation"}
--PacketCode[6108] = {server = 6107, client = 6108, des = "gccaipiaorevocation", func = "CaiPiaoXyftRevocation"}

--通知下注 封盘 维护 状态更新
PacketCode[6110] = {server = 6109, client = 6110, des = "gccaipiaoupdate", func = "CaiPiaoXyftUpdate"}

--历史单个结果
PacketCode[6112] = {server = 6111, client = 6112, des = "gccaipiaohistory", func = "CaiPiaoXyftHistory"}

-------------------------------------------------------------------------------------------------------
                                    --6201 - 6300   六合彩
--------------------------------------------------------------------------------------------------------
--进入
PacketCode[6201] = {server = 6201, client = 6202, des = "cgcaipiaoenter", func = "CaiPiaoLhcEnter"}
PacketCode[6202] = {server = 6201, client = 6202, des = "gccaipiaoenter", func = "CaiPiaoLhcEnter"}

--离开
PacketCode[6203] = {server = 6203, client = 6204, des = "cgcaipiaoleave", func = "CaiPiaoLhcLeave"}
PacketCode[6204] = {server = 6203, client = 6204, des = "gccaipiaoleave", func = "CaiPiaoLhcLeave"}

--下注
PacketCode[6205] = {server = 6205, client = 6206, des = "cgcaipiaopourjetton", func = "CaiPiaoLhcPourJetton"}
PacketCode[6206] = {server = 6205, client = 6206, des = "gccaipiaopourjetton", func = "CaiPiaoLhcPourJetton"}

----撤回
--PacketCode[6207] = {server = 6207, client = 6208, des = "cgcaipiaorevocation", func = "CaiPiaoLhcRevocation"}
--PacketCode[6208] = {server = 6207, client = 6208, des = "gccaipiaorevocation", func = "CaiPiaoLhcRevocation"}

--通知下注 封盘 维护 状态更新
PacketCode[6210] = {server = 6209, client = 6210, des = "gccaipiaoupdate", func = "CaiPiaoLhcUpdate"}

--历史单个结果
PacketCode[6212] = {server = 6211, client = 6212, des = "gccaipiaohistory", func = "CaiPiaoLhcHistory"}

-------------------------------------------------------------------------------------------------------
                                   --6301 -6400   北京赛车
--------------------------------------------------------------------------------------------------------
--进入
PacketCode[6301] = {server = 6301, client = 6302, des = "cgcaipiaoenter", func = "CaiPiaoBjscEnter"}
PacketCode[6302] = {server = 6301, client = 6302, des = "gccaipiaoenter", func = "CaiPiaoBjscEnter"}

--离开
PacketCode[6303] = {server = 6303, client = 6304, des = "cgcaipiaoleave", func = "CaiPiaoBjscLeave"}
PacketCode[6304] = {server = 6303, client = 6304, des = "gccaipiaoleave", func = "CaiPiaoBjscLeave"}

--下注
PacketCode[6305] = {server = 6305, client = 6306, des = "cgcaipiaopourjetton", func = "CaiPiaoBjscPourJetton"}
PacketCode[6306] = {server = 6305, client = 6306, des = "gccaipiaopourjetton", func = "CaiPiaoBjscPourJetton"}

----撤回
--PacketCode[6307] = {server = 6307, client = 6308, des = "cgcaipiaorevocation", func = "CaiPiaoBjscRevocation"}
--PacketCode[6308] = {server = 6307, client = 6308, des = "gccaipiaorevocation", func = "CaiPiaoBjscRevocation"}

--通知下注 封盘 维护 状态更新
PacketCode[6310] = {server = 6309, client = 6310, des = "gccaipiaoupdate", func = "CaiPiaoBjscUpdate"}

--历史单个结果
PacketCode[6312] = {server = 6311, client = 6312, des = "gccaipiaohistory", func = "CaiPiaoBjscHistory"}



ReturnCode["caipiao_not_exist"] = 6001
ReturnCode["caipiao_state_notpour"] = 6002
ReturnCode["caipiao_jetton_notenough"] = 6003
ReturnCode["caipiao_pourtype_error"] = 6004
ReturnCode["caipiao_jetton_less"] = 6005
ReturnCode["caipiao_jetton_error"] = 6006
ReturnCode["orderid_not_exist"] = 6007
ReturnCode["orderid_revocation_fail"] = 6008

ReturnCode["orderid_xutou_fail_0"] = 6009
ReturnCode["orderid_xutou_fail_1"] = 6010
ReturnCode["orderid_xutou_fail_2"] = 6011


ReturnCode[6001] = "获取信息失败"
ReturnCode[6002] = "该期已封盘，下注失败"
ReturnCode[6003] = "您的筹码不足，下注失败"
ReturnCode[6004] = "下注失败，请选择正确的下注区域"
ReturnCode[6005] = "单注下注额不能少于2元"
ReturnCode[6006] = "下注金额错误"
ReturnCode[6007] = "订单不存在"
ReturnCode[6008] = "订单已封盘，撤单失败"

ReturnCode[6009] = "该期已封盘，续投失败"
ReturnCode[6010] = "上期未下注，续投失败"
ReturnCode[6011] = "您的筹码不足，续投失败"

--------------------------------------------------------------


