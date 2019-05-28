--进入小马快跑


module("DouNiuCreate", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_douniu_2_pb.cgdouniucreate()
	local gcmsg = msg_douniu_pb.gcdouniujoinroom()
	
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "douniu", "DouNiuCreate", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuCreate", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	--玩家同时创建的房间不能超过10个
	local getList = DouNiuModel.GetUserTableList(cgmsg.userid)
	local mun = 0
	for k,v in pairs(getList) do
		mun = mun + 1
	end
	if mun == 300 then
		gcmsg.result = ReturnCode["douniu_create_exceed"]
		ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	--房主付费扣钻石
	if cgmsg.payway == 1 then
		if false == DouNiuService.FeeDeductions(pInfo,cgmsg.paynum,cgmsg.paytype) then
			gcmsg.result = ReturnCode["douniu_money_short"]
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()	
		end
	end
	
	--随机获得房间ID
	gcmsg.hinfo.tableid = DouNiuModel.GetRandomID() 
	ThreadManager.DouNiuLock(gcmsg.hinfo.tableid)
	if gcmsg.hinfo.tableid == 0 then
		gcmsg.result = ReturnCode["douniu_create_shibai"]
		ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)
		LogBehavior.Error(cgmsg.userid,"douniu","DouNiuCreate", ReturnCode["douniu_create_shibai"], "get tableID is 0")
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end	
	
	--给房间信息赋值
	--print("cgmsg.difen"..cgmsg.difen)
	gcmsg.hinfo.paytype = cgmsg.paytype
	gcmsg.hinfo.bankermultiple = cgmsg.bankermultiple
	gcmsg.hinfo.frameid = 0
	gcmsg.hinfo.viptable = 1
	gcmsg.hinfo.dntype = cgmsg.dntype
	gcmsg.hinfo.difen = cgmsg.difen
	gcmsg.hinfo.state =  g_douNiuDefine.state_ready
	gcmsg.hinfo.timemark = g_douNiuTime.ready_time
	gcmsg.hinfo.maxplaynum = cgmsg.maxplaynum  --玩的局数
	gcmsg.hinfo.ownerid = cgmsg.userid    --房主的ID
	gcmsg.hinfo.usernum = 0
	gcmsg.hinfo.standuser:append(cgmsg.userid)
	gcmsg.hinfo.tabletype = g_douNiuDefine.table_srf   --私人房
	gcmsg.hinfo.payway = cgmsg.payway
	gcmsg.hinfo.autoready = g_douNiuDefine.autoready_NO --cgmsg.autoready 
	gcmsg.hinfo.integralmax = 0		--cgmsg.integralmax      --携带上限
	gcmsg.hinfo.typeodds = cgmsg.typeodds 
	gcmsg.hinfo.specialtype = cgmsg.strvipinfo
	gcmsg.hinfo.paynum = cgmsg.paynum
	gcmsg.hinfo.usermaxnum = cgmsg.maxuser
	gcmsg.hinfo.autoStart = cgmsg.maxuser --cgmsg.autoStart
	gcmsg.hinfo.TZmax = cgmsg.TZmax
	gcmsg.hinfo.julebuid = cgmsg.julebuid
	gcmsg.hinfo.restrict = cgmsg.restrict
	gcmsg.hinfo.banrubcard = 0			--禁止搓牌
	gcmsg.hinfo.playerbuycode = cgmsg.playerbuycode 	--闲家买码
	gcmsg.hinfo.pourrestrict = cgmsg.pourrestrict 		--下注限制
	gcmsg.hinfo.kongwildcard = cgmsg.kongwildcard 		--王牌癞子
	gcmsg.hinfo.pourdouble = cgmsg.pourdouble 			--下注加倍
	gcmsg.hinfo.julebutype = cgmsg.julebutype
	--aa查看钻石够不够
	if false == DouNiuService.DouNiucheck(gcmsg.hinfo, pInfo) then 
		gcmsg.result = ReturnCode["douniu_money_short"]
		ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)
		return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	--在自动准备的情况下
	if gcmsg.hinfo.autoready == g_douNiuDefine.autoready_OK  then
		--把玩家加入坐下列表
		local chairidList = {0,0,0,0,0,0,0,0,0,0}
		for k,v in ipairs(gcmsg.hinfo.situser) do
			chairidList[v.chairid] = 1
		end
		local addItem = gcmsg.hinfo.situser:add()
		for k,v in ipairs(chairidList) do
			if v == 0 then
				addItem.chairid = k
				break
			end
		end				
		addItem.userid = cgmsg.userid
		addItem.state = g_douniuPlayStatus.status_ready
		gcmsg.hinfo.usernum = gcmsg.hinfo.usernum + 1

		if gcmsg.hinfo.dntype == g_douNiuType.type_gdzj and cgmsg.userid == gcmsg.hinfo.ownerid then
			addItem.carryjetton = gcmsg.hinfo.integralmax
		else
			addItem.carryjetton = 0
		end
		
		--把玩家从站立玩家列表删除
		for k,v in ipairs(gcmsg.hinfo.standuser) do
			if v == cgmsg.userid then
				gcmsg.hinfo.standuser:remove(k)
				break
			end
		end
	end	
	--把玩家加入房间
	DouNiuModel.SetUserTableID(cgmsg.userid, gcmsg.hinfo.tableid)
	--把房间加入玩家的房间列表
	DouNiuModel.SetUserTableList(cgmsg.userid, gcmsg.hinfo.tableid)
	redisItem:hset(DouNiuModel.table_list, gcmsg.hinfo.tableid, gcmsg.hinfo.usernum,DouNiuModel.redis_index)
	--加入俱乐部
	DouNiuEvent.JulebuGameCreate(gcmsg.hinfo)
	
	DouNiuModel.SetTableInfo(gcmsg.hinfo,1)
	ThreadManager.DouNiuUnLock(gcmsg.hinfo.tableid)

	for k,v in ipairs(gcmsg.hinfo.situser)do 
		local pInfosit = PlayerModel.GetPlayerInfo(v.userid)
		local addPInfo = gcmsg.pslist:add()
		PlayerModel.ChangeSimpleInfo(pInfo,addPInfo)
	end
	gcmsg.strvipinfo = gcmsg.hinfo.specialtype
	gcmsg.result = 0
	--给玩家发生进入房间的协议
	SendMessage(cgmsg.userid, PacketCode[3010].client, gcmsg:ByteSize(), gcmsg:SerializeToString())	
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end
