
module('httpManager', package.seeall)




n_httpController = {

	--["getServerList"] = getServerList,
	--["AccountLogin"] = AccountLogin,
	--["AccountRegister"] = AccountRegister,
	--["SelectServer"] = SelectServer,
	--["UpdateServerInfo"] = UpdateServerInfo,
	--["HttpSendLog"] = HttpSendLog
	["hlyd"] = "HttpHlyd",
	["debug"] = "HttpDebug",
	["pay"] = "HttpPay",
	["mjqj"] = "HttpMjqj",
	["game"] = "HttpGame",
	["play"] = "HttpPlay",
}


function httpManager:createController(packetid)

	--local controller = _G[n_httpController[packetid]]
	return _G[n_httpController[packetid]]
	
end



