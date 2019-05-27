--获取房主自己的房间列表

module("GdmjTableList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjtablelist()
	local gcmsg = msg_gdmj_pb.gcgdmjtablelist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjTableList", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	local getList = GdmjModel.GetUserTableList(cgmsg.userid)
	--这里先对tableList进行排序
	local tableList = {}
	for k,v in pairs(getList) do
		local indexID = 1
		for i = 1,#tableList do
			if v > tableList[i][2] then
				indexID = i
				break
			end
		end
		local item = {}
		item[1] = k
		item[2] = v
		table.insert(tableList, indexID, item)
	end
	

	if tableList ~= nil then

		local startPos = (cgmsg.pagenum - 1)*20 + 1
		local endPos = startPos + 20
		endPos = #tableList > endPos and endPos or #tableList
		
		for i =startPos,endPos do
			--每一次获取一个
			local tInfo = GdmjModel.GetTableInfo(tableList[i][1])
			if tInfo ~= nil and tInfo.status ~= g_gdmjStatus.status_delete then
				--因为解散是放到循环线程里面做，所以这里需要判断状态是不是解散的状态，如果是解散的状态，
				--就不能够再把这个列表加进来
				gcmsg.tableid:append(tInfo.tableid)
				gcmsg.pourjetton:append(tInfo.pourjetton)
				gcmsg.gamenum:append(tInfo.maxvipnum)
				local userNum = 0
				for k,v in ipairs(tInfo.situser) do
					if v ~=0 then
						userNum = userNum + 1
					end
				end
				gcmsg.usernum:append(userNum)
				gcmsg.createtime:append(tonumber(tableList[i][2]))
			end
		end
	end
		
	gcmsg.result = 0
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end