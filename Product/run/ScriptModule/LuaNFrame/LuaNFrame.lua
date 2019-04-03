-- json
json = cjson.new()

Do = Do or {}

LuaNFrame = LuaNFrame or {}

-- Net.*简化Do.*的消息处理，可直接收发lua table消息 --
Net = Net or {}

function LuaNFrame.init(pluginManager, luaModule)
	CPPNFrame:init(pluginManager, luaModule)
end

--执行加载函数
function LuaNFrame.load_script_file()
	CPPNFrame:load_script_file()
end

function LuaNFrame.InsertLoadFunc(func)
    CPPNFrame:InsertLoadFunc(func)
end

--添加服务器秒定时器
function LuaNFrame.addtimer(luaFunc, sec, ...)
	local param_table = {...}
	if #param_table == 0 then
		param_table = nil
	end
	return CPPNFrame:AddTimer(luaFunc, sec*1000, param_table)
end

--每嗝1毫秒的定时器示例
--LuaNFrame.addtimer("testtimer",1,"wanghaijun")
function LuaNFrame.addtimermsec(luaFunc, msec, ...)
	local param_table = {...}
	if #param_table == 0 then
		param_table = nil
	end
	return CPPNFrame:AddTimer(luaFunc, msec, param_table)
end

--停止服务器定时器
function LuaNFrame.stoptimer(timer)
	if timer == nil then return end
	CPPNFrame:StopTimer(timer)
end

-- 关于闹钟实例：
--  原理解释：
	-- intervalSec 表示从1970年开始到现在，把所有的时间以intervalSec为单位划分用 “---”表示
	-- sec 表示，在intervalSec这个单位时间段内需要触发闹钟的时刻用"*"表示
	-- 所以要求sec < intervalSec
	-- 图示：1970开始｜-*---｜-*--｜-*--|.......|-*--|now
-- 实例：
-- （1）每隔物理10秒的第1秒时钟实例 ：LuaNFrame.addclocker("testclocker",1,10,"wanghaijun");
	--function testclocker(text,clocker)
	--	LuaNFrame.debug("testclocker:"..text)
	--  clocker.Stop()
	--end
--  (2) 每天上午8点触发闹钟实例：LuaNFrame.addclocker("OnClocker", 8*3600, 24*3600, "wanghaijun");
	--function OnClocker(text,clocker)
	--	LuaNFrame.debug("每日触发实列")
	--end
-- (3) 每周（7*24*3600）的第34个小时触发闹钟：LuaNFrame.addclocker("OnClocker", 34*3600, 7*24*3600, "wanghaijun");
-- 
function LuaNFrame.addclocker(luaFunc, sec, intervalSec, ...)
	local param_table = {...}
	return CPPNFrame:AddClocker(luaFunc, sec, intervalSec , param_table)
end

--关于日历
--function testcalender(text,clocker)
--	LuaNFrame.debug("testcalender:"..text)
--  clocker.Stop()
--LuaNFrame.addcalender("testcalender" , "2015-09-10 06:05:00","addcalender")
LuaNFrame.addcalender = function(name, interval, ...)
	return go.addCalender(name,interval, ...)
end

--创建全局唯一的UUID
function LuaNFrame.GetUUID()
    return CPPNFrame:GetUUID()
end

--获得服务器开启时间，单位ms
function LuaNFrame.GetInitTime()
    return CPPNFrame:GetInitTime()
end

--获得服务器当前时间，单位ms
function LuaNFrame.GetNowTime()
    return CPPNFrame:GetNowTime()
end

--通过字符串获得MD5, 返回MD5字符串
function LuaNFrame.GetMD5(str)
    return CPPNFrame:GetMD5(str)
end

--通过字符串获得对应的CRC32, 返回数字
function LuaNFrame.GetCRC32(str)
    return CPPNFrame:GetCRC32(str)
end

--通过字符串获得对应的CRC16, 返回数字
function LuaNFrame.GetCRC16(str)
    return CPPNFrame:GetCRC16(str)
end

--通过字符串获得对应的Base64Encode, 返回字符串
function LuaNFrame.Base64Encode(str)
    return CPPNFrame:Base64Encode(str)
end

--通过字符串获得对应的Base64Decode, 返回字符串
function LuaNFrame.Base64Decode(str)
    return CPPNFrame:Base64Decode(str)
end

--设置LOG等级
function LuaNFrame.SetLogLevel(level)
    CPPNFrame:SetLogLevel(level)
end

