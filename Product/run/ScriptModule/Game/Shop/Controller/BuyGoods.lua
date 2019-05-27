module("BuyGoods", package.seeall)

--购买物品
function execute(packetID, operateID, buffer)
	local cgmsg = msg_shop_pb.cgbuygoods()
	local gcmsg = msg_shop_pb.gcbuygoods()
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "shop", "BuyGoods", 0, "购买物品重复下单,shopid="..cgmsg.shopid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		--如果在缓存中找不到对应的玩家的playerInfo, 则让玩家退出从新登陆拉去玩家信息
		LogBehavior.Error(cgmsg.userid, "shop", "BuyGoods", ReturnCode["player_not_exist"], "人物信息不存在")
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	
	local function PayForMoyey()
			--这里是掉起付费的接口的，取到对应的交易ID
		local timeSec = TimeUtils.GetTime()
		local orderID = ShopModel.GetOrderID(pInfo,cgmsg.shoptype,cgmsg.shopid,timeSec)
		
		if orderID == 0 then
			--gcmsg.result = ReturnCode["buy_diamond_error"]
			LogBehavior.Error(cgmsg.userid, "pay", "BuyDiamond", ReturnCode["buy_money_fail"], "下单数据库返回订单号为0")
			--return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			return ReturnCode["buy_diamond_error"]
		end
		
		local bdInfo = st_shop_pb.buymoneyinfo()
		bdInfo.orderid = orderID
		bdInfo.userid = cgmsg.userid
		bdInfo.buyid = cgmsg.shopid
		bdInfo.timeid = timeSec
		bdInfo.status = 0
		bdInfo.channel = pInfo.channel
		bdInfo.shoptype = cgmsg.shoptype
		
		local sign = ShopModel.GetSign(bdInfo)

		sqlCase = "update dy_order set sign='"..sign.."' where id="..orderID
		gcmsg.sign = sign
		bdInfo.sign = sign
		gcmsg.shopid = cgmsg.shopid   --在这里，payid就是shopid
		gcmsg.orderid = tostring(orderID)
		--SqlServer.rpush(sqlCase)		
		mysqlItem:execute(sqlCase)
		ShopModel.SetBuyMoneyInfo(bdInfo)	
		return 0
	end
	
	
	if cgmsg.shoptype == g_shopDefine.shop_money then   --这里表示是直接用钱购买钻石的
		local payItem = ShopService.GetPayItem(cgmsg.shopid)  --db_pay[cgmsg.shopid]
		
		if payItem == nil then
			gcmsg.result = ReturnCode["buy_money_error"]
			LogBehavior.Error(cgmsg.userid, "pay", "BugGoods", ReturnCode["buy_money_error"], "购买商品不存在，shopid="..cgmsg.shopid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		PlayerStatsModel.CallPay(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
		
		local retPay = PayForMoyey()
		if retPay ~= 0 then
			gcmsg.result = retPay
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
	else
		--g_shopDefine.shop_goods = 1002 --购买普通物品
		local shopItem = ShopService.GetShopItem(cgmsg.shopid)  --db_shop[cgmsg.shopid]
		if shopItem == nil then
			--购买的物品不存在
			gcmsg.result = ReturnCode["shop_item_unexist"]
			LogBehavior.Error(pInfo, "shop","BuyGoods", 0, "购买物品不存在,shopid="..cgmsg.shopid)
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		--先检查需要的物品
		
		if shopItem[db_shopIndex.moneytype1] == g_shopDefine.pay_money then
			--这个是直接用钱购买的
			local retPay = PayForMoyey()
			if retPay ~= 0 then
				gcmsg.result = retPay
				return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
		else
			local sendList = {}
			--剩下的就是需要物品的了
			if shopItem[db_shopIndex.moneytype1] == g_shopDefine.pay_diamond then
				
				if pInfo.money < shopItem[db_shopIndex.price1] then
					gcmsg.result = ReturnCode["money_not_enough"]
					LogBehavior.Error(pInfo, "shop","BuyGoods", 0, "钻石不足,shopid="..cgmsg.shopid..",money"..pInfo.money)
					return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()			
				end
			
			end
			
			if shopItem[db_shopIndex.moneytype2] == g_shopDefine.pay_popular then
				
				if pInfo.popularity < shopItem[db_shopIndex.price2] then
					gcmsg.result = ReturnCode["popular_not_enough"]
					LogBehavior.Error(pInfo, "shop","BuyGoods", 0, "钻石不足,shopid="..cgmsg.shopid..",money"..pInfo.money)
					return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()			
				end

				PlayerModel.DecPopular(pInfo, shopItem[db_shopIndex.price2], "shop", "购买物品"..shopItem[db_shopIndex.name])
				
				table.insert(sendList, "popularity")
			end

			local getList = {}
		
			if math.floor(tonumber(shopItem[db_shopIndex.goodsid])/1000) == g_goodsCategory.gift_bag then
				
				getList = GiftBagModel.GetGoodsList(shopItem[db_shopIndex.goodsid], shopItem[db_shopIndex.amount])
			else
				local item = {}
				item[1] = tonumber(shopItem[db_shopIndex.goodsid])
				item[2] = shopItem[db_shopIndex.amount]
				table.insert(getList, item)
			end

			PlayerModel.DecMoney(pInfo, shopItem[db_shopIndex.price1], "shop", "购买物品"..shopItem[db_shopIndex.name])
			table.insert(sendList, "money")
			table.insert(sendList, "jetton")
			
			local gAllList = GoodsModel.GetGoodsListAll(cgmsg.userid)
			PlayerModel.AddGoodsList(pInfo, gAllList, getList, "shop", "shopID="..cgmsg.shopid..":"..cgmsg.userid.."使用了"..shopItem[db_shopIndex.price1].."钻石购买了"..shopItem[db_shopIndex.name])	
			
			gcmsg.result = 0
			PlayerModel.SetPlayerInfo(pInfo)
			GoodsModel.SetGoodsList(cgmsg.userid, gAllList)
			PlayerModel.SendPlayerInfo(pInfo,sendList)
			GoodsModel.SendAddGoodsList(pInfo, getList)

		end
		
	end

	gcmsg.shoptype = cgmsg.shoptype
	gcmsg.userid = cgmsg.userid
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end