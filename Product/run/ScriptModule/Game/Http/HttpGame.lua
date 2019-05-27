module("HttpGame", package.seeall)

function sendusermail(rcvData)
	if rcvData == nil then
		LogFile("payerror","recv data orderid is nil")
		return "success";
	end
	
	local jsonArr = luajson.decode(rcvData)
	MailModel.LoadMailList(jsonArr["userid"])
	
	NoticeModel.SendNotice(jsonArr["userid"], g_noticeType.mail_unread)
	return "success";	
end

function getgoodslist(rcvData)

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

function addbroadcast(rcvData)
	
	local jsonArr = luajson.decode(rcvData)
	BroadCastModel.LoadBroadByID(jsonArr['id'])
	
end

function delbroadcast(rcvData)
	local jsonArr = luajson.decode(rcvData)
	BroadCastModel.Delete(jsonArr['id'])
end

function specialinfo(rcvData)
	local jsonArr = luajson.decode(rcvData)
	SpecialModel.LoadSpecialInfo(tonumber(jsonArr['userid']))
	return "success";
end

function playerinfo(rcvData)
	local jsonArr = luajson.decode(rcvData)
	PlayerModel.LoadPlayer(tonumber(jsonArr['userid']))
	return "success";
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
	PlayerModel.AddMoney(pInfo, tonumber(jsonArr['amount']), "sys", "赠送钻石")
	PlayerModel.SendMoneyJetton(pInfo)
	PlayerModel.SetPlayerInfo(pInfo)
	return "success"
end

function deluser(rcvData)
	local jsonArr = luajson.decode(rcvData)	
	if jsonArr['userid'] == nil then
		return "error";
	end
	local pInfo = PlayerModel.GetPlayerInfo(jsonArr['userid'])
	if pInfo == nil then
		return "error"
	end
	LogFile("debug","account"..pInfo.account)
	redisItem:del("account"..pInfo.account, PlayerModel.redis_index)  --删除这个就好的了。
end


function notify(rcvData)
	
	local sqlCase = "select * from dy_orderinfo where orderid="..rcvData
	mysqlItem:executeQuery(sqlCase)
	
	local sqlData = mysqlItem:fetch({})
	
	local jsonArr = {}
	if sqlData ~= nil then
		jsonArr['id'] = sqlData[1]
		jsonArr['userid'] = sqlData[2]
		jsonArr['orderid'] = tonumber(sqlData[4])
		jsonArr['total_fee'] = tonumber(sqlData[9])
		jsonArr['state'] = tonumber(sqlData[10])
		jsonArr['payid'] = tonumber(sqlData[3])
		if jsonArr['state'] ~= 0 then
			LogFile("pay", "state error state="..jsonArr['state']..",id="..rcvData)
			return "error"
		end
		
		if jsonArr['total_fee'] <= 0 then
			LogFile("pay", "paynum error state="..jsonArr['state']..",id="..rcvData)
			return "error"			
		end

	else
		LogFile("pay", "order not exist id="..rcvData)
		return "error"
	end
	

	local function updateMysql(status)
		local sqlCase = "update dy_order set status="..status.." where id="..rcvData
		mysqlItem:execute(sqlCase)
		--SqlServer.rpush(sqlCase)
	end


	local bdInfo = ShopModel.GetBuyMoneyInfo(jsonArr["orderid"])
	
	if bdInfo.orderid == 0 then
		LogFile("payerror", "get buyifno from redis is nil, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
		updateMysql(-4)
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

		
	PlayerStatsModel.PaySuccess(pInfo,payItem[db_payIndex.paymoney])  --调起付费的接口
	

	LogFile("pay", "pay success, orderid="..jsonArr["orderid"]..",userid="..jsonArr["userid"])
	bdInfo.status = 1
	updateMysql(1)
	ShopModel.SetBuyMoneyInfo(bdInfo)

	
	sqlCase = "update dy_orderinfo set state = 2, remark='发货成功' where id="..jsonArr['id']
	mysqlItem:execute(sqlCase)
	
	return "success"
end

function addjulebu(rcvData)
	--从网页下发的接口，如果有玩家扫了俱乐部的二维码后，而且该用户已经创建角色，但是还没有加入俱乐部，那边就加入到俱乐部中。
	local jsonArr = luajson.decode(rcvData)	
	if jsonArr['userid'] == nil or jsonArr['julebuid'] == nil or jsonArr['higherid'] == nil then
		LogFile("error", "addjulebu error rcvData="..rcvData)
		return "error";
	end	

	local jInfo = JulebuModel.GetJulebuInfo(jsonArr['julebuid'])
	if jInfo == nil then
		return nil
	end

	
	local sqlCase="insert into dy_julebu_member(userid,julebuid,higherid,status) values("..jsonArr['userid']..","..jsonArr['julebuid']..","..jsonArr['higherid']..",99)"

	LogFile("debug", sqlCase)
	mysqlItem:execute(sqlCase)
	
	sqlCase = "select id from dy_julebu_member where userid="..jsonArr['userid'].." and julebuid="..jsonArr['julebuid']
	LogFile("debug", sqlCase)
	local julebuID = tonumber(jsonArr['julebuid'])
	mysqlItem:executeQuery(sqlCase)
	sqlData = mysqlItem:fetch()
	if sqlData == nil then
		return "error2"
	end

	local pInfo = PlayerModel.GetPlayerInfo(jsonArr['userid'])
	
	local aItem = st_julebu_pb.julebumember()
	aItem.userid = pInfo.userid
	aItem.nickname = pInfo.nickname
	aItem.faceid = pInfo.face_1
	aItem.id = tonumber(sqlData)
	aItem.higherid = tonumber(jsonArr['higherid'])

	JulebuModel.AddToMemberList(julebuID, aItem)
	JulebuModel.SetJulebuInfo(jInfo)
	JulebuModel.SetUserJulebuList(pInfo.userid, jInfo.julebuid)	

	jInfo.membernum = jInfo.membernum + 1
	sqlCase = "update dy_julebu set membernum="..jInfo.membernum.." where id="..jInfo.julebuid
	mysqlItem:execute(sqlCase)	
	return "success"
end