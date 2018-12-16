GameServerModule = {}

Tcp = Tcp or {}

GameServerModule.gameserver_linkid = 0

StartOver = StartOver or function()

end

InitTimer = InitTimer or function()
    -- body
end

UserInfo = UserInfo or {}

UserInfo.Update = UserInfo.Update or function()

end

function GameServerModule.Init()
    unilight.initmongodb('mongodb://14.17.104.12:28900', "ttr-1")
    Do.dbready()
    
    TcpClient.addRecvCallBack(NF_SERVER_TYPES.NF_ST_PROXY, 0, "GameServerModule.NetServerRecvHandleJson")

    unilight.addtimer("UserInfo.Update", 1)

    unilight.response = function(w, req)
		req.st = os.time()
		local s = table2json(req)
		w.SendString(s)
		unilight.debug("[send] " .. s)
    end
    
    -- 当tcp上线时
	Tcp.account_connect = function(laccount)
		UserInfo.Connected(laccount.Id)
	end

	-- 当tcp掉线时
	Tcp.account_disconnect = function(laccount)
		UserInfo.Disconnected(laccount.Id)
	end

	Tcp.reconnect_login_ok = function(laccount)
		unilight.debug("游戏玩家断线重连了。。。。。。。。。。。。。")
        UserInfo.ReconnectLoginOk(laccount)
    end
    
    StartOver()

    InitTimer()
end

--特殊协议
function GameServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug(tostring(valueId) .. " | recv msg |" .. strMsg)
    local table_msg = json2table(strMsg)
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["do"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Cmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
                strcmd = "Cmd" .. strcmd
                if type(Net[strcmd]) == "function" then
                    local laccount = go.roomusermgr.GetRoomUserById(valueId)
                    if laccount ~= nil then
                        Net[strcmd](table_msg, laccount)
                    end
                end
            end
        end
    end
    -- body
end

function GameServerModule.AfterInit()

end


function GameServerModule.Execute()

end

function GameServerModule.BeforeShut()

end

function GameServerModule.Shut()

end
