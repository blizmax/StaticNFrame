
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
	end
		
	strDes = strDes..m_tInfo.maxvipnum.."局"
	if m_tInfo.payway == 2 then
		strDes = strDes..",AA付费"
	end
	
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
	end
	JulebuService.CreateGame(g_JulebuDefine.modules_gdmj, m_tInfo.julebuid, m_tInfo.tableid, m_tInfo.maxplayernum, m_tInfo.playernum, strDes)
end

function GdmjEvent.JulebuGameEnd(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.GameEnd(tInfo.julebuid, tInfo.tableid)
end

function GdmjEvent.JulebuGameUpdate(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.GameUpdate(tInfo.julebuid, tInfo.tableid, tInfo.playernum)
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
	JulebuService.GameCount(julebuID, tableID, userArrTemp,g_JulebuDefine.modules_gdmj)
end

function GdmjEvent.JulebuGameStart(tInfo)
	if tInfo == nil or tInfo.julebuid == 0 then
		return
	end
	JulebuService.JulebuGameStart(tInfo.julebuid, tInfo.tableid)		
end

function GdmjEvent.CheckJiFen(julebuID, userID)
	return JulebuService.CheckJiFen(julebuID, userID)
end

function GdmjEvent.AddJiFen(julebuID, userID, JiFen)
	--把积分同步到俱乐部中
	JulebuModel.AddUserJiFen(julebuID, userID, JiFen)
end