--设置LOG立马刷新等级
function LuaNFrame.SetFlushOn(level)
    CPPNFrame:SetFlushOn(level)
end

--使用方法，BeginProfiler -- EndProfiler必须成对出现
function LuaNFrame.BeginProfiler(funname)
    CPPNFrame:BeginProfiler(funname)
end

function LuaNFrame.EndProfiler()
    return CPPNFrame:EndProfiler()
end

--serverNetEventModule 注册服务器与服务器之间的网络回调，主要有连接回调，断线回调
--比如说，luaFuncStr格式：luaFuncStr（eMsgType nEvent, uint32_t unLinkId, NF_SHARE_PTR<NFServerData> pServerData）
--
function LuaNFrame.AddServerEventCallBack(eSourceType, eTargetType, luaFuncStr)
    CPPNFrame:AddServerEventCallBack(eSourceType, eTargetType, luaFuncStr)
end

--serverNetEventModule 注册账号网络回调，主要有玩家连接回调，断线回调，重连回调
--比如说，luaFuncStr格式：luaFuncStr（uint32_t nEvent, uint32_t unLinkId, NF_SHARE_PTR<AccountInfo> pServerData）
--
function LuaNFrame.AddAccountEventCallBack(eServerType, luaFuncStr)
    CPPNFrame:AddAccountEventCallBack(eServerType, luaFuncStr)
end

LuaNFrame.debug = function(...)
	local cStackInfo = debug.getinfo(2, "Sl")
	if cStackInfo then
		CPPNFrame:debug("["..tostring(cStackInfo.short_src)..":"..tostring(cStackInfo.currentline).."] | "..tostring(...))
	else
		CPPNFrame:debug(tostring(...))
	end
end

LuaNFrame.info = function(...)
	local cStackInfo = debug.getinfo(2, "Sl")
	if cStackInfo then
		CPPNFrame:info("["..tostring(cStackInfo.short_src)..":"..tostring(cStackInfo.currentline).."] | "..tostring(...))
	else
		CPPNFrame:info(tostring(...))
	end
end

LuaNFrame.warn = function(...)
	local cStackInfo = debug.getinfo(2, "Sl")
	if cStackInfo then
		CPPNFrame:warn("["..tostring(cStackInfo.short_src)..":"..tostring(cStackInfo.currentline).."] | "..tostring(...))
	else
		CPPNFrame:warn(tostring(...))
	end
end

LuaNFrame.error = function(...)
	local cStackInfo = debug.getinfo(2, "Sl")
	if cStackInfo then
		CPPNFrame:error("["..tostring(cStackInfo.short_src)..":"..tostring(cStackInfo.currentline).."] | "..tostring(...))
	else
		CPPNFrame:error(tostring(...))
	end
end

LuaNFrame.SUCCESS = "0"
LuaNFrame.DB_ERROR = "2"
LuaNFrame.SCRIPT_ERROR = "4"

function LuaNFrame.getdebuglevel()
	return 0
end

-- request --

LuaNFrame.getreq = function(jsreq)
	if type(jsreq) == "table" then
		return jsreq
	end
	if type(jsreq) == "userdata" then
		LuaNFrame.error("LuaNFrame.getreq err")
		return ""
	end
	if jsreq == "" or (string.sub(jsreq,1,1) ~="{" and string.sub(jsreq,1,1) ~="[") then
		LuaNFrame.error("LuaNFrame.getreq err:"..jsreq)
		return {err=jsreq,}
	else
		return decode_repair(json.decode(jsreq))
	end
end

-- response --

LuaNFrame.success = function(w, req)
	req.errno = LuaNFrame.SUCCESS
	if req.data ~= nil then
		req.data.errno = LuaNFrame.SUCCESS
	end
	LuaNFrame.response(w, req)
end

LuaNFrame.faildb = function(w, req)
	req.errno = LuaNFrame.DB_ERROR
	LuaNFrame.response(w, req)
end

LuaNFrame.fail = function(w, req, errno, reason)
	if errno == nil or errno == LuaNFrame.SUCCESS then
		errno = "call LuaNFrame.fail with invalid errno"
	end
	req.data.errno = errno
	if reason ~= nil then
		req.data.reason = reason
	end
	LuaNFrame.response(w, req)
end

LuaNFrame.scripterror = function(w, err)
	LuaNFrame.error(err)
	LuaNFrame.response(w, {errno=LuaNFrame.SCRIPT_ERROR, data=nil})
end

