--进入小马快跑


module("PszCreate", package.seeall)

function execute(packetID, operateID, buffer)

	local cgmsg = msg_psz_pb.cgpszcreate()
	local gcmsg = msg_psz_pb.gcpszjoinroom()
	
	cgmsg:ParseFromString(buffer)
	--print(cgmsg)
	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "psz", "PszCreate", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	
	if pInfo == nil then
		LogBehavior.Error(cgmsg.userid,"psz","PszCreate", ReturnCode["player_not_exist"], "该玩家不存在")
		gcmsg.result = ReturnCode["player_not_exist"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
	end
	
	if false == PszEvent.CheckJiFen(cgmsg.julebuid, cgmsg.userid) then
		--低于50就不能进入房间了
		gcmsg.result = ReturnCode["jlb_create_error"]
		return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()				
	end
	
	local tableided = PszModel.GetUserTableID(cgmsg.userid)
	
	--玩家已经在房间里面，把玩家拉进房间
	if tableided ~= nil then
		PszModel.GetTableInfo(tableided, gcmsg.hinfo)
		--房间内容为空时，把玩家提出房间
		if gcmsg.hinfo == nil or gcmsg.hinfo == 0 then		
			gcmsg.result = ReturnCode["psz_create_shibai"]
			PszModel.DelUserTableID(cgmsg.userid)
			return cgmsg.userid, 0 , gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		for i = 1, #gcmsg.hinfo.situser do	
			local tempInfo = PlayerModel.GetPlayerInfo(gcmsg.hinfo.situser[i].userid)
			local addItem = gcmsg.pslist:add()
			PlayerModel.ChangeSimpleInfo(tempInfo, addItem)
			addItem.jifen = gcmsg.hinfo.situser[i].jifen
			if gcmsg.hinfo.situser[i].pokerstate == g_PszPokerStatus.poker_lookno then
				gcmsg.hinfo.situser[i].psztype = 0
				while #gcmsg.hinfo.situser[i].handpoker > 0 do
					gcmsg.hinfo.situser[i].handpoker:remove(1) 
				end
			end
		end
		gcmsg.result = 0
		SendMessage(cgmsg.userid, PacketCode[3310].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
	--玩家没在房间里面
	else 
		--玩家同时创建的房间不能超过10个
		local getList = PszModel.GetUserTableList(cgmsg.userid)
		local mun = 0
		for k,v in pairs(getList) do
			mun = mun + 1
		end
		if mun == 10 then
			gcmsg.result = ReturnCode["psz_create_exceed"]
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		--房主付费扣钻石
		if cgmsg.payway == 1 then
			if false == PszService.FeeDeductions(pInfo,cgmsg.paynum,cgmsg.paytype) then	
				gcmsg.result = ReturnCode["psz_money_short"]
				return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()	
			end
		end
		
		--随机获得房间ID
		gcmsg.hinfo.tableid = PszModel.GetRandomID() 
		ThreadManager.PszLock(gcmsg.hinfo.tableid )
		
		if gcmsg.hinfo.tableid == 0 then
			gcmsg.result = ReturnCode["psz_create_shibai"]
			ThreadManager.PszUnLock(gcmsg.hinfo.tableid)
			LogBehavior.Error(cgmsg.userid,"psz","PszCreate", ReturnCode["psz_create_shibai"], "get tableID is 0")
			return cgmsg.userid, 0, gcmsg:ByteSize(), gcmsg:SerializeToString()
		end
		
		--给房间信息赋值
		--print("difen=="..cgmsg.difen)
		
		gcmsg.hinfo.maxuser = cgmsg.maxuser --g_PszDefine.max_user
		gcmsg.hinfo.paytype = cgmsg.paytype
		gcmsg.hinfo.difen = cgmsg.difen
		gcmsg.hinfo.state =  g_PszDefine.state_ready
		gcmsg.hinfo.timemark = g_PszTime.ready_time
		gcmsg.hinfo.maxplaynum = cgmsg.maxplaynum  --玩的局数
		gcmsg.hinfo.payway = cgmsg.payway
		gcmsg.hinfo.paynum = cgmsg.paynum
		gcmsg.hinfo.doublecomp = cgmsg.doublecomp
		gcmsg.hinfo.boutcapp = cgmsg.boutcapp
		gcmsg.hinfo.mincomp = cgmsg.mincomp
		gcmsg.hinfo.menpai = cgmsg.menpai
		gcmsg.hinfo.psztype = cgmsg.psztype
		gcmsg.hinfo.tongpai = cgmsg.tongpai    --同牌输赢规则
		gcmsg.hinfo.tonghua = cgmsg.tonghua    --同花收喜规则
		gcmsg.hinfo.baozi = cgmsg.baozi        --豹子收喜规则
		gcmsg.hinfo.qipai = cgmsg.qipai        --弃牌规则
		gcmsg.hinfo.JQK = cgmsg.JQK            --123>JQk
		gcmsg.hinfo.chibaozi = cgmsg.chibaozi  --235吃豹子规则
		gcmsg.hinfo.kanpai = cgmsg.kanpai      --顺序看牌规则
		gcmsg.hinfo.shunzi = cgmsg.shunzi      --顺子是否大于金花
		gcmsg.hinfo.jifen = cgmsg.jifen        --新规则底分
		--gcmsg.hinfo.multiple = cgmsg.multiple  --下注翻倍用的
		--gcmsg.hinfo.autoready = cgmsg.autoready 
		
		gcmsg.hinfo.ownerid = cgmsg.userid    --房主的ID
		gcmsg.hinfo.usernum = 0
		gcmsg.hinfo.standuser:append(cgmsg.userid)
		gcmsg.hinfo.tabletype = g_PszDefine.table_srf   --私人房

		gcmsg.hinfo.restrict = cgmsg.restrict     --限制，游戏开始后能不能进入房间
		gcmsg.hinfo.autoready = g_PszDefine.autoready_OK
		gcmsg.hinfo.julebuid = cgmsg.julebuid 
		gcmsg.hinfo.julebutype = 0
		gcmsg.strvipinfo = cgmsg.strvipinfo
		
		--aa查看钻石够不够
		if false == PszService.Pszcheck(gcmsg.hinfo, pInfo) then 
			gcmsg.result = ReturnCode["psz_money_short"]
			ThreadManager.PszUnLock(gcmsg.hinfo.tableid)
			return cgmsg.userid, 0, gcmsg:ByteSize(),gcmsg:SerializeToString()
		end
		
		--把玩家加入房间
		PszModel.SetUserTableID(cgmsg.userid, gcmsg.hinfo.tableid)
		--把房间加入玩家的房间列表
		PszModel.SetUserTableList(cgmsg.userid, gcmsg.hinfo.tableid)
		
		redisItem:hset(PszModel.table_list, gcmsg.hinfo.tableid, gcmsg.hinfo.usernum,PszModel.redis_index)
		
		--给玩家发创建房间成功的协议
		local cgmsgc = msg_psz_pb.gcpszcreate()
		cgmsgc.tableid = gcmsg.hinfo.tableid 
		cgmsgc.hinfo:ParseFromString(gcmsg.hinfo:SerializeToString())
		cgmsgc.result = 0
		SendMessage(cgmsg.userid, PacketCode[3304].client, cgmsgc:ByteSize(), cgmsgc:SerializeToString())

		gcmsg.result = 0
		--给玩家发生进入房间的协议
		SendMessage(cgmsg.userid, PacketCode[3310].client, gcmsg:ByteSize(), gcmsg:SerializeToString())
		--把房间加入俱乐部
		PszEvent.JulebuGameCreate(gcmsg.hinfo)
		
		--在自动准备的情况下
		if gcmsg.hinfo.autoready == g_PszDefine.autoready_OK  then
			----把玩家加入坐下玩家列表
			PszSitdown.UserSitdown(gcmsg.hinfo, pInfo)
			gcmsg.hinfo.usernum = gcmsg.hinfo.usernum  + 1
		end		
		--进入房间默认自动准备
		for k,v in ipairs(gcmsg.hinfo.situser) do 
			if cgmsg.userid == v.userid then
				local gcmsgNext = msg_psz_pb.gcpsznext()
				gcmsgNext.chairid = v.chairid
				gcmsgNext.userid = cgmsg.userid
				gcmsgNext.result = 0
				SendMessage(cgmsg.userid, PacketCode[3326].client, gcmsgNext:ByteSize(), gcmsgNext:SerializeToString())
				break
			end
		end
	end	
	
	ThreadManager.PszUnLock(gcmsg.hinfo.tableid)
	PszModel.SetTableInfo(gcmsg.hinfo,1)
	return cgmsg.userid, 0 , 0, gcmsg:SerializeToString()
end
