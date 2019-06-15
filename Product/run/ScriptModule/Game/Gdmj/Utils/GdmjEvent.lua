
--该类是通用与

GdmjEvent = {}

--这是用来和第三方模块通信的接口



function GdmjEvent.JulebuGameCreate(m_tInfo, strVipInfo)
	--开始配置
	--
	
	if m_tInfo.julebuid == 0 then
		return
	end
	local strDes = ""
	if m_tInfo.mjtype == g_gdmjType.type_tdh then
		strDes = "推倒胡,"
	elseif m_tInfo.mjtype == g_gdmjType.type_ztdh then
		strDes = "做推倒胡,"
	elseif m_tInfo.mjtype == g_gdmjType.type_jph then
		strDes = "鸡平胡,"
	elseif m_tInfo.mjtype == g_gdmjType.type_hzz  then
		strDes = "惠州庄,"
	elseif m_tInfo.mjtype == g_gdmjType.type_hdz then
		strDes = "惠东庄"
	elseif m_tInfo.mjtype == g_gdmjType.type_hbz then
		strDes = "惠博庄"
	elseif m_tInfo.mjtype == g_gdmjType.type_hzmj then
		strDes = "红中麻将"
	elseif m_tInfo.mjtype == g_gdmjType.type_rpmj then
		strDes = "绕平客家麻将"
	end
		
	strDes = strDes..m_tInfo.maxvipnum.."局"
	if m_tInfo.payway == 2 then
		strDes = strDes..",AA付费"
	end

	local m_vipRoomInfo = {}
	
	if m_tInfo.mjtype == g_gdmjType.type_tdh then
		m_vipRoomInfo = st_gdmj_pb.gdmjtdh()
		m_vipRoomInfo:ParseFromString(strVipInfo)
		
		if m_vipRoomInfo.typegui == 1 then
			strDes = strDes..",无鬼"
		elseif m_vipRoomInfo.typegui == 2 then
			strDes = strDes..",白板做鬼"
		elseif m_vipRoomInfo.typegui == 3 then
			strDes = strDes..",翻鬼"
		end
		if m_vipRoomInfo.manum > 0 then
			strDes = strDes..","..m_vipRoomInfo.manum.."马"
		end
		if m_vipRoomInfo.kehuqidui ~= 0 then
			strDes = strDes..",可胡七对"
		end
		if m_vipRoomInfo.qiduijiafan ~= 0 then
			strDes = strDes..",七对加番"
		end
		if m_vipRoomInfo.keqiangganghu ~= 0 then
			strDes = strDes..",可抢杠胡"
		end
		if m_vipRoomInfo.minggangkeqiang ~= 0 then
			strDes = strDes..",明杠可抢"
		end
		if m_vipRoomInfo.qianggangquanbao ~= 0 then
			strDes = strDes..",抢杠全包"
		end
		if m_vipRoomInfo.gangbaoquanbao ~= 0 then
			strDes = strDes..",杠爆全包"
		end
		if m_vipRoomInfo.wuguijiabei ~= 0 then
			strDes = strDes..",无鬼加倍"
		end
		if m_vipRoomInfo.budaifeng ~= 0 then
			strDes = strDes..",不带风"
		end
		if m_vipRoomInfo.gengzhuang ~= 0 then
			strDes = strDes..",跟庄"
		end			
		if m_vipRoomInfo.jiejiegao ~= 0 then
			strDes = strDes..",节节高"
		end	
	elseif m_tInfo.mjtype == g_gdmjType.type_ztdh then
		m_vipRoomInfo = st_gdmj_pb.gdmjztdh()
		m_vipRoomInfo:ParseFromString(strVipInfo)		
		if m_vipRoomInfo.typegui == 1 then
			strDes = strDes..",无鬼"
		elseif m_vipRoomInfo.typegui == 2 then
			strDes = strDes..",白板做鬼"
		elseif m_vipRoomInfo.typegui == 3 then
			strDes = strDes..",翻鬼"
		end
		if m_vipRoomInfo.shuanggui ~= 0 then
			strDes = strDes..",双鬼"
		end
		if m_vipRoomInfo.manum > 0 then
			strDes = strDes..","..m_vipRoomInfo.manum.."马"
		end
		if m_vipRoomInfo.kechihu ~= 0 then
			strDes = strDes..",可吃胡"
		end		
		if m_vipRoomInfo.kehuqidui ~= 0 then
			strDes = strDes..",可胡七对"
		end

		if m_vipRoomInfo.keqiangganghu ~= 0 then
			strDes = strDes..",可抢杠胡"
		end
		if m_vipRoomInfo.minggangkeqiang ~= 0 then
			strDes = strDes..",明杠可抢"
		end
		if m_vipRoomInfo.qianggangquanbao ~= 0 then
			strDes = strDes..",抢杠全包"
		end
		if m_vipRoomInfo.gangbaoquanbao ~= 0 then
			strDes = strDes..",杠爆全包"
		end

		if m_vipRoomInfo.budaifeng ~= 0 then
			strDes = strDes..",不带风"
		end
		if m_vipRoomInfo.gengzhuang ~= 0 then
			strDes = strDes..",跟庄"
		end			
		if m_vipRoomInfo.siguihupai ~= 0 then
			strDes = strDes..",四鬼胡牌"
		end
		if m_vipRoomInfo.qiduisibei ~= 0 then
			strDes = strDes..",七对四倍"
		end
		if m_vipRoomInfo.pengpenghu ~= 0 then
			strDes = strDes..",碰碰胡两倍"
		end			
		if m_vipRoomInfo.qingyises ~= 0 then
			strDes = strDes..",清一色四倍"
		end
		if m_vipRoomInfo.quanfeng ~= 0 then
			strDes = strDes..",全风八倍"
		end
		if m_vipRoomInfo.shisanyao ~= 0 then
			strDes = strDes..",十三幺八倍"
		end
		if m_vipRoomInfo.yaomeyao ~= 0 then
			strDes = strDes..",幺么九6倍"
		end
		if m_vipRoomInfo.yaojiujike ~= 0 then
			strDes = strDes..",含幺九即可"
		end
		if m_vipRoomInfo.wuguijiabei ~= 0 then
			strDes = strDes..",无鬼加倍"
		end
		if m_vipRoomInfo.siguiliangbei ~= 0 then
			strDes = strDes..",四鬼两倍"
		end		
	elseif m_tInfo.mjtype == g_gdmjType.type_jph then
		m_vipRoomInfo = st_gdmj_pb.gdmjjph()
		m_vipRoomInfo:ParseFromString(strVipInfo)			
		if m_vipRoomInfo.fantype == 1 then
			strDes = strDes..",无番起胡"
		elseif m_vipRoomInfo.fantype == 2 then
			strDes = strDes..",1番起胡"
		end
		if m_vipRoomInfo.maxfan ~= 0 then
			strDes = strDes..","..m_vipRoomInfo.maxfan.."封顶"
		end
	elseif m_tInfo.mjtype == g_gdmjType.type_hzz or m_tInfo.mjtype == g_gdmjType.type_hdz then
		m_vipRoomInfo = st_gdmj_pb.gdmjhzz()
		m_vipRoomInfo:ParseFromString(strVipInfo)
		if m_vipRoomInfo.manum ~= 0 then
			strDes = strDes..","..m_vipRoomInfo.manum.."马"
		end		
		if m_vipRoomInfo.bukejihu ~= 0 then
			strDes = strDes..",不可鸡胡"
		end
		if m_vipRoomInfo.menqingjiafen ~= 0 then
			strDes = strDes..",门清加分"
		end
		if m_vipRoomInfo.genzhuang ~= 0 then
			strDes = strDes..",跟庄"
		end
		if m_vipRoomInfo.duohuahupai ~= 0 then
			strDes = strDes..",多花胡牌"
		end
		if m_vipRoomInfo.ganggenma ~= 0 then
			strDes = strDes..",杠跟马"
		end
	elseif m_tInfo.mjtype == g_gdmjType.type_hbz then
		m_vipRoomInfo = st_gdmj_pb.gdmjhzz()
		m_vipRoomInfo:ParseFromString(strVipInfo)
		if m_vipRoomInfo.manum ~= 0 then
			strDes = strDes..","..m_vipRoomInfo.manum.."马"
		end		
		if m_vipRoomInfo.bukejihu ~= 0 then
			strDes = strDes..",不可鸡胡"
		end
		if m_vipRoomInfo.menqingjiafen ~= 0 then
			strDes = strDes..",门清加分"
		end
		if m_vipRoomInfo.genzhuang ~= 0 then
			strDes = strDes..",跟庄"
		end
		if m_vipRoomInfo.duohuahupai ~= 0 then
			strDes = strDes..",多花胡牌"
		end
		if m_vipRoomInfo.ganggenma ~= 0 then
			strDes = strDes..",杠跟马"
		end		
		if m_vipRoomInfo.qgmaima ~= 0 then
			strDes = strDes..",抢杠胡买马"
		end
		if m_vipRoomInfo.bhhpjiafen ~= 0 then
			strDes = strDes..",补花胡牌加分"
		end
		if m_vipRoomInfo.kehuqidui ~= 0 then
			strDes = strDes..",可胡七对"
		end
		if m_vipRoomInfo.ganggenma ~= 0 then
			strDes = strDes..",流局算杠"
		end
	elseif m_tInfo.mjtype == g_gdmjType.type_hzmj then
		m_vipRoomInfo = st_gdmj2_pb.gdmjhzmj()
		m_vipRoomInfo:ParseFromString(strVipInfo)
		if m_vipRoomInfo.manum ~= 0 then
			if m_vipRoomInfo.manum == 99 then
				strDes = strDes..",一马全中"
			else
				strDes = strDes..","..m_vipRoomInfo.manum.."马"
			
			end
			
		end		
		if m_vipRoomInfo.wuhzjiafen ~= 0 then
			strDes = strDes..",无红中加分"
		end
		if m_vipRoomInfo.menqingjiafen ~= 0 then
			strDes = strDes..",门清加分"
		end
		if m_vipRoomInfo.dachuhzjiafen ~= 0 then
			strDes = strDes..",打出红中加分"
		end
		if m_vipRoomInfo.zhuangxianjiafen ~= 0 then
			strDes = strDes..",庄闲加分"
		end
		if m_vipRoomInfo.kehuqidui ~= 0 then
			strDes = strDes..",可胡七对"
		end		
		if m_vipRoomInfo.gskhjiama ~= 0 then
			strDes = strDes..",杠上开花加码"
		end
		if m_vipRoomInfo.qianggangquanbao ~= 0 then
			strDes = strDes..",抢杠全包"
		end
		if m_vipRoomInfo.gangbaoquanbao ~= 0 then
			strDes = strDes..",杠爆全包"
		end	
	elseif m_tInfo.mjtype == g_gdmjType.type_rpmj then
		m_vipRoomInfo = st_gdmj2_pb.gdmjrpmj()
		m_vipRoomInfo:ParseFromString(strVipInfo)
		if m_vipRoomInfo.manum ~= 0 then
			if m_vipRoomInfo.manum == 99 then
				strDes = strDes..",一马全中"
			else
				strDes = strDes..","..m_vipRoomInfo.manum.."马"
			
			end
			
		end		
		if m_vipRoomInfo.kehuqidui ~= 0 then
			strDes = strDes..",可胡七对"
		end
		if m_vipRoomInfo.qiduisibei ~= 0 then
			strDes = strDes..",豪华七对四倍"
		end			

		if m_vipRoomInfo.gangbaoquanbao ~= 0 then
			strDes = strDes..",包杠"
		end
		if m_vipRoomInfo.qingyise ~= 0 then
			strDes = strDes..",清一色"
		end	
		if m_vipRoomInfo.shisanyao ~= 0 then
			strDes = strDes..",十三幺"
		end		
	end
	JulebuService.CreateGame(g_JulebuDefine.modules_gdmj, m_tInfo.julebuid, m_tInfo.tableid, m_tInfo.maxplayernum, m_tInfo.playernum, 
							strDes, m_tInfo.julebutype, m_tInfo.mjtype, m_tInfo.situser, m_tInfo.maxvipnum, m_tInfo.usevipnum)
