

--1100~1200 是商城的ID区间
PacketCode[1101] = {server = 1101, client = 1102, des = "cgbuygoods", func = "BuyGoods"}
PacketCode[1102] = {server = 1101, client = 1102, des = "gcbuygoods", func = "BuyGoods"}


PacketCode[1103] = {server = 1103, client = 1104, des = "cgconvertgoods", func = "ConvertGoods"}
PacketCode[1104] = {server = 1103, client = 1104, des = "gcconvertgoods", func = "ConvertGoods"}

PacketCode[1105] = {server = 1105, client = 1106, des = "cgsetconvertinfo", func = "SetConvertInfo"}
PacketCode[1106] = {server = 1105, client = 1106, des = "gcsetconvertinfo", func = "SetConvertInfo"}


PacketCode[1107] = {server = 1107, client = 1108, des = "cggetconvertinfo", func = "GetConvertInfo"}
PacketCode[1108] = {server = 1107, client = 1108, des = "gcgetconvertinfo", func = "GetConvertInfo"}

PacketCode[1109] = {server = 1109, client = 1110, des = "cggetconvertlog", func = "GetConvertLog"}
PacketCode[1110] = {server = 1109, client = 1110, des = "gcgetconvertlog", func = "GetConvertLog"}

PacketCode[1111] = {server = 1111, client = 1112, des = "cggetshopvip", func = "GetShopVIP"}
PacketCode[1112] = {server = 1111, client = 1112, des = "gcgetshopvip", func = "GetShopVIP"}

PacketCode[1113] = {server = 1113, client = 1114, des = "cggetshopconf", func = "GetShopConf"}
PacketCode[1114] = {server = 1113, client = 1114, des = "gcgetshopconf", func = "GetShopConf"}


--PacketCode[1115] = {server = 1115, client = 1116, des = "cggetshopconf", func = "GetShopConf"}
PacketCode[1116] = {server = 1115, client = 1116, des = "gcpayconfirm", func = "PayConfirm"}

--商城返回的错误码
ReturnCode["buy_money_error"] = 1101
ReturnCode[1101] = "购买的物品不存在"

ReturnCode["buy_money_fail"] = 1102
ReturnCode[1102] = "购买钻石失败"

ReturnCode["shop_item_unexist"] = 1103
ReturnCode[1103] = "购买钻石失败"

ReturnCode["tick_not_enough"] = 1104
ReturnCode[1104] = "您的兑换券不足呢，多点玩游戏能获得更多兑换券呢。"

ReturnCode["convert_not_exist"] = 1105
ReturnCode[1105] = "兑换的物品不存在哦~~~"

ReturnCode["convert_address_error"] = 1106
ReturnCode[1106] = "您还没有填写收货信息呢，请先填写收货信息吧^_^"

