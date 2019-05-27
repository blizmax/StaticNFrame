module("HttpHlyd", package.seeall)

function index(operateIndex, buffer)
	--MailModel.LoadGMMail()
 	return "success"
end


--该函数是GM给玩家发送物品的时候用
function buydiamond(rcvData)
	if rcvData == nil then
		
		return "success";
	end

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
	if payItem[db_payIndex.extramoney] ~= 0 then
		PlayerModel.AddMoney(pInfo, payItem[db_payIndex.extramoney], "pay", "购买赠送");
	end
	
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	updateMysql(1)
	
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	
	AgencyModel.AgencyDivide(jsonArr["userid"], payItem[db_payIndex.paymoney], pInfo.nickname)
	--LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	
	ShopModel.SetBuyMoneyInfo(bdInfo)
	
	return "success"
end

--给玩家添加附件
function usermail(rcvData)
	if rcvData == nil then
		
		return "success";
	end
	local jsonArr = luajson.decode(rcvData)
	--
	--luaDump(jsonArr)
	MailModel.LoadMailList(jsonArr["userid"])
	
	NoticeModel.SendNotice(jsonArr["userid"], g_noticeType.mail_unread)	
	
	return "success";
end

--给玩家添加附件
function usermailadjunct(rcvData)
	if rcvData == nil then
		
		return "success";
	end
	local jsonArr = luajson.decode(rcvData)
	--
	--luaDump(jsonArr)
	MailModel.LoadMailList(jsonArr["userid"])
	
	NoticeModel.SendNotice(jsonArr["userid"], g_noticeType.mail_unget)	
	
	return "success";
end


function shoplist(rcvData)


	--MailModel.LoadGMMail()

	local dataList = {}
	for k,v in pairs(db_shop) do
		if type(k) == "number"  and tonumber(v[db_shopIndex.isexist]) > 0 then
			table.insert(dataList, v)
		end
	end
	
	table.sort(dataList, function(a, b)
		return tonumber(a[1]) < tonumber(b[1])
	end)
	dataList = luajson.encode(dataList)
 	
	return dataList
end

function goodslist(rcvData)


	--MailModel.LoadGMMail()

	local dataList = {}
	for k,v in pairs(db_goods) do
		if type(k) == "number"  and tonumber(v[db_goodsIndex.isexist]) > 0 then
			table.insert(dataList, v)
		end
	end
	
	table.sort(dataList, function(a, b)
		return tonumber(a[1]) < tonumber(b[1])
	end)
	dataList = luajson.encode(dataList)
 	return dataList
end

function give(recvData)
	
	local jsonArr = luajson.decode(recvData)
	
	local signStrTemp = 'userid='..jsonArr['userid']..'&amount='..jsonArr['amount']..'&sign=w2ctsethjcyu7r9b'
	local sign = md5(signStrTemp)

	if sign ~= jsonArr['sign'] then
		return 'error'
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	if pInfo == nil then
		return 'error'
	end
	PlayerModel.AddJetton(pInfo,jsonArr["amount"],"jetton","give")
	PlayerModel.SetPlayerInfo(pInfo)

	PlayerModel.SendJetton(pInfo)	
	return "success"
end

function  recycle( recvData )
	local jsonArr = luajson.decode(recvData)
	local signStrTemp = 'userid='..jsonArr['userid']..'&amount='..jsonArr['amount']..'&sign=w2ctsethjcyu7r9b'
	local sign = md5(signStrTemp)

	if sign ~= jsonArr['sign'] then
		return 'error'
	end
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	if pInfo.jetton < tonumber(jsonArr["amount"]) then
		--如果对方的钱不够
		return error
	end
	PlayerModel.DecJetton(pInfo, jsonArr["amount"],"jetton","recycle")
	PlayerModel.SetPlayerInfo(pInfo)
	PlayerModel.SendJetton(pInfo)
	return "success"
end


