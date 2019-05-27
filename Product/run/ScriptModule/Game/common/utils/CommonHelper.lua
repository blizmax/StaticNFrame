
CommonHelper = {}

--从大到小
function CommonHelper.DBSortDesc(dbtable,index)

	index = index == nil and "id" or index
	index = dbtable["id"][index]
	
	local temp = {}
	for k, v in pairs(dbtable) do		
		if type(v) ~= "function" and type(v[index]) == "number" then
			table.insert(temp, v)
		end
	end
	
	table.sort(temp, function(a,b)
		return a[index] > b[index]
	end)
	return temp
end

--从小到大
function CommonHelper.DBSortAsc(dbtable,index)

	index = index == nil and "id" or index
	index = dbtable["id"][index]
	
	local temp = {}
	for k, v in pairs(dbtable) do
		if type(v) ~= "function" and type(v[index]) == "number" then
			table.insert(temp, v)
		end
	end
	table.sort(temp, function(a,b)
		return a[index] < b[index]
	end)
	return temp
end

--[[
	--从后面往前面找,找到最小的一个，把这个插入
	-- {2,5,6,8}
	--输入4
	--得到{2,4,5,6,8}
	--注意：不会对算法进行排序
]]
function table.AscInsert(t,value)
	
	table.insert(t,value)
	for i = #t, 2, -1 do
		if t[i] < t[i-1] then
			local temp = t[i]
			t[i] = t[i-1]
			t[i-1] = temp
		else
			break
		end
	end
end


--[[
	--从后面往前面找,找到最比小的一个，把这个插入
	-- {9,7,4,1}
	--输入6
	--得到{9,7,6,4,1}
	--注意：不会对算法进行排序
]]
function table.DescInsert(t,value)
	
	table.insert(t,value)
	for i = #value, 2, -1 do
		if t[i] > t[i-1] then
			local temp = t[i]
			t[i] = t[i-1]
			t[i-1] = temp
		else
			break
		end
	end
end
