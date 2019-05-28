--获取房主自己的房间列表

module("DouNiuTableList", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_pb.cgdouniutablelist()
	local gcmsg = msg_douniu_pb.gcdouniutablelist()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuTableList", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	local getList = DouNiuModel.GetUserTableList(cgmsg.userid) 
	--这里先对tableList进行排序

	local tableList = {}
	if getList ~= nil then
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
	end

	if #tableList > 0 then
	
		local startPos = (cgmsg.pagenum - 1)*30 + 1
		local endPos = startPos + 30
		endPos = #tableList > endPos and endPos or #tableList
		
		for i =startPos,endPos do
			--每一次获取一个
			local tInfo =  DouNiuModel.GetTableInfo(tableList[i][1]) 
			if tInfo ~= nil and tInfo.state ~= g_douNiuDefine.status_delete then
				--因为解散是放到循环线程里面做，所以这里需要判断状态是不是解散的状态，如果是解散的状态，
				--就不能够再把这个列表加进来
				gcmsg.tableid:append(tInfo.tableid)
				gcmsg.dntype:append(tInfo.dntype)
				gcmsg.pourjetton:append(tInfo.difen)
				gcmsg.gamenum:append(tInfo.usevipnum)
				gcmsg.maxplaynum:append(tInfo.maxplaynum)
				gcmsg.julebuid:append(tInfo.julebuid)
				gcmsg.julebutype:append(tInfo.julebutype)
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