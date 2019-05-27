


module("GetRankList", package.seeall)


function execute(packetID, operateID, buffer)
	
	local cgmsg = msg_rank_pb.cggetranklist()
	local gcmsg = msg_rank_pb.gcgetranklist()

	cgmsg:ParseFromString(buffer)

	local checkMsg = RedisHelper.CheckRedis(cgmsg.userid,operateID)
	
	if checkMsg ~= nil then
		gcmsg:ParseFromString(checkMsg)
		LogBehavior.Warning(cgmsg.userid, "hlyd", "GetRankList", 0,"缓存已存在")
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
	end	
	
	
	if cgmsg.pagenum > 10 then
		gcmsg.result = 0
		gcmsg.ranktype = cgmsg.ranktype
		return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()		
	end
	
	if cgmsg.ranktype == RankDefine.hlyd_popular then
		--欢乐赢豆中的魅力榜
		gcmsg.myrank,gcmsg.myscore = PopularModel.GetMyRank(cgmsg.userid)
		local rankList = PopularModel.GetRankList(cgmsg.pagenum)
		for i = #rankList,1,-1 do
		--for k,v in ipairs(rankList) do
			local pInfo = PlayerModel.GetPlayerInfo(rankList[i][1])
			if pInfo ~= nil then
				local psInfo = gcmsg.plist:add()
				PlayerModel.ChangeSimpleInfo(pInfo, psInfo)
				psInfo.jetton = tonumber(rankList[i][2])   --魅力榜
			end
		end
	elseif cgmsg.ranktype == RankDefine.hlyd_jetton or cgmsg.ranktype == RankDefine.gdmj_jetton then
		gcmsg.myrank,gcmsg.myscore = JettonModel.GetMyRank(cgmsg.userid)
		local rankList = JettonModel.GetRankList(cgmsg.pagenum)
		
		for i=#rankList,1,-1 do
			local pInfo = PlayerModel.GetPlayerInfo(rankList[i][1])
			if pInfo ~= nil then
				local psInfo = gcmsg.plist:add()
				PlayerModel.ChangeSimpleInfo(pInfo, psInfo)
				psInfo.jetton = tonumber(rankList[i][2])   --金币榜
			end
		end
	end
	gcmsg.result = 0
	gcmsg.ranktype = cgmsg.ranktype
	return cgmsg.userid,0,gcmsg:ByteSize(),gcmsg:SerializeToString()
end



