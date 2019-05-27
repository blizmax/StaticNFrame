

--1100~1200 是商城的ID区间
PacketCode[1101] = {server = 1101, client = 1102, des = "cgbuygoods", func = "BuyGoods"}
PacketCode[1102] = {server = 1101, client = 1102, des = "gcbuygoods", func = "BuyGoods"}




--商城返回的错误码
ReturnCode["buy_money_error"] = 1101
ReturnCode[1101] = "购买的物品不存在"

ReturnCode["buy_money_fail"] = 1102
ReturnCode[1102] = "购买钻石失败"

ReturnCode["shop_item_unexist"] = 1103
ReturnCode[1103] = "购买钻石失败"

