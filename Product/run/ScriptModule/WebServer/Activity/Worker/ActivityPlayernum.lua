--上报合法性校验的信息


module("ActivityPlayernum", package.seeall)

function work(buffer)
	--每5分钟调用该函数，
	
	local currUserID = ActivityModel.GetPlayerUserID()
	
	if currUserID == nil then
		
		redisItem:del( ActivityModel.activity_playernm, ActivityModel.redis_index )
		currUserID = 6619999
	end
	
	--luaDump(currUserID)
	
	local sqlCase = "select userid,agent1,wiringid from ag_player where userid>"..currUserID.." order by userid limit 200"
	--print(sqlCase)
	mysqlItem:executeQuery(sqlCase)
	
	local itemList = {}
	local nextID = 0
	while true do
		local sqlData = mysqlItem:fetch({})
		if sqlData == nil then
			break
		end
		local addItem = {}
		addItem[1] = tonumber(sqlData[1])
		addItem[2] = tonumber(sqlData[2])
		addItem[3] = tonumber(sqlData[3])
		nextID = tonumber(sqlData[1])
		table.insert(itemList, addItem)
	end
	--luaDump(itemList)
	if #itemList == 0 then
		return nil
	end

	for k,v_item in ipairs(itemList) do
		
		--如果有排线码，那么就用排线码的id
		local realUserID = tonumber(v_item[2])
		if v_item[3] ~= 0 then
			local wInfo = UnlimitedModel.GetWiringInfoByID(v_item[3])
			if wInfo ~= nil then
				realUserID = wInfo.ownerid
			end
		end
		--print(realUserID)
		if realUserID ~= 0 then
			ActivityModel.AddPlayerNum(realUserID)
		end
	end	
	ActivityModel.SetPlayerUserID(nextID)  --下一次的ID从这里开始
end




