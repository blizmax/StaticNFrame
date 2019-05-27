GoodsModel = {}
GoodsModel.redis_index = "redis_goods"
GoodsModel.day_list = {2001,2002,2003,2004,2005,7001,7002}
--这里需要看看要不要对添加的物品进行统计
--由于要对物品进行有效判断，所以所有添加的物品即便过期了，也会存在物品表中。
--客户端过来getgoodslist的时候，需要判断是否有效的才发放
function GoodsModel.InsertGoods(addItem)
	
	local sqlCase = "insert into dy_goods(userid,goodsid,amount,validity,createdate,iswear) values("..addItem.userid..","..
	addItem.goodsid..","..addItem.amount..","..addItem.validity..",'"..addItem.createdate.."',"..addItem.iswear..")"
	mysqlItem:execute(sqlCase)
	
	sqlCase = "select id from dy_goods where userid="..addItem.userid.." and goodsid="..addItem.goodsid.." and createdate='"..addItem.createdate.."'"
	
	mysqlItem:executeQuery(sqlCase)
	
	local sqlData = mysqlItem:fetch({})
	
	if sqlData == nil then
		LogBehavior.Error(addItem.userid,"goods","GoodsModel.InsertGoods",-1,"物品插入数据库失败,userID="..addItem.userid..",goodsID="..addItem.goodsid)
	else
		addItem.itemid = tonumber(sqlData[1])
	end
	
	return addItem
end

function GoodsModel.GetGoodsList(userID,gList)
	local gAllList = GoodsModel.GetGoodsListAll(userID)
	

	if gList == nil then
		gList = st_human_pb.goodslist()
	end
	
	for i = 1,#gAllList.glist do
		if gAllList.glist[i].amount > 0 and gAllList.glist[i].validity ~= 0 then
			local addItem = gList.glist:add()
			addItem:ParseFromString(gAllList.glist[i]:SerializeToString())
		end
	end
	
	return gList
end


function GoodsModel.GetGoodsListAll(userID)
	
	if true == redisItem:exists("goodslist"..userID, GoodsModel.redis_index) then
		local gList = st_human_pb.goodslist();
		local strGet = redisItem:get("goodslist"..userID, GoodsModel.redis_index)
		gList:ParseFromString(strGet)
		return gList
	end
	return GoodsModel.LoadGoodsList(userID)
end

function GoodsModel.LoadGoodsList(userID)
	
	local currDate = TimeUtils.GetDayString()
	local sqlCase = "select * from dy_goods where userid="..userID --全部加载到列表中
	
	local gAllList = st_human_pb.goodslist()
	
	mysqlItem:executeQuery(sqlCase)
	
	for i = 1, 1000 do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		
		local item = gAllList.glist:add()
		item.itemid = tonumber(sqlData[1])
		item.userid = tonumber(userID)
		item.goodsid = tonumber(sqlData[3])
		item.amount = tonumber(sqlData[4])
		item.validity = tonumber(sqlData[5])
		item.createdate = sqlData[6]
		item.iswear = tonumber(sqlData[7])
		
	end
	LogBehavior.Info(userID, "goods", "LoadGoodsList", 0, "从数据库加载玩家物品列表")
	GoodsModel.SetGoodsList(userID, gAllList)
	return gAllList
end

function GoodsModel.SetGoodsList(userID, gAllList)
	redisItem:set("goodslist"..userID, gAllList:SerializeToString(), GoodsModel.redis_index)
end

function GoodsModel.DayCheck(userID)
	
	local gAllList = GoodsModel.GetGoodsListAll(userID)
	local currDate = TimeUtils.GetTableTime()
	local strDate = TimeUtils.GetTimeString(currDate)
	local cCount = 0
	for i = 1, #gAllList.glist do
		
		if gAllList.glist[i].validity > 0 then
			local validity = TimeUtils.DifferentDay(gAllList.glist[i].createdate, currDate)	
			
			validity = gAllList.glist[i].validity - validity
			validity = validity < 0 and 0 or validity
			gAllList.glist[i].validity = validity
			gAllList.glist[i].createdate = strDate
			local sqlCase = "update dy_goods set validity="..validity..",createdate='"..strDate.."' where id="..gAllList.glist[i].itemid
			SqlServer.rpush(sqlCase)
			cCount = cCount + 1

--[[	先注释，任务系统的		
			if validity > 0 then
				for k,v in ipairs(GoodsModel.day_list) do
					--检查是否存在需要领取的周卡和VIP每日领取
					if v == gAllList.glist[i].goodsid then
						MissionManager.MissionUpdate(userID, v, 1)
						break
					end
				end
			end--]]
			
		end
	end
	
	if cCount > 0 then
		GoodsModel.SetGoodsList(userID, gAllList)
	end
	
end

function GoodsModel.DayFirstLogin(userID)
	
	GoodsModel.DayCheck(userID)
end


function GoodsModel.GetPropEachList(amount)
	local gList = st_human_pb.goodslist();
	for k,v in pairs(db_goods) do
		if tonumber(v[db_goodsIndex.category]) == g_goodsCategory.prop_each then
			
			local addItem = gList.glist:add()
			addItem.amount = amount
			addItem.goodsid = k
			addItem.validity = tonumber(v[db_goodsIndex.category])
			
		end
		
	end
	return gList
