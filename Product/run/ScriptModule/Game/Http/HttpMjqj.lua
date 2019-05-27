module("HttpMjqj", package.seeall)
--麻将全集的

function mmmjagency(rcvData)
	if rcvData == nil then
		LogFile("error","mmmjagency rcvData is nil")
		return error
	end
	local pInfo = PlayerModel.GetPlayerInfo(rcvData)
	if pInfo == nil then
		LogFile("error","mmmjagency pInfo is nil")
		return error
	end
	pInfo.agencystatus = pInfo.agencystatus + 10
	PlayerModel.SetPlayerInfo(pInfo)
	PlayerModel.SendPlayerInfo(pInfo, {"agencystatus"})
	local sqlCase = "update dy_player set agencystatus="..pInfo.agencystatus.." where userid="..pInfo.userid
	
	AgencyModel.LoadAgencyInfo(tonumber(rcvData))   --重新加载代理系统
	mysqlItem:execute(sqlCase)
end

function impower(rcvData)
	
	if rcvData == nil then
		LogFile("error","impower rcvData is nil")
		return error
	end
	local pInfo = PlayerModel.GetPlayerInfo(rcvData)
	if pInfo == nil then
		LogFile("error","impower pInfo is nil")
		return error
	end
	AgencyModel.LoadAgencyInfo(tonumber(rcvData))   --重新加载代理系统
	return "success"
end

