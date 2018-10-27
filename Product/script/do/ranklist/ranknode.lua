require "script/gxlua/class"
require "script/do/common/Common"

CreateClass("RankNode")   --排行数据节点

--玩家的单个好友数据初始化
function RankNode:Init(uid, value)
    self.uid = uid or 0
    self.value = value or 0
end

function RankNode:GetUid()
    return self.uid
end

function RankNode:GetValue()
    return self.value
end