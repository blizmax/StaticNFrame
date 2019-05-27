
GdmjService = {}

GdmjService.tableList = {}
GdmjService.list_tdh = {}
GdmjService.dismissList = {}   --这个是的需要删除的类别
function GdmjService.Init()
	--初始化函数，每次进来的时候，检查缓存中是否有牌桌
	local IDList = GdmjModel.GetTableList()
	for k,v in ipairs(IDList) do
		--local tItem = require("Gdmj.Struct.")
		local addItem = GdmjService.CreateItem(v)
		if addItem ~= nil then
			GdmjService.tableList[tonumber(v)] = addItem
			table.insert(GdmjService.list_tdh,tonumber(v))
		end
	end
end

function GdmjService.CreateItem(tableID)
	local tInfo = GdmjModel.GetTableInfo(tableID)
	if tInfo == nil then
		return nil
	end
	local userList = {}
	for k,v in ipairs(tInfo.situser) do
		if v ~= 0 then
			local addUser = GdmjModel.GetChairUser(tableID,v)
			table.insert(userList, addUser)
		end
	end
	
	local addItem = GdmjStruct.new(#tInfo.situser)
	
	addItem:SetInfo(tInfo)
	--同时把玩家信息更新
	for k,v in ipairs(userList) do
		addItem.m_userList[v.chairid] = v  --把玩家的信息也加载到结构体中
		local playDetail = GdmjModel.GetPlayDetail(tInfo.tableid, v.userid)
		addItem.m_detailList[v.chairid] = playDetail
	end
	return addItem

end



function GdmjService.ServerLoop()
	
	for k,v in ipairs(GdmjService.list_tdh) do
		
		ThreadManager.GdmjLock(v)
		
		GdmjService.tableList[v]:UpdateFromRedis()  --检查从缓存总更新

		if GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_tdh then
			TdhService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_ztdh then
			ZptdhService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_jph then
			JphService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_jbc then
			JbcService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_hzz  or GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_hdz then
			HzzmjService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_hbz then
			HbzmjService.DoWork(GdmjService.tableList[v])
		elseif GdmjService.tableList[v].m_tInfo.mjtype == g_gdmjType.type_hzmj then
			HzmjService.DoWork(GdmjService.tableList[v])
		end
		
		GdmjService.tableList[v]:SetRedis()         --把更新设置到缓存中
		
		ThreadManager.GdmjUnLock(v)
	end
	
	
	
	local newList = GdmjModel.GetNewTableList()
	
	for k,v in ipairs(newList) do
		ThreadManager.GdmjLock(v)
		
		local addItem = GdmjService.CreateItem(v)
		if addItem ~= nil then
			if GdmjService.tableList[tonumber(v)] == nil then
				table.insert(GdmjService.list_tdh,tonumber(v))
			end
			GdmjService.tableList[tonumber(v)] = addItem
			
		end
		GdmjModel.DelModify(v)  --操作完成的时候，把这个删除
		ThreadManager.GdmjUnLock(v)		
		
	end
	
	for i = #GdmjService.dismissList,1,-1 do
		--从后面往前面删除
		--1.删除牌桌信息
		--2.删除牌桌个性化信息
		--3.删除牌桌玩家数据
		--4.删除玩家恢复ID
		--5.在房主手中删除牌桌ID
		--把这个牌桌从房主手中删除
		
		if GdmjService.tableList[GdmjService.dismissList[i]] ~= nil then
			GdmjModel.DelUserTableList(GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.tableuserid, GdmjService.dismissList[i])
			
			for k1,v1 in ipairs(GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.situser) do
				--从恢复的牌桌列表中删除
				GdmjModel.DelUserTableID(v1)
			end
			for k1,v1 in ipairs(GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.standuser) do
				GdmjModel.DelUserTableID(v1)
			end
			GdmjEvent.JulebuGameEnd(GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo)
			if GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.viptable == 0 then
				GdmjModel.DelJCBTableID(GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.mjtype,GdmjService.tableList[GdmjService.dismissList[i]].m_tInfo.tabletype,GdmjService.dismissList[i])
			end
		end
		GdmjModel.DelChairUser(GdmjService.dismissList[i])
		GdmjModel.DelRoomInfo(GdmjService.dismissList[i])
		GdmjModel.DelTableInfo(GdmjService.dismissList[i])   --在这里做清空删除的操作
		GdmjModel.DelPlayDetail(GdmjService.dismissList[i])
		GdmjModel.DelModify(GdmjService.dismissList[i])
		--table.remove(GdmjService.tableList,GdmjService.dismissList[i])  --同时在这里也删除
		GdmjService.tableList[ GdmjService.dismissList[i] ] = nil
		
		--在这里做删除的操作
		for k,v in ipairs(GdmjService.list_tdh) do
			if v == GdmjService.dismissList[i] then
				table.remove(GdmjService.list_tdh, k)
				break
			end
		end
		
		table.remove(GdmjService.dismissList, i)
		--暂时先到处理
	end
end

function GdmjService.PlayerAction(tItem,cgmsg,gcmsg)
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh then
		TdhService.PlayingAction(tItem,cgmsg,gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		ZptdhService.PlayingAction(tItem, cgmsg, gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jph then
		JphService.PlayingAction(tItem, cgmsg, gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
		JbcService.PlayingAction(tItem, cgmsg, gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
		HzzmjService.PlayingAction(tItem, cgmsg, gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hbz then
		HbzmjService.PlayingAction(tItem, cgmsg, gcmsg)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzmj then
		HzmjService.PlayingAction(tItem, cgmsg, gcmsg)
	end
	
end


function GdmjService.CheckMoving(tItem, isLog)
	
	if tItem.m_tInfo.mjtype == g_gdmjType.type_tdh then
		TdhService.CheckMoving(tItem)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		ZptdhService.CheckMoving(tItem)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jph then
		JphService.CheckMoving(tItem)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_jbc then
		JbcService.CheckMoving(tItem)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzz or tItem.m_tInfo.mjtype == g_gdmjType.type_hdz then
		HzzmjService.CheckMoving(tItem, isLog)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hbz then
		HbzmjService.CheckMoving(tItem, isLog)
	elseif tItem.m_tInfo.mjtype == g_gdmjType.type_hzmj then
		HzmjService.CheckMoving(tItem, isLog)
	end	
	
end

function GdmjService.PlayCountDissovle(tItem)
	
end