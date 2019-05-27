
module("JulebuNowJiFen", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebunowjifen()
	local gcmsg = msg_julebu_pb.gcjulebunowjifen()
	
	cgmsg:ParseFromString(buffer)
	
	
	
	local jifenTemp = JulebuModel.GetTodayJiFen(cgmsg.julebuid)
	--这里需要进行排序
	local jifenList = {}
	local function insertJiFen(listTemp, item)   --先进行排序
		--if item[2] == 0 then
		--	return   --积分是0的，不用参加排行榜
		--end
		local tempItem = table.clone(item)
		table.insert(listTemp, item)
		for n = #listTemp, 2, -1 do
			if listTemp[n - 1][2] < listTemp[n][2] then
				listTemp[n][1] = listTemp[n-1][1]
				listTemp[n][2] = listTemp[n-1][2]
				listTemp[n - 1][1] = tempItem[1]
				listTemp[n - 1][2] = tempItem[2]
			else
				break
			end
		end
	end
	
	for k,v in pairs(jifenTemp) do
		
		local addItem = {}
		addItem[1] = k
		addItem[2] = tonumber(v)
		insertJiFen(jifenList, addItem)
	end
	
	
	
	
	local memList = JulebuModel.GetMemberList(cgmsg.julebuid)   --或者玩家列表
	for i = 1,#jifenList do
		if memList[jifenList[i][1]] ~= nil then
			--这里需要判断一下
			local addItem = gcmsg.mlist:add()
			addItem:ParseFromString(memList[jifenList[i][1]])
			local jiFen = JulebuModel.GetUserJiFen(cgmsg.julebuid, addItem.userid)
			jiFen = jiFen == nil and 0 or tonumber(jiFen)
			addItem.jifen = jiFen
		end
	end
	
	
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end