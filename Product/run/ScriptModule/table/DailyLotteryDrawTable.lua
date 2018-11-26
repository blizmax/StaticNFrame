-- FILE: welfare.xlsx SHEET: 转盘 KEY: id
DailyLotteryDrawTable = {
[1]={["id"]=1,["reward"]="1_30",["weight"]=20},
[2]={["id"]=2,["reward"]="1013_600",["weight"]=18},
[3]={["id"]=3,["reward"]="1001_1",["weight"]=18},
[4]={["id"]=4,["reward"]="1_50",["weight"]=12},
[5]={["id"]=5,["reward"]="2_88000",["weight"]=9},
[6]={["id"]=6,["reward"]="1_100",["weight"]=6},
[7]={["id"]=7,["reward"]="1013_1800",["weight"]=9},
[8]={["id"]=8,["reward"]="1001_2",["weight"]=9},
}
setmetatable(DailyLotteryDrawTable, {__index = function(__t, __k) if __k == "query" then return function(id) return __t[id] end end end})