end

function GdmjEvent.JulebuGameEnd(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.GameEnd(tInfo.julebuid, tInfo.tableid, tInfo.julebutype, tInfo.mjtype)
end

function GdmjEvent.JulebuGameUpdate(tInfo, userid)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.GameUpdate(tInfo.julebuid, tInfo.tableid, tInfo.playernum, tInfo.julebutype, userid)
end


function GdmjEvent.JulebuGameCount(julebuID, tableID, userArr)
	if julebuID == 0 then
		return
	end
	
	local userArrTemp = {}
	userArrTemp['tableid'] = tableID
	userArrTemp['time'] = TimeUtils.GetTimeString()
	userArrTemp['userlist'] = userArr	
	userArrTemp['module'] = "麻将"

	local tInfo = GdmjModel.GetTableInfo(tableID)
	if tInfo ~= nil and tInfo.julebuid ~= 0 then
		if tInfo.julebutype == 2 then
			GdmjEvent.GameCount(julebuID, tableID, userArrTemp, g_JulebuDefine.modules_gdmj)
		else
			JulebuService.GameCount(julebuID, tableID, userArrTemp, g_JulebuDefine.modules_gdmj)
		end
	end
	
end

function GdmjEvent.GameCount(julebuID, tableID, userArr, moduleType)

	--这里需要加一个防护，防止重复计费
	--已经出现两次重复计费的
		
	if true == JulebuModel.IsCount(julebuID..tableID) then
		--如果已经被统计过了，就不用重复统计了。
		--LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, julebuID,   "recount game count julebuID="..julebuID..", tableID="..tableID)
		return
	end
	
	table.sort(userArr['userlist'], function(a,b) return a['jifen'] > b['jifen'] end)
	
	local pumpingNum = 1
	local pumpingRatio = 0
	local pumpingOrigin = 0
	local vipRoomInfo = st_gdmj2_pb.gdmjrpmj()
	
	local jInfo = JulebuModel.GetJulebuInfo(julebuID)
	--检查更新昨日数据
	local temptime = os.date("*t", os.time())
	if temptime.day ~= jInfo.updateday and temptime.hour > 7 then
		jInfo.updateday = temptime.day
		jInfo.yesterdaypump = jInfo.todaypump
		jInfo.todaypump = 0
	end
	
	local strVipInfo = GdmjModel.GetRoomInfoStr(tableID)
	if strVipInfo ~= nil then
		--设置VIPInfo，在这里设定vipInfo的字符串
		vipRoomInfo:ParseFromString(strVipInfo)

		if vipRoomInfo.faceaward_figure == 1 then
			pumpingNum = 99
		elseif vipRoomInfo.faceaward_figure == 2 then 
			pumpingNum = 1 
		elseif vipRoomInfo.faceaward_figure == 3 then 
			pumpingNum = 1 
		elseif vipRoomInfo.faceaward_figure == 4 then 
			pumpingNum = 2 
		elseif vipRoomInfo.faceaward_figure == 5 then 
			pumpingNum = 3 
		elseif vipRoomInfo.faceaward_figure == 6 then 
			pumpingNum = 4 
		end

		pumpingRatio = vipRoomInfo.faceaward_num
		pumpingOrigin = vipRoomInfo.faceaward_count
	else
		if jInfo ~= 0 then
			if jInfo.faceaward_figure == 1 then
				pumpingNum = 99
			elseif jInfo.faceaward_figure == 2 then 
				pumpingNum = 1 
			elseif jInfo.faceaward_figure == 3 then 
				pumpingNum = 1 
			elseif jInfo.faceaward_figure == 4 then 
				pumpingNum = 2 
			elseif jInfo.faceaward_figure == 5 then 
				pumpingNum = 3 
			elseif jInfo.faceaward_figure == 6 then 
				pumpingNum = 4 
			end
	
			pumpingRatio = jInfo.faceaward_num
			pumpingOrigin = jInfo.faceaward_count
		end
	end


	local tmpNum = 1
	for k,v in ipairs(userArr['userlist']) do
		local Count = v['jifen']
		if tonumber(k) == 1 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 1)
		end
		
		if Count >= 500 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 4)
		elseif Count >= 300 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 3)
		elseif Count >= 100 then
			JulebuService.RecordDayingjia(julebuID, v['userid'], 2)
		end	
		local pumpingcount = 0
		if Count > 0 and tmpNum <= pumpingNum and pumpingOrigin <= Count and pumpingRatio > 0 and pumpingRatio <= Count then
			pumpingcount = pumpingRatio
			--pumpingcount =  > Count and Count or pumpingRatio
			Count = Count - pumpingcount
			tmpNum = tmpNum + 1
			
			jInfo.todaypump = jInfo.todaypump + pumpingcount

			local jMember = JulebuModel.GetUserMemberInfo(julebuID, v['userid'])

			
			LogBehavior.Info(v['userid'], "julebu="..julebuID, "rate", pumpingcount, "tableID="..tableID)  --记录返奖的额度

			--LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, julebuID,   "userid="..v['userid'].."higherid="..jMember.higherid..", pumpingNum="..pumpingNum..", Count="..Count)
			if jMember.higherid ~= 0 then
				--说明这个玩家是被邀请进来的。需要把一半的佣金分给这个兄弟
				local num = 0.5
				if jInfo.proportion then
					if jInfo.proportion == 1 then
						num = 0.6
					elseif jInfo.proportion == 2 then
						num = 0.7
					elseif jInfo.proportion == 3 then
						num = 0.8
					end
				end
				local getJifen = math.floor(pumpingcount * num)
				pumpingcount = pumpingcount - getJifen
				JulebuModel.AddGetJiFen(julebuID, jMember.higherid, v['userid'], getJifen)
			else
				
			end
			LogBehavior.Info(v['userid'], "julebu="..julebuID, "rate", pumpingcount, "tableID="..tableID)  --记录返奖的额度
		end
		
		
		JulebuService.StatisticsPartnerRecordDay(jInfo, v['userid'], v['jifen'], tonumber(k), pumpingcount) --把这个也到合伙人里面去
		JulebuModel.AddUserJiFen(julebuID, v['userid'], Count, tableID)  --把积分同步更新]]
	end
	
	JulebuModel.SetJulebuInfo(jInfo)
	JulebuModel.SetJiFenCheck(julebuID..tableID) 
