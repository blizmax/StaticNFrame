-- json
json = cjson.new()

Do = Do or {}

unilight = unilight or {}

-- Net.*简化Do.*的消息处理，可直接收发lua table消息 --
Net = Net or {}

function unilight.init(pluginManager)
	LuaNFrame:init(pluginManager)
end

--执行加载函数
function unilight.load_script_file()
	LuaNFrame:load_script_file()
end

function unilight.InsertLoadFunc(func)
    LuaNFrame:InsertLoadFunc(func)
end

--创建全局唯一的UUID
function unilight.GetUUID()
    return LuaNFrame:GetUUID()
end

--获得服务器开启时间，单位ms
function unilight.GetInitTime()
    return LuaNFrame:GetInitTime()
end

--获得服务器当前时间，单位ms
function unilight.GetNowTime()
    return LuaNFrame:GetNowTime()
end

--通过字符串获得MD5, 返回MD5字符串
function unilight.GetMD5(str)
    return LuaNFrame:GetMD5(str)
end

--通过字符串获得对应的CRC32, 返回数字
function unilight.GetCRC32(str)
    return LuaNFrame:GetCRC32(str)
end

--通过字符串获得对应的CRC16, 返回数字
function unilight.GetCRC16(str)
    return LuaNFrame:GetCRC16(str)
end

--通过字符串获得对应的Base64Encode, 返回字符串
function unilight.Base64Encode(str)
    return LuaNFrame:Base64Encode(str)
end

--通过字符串获得对应的Base64Decode, 返回字符串
function unilight.Base64Decode(str)
    return LuaNFrame:Base64Decode(str)
end

unilight.debug = function(...)
	LuaNFrame:debug(...)
end

unilight.info = function(...)
	LuaNFrame:info(...)
end

unilight.warn = function(...)
	LuaNFrame:warn(...)
end

unilight.error = function(...)
    LuaNFrame:error(...)
end

unilight.SUCCESS = "0"
unilight.DB_ERROR = "2"
unilight.SCRIPT_ERROR = "4"


--特殊协议
function unilight.NetServerRecvHandleJson(unLinkId, valueId, nMsgId, strMsg)
    unilight.debug("unLinkId:" .. unLinkId .. " valueId:" .. valueId .. " nMsgId:" .. nMsgId .. " strMsg:" .. strMsg)
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
					local laccount = {}
					laccount.Id = valueId
					laccount.unLinkId = unLinkId
					laccount.SendString = TcpServer.sendJsonMsg
                    Net[strcmd](table_msg, laccount)
                end
            end
        end
    end
    -- body
end

-- request --

unilight.getreq = function(jsreq)
	if type(jsreq) == "table" then
		return jsreq
	end
	if type(jsreq) == "userdata" then
		unilight.error("unilight.getreq err")
		return ""
	end
	if jsreq == "" or (string.sub(jsreq,1,1) ~="{" and string.sub(jsreq,1,1) ~="[") then
		unilight.error("unilight.getreq err:"..jsreq)
		return {err=jsreq,}
	else
		return decode_repair(json.decode(jsreq))
	end
end

-- response --

unilight.success = function(w, req)
	req.errno = unilight.SUCCESS
	if req.data ~= nil then
		req.data.errno = unilight.SUCCESS
	end
	unilight.response(w, req)
end

unilight.faildb = function(w, req)
	req.errno = unilight.DB_ERROR
	unilight.response(w, req)
end

unilight.fail = function(w, req, errno, reason)
	if errno == nil or errno == unilight.SUCCESS then
		errno = "call unilight.fail with invalid errno"
	end
	req.data.errno = errno
	if reason ~= nil then
		req.data.reason = reason
	end
	unilight.response(w, req)
end

unilight.scripterror = function(w, err)
	unilight.error(err)
	unilight.response(w, {errno=unilight.SCRIPT_ERROR, data=nil})
end

unilight.response = function(w, req)
	local s = json.encode(encode_repair(req))
    local msgname = req["do"]
    local nu = unilight.getmsgnu(msgname)
    if nu ~= nil then 
        local msgname = req["do"]
        local msgdata = req["data"]
        msgdata.errno = nil
        local rawdata = pb.encode(msgname,msgdata)
        w.SendLuaProtoRawdata(nu.bycmd, nu.byparam, rawdata)
	    unilight.debug("[proto send] " .. s)
        return true
    else
	    --req.st = os.time() + unilight.tzoffset()
		--w.SendString(s)
		w.SendString(s, w)
	    unilight.debug("[js send] " .. s)
    end
end

-- Net.*简化Do.*的消息处理，可直接收发lua table消息 --
Net = {}
setmetatable(Net,
{
	__index = Do,
	__newindex = function(t, k, handle)
		if handle == nil then
			Do[k] = nil
		else
			Do[k] = function(reqdata, w, msgname)
                local req = {}
                if msgname ~= nil and 0 == w.GetMsgType() then
                    local data =  pb.decode(msgname, reqdata)
                    req = {
                        ["do"] = msgname, 
                        ["data"] = data,
                    }
                else
                    req = unilight.getreq(reqdata)
                end
				local r0, r1 = handle(req,w)
				if w == nil then
					unilight.error("unsupported w is null")
					return r0							-- return r0 for server test
				elseif r0 == nil then
					--unilight.info("return is nil")
					--unilight.success(w, req)			-- return {data} by zwl
					return 
				elseif type(r0) == "table" then
					unilight.success(w, r0)				-- return {data}
				elseif r0 == unilight.DB_ERROR then
					unilight.faildb(w, r1 or {})		-- return unilight.DB_ERROR, {data}?
				elseif r0 == unilight.SCRIPT_ERROR then
					unilight.scripterror(w, r1 or {})	-- return unilight.SCRIPT_ERROR, {data}?
				elseif r0 == unilight.SUCCESS then
					unilight.success(w, r1 or {}) 		-- return unilight.SUCCESS, {data}?
				elseif type(r0) == "string" then
					unilight.fail(w, req, r0, r1) 		-- return "errno", "reason"
				else
					unilight.error("unsupported return type")
				end
			end
		end
	end,
})
