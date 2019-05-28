
GdmjStruct = class("GdmjStruct")

--推倒胡的结构体

function GdmjStruct:ctor(maxUser)
	self.m_maxUser = maxUser   --最多会有四个玩家
	self.m_tInfo = nil --st_gdmj_pb.gdmjinfo()
	self.m_userList = {}
	self.m_userModify = {} --哪个玩家被修改了的列表
	self.m_detailList = {}    --统计玩家在牌局的数
	self.m_nextInfo = st_gdmj_pb.gdmjnextaction()  --指示下一个行动的，在PlayAction中使用到
	for i = 1,self.m_maxUser do   --一开始的时候，初始化4个座位的玩家
		self.m_userList[i] = 0 --st_gdmj_pb.gdmjuser()   这里不用给他初始化
		self.m_detailList[i] = 0
		self.m_userModify[i] = 0
		self.m_nextInfo.actchairid:append(0)
	end
	
	--self.m_nextAction = st_gdmj_pb.gdmjnextaction()
	self.m_vipRoomInfo = nil --= st_gdmj_pb.gdmjtdh()   --个性化的信息
	self.m_isModify = false    --如果该标志是true，表示需要更新到缓存中
	
end

function GdmjStruct:SetInfo(tInfo)
	--self.m_tInfo:ParseFromString(strInfo)
	self.m_tInfo = tInfo
	self.m_tableID = tInfo.tableid  --把一个tableID单独放出来，方便查看



	if self.m_tInfo.mjtype == g_gdmjType.type_tdh then
		self.m_vipRoomInfo = st_gdmj_pb.gdmjtdh()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end
	elseif self.m_tInfo.mjtype == g_gdmjType.type_ztdh then
		self.m_vipRoomInfo = st_gdmj_pb.gdmjztdh()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end	
	elseif self.m_tInfo.mjtype == g_gdmjType.type_jph then
		self.m_vipRoomInfo = st_gdmj_pb.gdmjjph()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end	
	elseif self.m_tInfo.mjtype == g_gdmjType.type_hzz or self.m_tInfo.mjtype == g_gdmjType.type_hdz then
		self.m_vipRoomInfo = st_gdmj_pb.gdmjhzz()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end
	elseif self.m_tInfo.mjtype == g_gdmjType.type_hbz then
		self.m_vipRoomInfo = st_gdmj_pb2.gdmjhbz()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end	
	elseif self.m_tInfo.mjtype == g_gdmjType.type_hzmj then
		self.m_vipRoomInfo = st_gdmj2_pb.gdmjhzmj()
		local strGet = GdmjModel.GetRoomInfoStr(self.m_tInfo.tableid)
		if strGet ~= nil then
			self.m_vipRoomInfo:ParseFromString(strGet)
		else
			LogFile("error", "get vip room info error tableid="..self.m_tInfo.tableid)
		end			
	end

	
end

function GdmjStruct:SetUser(strInfo,pos)
	if pos < 1 or pos > self.m_maxUser then
		LogFile("error","GdmjStruct SetUser pos="..pos)
	end
	self.m_userList[pos]:ParseFromString(strInfo)
end

function GdmjStruct:UpdateFromRedis() --把所以玩家的属性更新到缓存中
	--稍后实现该结构
	if true == GdmjModel.isModify(self.m_tableID) then
		--表示需要刷新的
		local tInfo = GdmjModel.GetTableInfo(self.m_tableID)
		self.m_tInfo = tInfo
		
		for k,v in ipairs(self.m_tInfo.situser) do
			if v ~= 0 then
				local mjUser = GdmjModel.GetChairUser(self.m_tableID,v)
				if mjUser ~= nil then
					self.m_userList[k] = mjUser     --同时也更新这个数据
				end
				local userDtail = GdmjModel.GetPlayDetail(self.m_tableID, v)
				if userDtail ~= nil then
					self.m_detailList[k] = userDtail
				end
			end
		end
	end
end

function GdmjStruct:SetRedis()
	if self.m_isModify == true then
		self.m_isModify = false
		GdmjModel.SetTableInfo(self.m_tInfo,1)
		
		--暂时先放这里

		for i = 1,self.m_maxUser do
			if self.m_userModify[i] ~= 0 then
				GdmjModel.SetChairUser(self.m_tableID, self.m_userList[i]) --同时检查有那个玩家的数据更新的
				self.m_userModify[i] = 0
				GdmjModel.SetPlayDetail(self.m_tableID, self.m_userList[i].userid, self.m_detailList[i])
			end
		end		
		
	end	
	

end

function GdmjStruct:GetVipInfo()
	

	
end


return GdmjStruct


