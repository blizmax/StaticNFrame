CPPNFrame = CPPNFrame or {}

--数据库

function CPPNFrame:initmongodb(url, dbname)
    self.mongoModule:AddMongoServer(0, url, dbname)
    self.asynMongoModule:AddMongoServer(0, url, dbname)
    return true
end

--[[
    功能：创建表
    参数：
        name   : string, 表名
        primary: string, 主键名
    实例：
        CPPNFrame:createdb("userinfo", "_id") // 创建一个"userinfo"表，主键为"_id"

        returns its Collection handle. On error, returns nil and the error message.
]]
function CPPNFrame:createdb(name, primary)
    self.mongoModule:CreateCollection(0, name, primary)
    self.asynMongoModule:CreateCollection(0, name, primary)
    return true
end

--[[
    功能: 删除表
    参数：
        name   : string, 表名
    实例：
        CPPNFrame:droptable("userinfo") // 删除表"userinfo"
]]
function CPPNFrame:droptable(name)
    return self.mongoModule:DropCollection(name)
end

--[[
    功能：获取表中一条记录
    参数：
        name: string           , 表名
        id  : 根据存储类型一致 ，主键值
    实例：
        CPPNFrame:getdata("userinfo", 100000) // 获取表"userinfo"中主键值为100000的那条记录
]]
function CPPNFrame:getdata(name, key)
    local data = self.mongoModule:FindOneByKey(0, name, key)
    if data ~= nil and data ~= "" then
        return json2table(data)
    end
end

--[[
    功能：保存一条记录
    参数：
            name: string, 表名
            data: table , 需要保存的数据的全部信息，如果和表里已有的记录冲突，替换整条记录,
    实例：
        local userInfo = {
            _id = 100000,
            chips = 200000,
            base = {
                headurl = "http://baidu.com"
            }
        }
        unilight.savedata("userinfo", userInfo)
]] 
function CPPNFrame:savedata(name, data, asyn)
    local json_data = table2json(data)
    if type(data.uid) == "number" or type(data.uid) == "string" then
        local uid = tonumber(data.uid)
        if asyn == nil or asyn == false then
            return self.mongoModule:UpdateOneByKey(0, name, json_data, uid)
        else
            return self.asynMongoModule:UpdateOneByKey(0, name, json_data, uid)
        end
    else
        if asyn == nil or asyn == false then
            return self.mongoModule:UpdateOne(0, name, json_data)
        else
            return self.asynMongoModule:UpdateOne(0, name, json_data)
        end
    end
end

--[[
    功能：获取条记录的指定字段数据
    参数：
        name      :string          , 表名
        id        :根据具体使用类型， 键值
        fieldpath :string          , 指定字段
    实例：
        unilight.getfield("userinfo", 100000, "base.property") // 获取表"userinfo"中，key为100000的"base.property"字段数据
]]
function CPPNFrame:getfield(name, id, fieldpath)
    local data = self.mongoModule:FindFieldByKey(0, name, fieldpath, id)

    if type(data) == "string" and data ~= "" then
        data = json2table(data)
    else
        return
    end

    local fields = string.split(fieldpath, ".")
    for index, name in ipairs(fields) do
        data = data[name]
    end
    return data
end

--[[
    功能：保存一条记录里的某部分字段
    参数：
        name     : string           , 表名
        id       : 根据存储类型一致 ，主键值
        fieldpath: string           , 保存的记录键名
        data     : table            , 需要保存据信息，如果表里已有记录冲空，替换整条记录
    返回： nil 表示成功
            string 表示失败
    实例：
    (1)	表"userinfo"原来有一条记录
        userInfo = {
            _id = 100000,
            chips = 200000,
            base = {
                headurl = "http://baidu.com",
                property = {
                    name = "zwl",
                    age = 27,
                },
            },
        }

    (2) 将userInfo.base.property修改成
        local property = {
            Name = "zhaowolong",
            age = 28,
            addr = "深圳",
        }
        unilight.savefield("userinfo", 100000, "base.property", property)

    (3) 表"userinfo" 中键值为：100000的最新记录为：
        userInfo = {
            _id = 100000,
            chips = 200000,
            base = {
                headurl = "http://baidu.com",
                property = {
                    Name = "zhaowolong",
                    age = 28,
                    addr = "深圳"
                },
            }
        }
]]
----------------------------------WARNNING-------------------------------------
-- data将覆盖指定的fieldpath，记得是覆盖
-----------------------------------------------------------------------
function CPPNFrame:savefield(name, id, fieldpath, data, asyn)
    if id == nil or type(id) == "userdata" or data == nil or type(data) == "userdata" then
        unilight.error("id or data is null or type() is userdata")
        return "datatype error "
    end

    local tmp = {}
    tmp[fieldpath] = data
    local json_str = table2json(tmp)

    if asyn == nil or asyn == false then
        return self.mongoModule:UpdateFieldByKey(0, name, json_str, id)
    else
        return self.asynMongoModule:UpdateFieldByKey(0, name, json_str, id)
    end
end

--[[
    功能： 获取表里的所有记录
    参数：
            name:string,表名
    实例：
        local res = unilight.getAll("userinfo")
]]
function CPPNFrame:getAll(name)
    local data = self.mongoModule:FindAll(0, name)
    return json2table(data)
end