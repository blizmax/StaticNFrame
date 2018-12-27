WorldServerModule = {}

function WorldServerModule.Init()
    --vscode luaide调试工具需要
    --breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
    
    unilight.createdb("friendinfo","uid")					-- 玩家好友信息
    unilight.createdb("userQQAppId", "uid")					-- 玩家QQAPPID信息
    unilight.createdb("ranklist", "uid")					-- 排行榜数据库信息

    TcpServer.addRecvCallBack(NF_SERVER_TYPES.NF_ST_WORLD, 0, "WorldServerModule.GameRecvHandleJson")

    unilight.AddServerEventCallBack(NF_SERVER_TYPES.NF_ST_WORLD, NF_SERVER_TYPES.NF_ST_GAME, "WorldServerModule.WorldServerNetEvent")
    
    --初始化排行榜
    if RankListMgr ~= nil then
        RankListMgr:Init()
    end
    --好友管理系统初始化
    if FriendManager ~= nil then
        FriendManager:Init()
    end

	local CYCLE_MIN = 60
	local CYCLE_HOUR = 3600
	local CYCLE_DAY = CYCLE_HOUR * 24
    local CYCLE_WEEKLY = CYCLE_DAY * 7
    
    unilight.addclocker("WorldServerModule.GlobalDayZeroClock", 0, CYCLE_DAY) 	-- 0点时钟
    unilight.addclocker("WorldServerModule.GlobalWeekZeroClock", 0, CYCLE_WEEKLY) 	-- 0点时钟
end

--保存当前是每周几天和每月第几天
function WorldServerModule.GlobalDayZeroClock()
	--清理好友系统
	FriendManager:AllFriendZeroClear()
	FriendManager.RefreshUserFriendToDB()
end

--保存当前是每周几天和每月第几天
function WorldServerModule.GlobalWeekZeroClock()

end

function WorldServerModule.WorldServerNetEvent(nEvent, unLinkId, serverData)
    local cmd = {}
    if nEvent == NF_MSG_TYPE.eMsgType_CONNECTED then
        Zone.zone_connect(cmd, serverData)
    end
    if nEvent == NF_MSG_TYPE.eMsgType_DISCONNECTED then
        Zone.zone_disconnect(cmd, serverData)
    end
end

--特殊协议
function WorldServerModule.GameRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    local table_msg = json2table(strMsg)
    if type(table_msg["do"]) == "string" then
        if table_msg["do"] == "Cmd.UserUpdate_C" then
        else
            unilight.debug(tostring(valueId) .. " | recv game msg |" .. strMsg)
        end
    end
    --协议规则
    if table_msg ~= nil then
        local cmd = table_msg["do"]
        if type(cmd) == "string" then
            local i, j = string.find(cmd, "Cmd.")
            local strcmd = string.sub(cmd, j+1, -1)
            if strcmd ~= "" then
                strcmd = "Cmd" .. strcmd
                if type(Zone[strcmd]) == "function" then
                    local zonetask = ZoneInfo.ZoneLinkTaskMap[unLinkId]
                    if zonetask ~= nil then
                        Zone[strcmd](table_msg, zonetask)
                    end
                end
            end
        end
    end
    -- body
end

function WorldServerModule.AfterInit()

end


function WorldServerModule.Execute()

end

function WorldServerModule.BeforeShut()

end

function WorldServerModule.Shut()
    FriendManager.RefreshUserFriendToDB()
    unilight.info("Server.ServerStop:停机数据处理完成")
end
