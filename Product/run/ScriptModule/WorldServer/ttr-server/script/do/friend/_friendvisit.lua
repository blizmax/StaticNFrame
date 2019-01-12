CreateClass("FriendVisit")   --好友互访数据

function FriendVisit:Init(uid)
    self.uid = uid
    --当前地图ID, 第一个地图的ID为101
    self.cur_map_id = 101;
    --地图数据
    self.builds = {}
end

function FriendVisit:GetCurMapId()
    return self.cur_map_id
end

function FriendVisit:SetCurMapId(mapid)
    self.cur_map_id = mapid
    self.builds = {}
end

function FriendVisit:GetBuilds()
    return self.builds
end

function FriendVisit:AddBuild(buildid, level, buildlv)
    self.builds[buildid] = {
        id = buildid,
        level = level,
        buildlv = buildlv,
    }
end

function FriendVisit:SetLevel(buildid, level)
    if self.builds[buildid] ~= nil then
        self.builds[buildid].level = level
    end
end

function FriendVisit:SetBuildLevel(buildid, buildlv)
    if self.builds[buildid] ~= nil then
        self.builds[buildid].buildlv = buildlv
    end
end

function FriendVisit:SetDBTable(data)
    if data == nil then return end

    self.cur_map_id = data.cur_map_id or self.cur_map_id

    if data.builds ~= nil then
        for id, info in pairs(data.builds) do
            self.builds[id] = {
                id = info.id,
                level = info.level,
                buildlv = info.buildlv,
            }
        end
    end
end



