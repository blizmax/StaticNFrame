-- FILE: dailyshare.xlsx SHEET: Sheet1 KEY: id
DailyShare = {
[1]={["id"]=1,["reward"]="2_100"},
[2]={["id"]=2,["reward"]="1_100"},
[3]={["id"]=3,["reward"]="2_1000"},
[4]={["id"]=4,["reward"]="2_10000"},
[5]={["id"]=5,["reward"]="1_1000"},
}
setmetatable(DailyShare, {__index = function(__t, __k) if __k == "query" then return function(id) return __t[id] end end end})
