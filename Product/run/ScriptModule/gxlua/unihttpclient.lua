unilight = unilight or { }

-- HttpGet.*简化Do.*的消息处理，可直接收发lua table消息 --
HttpGetDo = HttpGetDo or {}
HttpGet = {}
setmetatable(HttpGet,
{
	__index = HttpGetDo,
	__newindex = function(t, k, handle)
		if handle == nil then
			HttpGetDo[k] = nil
		else
			HttpGetDo[k] = function(state_code, respData, strUseData)
				unilight.debug("Http Get | " .. k)
				handle(state_code, respData, strUseData)
			end
		end
	end,
})

--[[
	向指定url请求GET http服务
	resFunc:http请求回调函数
	url:请求http服务器的url
	msg:请求的数据,这时里是一个lua的table
    heads 在这里是一个 map[string]string 选定对应参考与值
    
    return bool
]]

function unilight.HttpRequestGet(uri, resFunc, strUseData)
    return LuaNFrame:HttpRequestGet(uri, resFunc, {}, strUseData)
end


--C++将调用这个函数作为httpclient回调
function unilight.HttpRequestGetCallBack(luaFunc, state_code, respData, strUseData)
	local callbackpara = json2table(strUseData)
	if HttpGet[luaFunc] ~= nil then
		HttpGet[luaFunc](state_code, respData, callbackpara)
	end
end