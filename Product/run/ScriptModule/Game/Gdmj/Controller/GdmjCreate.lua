
module("GdmjCreate", package.seeall)

--麻将创建桌子
--每个晚上手上，最多只能有8个空的房子。

function execute(packetID, operateID, buffer)

	local cgmsg = msg_gdmj_pb.cggdmjcreate()
	local gcmsg = msg_gdmj_pb.gcgdmjcreate()
	
	cgmsg:ParseFromString(buffer)
	
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "gdmj", "GdmjCreate", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"gdmj","GdmjCreate", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end

	
	if false == GdmjEvent.CheckJiFen(cgmsg.julebuid, cgmsg.userid) then
		--低于50就不能进入房间了
		gcmsg.result = ReturnCode["jlb_create_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
	end
	
	if cgmsg.payway == 1 then
		if cgmsg.paytype == g_gdmjDefine.pay_gold then
			--掌上币不够
			if pInfo.gold < cgmsg.paynum then
				--掌上币不够
				gcmsg.result = ReturnCode["gdmj_create_fail"]
				return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()
			end
			PlayerModel.DecGold(pInfo, cgmsg.paynum, "gdmj", "create room")
			PlayerModel.SendPlayerInfo(pInfo,{"gold"})
		else
			if pInfo.money < cgmsg.paynum then
				gcmsg.result = ReturnCode["money_not_enough"]
				return cgmsg.userid, gcmsg.result, gcmsg:ByteSize(), gcmsg:SerializeToString()				
			end
			PlayerModel.DecMoney(pInfo, cgmsg.paynum, "gdmj", "create room")
			PlayerModel.SendPlayerInfo(pInfo,{"money"})
		end

	end
	cgmsg.playernum = cgmsg.playernum == 0 and 4 or cgmsg.playernum
	
	local gcEnter = msg_gdmj_pb.gcgdmjenter()
	--在这里要先扣除房卡
	
	tableID = GdmjModel.GetRandomID()
	ThreadManager.GdmjLock(tableID)

	gcEnter.mjinfo.tableid = tableID
	gcEnter.mjinfo.mjtype = cgmsg.mjtype
	gcEnter.mjinfo.tableid = tableID
	gcEnter.mjinfo.maxvipnum = cgmsg.playnum
	gcEnter.mjinfo.pourjetton = 1
	gcEnter.mjinfo.viptable = 1    --设置成vip桌子
	gcEnter.mjinfo.tableuserid = cgmsg.userid
	gcEnter.mjinfo.status = g_gdmjStatus.status_ready
	gcEnter.mjinfo.maxplayernum = cgmsg.playernum
	gcEnter.mjinfo.fengid = g_gdmjFengQuan[1]  --初始化设定为东风东局
	gcEnter.mjinfo.timemark = g_gdmjTime.ready_time
	gcEnter.mjinfo.tableuserid = pInfo.userid
	gcEnter.mjinfo.paytype = cgmsg.paytype
	gcEnter.mjinfo.paynum = cgmsg.paynum
	gcEnter.mjinfo.payway = cgmsg.payway
	gcEnter.mjinfo.julebuid = cgmsg.julebuid
	--gcEnter.mjinfo.playernum = 0
	for i = 1,cgmsg.playernum do    --先暂时在这里设置
		gcEnter.mjinfo.nextinfo.actchairid:append(0)   --初始化四个玩家
		gcEnter.mjinfo.viprecord.score:append(0)
		gcEnter.mjinfo.situser:append(0)    --广东麻将有四个人的，把四个人的顺序都初始化出来
	end
	
		
	--在这里房主还没有做下去的，同时，增加房主的房间列表
	--gcmsg
	
	--这里先让玩家坐下，后面创建房间的时候，不能够坐下
	
	local mjUser = gcEnter.userlist:add()   --st_gdmj_pb.gdmjuser()
	
	mjUser.userid = cgmsg.userid
	
	PlayerModel.ChangeSimpleInfo(pInfo,mjUser.psinfo)
	
	--GdmjModel.SitDown(gcEnter.mjinfo, mjUser,1)  --这里不能坐下来
	--
	gcEnter.mjinfo.standuser:append(cgmsg.userid)
	
	GdmjModel.SetUserTableList(cgmsg.userid, tableID)  --加入到玩家的列表中
	GdmjModel.SetUserTableID(cgmsg.userid, tableID)    --自己肯定进入到那个房间了
	
	GdmjModel.SetTableInfo(gcEnter.mjinfo,1)
	
	GdmjModel.SetVipRoomByStr(tableID, cgmsg.strvipinfo) --同时需要设置牌桌信息
	
	ThreadManager.GdmjUnLock(tableID)
	GdmjEvent.JulebuGameCreate(gcEnter.mjinfo, cgmsg.strvipinfo)
	gcEnter.strvipinfo = cgmsg.strvipinfo
	gcEnter.result = 0
	
	SendMessage(cgmsg.userid, PacketCode[2202].client,gcEnter:ByteSize(), gcEnter:SerializeToString())
	PlayerModel.SetPlayerInfo(pInfo)
	gcmsg.result = 0
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end

