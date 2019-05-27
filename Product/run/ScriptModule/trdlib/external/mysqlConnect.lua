
if g_platfrom == 'luaself' then
    require "luasql.mysql"
end

local mysqlConnect = class("mysqlConnect")

function mysqlConnect:ctor()
	LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  'mysql connect start:dbtype='..g_dbtype..',dbuser='..g_dbUser..',dbhost='..g_dbHost..',dbPort='..g_dbPort)
	self.m_env = luasql.mysql()
	self.m_connect = assert(self.m_env:connect(g_dbtype, g_dbUser, g_dbPassword, g_dbHost, g_dbPort))
	self.m_connect:execute[[set names utf8]]
	self.m_connect:execute("use "..g_dbDatabase)

	LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  'mysql connect success')
end


function mysqlConnect:reConnect()
	--body
	LuaNFrame.Info(NFLogId.NF_LOG_SYSTEMLOG, 0,  'mysql reconnect dbtype='..g_dbtype..',dbuser='..g_dbUser..',dbhost='..g_dbHost..',dbPort='..g_dbPort)
	self:release()

	self.m_env = luasql.mysql()
	self.m_connect = assert(self.m_env:connect(g_dbtype, g_dbUser, g_dbPassword, g_dbHost, g_dbPort))
	self.m_connect:execute[[set names utf8]]
	self.m_connect:execute("use "..g_dbDatabase)	
end

function mysqlConnect:executeQuery(sql)
	
	self.cursor = self.m_connect:execute(sql)

	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		self.cursor = self.m_connect:execute(sql)
	end

	return self.cursor
end

function mysqlConnect:executeUpdate(sql)
	
	local res = self.m_connect:execute(sql)
	

	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		self.cursor = self.m_connect:execute(sql)
	end

	return res
end

function mysqlConnect:execute(sql)
	--print("sql="..sql)
	local res = self.m_connect:execute(sql)
	
	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		self.cursor = self.m_connect:execute(sql)
	end

	return res
end


function mysqlConnect:release()

	self.m_connect:close()
	self.m_env:close()

end
function mysqlConnect:numrows( )
	return self.cursor:numrows()
	-- body
end

function mysqlConnect:fetch(...)
	return self.cursor:fetch(...)
end

return mysqlConnect
