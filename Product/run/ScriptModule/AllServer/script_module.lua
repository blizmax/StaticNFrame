ScriptModule = {}
function ScriptModule.Init(...)
	for i=1, #(ScriptList) do
		if (type(ScriptList[i].tbl.Init) == "function") then
			ScriptList[i].tbl.Init(...);
		end
	end
end

function ScriptModule.AfterInit(...)
	for i=1, #(ScriptList) do
		if (type(ScriptList[i].tbl.AfterInit) == "function") then
			ScriptList[i].tbl.AfterInit(...);
		end
	end
end

function ScriptModule.Execute(...)
	for i=1, #(ScriptList) do
		if (type(ScriptList[i].tbl.Execute) == "function") then
			ScriptList[i].tbl.Execute(...);
		end
	end
end

function ScriptModule.BeforeShut(...)
	for i=1, #(ScriptList) do
		if (type(ScriptList[i].tbl.BeforeShut) == "function") then
			ScriptList[i].tbl.BeforeShut(...);
		end
	end
end

function ScriptModule.Shut(...)
	for i=1, #(ScriptList) do
		if (type(ScriptList[i].tbl.Shut) == "function") then
			ScriptList[i].tbl.Shut(...);
		end
	end
end