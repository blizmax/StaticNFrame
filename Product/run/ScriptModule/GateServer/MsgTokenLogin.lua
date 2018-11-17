function Net.UserLoginTokenLoginUserPmd_C(cmd, laccount)
    local account = cmd.accountid
    local unlinkid = laccount.unLinkId
    AccountMgr:AddAccount(account, unlinkid)
end

function Net.UserJsMessageForwardUserPmd_CS(cmd, laccount)
    laccount.unLinkId = GateServerModule.gameserver_unlinkid
    laccount.SendString = TcpClient.sendJsonMsg
    
    if type(cmd["msg"]) == "string" then
        laccount.SendString(cmd["msg"], laccount)
    end
end