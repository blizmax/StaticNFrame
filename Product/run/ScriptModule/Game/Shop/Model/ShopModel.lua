
ShopModel = {}
ShopModel.redis_index = "redis_shop"
ShopModel.buy_money = "buy_money"

function ShopModel.SetBuyMoneyInfo(bdInfo)
	redisItem:setex(ShopModel.buy_money..bdInfo.orderid, g_hourSeconds, bdInfo:SerializeToString(), ShopModel.redis_index)
end

function ShopModel.GetSign(bdInfo)
	local sign = "userid="..bdInfo.userid.."&buyid="..bdInfo.buyid.."&orderid="..bdInfo.orderid.."&key="..g_signKey
	return md5(sign)
end

function ShopModel.MakeSign(userID,payID,orderID)
	local sign = "userid="..userID.."&buyid="..payID.."&orderid="..orderID.."&key="..g_signKey
	return md5(sign)
end

function ShopModel.GetBuyMoneyInfo(orderid, bdInfo)
	if bdInfo == nil then
		bdInfo = st_shop_pb.buymoneyinfo()
	end

	local strGet = redisItem:get("buydiamond"..orderid, ShopModel.redis_index)
	if strGet ~= nil then
		
		bdInfo:ParseFromString(strGet)
	else
	
		local sqlCase = "select userid,buyid,sign,channel,status from dy_order where id="..orderid 
		mysqlItem:executeQuery(sqlCase)
		local sqlData = mysqlItem:fetch({})
		if sqlData ~= nil then
			
			bdInfo.orderid = tonumber(orderid)
			bdInfo.userid = tonumber(sqlData[1])
			bdInfo.buyid = tonumber(sqlData[2])
			bdInfo.sign = sqlData[3]
			bdInfo.channel = sqlData[4]
			bdInfo.status = tonumber(sqlData[5])
		end
		
	end
	return bdInfo	
end


function ShopModel.GetOrderID(pInfo,shopType,buyID,timeSec)
	
	local sqlCase = "insert into dy_order(userid,buyid,timeid,status,channel,shoptype) values("..pInfo.userid..","..buyID..","..timeSec..",0,'"..pInfo.channel.."',"..shopType..")"
	mysqlItem:execute(sqlCase)
	sqlCase = "select id from dy_order where userid="..pInfo.userid.." and buyid="..buyID.." and timeid="..timeSec.." and status=0"
	
	mysqlItem:executeQuery(sqlCase)
	
	local sqlData = mysqlItem:fetch()
	if sqlData == nil then
		return 0
	end
	return tonumber(sqlData)   --取得订单ID
end