LuaNFrame.response = function(w, req)
	req.st = os.time()
	local s = table2json(req)
	if w ~= nil then
		w.SendString(s)
	end
	
	if type(req["do"]) == "string" then
		if req["do"] == "Cmd.SendUserMoneyCmd_S" then
			return
		elseif req["do"] == "Cmd.UserTravelAngerUpdate_S" then
			return
		elseif req["do"] == "Cmd.Ping_S" then
			return
		end
	end
	--LuaNFrame.debug("[send] " .. s)
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
                req = LuaNFrame.getreq(reqdata)
                
				local r0, r1 = handle(req,w)
				if w == nil then
					LuaNFrame.error("unsupported w is null")
					return r0							-- return r0 for server test
				elseif r0 == nil then
					--LuaNFrame.info("return is nil")
					--LuaNFrame.success(w, req)			-- return {data} by zwl
					return 
				elseif type(r0) == "table" then
					LuaNFrame.success(w, r0)				-- return {data}
				elseif r0 == LuaNFrame.DB_ERROR then
					LuaNFrame.faildb(w, r1 or {})		-- return LuaNFrame.DB_ERROR, {data}?
				elseif r0 == LuaNFrame.SCRIPT_ERROR then
					LuaNFrame.scripterror(w, r1 or {})	-- return LuaNFrame.SCRIPT_ERROR, {data}?
				elseif r0 == LuaNFrame.SUCCESS then
					LuaNFrame.success(w, r1 or {}) 		-- return LuaNFrame.SUCCESS, {data}?
				elseif type(r0) == "string" then
					LuaNFrame.fail(w, req, r0, r1) 		-- return "errno", "reason"
				else
					LuaNFrame.error("unsupported return type")
				end
			end
		end
	end,
})

-- 收到大厅的回调
Lobby = Lobby or {}
Lby = Lby or {}
setmetatable(Lby,
{
	__index = Lobby,
	__newindex = function(t, k, handle)
		if handle == nil then
			Lobby[k] = nil
		else
			Lobby[k] = function(reqdata, w, msgname)
                local req = {}

				if reqdata ~= nil then
					req = LuaNFrame.getreq(reqdata)
				end

				local r0, r1 = handle(req, w)
				req.msgtype = "lobbymessage"

				if w == nil then
					return r0							-- return r0 for server test
				elseif r0 == nil then
					return								-- return nil?
				elseif type(r0) == "table" then
					LuaNFrame.success(w, r0)				-- return {data}
				elseif r0 == LuaNFrame.DB_ERROR then
					LuaNFrame.faildb(w, r1 or {})		-- return LuaNFrame.DB_ERROR, {data}?
				elseif r0 == LuaNFrame.SCRIPT_ERROR then
					LuaNFrame.scripterror(w, r1 or {})	-- return LuaNFrame.SCRIPT_ERROR, {data}?
				elseif r0 == LuaNFrame.SUCCESS then
					LuaNFrame.success(w, r1 or {}) 		-- return LuaNFrame.SUCCESS, {data}?
				end
			end
		end
	end,
})
 
-- 收到区服务器的回调
Zone = Zone or {}
ZoneServer = ZoneServer or {}
setmetatable(Zone,
{
	__index = ZoneServer,
	__newindex = function(t, k, handle)
		if handle == nil then
			ZoneServer[k] = nil
		else
			ZoneServer[k] = function(reqdata, w, msgname)
                local req = {}
                local req = reqdata 

				if reqdata ~= nil and type(reqdata) == "string" then
					req = LuaNFrame.getreq(reqdata)
					req.msgtype = "lobbymessage"
				end
                
				local r0, r1 = handle(req, w)

				--req.msgtype = "lobbymessage"
				if w == nil then
					return r0							-- return r0 for server test
				elseif r0 == nil then
					return								-- return nil?
				elseif type(r0) == "table" then
					LuaNFrame.success(w, r0)				-- return {data}
				elseif r0 == LuaNFrame.DB_ERROR then
					LuaNFrame.faildb(w, r1 or {})		-- return LuaNFrame.DB_ERROR, {data}?
				elseif r0 == LuaNFrame.SCRIPT_ERROR then
					LuaNFrame.scripterror(w, r1 or {})	-- return LuaNFrame.SCRIPT_ERROR, {data}?
				elseif r0 == LuaNFrame.SUCCESS then
					LuaNFrame.success(w, r1 or {}) 		-- return LuaNFrame.SUCCESS, {data}?
				end
			end
		end
	end,
})
