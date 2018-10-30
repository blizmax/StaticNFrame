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

--特殊协议1111
function unilight.NetRecvHandleJson1111(unLinkId, valueId, nMsgId, strMsg)
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
                    Net[strcmd](table_msg, laccount)
                end
            end
        end
    end
    -- body
end