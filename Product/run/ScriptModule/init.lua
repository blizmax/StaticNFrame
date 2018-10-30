function init_script_system(pluginManager)
	local breakSocketHandle,debugXpCall = require("LuaDebug")("localhost",7003)
	
	--初始化LuaNFrame
	unilight.init(pluginManager)
end