end

--这个添加非礼包，非货币类的物品
function GoodsModel.AddGoods(pInfo, gAllList, goodsID, amount, modules, logMsg)
	local currTime = TimeUtils.GetTimeString()
	local gBase = db_goods[goodsID]
	if gBase == nil then
		--这里记录log，一定要记录log
		LogBehavior.Error(pInfo, modules,"GoodsModel.AddGoods",-1,logMsg.."，物品基础属性为空,goodsID="..goodsID)
		return nil
	end
	local raiseType = tonumber(gBase[db_goodsIndex.raisetype])

	if raiseType == g_raiseType.num_money then
		--记录出错log
		LogBehavior.Error(pInfo, modules,"GoodsModel.AddGoods",-1,logMsg.."，物品类型出错,raiseType="..raiseType)
		return nil
	end
	
	
	local isExist = false
	
	for k = 1, #gAllList.glist do
		if gAllList.glist[k].goodsid == tonumber(gBase[db_goodsIndex.id]) then
			
			
			if raiseType == g_raiseType.num_ulimit then
				--这个是限制使用数量，不限制时间的
				gAllList.glist[k].amount = gAllList.glist[k].amount + amount
				local sqlCase = "update dy_goods set amount="..gAllList.glist[k].amount.." where id="..gAllList.glist[k].itemid
				SqlServer.rpush(sqlCase)
				isExist = true
			elseif raiseType == g_raiseType.num_one then
				--这个是限制使用时间，数量只有1的,只有使用时间等于0的情况下，才能重新使用
				if gAllList.glist[k].validity == 0 then
					gAllList.glist[k].validity = tonumber(gBase[db_goodsIndex.validity])
					gAllList.glist[k].createdate = currTime
					gAllList.glist[k].iswear = 0
					local sqlCase = "update dy_goods set validity="..gAllList.glist[k].validity..",createdate="..gAllList.glist[k].createdate..",iswear=0".." where id="..gAllList.glist[k].itemid
					SqlServer.rpush(sqlCase)
					isExist = true
				end
			elseif raiseType == g_raiseType.num_time then
				--这个是时间上的累加，周卡，月卡，VIP贵宾卡
				gAllList.glist[k].validity = gAllList.glist[k].validity + tonumber(gBase[db_goodsIndex.validity])
				local sqlCase = "update dy_goods set validity="..gAllList.glist[k].validity.." where id="..gAllList.glist[k].itemid
				SqlServer.rpush(sqlCase)
				isExist = true
			end 
			
		end
	end
	if isExist == false then
		local addItem = gAllList.glist:add()
		addItem.goodsid = goodsID
		addItem.amount = amount
		addItem.validity = tonumber(gBase[db_goodsIndex.validity])
		addItem.userid = pInfo.userid
		addItem.createdate = currTime
		addItem.iswear = 0
		GoodsModel.InsertGoods(addItem)
		
	end

	--下面对购买了VIP卡和周卡月卡的情况进行校验
	if gBase[db_goodsIndex.category] == g_goodsCategory.vip then
		--如果购买的是VIP卡
		local gVIPBase = db_privilege[goodsID]
		if gVIPBase ~= nil then

			--pInfo.jetton = pInfo.jetton + gVIPBase[db_privilegeIndex.jetton]
			--MissionManager.LoginGoods(pInfo.userid, goodsID)
			PlayerModel.AddJetton(pInfo, gVIPBase[db_privilegeIndex.jetton], "goods","购买VIP="..gBase[db_goodsIndex.name])
			MissionManager.MissionUpdate(pInfo.userid, goodsID,1)
			
		end
		
	elseif gBase[db_goodsIndex.category] == g_goodsCategory.prop_card then
		
		PlayerModel.AddJetton(pInfo, gBase[db_goodsIndex.quality], "goods","购买"..gBase[db_goodsIndex.name])
		
		--MissionManager.MissionUpdate(pInfo.userid, goodsID,1)
	end


	LogBehavior.Info(pInfo, modules, "GoodsModel.AddGoods", 0, logMsg.."添加物品成功,goodsID="..goodsID..",amount="..amount)
	
end

function GoodsModel.SendAddGoodsList(pInfo, getList)

	local gcmsg = msg_human_pb.gcupdategoodslist()
	
	for k,v in ipairs(getList) do
		gcmsg.idlist:append(v[1])
		gcmsg.numlist:append(v[2])
	end
	
	gcmsg.result = 0
	SendMessage(pInfo.userid, PacketCode[1028].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
end

function GoodsModel.UseGoods(pgInfo)

	local sqlCase = "update dy_goods set amount="..pgInfo.amount.." where id="..pgInfo.itemid
	SqlServer.rpush(sqlCase)
end

function GoodsModel.GetHornAmount(userID)
	--获取喇叭数量
	local puList = GoodsModel.GetGoodsList(userID)
	local amount = 0
	for k,v in ipairs(puList) do
		if v.goodsid == 8001 then
			amount = v.amount
			break
		end
	end
	return g_noticeType.horn_amount,amount
end
