--执行游戏服务器信息

function LuaNFrame.SendMsgToRebot(unLinkId, nPlayerId, nMsgId, nLen, strData)
		local operateId = RebotModel.GetRebotOperateId(unLinkId)
		LuaNFrame.SendMsgToMaster(unLinkId, operateId, nMsgId, nLen, strData)
end

function LuaNFrame.DispatchRebotTcp(unLinkId, valueId, nMsgId, strMsg)
	local function TcpExecute()
		local retMsgID,controller = tcpManager:createController(nMsgId)
	
		if controller == nil then
			LuaNFrame.Error(NFLogId.NF_LOG_SYSTEMLOG, valueId, "nMsgId:"..nMsgId.." not handled!")
		else
			controller.execute(nMsgId, unLinkId, strMsg)
		end
	end
	
	local status, msg = xpcall (TcpExecute, __G__TRACKBACK__)

	if not status then
		LuaNFrame.SendErrorLog(valueId, "LuaNFrame.DispatchRebotTcp error, unLinkId:"..tostring(unLinkId).." valueId:"..tostring(valueId).." nMsgId:"..nMsgId, msg)
    end
end

function LuaNFrame.DispatchRebotEvent(unLinkId, valueId, nMsgId, strMsg)
	local function TcpExecute()
		if nMsgId == NF_ACCOUNT_EVENT_TYPE.eAccountEventType_CONNECTED then
			RebotModel.AccountConnnect(unLinkId)
            --RebotAccount.LoginServer(unLinkId)
        elseif nMsgId == NF_ACCOUNT_EVENT_TYPE.eAccountEventType_DISCONNECTED then
            RebotAccount.DisConnect(unLinkId)
        end
	end
	
	local status, msg = xpcall (TcpExecute, __G__TRACKBACK__)

	if not status then
		LuaNFrame.SendErrorLog(valueId, "LuaNFrame.DispatchRebotEvent error, unLinkId:"..tostring(unLinkId).." valueId:"..tostring(valueId).." nMsgId:"..nMsgId, msg)
    end
end