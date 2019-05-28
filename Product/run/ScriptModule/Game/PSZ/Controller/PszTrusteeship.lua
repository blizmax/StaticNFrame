module("PszTrusteeship", package.seeall)

--这是托管

function execute(packetID, operateID, buffer)
	local cgmsg = msg_psz_2_pb.cgpsztrusteeship()
	local gcmsg = msg_psz_2_pb.gcpsztrusteeship()
	
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.senderid, "psz", "PszTrusteeship", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end

	ThreadManager.PszLock(cgmsg.tableid)
	
	local tInfo = PszModel.GetTableInfo(cgmsg.tableid)
	
	if tInfo == nil then
		gcmsg.result = ReturnCode["psz_not_exist"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local is = false
	for i = 1 , #tInfo.situser do
		if cgmsg.userid == tInfo.situser[i].userid then
			tInfo.situser[i].trusteeship = cgmsg.trusteeship
			if cgmsg.trusteeship == g_PszDefine.trusteeship_no then
				tInfo.situser[i].followbet = 0
				tInfo.situser[i].addbet = 0
				tInfo.situser[i].addbetpoint = 0
				tInfo.situser[i].lookcards = 0
				tInfo.situser[i].comparecards = 0
				tInfo.situser[i].discard = 0
			elseif cgmsg.trusteeship == g_PszDefine.trusteeship_yes then
				tInfo.situser[i].followbet = cgmsg.followbet
				tInfo.situser[i].addbet = cgmsg.addbet
				tInfo.situser[i].addbetpoint = cgmsg.addbetpoint
				tInfo.situser[i].lookcards = cgmsg.lookcards
				tInfo.situser[i].comparecards = cgmsg.comparecards
				tInfo.situser[i].discard = cgmsg.discard
				
				gcmsg.followbet = cgmsg.followbet
				gcmsg.addbet = cgmsg.addbet
				gcmsg.addbetpoint = cgmsg.addbetpoint
				gcmsg.lookcards = cgmsg.lookcards
				gcmsg.comparecards = cgmsg.comparecards
				gcmsg.discard = cgmsg.discard
			end
			is = true
		end
	end
	
	if is == false then 
		gcmsg.result = ReturnCode["psz_opt_error"]
		ThreadManager.PszUnLock(cgmsg.tableid)
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	PszModel.SetTableInfo(tInfo, 1)
	ThreadManager.PszUnLock(cgmsg.tableid)	
	
	--print("cgmsg.volun = "..cgmsg.volun)
	gcmsg.userid = cgmsg.userid
	gcmsg.trusteeship = cgmsg.trusteeship
	gcmsg.result = 0
	
	local userIDList =  PszService.GetUserIDList( tInfo, cgmsg.userid )
	SendMessage(userIDList, PacketCode[3344].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
	--print(gcmsg)
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end