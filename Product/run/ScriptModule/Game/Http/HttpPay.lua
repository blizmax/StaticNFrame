module("HttpPay", package.seeall)

function mmmjpay(rcvData)
	--LogFile("pay",rcvData)
	
	
	if rcvData == nil then
		LogFile("payerror","recv data is nil")
		return "success"
	end
	
	local jsonArr = luajson.decode(rcvData)
	if jsonArr["orderid"] == nil then
		LogFile("payerror", "recv data is orderid nil")
		return "success"
	end
	if jsonArr["userid"] == nil then
		LogFile("payerror", "recv data is userid nil")
		return "success"
	end	

	if jsonArr["payid"] == nil then
		LogFile("payerror", "recv data is payid nil")
		return "success"
	end
	if jsonArr["paymoney"] == nil then
		LogFile("payerror", "recv data is paymoney nil")
		return "success"
	end

	if jsonArr["app_sign"] == nil then
		LogFile("payerror", "recv data is app_sign nil")
		return "success"
	end	

	local function updateMysql(status)
		local sqlCase =  "update dy_order set status="..status.." where id="..jsonArr["orderid"]
		mysqlItem:execute(sqlCase)
	end
	
	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	if bdInfo.orderid == 0 then
		LogFile("payerror","get buyinfo from redis in nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
		return "success"
	end

	if bdInfo.status ~= 0 then
		LogFile("payerror", "order status not math, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"]..",status="..bdInfo.status)
		updateMysql(-99)
		return "success"
	end
	
	local sign = ShopModel.MakeSign(jsonArr["userid"], jsonArr["payid"], jsonArr["orderid"])
	if sign ~= bdInfo.sign then
		LogFile("payerror", "recv data check sign fail, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-5)
		return "success"
	end

	local pInfo = PlayerModel.GetPlayerInfo(jsonArr["userid"])
	if pInfo == nil then
		LogFile("payerror", "recv data pInfo is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-6)
		return "success"
	end
	
	local payItem = db_paygdmj.GetItem(jsonArr["payid"])
	if payItem == nil then
		LogFile("payerror", "mmmjpay payItem is nil userid="..jsonArr["userid"])
		updateMysql(-7)
		return "success"
	end
	--
	--
	if tonumber(jsonArr["paymoney"])/100 ~= payItem[db_paygdmjIndex["paymoney"]] then
		LogFile("paywarning", "pay money not match paymoney="..jsonArr["paymoney"]..",userid="..jsonArr["userid"])
	end
	
	local getMoney = payItem[db_paygdmjIndex["getmoney"]] + payItem[db_paygdmjIndex["extramoney"]]
	
	PlayerModel.AddMoney(pInfo, getMoney,"pay","购买")
	PlayerModel.SetPlayerInfo(pInfo)
	PlayerModel.SendPlayerInfo(pInfo,{"money"})
	--下面开始检查代理
	
	if math.floor(pInfo.agencystatus/10) == 2 then
		--他自己已经是代理了。
		--这个时候，他自己充的钱，给自己返现30%，给上级代理返现10%
		local retMoney = payItem[db_paygdmjIndex["paymoney"]]*3/10  --返现30%
		local sqlCase = "update ag_player set allmoney=allmoney+"..retMoney..",takemoney=takemoney+"..retMoney.." where userid="..pInfo.userid
		mysqlItem:execute(sqlCase)
		
		--看看这个人上面有没有绑定代理
		
		sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..pInfo.userid..","..pInfo.userid..",'"..pInfo.nickname.."',99,"..payItem[db_paygdmjIndex["paymoney"]]..","..retMoney..")"
		mysqlItem:execute(sqlCase)
		if math.mod(pInfo.agencystatus,10) ~= 0 then
			--这里判断代理分离就好了
			sqlCase = "select userid from ag_bindlist where binduserid="..pInfo.userid
			mysqlItem:executeQuery(sqlCase)
			local sqlData = mysqlItem:fetch()
			
			if sqlData ~= nil then
				--表示这里是有代理的
				retMoney = payItem[db_paygdmjIndex["paymoney"]]*1/10  --这里是拿1成
				sqlCase = "update ag_player set allmoney=allmoney+"..retMoney..",takemoney=takemoney+"..retMoney.." where userid="..sqlData
				mysqlItem:execute(sqlCase)   
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..sqlData..","..pInfo.userid..",'"..pInfo.nickname.."',99,"..payItem[db_paygdmjIndex["paymoney"]]..","..retMoney..")"
				mysqlItem:execute(sqlCase)  --这里需要立刻执行
			end
		end
		
	else
		--如果这个人不是代理，则看看是否绑定了
		if math.mod(pInfo.agencystatus,10) ~= 0 then
			--如果是绑定的
			local sqlCase = "select userid from ag_bindlist where binduserid="..pInfo.userid
			mysqlItem:executeQuery(sqlCase)
			local sqlData = mysqlItem:fetch()
			if sqlData ~= nil then
				--上级代理返利30%
				local retMoney = payItem[db_paygdmjIndex["paymoney"]]*3/10  --返现30%
				sqlCase = "update ag_player set allmoney=allmoney+"..retMoney..",takemoney=takemoney+"..retMoney.." where userid="..sqlData
				mysqlItem:execute(sqlCase)
				
				sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..sqlData..","..pInfo.userid..",'"..pInfo.nickname.."',1,"..payItem[db_paygdmjIndex["paymoney"]]..","..retMoney..")"
				mysqlItem:execute(sqlCase)  --这里需要立刻执行				
				
				
				--在看看代理上面是否还有代理
				sqlCase = "select userid from ag_bindlist where binduserid="..sqlData
				mysqlItem:executeQuery(sqlCase)
				sqlData = mysqlItem:fetch()
				if sqlData ~= nil then
					--二级代理返利10%
					retMoney = payItem[db_paygdmjIndex["paymoney"]]*1/10  --这里是拿1成
					sqlCase = "update ag_player set allmoney=allmoney+"..retMoney..",takemoney=takemoney+"..retMoney.." where userid="..sqlData
					mysqlItem:execute(sqlCase)
					sqlCase = "insert into ag_paylist(userid,payuserid,paynick,aglevel,paymoney,getmoney) values("..sqlData..","..pInfo.userid..",'"..pInfo.nickname.."',1,"..payItem[db_paygdmjIndex["paymoney"]]..","..retMoney..")"
					mysqlItem:execute(sqlCase)  --这里需要立刻执行
					
				end
			end
		end		
	end
	updateMysql(1)
	return "success"
	
end

