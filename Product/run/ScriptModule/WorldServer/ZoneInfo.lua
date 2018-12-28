Zone = Zone or {}
ZoneInfo = ZoneInfo or { }
ZoneInfo.ZoneTaskMap = {}
ZoneInfo.ZoneLinkTaskMap = {}
ZoneInfo.FirstTask = nil
Zone.zone_connect = function(cmd, zonetask) 
    unilight.info("大厅服务器回调：新的区连进来了 " .. zonetask.GetGameId() .. ":" .. zonetask.GetZoneId())
    ZoneInfo.ZoneTaskMap[tostring(zonetask.GetGameId())..zonetask.GetZoneId()] = zonetask
    ZoneInfo.ZoneLinkTaskMap[zonetask.UnlinkId] = zonetask
    if ZoneInfo.FirstTask == nil then
        ZoneInfo.FirstTask = zonetask
    end
end

Zone.zone_disconnect = function(cmd, zonetask) 
    unilight.info("大厅服务器回调：区掉线了了 " .. zonetask.GetGameId() .. ":" .. zonetask.GetZoneId())
    ZoneInfo.ZoneTaskMap[tostring(zonetask.GetGameId())..zonetask.GetZoneId()] = nil
    ZoneInfo.ZoneLinkTaskMap[zonetask.UnlinkId] = nil

    if ZoneInfo.FirstTask == zonetask then
        ZoneInfo.FirstTask = nil
        for k, v in pairs(ZoneInfo.ZoneTaskMap) do
            ZoneInfo.FirstTask = v
        end
    end
end 

Zone.zone_change_props = function(cmd, zonetask)
    unilight.info("-----" .. cmd.GetMaxonlinenum() .. "" .. zonetask.GetGameId())
    unilight.info("-----" .. cmd.GetPriority() .. "" .. zonetask.GetZoneId())
end

ZoneInfo.SendCmdToMe = function(doinfo, data, zonetask)
    if type(doinfo) ~= "string" or type(data) ~= "table" then
       unilight.error("Zone.SendCmdToMe param error.........type(doinfo):"..type(doinfo).." type(data)" .. type(data))
       return
    end
    local send = {}
    send["do"] = doinfo
    send["data"] = data
    local s = json.encode(send)
    --unilight.info("SendCmdToMe:" .. s)
    zonetask.SendString(s)
end

ZoneInfo.SendCmdToMeById = function(doinfo, data, gameid, zoneid)
    if type(doinfo) ~= "string" or type(data) ~= "table" then
        unilight.error("Zone.SendCmdToMe param error.........type(doinfo):"..type(doinfo).." type(data)" .. type(data))
        return
    end
    local send = {}
    send["do"] = doinfo
    send["data"] = data
    local s = json.encode(send)
    --unilight.info("SendCmdToMeById:" .. s)
    local zonetask = ZoneInfo.ZoneTaskMap[tostring(gameid)..zoneid]
    if zonetask ~= nil then
        zonetask.SendString(s)
    end
end

ZoneInfo.SendCmdToFirst = function(doinfo, data)
    if type(doinfo) ~= "string" or type(data) ~= "table" then
        unilight.error("Zone.SendCmdToMe param error.........type(doinfo):"..type(doinfo).." type(data)" .. type(data))
        return
    end
    local send = {}
    send["do"] = doinfo
    send["data"] = data
    local s = json.encode(send)
    --unilight.info("SendCmdToFirst:" .. s)
    if ZoneInfo.FirstTask ~= nil then
        ZoneInfo.FirstTask.SendString(s)
    end
end

ZoneInfo.SendCmdToAll = function(doinfo, data, gameid, zoneid)
    if type(doinfo) ~= "string" or type(data) ~= "table" then
        unilight.error("Zone.SendCmdToMe param error.........type(doinfo):"..type(doinfo).." type(data)" .. type(data))
        return
    end
    local send = {}
    send["do"] = doinfo
    send["data"] = data
    local s = json.encode(send)
    --unilight.info("SendCmdToAll:" .. s)
    local key = tostring(gameid)..zoneid
    for k, zonetask in pairs(ZoneInfo.ZoneTaskMap) do
        if k ~= key then
            if zonetask ~= nil then
                zonetask.SendString(s)
            end
        end
    end
end