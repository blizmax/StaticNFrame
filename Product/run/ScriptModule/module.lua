
function register_module(tbl, name)
	local tmp = {tbl = tbl, tblName = name}
	table.insert(LuaNFrame.ScriptList, tmp)
	unilight.debug("load module:" .. name)
end


ScriptModule = {}
function ScriptModule.Init(...)
	ProFi = require 'ProFi'
	ProFi:start()

	--local mri = require("MemoryReferenceInfo")

	-- Set config.
	--mri.m_cConfig.m_bAllMemoryRefFileAddTime = false
	--mri.m_cConfig.m_bSingleMemoryRefFileAddTime = false
	--mri.m_cConfig.m_bComparedMemoryRefFileAddTime = false
	
	-- 打印当前 Lua 虚拟机的所有内存引用快照到文件(或者某个对象的所有引用信息快照)到本地文件。
	-- strSavePath - 快照保存路径，不包括文件名。
	-- strExtraFileName - 添加额外的信息到文件名，可以为 "" 或者 nil。
	-- nMaxRescords - 最多打印多少条记录，-1 打印所有记录。
	-- strRootObjectName - 遍历的根节点对象名称，"" 或者 nil 时使用 tostring(cRootObject)
	-- cRootObject - 遍历的根节点对象，默认为 nil 时使用 debug.getregistry()。
	-- MemoryReferenceInfo.m_cMethods.DumpMemorySnapshot(strSavePath, strExtraFileName, nMaxRescords, strRootObjectName, cRootObject)
	--collectgarbage("collect")
	--mri.m_cMethods.DumpMemorySnapshot("./logs/".. LuaNFrame.app_dir, "1-Before", -1)

	for i=1, #(LuaNFrame.ScriptList) do
		if (type(LuaNFrame.ScriptList[i].tbl.Init) == "function") then
			LuaNFrame.ScriptList[i].tbl.Init(...);
		end
	end
end

function ScriptModule.AfterInit(...)
	for i=1, #(LuaNFrame.ScriptList) do
		if (type(LuaNFrame.ScriptList[i].tbl.AfterInit) == "function") then
			LuaNFrame.ScriptList[i].tbl.AfterInit(...);
		end
	end
end

function ScriptModule.Execute(...)
	for i=1, #(LuaNFrame.ScriptList) do
		if (type(LuaNFrame.ScriptList[i].tbl.Execute) == "function") then
			LuaNFrame.ScriptList[i].tbl.Execute(...);
		end
	end
end

function ScriptModule.BeforeShut(...)
	for i=1, #(LuaNFrame.ScriptList) do
		if (type(LuaNFrame.ScriptList[i].tbl.BeforeShut) == "function") then
			LuaNFrame.ScriptList[i].tbl.BeforeShut(...);
		end
	end
end

function ScriptModule.Shut(...)
	for i=1, #(LuaNFrame.ScriptList) do
		if (type(LuaNFrame.ScriptList[i].tbl.Shut) == "function") then
			LuaNFrame.ScriptList[i].tbl.Shut(...);
		end
	end

	--local mri = require("MemoryReferenceInfo")
	--mri.m_cBases.OutputFilteredResult("./logs/"..LuaNFrame.app_dir.."/LuaMemRefInfo-All-[1-Before].txt", "ScriptModule", true, true)

	ProFi:stop()
	ProFi:writeReport( "./logs/"..LuaNFrame.app_dir.."/MyProfilingReport.txt" )
end