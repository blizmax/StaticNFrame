RebotServerModule = {}

function RebotServerModule.Init()
    local data = {}
    data["do"] = "request-zone-list"
    data["gameid"] = 3010
    unilight.HttpClientRequestPost("https://ttrserver.staticnframe.club:7000/httplogin", "httpPostReturn", data)
    --nilight.HttpClientRequestPost("http://127.0.0.1:7000/httplogin", "httpPostReturn", data)
end

function HttpClient.httpPostReturn(state_code, respData)

end

--特殊协议
function RebotServerModule.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
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

function RebotServerModule.AfterInit()

end


function RebotServerModule.Execute()

end

function RebotServerModule.BeforeShut()

end

function RebotServerModule.Shut()

end
