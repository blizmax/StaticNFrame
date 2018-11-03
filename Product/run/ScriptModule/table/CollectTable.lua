-- FILE: collect.xlsx SHEET: Sheet1
CollectTable = {
[1]={["id"]=1,["reward"]="1_100"},
}
setmetatable(CollectTable, {__index = function(__t, __k) if __k == "query" then return function(index) return __t[index] end end end})
