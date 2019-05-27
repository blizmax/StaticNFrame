
module("JulebuDayHistory", package.seeall)


function execute(packetID, operateID, buffer)

	local cgmsg = msg_julebu_pb.cgjulebudayhistory()
	local gcmsg = msg_julebu_pb.gcjulebudayhistory()
	
	cgmsg:ParseFromString(buffer)
	
	--获取战绩列表
	gcmsg.result = GetRecordList(cgmsg, gcmsg)
	
	gcmsg.result = 0
	return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
end

--获取个人战绩列表
function GetRecordList(cgmsg, gcmsg)
	gcmsg.result = 0
	
	local recordList = JulebuModel.GetJulebuModelRecordList(cgmsg.julebuid, cgmsg.userid)
	
	for k,v in ipairs(recordList) do
		local recordInfo = ZZPhzModel.GetPlayRecordInfo(v)
		if recordInfo ~= nil then
			local addItem = gcmsg.recordList:add()
			addItem.tableID = recordInfo.tableID
			addItem.gameType = recordInfo.gameType
			addItem.startDate = recordInfo.startDate
			addItem.needUsersNum = recordInfo.needUsersNum
			addItem.maxJetton = recordInfo.maxJetton
			addItem.kingNum = recordInfo.kingNum
			addItem.genXing = recordInfo.genXing
			addItem.huPai = recordInfo.huPai
			addItem.yiWuShi = recordInfo.yiWuShi
			addItem.totalTurns = recordInfo.totalTurns
			addItem.currTurns = recordInfo.currTurns
			addItem.dianPao = recordInfo.dianPao
			addItem.hongHei = recordInfo.hongHei
			addItem.ziMoDouble = recordInfo.ziMoDouble
			addItem.tunPerHuxi = recordInfo.tunPerHuxi
			addItem.shuangXing = recordInfo.shuangXing
			addItem.daNiao = recordInfo.daNiao
			addItem.hongHeiDian = recordInfo.hongHeiDian
			
			for m,n in ipairs(recordInfo.userList) do
				local addUser = addItem.userList:add()
				addUser.userID = n.userID
				addUser.nick = n.nick
			end
			
			for m,n in ipairs(recordInfo.simpleInfo) do
				local addInfo = addItem.simpleInfo:add()
				addInfo.recordID = n.recordID
				addInfo.playDate = n.playDate
				
				for x,y in ipairs(n.score) do
					addInfo.score:append(y)
				end
			end
			
			for _, v in ipairs(recordInfo.totalScore) do
				addItem.totalScore:append(v)
			end
		end
	end
	
	return gcmsg.result
end