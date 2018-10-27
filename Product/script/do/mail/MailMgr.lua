MailMgr = CreateClass("MailMgr")

MailMgr = {
	owner = nil,
	id = 0,
	loaded = false,
	mails = nil,
}

MAIL_MAX = 100
MAIL_KEEP_TIME = 604800 -- 7 * 24 * 3600

function MailMgr:new(o)
	o = o or {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function MailMgr:init(owner)
	self.owner = owner
	self.loaded = false
	self.mails = {}
end

function MailMgr:receive()
	return 0
end

function MailMgr:pull()
	if not self.loaded then
		self:loadFromDb()
		self:clean()
	end

	return self.mails
end

function MailMgr:clean()
	local indexes = {}
	local time = os.time()

	for i, mail in ipairs(self.mails) do
		if time - mail.time > MSG_KEEP_TIME then
			table.insert(indexes, i)
		end
	end

	local i = 0
	while #indexes > 0 do
		i = table.remove(indexes, #indexes)
		table.remove(self.mails, 1)
	end

	while #self.mails > MAIL_MAX do
		table.remove(self.mails, 1)
	end
end

function MailMgr:add(mailType, subject, content, items)
	if not self.loaded then
		self:loadFromDb()
		self:clean()
	end

	self.id = self.id + 1

	local mail = mailsys.create(self.id, mailType, subject, content, items)
	table.insert(self.mails, mail)

	while (#self.mails >= MAIL_MAX) do
		unilight.debug("mails is more than " .. MAIL_MAX .. ", remove the first")
		table.remove(self.mails, 1)
	end

	return mail
end

function MailMgr:addNew(mailType, subject, content, items)
	local mail = self:add(mailType, subject, content, items)
	
	--push client
	local res = {}
	res["do"] = "Cmd.MailNewCmd_S"
	res["data"] = {
		mail = mail
	}

	unilight.response(self.owner.laccount, res)
end

function MailMgr:saveToDb()
	if not self.loaded then
		return
	end

	local data = {
		uid = self.owner.uid,
		id = self.id,
		mails = self.mails,
	}

	unilight.savedata(mailsys.MAIL_DB, data)
end

function MailMgr:loadFromDb()
	local data = unilight.getdata(mailsys.MAIL_DB, self.owner.uid)

	if data ~= nil then
		--Safty: In the case, data is not nil, but data.id or data.mails is nil
		if data.id ~= nil then
			self.id = data.id
		end

		if self.mails ~= nil then
			self.mails = data.mails
		end
	end

	self.loaded = true
end
