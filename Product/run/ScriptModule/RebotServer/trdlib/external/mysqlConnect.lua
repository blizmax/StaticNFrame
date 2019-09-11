
if g_platfrom == 'luaself' then
    require "luasql.mysql"
end

local mysqlConnect = class("mysqlConnect")

function mysqlConnect:ctor(dbType, dbUser, dbPassword,dbHost,dbPort, dbDatabase)
	--LogFile("info",'mysql connect start:dbtype='..g_dbtype..',dbuser='..g_dbUser..',dbhost='..g_dbHost..',dbPort='..g_dbPort)
	self.m_dbType = dbType
	self.m_dbUser = dbUser
	self.m_dbPassword = dbPassword
	self.m_dbHost = dbHost
	self.m_dbPort = dbPort
	self.m_dbDatabase = dbDatabase
	self.m_env = luasql.mysql()
	
	self.m_connect = assert(self.m_env:connect(self.m_dbType, self.m_dbUser, self.m_dbPassword, self.m_dbHost, self.m_dbPort))
	self.m_connect:execute[[set names utf8]]
	self.m_connect:execute("use "..self.m_dbDatabase)

	LogFile("info",'mysql connect success')
end


function mysqlConnect:reConnect()
	--body
	LogFile("info",'mysql reconnect dbtype='..self.m_dbType..',dbuser='..self.m_dbUser..',dbhost='..self.m_dbPassword..',dbPort='..self.m_dbPort)

	self:release()

	self.m_env = luasql.mysql()
	self.m_connect = assert(self.m_env:connect(self.m_dbType, self.m_dbUser, self.m_dbPassword, self.m_dbHost, self.m_dbPort))
	self.m_connect:execute[[set names utf8]]
	self.m_connect:execute("use "..self.m_dbDatabase)	
end

function mysqlConnect:executeQuery(sql)
	--LogFile("sql",sql)
	
	self.cursor = self.m_connect:execute(sql)

	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		self.cursor = self.m_connect:execute(sql)
	end

	return self.cursor
end

function mysqlConnect:executeUpdate(sql)
	--LogFile("sql",sql)
	--print("sql="..sql)
	self.cursor = self.m_connect:execute(sql)
	

	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		self.cursor = self.m_connect:execute(sql)
	end

	return self.cursor
end

function mysqlConnect:execute(sql)
	--LogFile("sql",sql)
	--print("sql="..sql)
	self.cursor = self.m_connect:execute(sql)
	
	if self.cursor == nil then
		self:reConnect()
		--防止超时链接断掉出现
		
		self.cursor = self.m_connect:execute(sql)
	end

	return self.cursor
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
	
	if self.cursor == nil then
		return nil
	end
	
	return self.cursor:fetch(...)
end

return mysqlConnect