end

function GdmjEvent.JulebuGameStart(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.JulebuGameStart(tInfo.julebuid, tInfo.tableid, tInfo.julebutype)		
end

function GdmjEvent.CheckJiFen(julebuID, userID)
	local jInfo	 = JulebuModel.GetJulebuInfo(julebuID)
	if jInfo == nil then
		return 
	end
	return JulebuService.CheckJiFen(julebuID, userID, jInfo.gamecount)
end

function GdmjEvent.AddJiFen(julebuID, userID, JiFen,tableID ,julebutype)
	if julebuID == 0 or julebutype ~= 2 then
		return
	end
	--把积分同步到俱乐部中
	JulebuModel.AddUserJiFen(julebuID, userID, JiFen,tableID)
end

function GdmjEvent.IsBiSai(julebuID)
	--比赛场中，需要检查托管，所以，这里需要检查是否在比赛中
	if julebuID == 0 then
		return false
	end
	return JulebuModel.IsBiSai(julebuID)
end

function GdmjEvent.CreateGame(cgmsg, julebuowner)
	--玩家同时创建的房间不能超过10个
	local getList = GdmjModel.GetUserTableList(cgmsg.userid)
	local mun = 0
	for k,v in pairs(getList) do
		mun = mun + 1
	end
	if mun == 30 then
		return ReturnCode["pdk_create_fail"]
	end
	
	local gcmsg = msg_gdmj_pb.gcgdmjenter()
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		return
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
	gcEnter.mjinfo.julebutype = cgmsg.julebutype
	for i = 1,cgmsg.playernum do    --先暂时在这里设置
		gcEnter.mjinfo.nextinfo.actchairid:append(0)   --初始化四个玩家
		gcEnter.mjinfo.viprecord.score:append(0)
		gcEnter.mjinfo.situser:append(0)    --广东麻将有四个人的，把四个人的顺序都初始化出来
	end
	
	GdmjModel.SetUserTableList(cgmsg.userid, tableID)  --加入到玩家的列表中
	GdmjModel.SetTableInfo(gcEnter.mjinfo,1)
	GdmjModel.SetVipRoomByStr(tableID, cgmsg.strvipinfo) --同时需要设置牌桌信息
	GdmjEvent.JulebuGameCreate(gcEnter.mjinfo, cgmsg.strvipinfo)
	ThreadManager.GdmjUnLock(tableID)
	PlayerModel.SetPlayerInfo(pInfo)
	
	return 0
end

function GdmjEvent.GetCreateInfo(tInfo, userid)
	
	local cgCre = msg_gdmj_pb.cggdmjcreate()
	cgCre.userid = userid
	cgCre.mjtype = tInfo.mjtype
	cgCre.playernum = tInfo.maxplayernum
	cgCre.playnum = tInfo.maxvipnum
	cgCre.payway = tInfo.payway
	cgCre.paytype = tInfo.paytype
	cgCre.paynum = tInfo.paynum
	cgCre.julebuid = tInfo.julebuid
	cgCre.julebutype = tInfo.julebutype

	local strVip =  GdmjModel.GetRoomInfoStr(tInfo.tableid)
	if strVip ~= nil then
		cgCre.strvipinfo = strVip
	end
	
	return cgCre
end
