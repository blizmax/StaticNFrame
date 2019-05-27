module("GetMailGoods", package.seeall)

--获取邮件附件
function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_human_pb.cggetmailgoods()
	local gcmsg = msg_human_pb.gcgetmailgoods()
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid, operateID)
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "mail", "GetMailGoods", 0, "重复发送领取邮件物品协议，mailid="..cgmsg.mailid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local mItem = MailModel.GetMailItem(cgmsg.userid, cgmsg.mailid)
	
	--邮件不存在
	if mItem == nil then
		gcmsg.result = ReturnCode["mail_not_exist"]
		LogBehavior.Warning(cgmsg.userid, "mail", "GetMailGoods", ReturnCode["mail_not_exist"], "邮件不存在，mailid="..cgmsg.mailid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid, "mail", "GetMailGoods", ReturnCode["mail_not_exist"], "人物信息不存在，mailid="..cgmsg.mailid)
		return cgmsg.userid, ReturnCode["player_not_exist"], 0, ""
	end
	
	if mItem.mailstate == g_mailState.state_read then
		gcmsg.result = ReturnCode["mail_goods_got"]
		LogBehavior.Error(cgmsg.userid, "mail", "GetMailGoods", ReturnCode["mail_goods_got"], "邮件状态不匹配，mailid="..cgmsg.mailid..",mailstate="..mList.mlist[index].mailstate)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	
	local gAllList = GoodsModel.GetGoodsListAll(cgmsg.userid)
	local arrTable = luajson.decode(mItem.externdata)
	if arrTable == nil or type(arrTable) ~= "table" or #arrTable == 0 then
		
		gcmsg.result = ReturnCode["mail_goods_unexist"]
		LogBehavior.Error(cgmsg.userid, "mail", "GetMailGoods", ReturnCode["mail_goods_unexist"], "邮件附件不匹配，mailid="..cgmsg.mailid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	local awardList = {}
	local isMoney = false
	for k,v in ipairs(arrTable) do
		local category = math.modf(v[1]/1000)
		if category == g_goodsCategory.gift_bag then
			local tempList = GiftBagModel.GetGoodsList(v[1],v[2])
			
			for m,n in ipairs(tempList) do
				table.insert(awardList, n)
			end
		else
			local item = {}
			item[1] = tonumber(v[1])
			item[2] = tonumber(v[2])
			table.insert(awardList, item)
		end
		
		if category == g_goodsCategory.currency then
			isMoney = true
		end
	end
	
	PlayerModel.AddGoodsList(pInfo, gAllList, awardList, "mail", "get mail goods")
	mItem.mailstate = g_mailState.state_read

	gcmsg.result = 0
	gcmsg.mailid = cgmsg.mailid
	gcmsg.mailstate = mItem.mailstate
	
	local sqlCase = "update dy_mail set mailstate="..mItem.mailstate.." where id="..cgmsg.mailid
	SqlServer.rpush(sqlCase)
	for k,v in ipairs(awardList) do
		local addItem = gcmsg.getlist:add()
		addItem.goodsid = v[1]
		addItem.amount = v[2]
	end
	GoodsModel.SetGoodsList(cgmsg.userid, gAllList)
	MailModel.SetMailItem(cgmsg.userid, mItem)
	
	if isMoney == true then
		PlayerModel.SetPlayerInfo(pInfo)
		PlayerModel.SendPlayerInfo(pInfo, {"jetton","money","gold"})
	end
	
	GoodsModel.SendAddGoodsList(pInfo, awardList)
	
	LogBehavior.Info(pInfo,"mail","GetMailGoods",0,"领取附件成功,mailid="..cgmsg.mailid)
	
	return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
end