--该函数是GM给玩家发送物品的时候用
function xymjbuy(rcvData)


	if rcvData == nil then
		LogFile("payerror","recv data orderid is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)
	
	
	local function updateMysql(status)
		local sqlCase = "update dy_order set status="..status.." where id="..jsonArr['orderid']
		mysqlItem:execute(sqlCase)
		--SqlServer.rpush(sqlCase)
	end
	
	if jsonArr["orderid"] == nil then
		LogFile("payerror","recv data orderid is nil")

		return "success";
	end
	
	if jsonArr['userid'] == nil then
		LogFile("payerror", "recv data userid is nil, orderid="..jsonArr["orderid"])
		updateMysql(-1)
		return "success";
	end
	
	if jsonArr['payid'] == nil then
		LogFile("payerror", "recv data payid is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-2)
		return "success";
	end
	--[[
	if jsonArr['app_sign'] == nil then
		LogFile("payerror", "recv data sign is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-3)
		return "success";
	end
	]]	
	
	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	
	if bdInfo.orderid == 0 then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"		
	end

	if bdInfo.status ~= 0 then
		LogFile("payerror", "bdinfo status error orderid="..jsonArr["orderid"]..",status="..bdInfo.status)
		return "success"
	end
	
	local sign = "userid="..jsonArr["userid"].."&buyid="..jsonArr["payid"].."&orderid="..jsonArr["orderid"].."&key="..g_signKey
	sign = md5(sign)
	if sign ~= bdInfo.sign then
		LogFile("payerror", "recv data check sign fail, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success"	
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	

	if pInfo == nil then
		LogFile("payerror", "recv data pInfo is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success"
	end

	local payItem =  ShopService.GetPayItem(tonumber(jsonArr['payid'])) --db_pay[jsonArr['payid']]
	
	if payItem == nil then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"	
	end

	
	PlayerModel.AddMoney(pInfo, payItem[db_payIndex.getmoney], "pay", "支付购买");
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	AgencyModel.XymjDivide(jsonArr["userid"],payItem[db_payIndex.paymoney], pInfo.nickname)  --分销系统
	updateMysql(1)
		
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	

	LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	
	ShopModel.SetBuyMoneyInfo(bdInfo)
	
	return "success"
end

function xymjwxbuy(rcvData)
	
	if rcvData == nil then
		LogFile("payerror","recv data is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)
	
	local function updateMysql(status)
		local sqlCase = "update dy_order set status="..status.." where id="..jsonArr['orderid']
		SqlServer.rpush(sqlCase)
	end
	if jsonArr["orderid"] == nil then
		LogFile("payerror","recv data orderid is nil")

		return "success";
	end
	
	if jsonArr['userid'] == nil then
		LogFile("payerror", "recv data userid is nil, orderid="..jsonArr["orderid"])
		updateMysql(-1)
		return "success";
	end
	
	if jsonArr['payid'] == nil then
		LogFile("payerror", "recv data payid is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-2)
		return "success";
	end
	if jsonArr['app_sign'] == nil then
		LogFile("payerror", "recv data sign is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-3)
		return "success";
	end
		
	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	
	if bdInfo.orderid == 0 then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success";		
	end
	
	if bdInfo.status ~= 0 then
		LogFile("payerror", "order status not math, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-99)
		return "success";			
	end

	local sign = "userid="..jsonArr["userid"].."&buyid="..jsonArr["payid"].."&orderid="..jsonArr["orderid"].."&key="..g_signKey
	sign = md5(sign)

	if sign ~= bdInfo.sign then
		LogFile("payerror", "recv data check sign fail, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";		
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	

	if pInfo == nil then
		LogFile("payerror", "recv data pInfo is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";			
	end

	local payItem =  ShopService.GetPayItem(tonumber(jsonArr['payid'])) --db_pay[jsonArr['payid']]
	
	if payItem == nil then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"	
	end

	
	PlayerModel.AddMoney(pInfo, payItem[db_payIndex.getmoney], "pay", "支付购买");
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	AgencyModel.XymjDivide(jsonArr["userid"],payItem[db_payIndex.paymoney], pInfo.nickname)  --分销系统
	updateMysql(1)
		
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	

	LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	
	ShopModel.SetBuyMoneyInfo(bdInfo)
	
	return "success"
end

function hlydwxbuy(rcvData)
	
	if rcvData == nil then
		LogFile("payerror","recv data is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)
	
	local function updateMysql(status)
		local sqlCase = "update dy_order set status="..status.." where id="..jsonArr['orderid']
		--SqlServer.rpush(sqlCase)
		mysqlItem:execute(sqlCase)
	end
	if jsonArr["orderid"] == nil then
		LogFile("payerror","recv data orderid is nil")

		return "success";
	end
	
	if jsonArr['userid'] == nil then
		LogFile("payerror", "recv data userid is nil, orderid="..jsonArr["orderid"])
		updateMysql(-1)
		return "success";
	end
	
	if jsonArr['payid'] == nil then
		LogFile("payerror", "recv data payid is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-2)
		return "success";
	end
	if jsonArr['app_sign'] == nil then
		LogFile("payerror", "recv data sign is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-3)
		return "success";
	end
	
	--local sqlCase = "select status from "
	
	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	
	if bdInfo.orderid == 0 then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success";		
	end
	
	if bdInfo.status ~= 0 then
		LogFile("payerror", "order status not math, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-99)
		return "success";			
	end

	local sign = "userid="..jsonArr["userid"].."&buyid="..jsonArr["payid"].."&orderid="..jsonArr["orderid"].."&key="..g_signKey
	sign = md5(sign) 

	if sign ~= bdInfo.sign then
		LogFile("payerror", "recv data check sign fail, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";		
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	

	if pInfo == nil then
		LogFile("payerror", "recv data pInfo is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";
	end

	local payItem =  ShopService.GetPayItem(tonumber(jsonArr['payid'])) --db_pay[jsonArr['payid']]
	
	if payItem == nil then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"	
	end

	
	PlayerModel.AddMoney(pInfo, payItem[db_payIndex.getmoney], "pay", "支付购买");

	if payItem[db_payIndex.extramoney] ~= 0 then
		PlayerModel.AddMoney(pInfo, payItem[db_payIndex.extramoney], "pay", "购买赠送");
	end	
	
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	AgencyModel.ZsmjDivide(jsonArr["userid"],payItem[db_payIndex.paymoney], pInfo.nickname)  --分销系统
	updateMysql(1)
		
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	
	LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	
	ShopModel.SetBuyMoneyInfo(bdInfo)
	
	return "success"
end



function mmmjwxbuy(rcvData)
	
	if rcvData == nil then
		LogFile("payerror","recv data is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)
	
	local function updateMysql(status)
		local sqlCase = "update dy_order set status="..status.." where id="..jsonArr['orderid']
		--SqlServer.rpush(sqlCase)
		mysqlItem:execute(sqlCase)
	end
	if jsonArr["orderid"] == nil then
		LogFile("payerror","recv data orderid is nil")

		return "success";
	end
	
	if jsonArr['userid'] == nil then
		LogFile("payerror", "recv data userid is nil, orderid="..jsonArr["orderid"])
		updateMysql(-1)
		return "success";
	end
	
	if jsonArr['payid'] == nil then
		LogFile("payerror", "recv data payid is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-2)
		return "success";
	end
	if jsonArr['app_sign'] == nil then
		LogFile("payerror", "recv data sign is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-3)
		return "success";
	end
	
	--local sqlCase = "select status from "
	
	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	
	if bdInfo.orderid == 0 then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success";		
	end
	
	if bdInfo.status ~= 0 then
		LogFile("payerror", "order status not math, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-99)
		return "success";			
	end

	local sign = "userid="..jsonArr["userid"].."&buyid="..jsonArr["payid"].."&orderid="..jsonArr["orderid"].."&key="..g_signKey
	sign = md5(sign) 

	if sign ~= bdInfo.sign then
		LogFile("payerror", "recv data check sign fail, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";		
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	

	if pInfo == nil then
		LogFile("payerror", "recv data pInfo is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success";
	end

	local payItem =  ShopService.GetPayItem(tonumber(jsonArr['payid'])) --db_pay[jsonArr['payid']]
	
	if payItem == nil then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"	
	end

	PlayerModel.AddMoney(pInfo, payItem[db_payIndex.getmoney], "pay", "支付购买");

	if payItem[db_payIndex.extramoney] ~= 0 then
		PlayerModel.AddMoney(pInfo, payItem[db_payIndex.extramoney], "pay", "购买赠送");
	end	
	
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	AgencyModel.MmmjDivide(jsonArr["userid"],payItem[db_payIndex.paymoney], pInfo.nickname)  --分销系统
	updateMysql(1)
		
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	
	LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	
	ShopModel.SetBuyMoneyInfo(bdInfo)
	
	return "success"
end


function zuanshi(rcvData)
	if rcvData == nil then
		LogFile("payerror","recv data is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)	
	if jsonArr['userid'] == nil then
		return "success";
	end	
	if jsonArr['amount'] == nil then
		return "success";
	end
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr['userid'])
	if pInfo == nil then
		return "success";
	end
	PlayerModel.AddMoney(pInfo, jsonArr['amount'], "mail", "赠送钻石")
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	return "success"
end