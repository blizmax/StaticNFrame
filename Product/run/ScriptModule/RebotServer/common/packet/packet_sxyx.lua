--5700~5800 是视讯百家乐的玩法

--进入
PacketCode[5801] = {server = 5801, client = 5802, des = "cgsxyxenter", func = "SxyxEnter"}
PacketCode[5802] = {server = 5801, client = 5802, des = "gcsxyxenter", func = "SxyxEnter"}

--离开
PacketCode[5803] = {server = 5803, client = 5804, des = "cgsxyxleave", func = "SxyxLeave"}
PacketCode[5804] = {server = 5803, client = 5804, des = "gcsxyxleave", func = "SxyxLeave"}

--坐下
PacketCode[5805] = {server = 5805, client = 5806, des = "cgsxyxsitdown", func = "SxyxSitDown"}
PacketCode[5806] = {server = 5805, client = 5806, des = "gcsxyxsitdown", func = "SxyxSitDown"}

--站起
PacketCode[5807] = {server = 5807, client = 5808, des = "cgsxyxstandup", func = "SxyxStandUp"}
PacketCode[5808] = {server = 5807, client = 5808, des = "gcsxyxstandup", func = "SxyxStandUp"}

--开始
PacketCode[5810] = {server = 5809, client = 5810, des = "gcsxyxstart", func = "SxyxStart"}

--下注
PacketCode[5811] = {server = 5811, client = 5812, des = "cgsxyxpourjetton", func = "SxyxPourJetton"}
PacketCode[5812] = {server = 5811, client = 5812, des = "gcsxyxpourjetton", func = "SxyxPourJetton"}

--无座玩家下注
PacketCode[5814] = {server = 5813, client = 5814, des = "gcsxyxgroupjetton", func = "SxyxGroupJetton"}

--撤回
PacketCode[5815] = {server = 5815, client = 5816, des = "cgsxyxrevocation", func = "SxyxRevocation"}
PacketCode[5816] = {server = 5815, client = 5816, des = "gcsxyxrevocation", func = "SxyxRevocation"}

--发牌
PacketCode[5818] = {server = 5817, client = 5818, des = "gcsxyxsendcard", func = "SxyxSendCard"}

--结算
PacketCode[5820] = {server = 5819, client = 5820, des = "gcsxyxcount", func = "SxyxCount"}

--下一局
PacketCode[5822] = {server = 5821, client = 5822, des = "gcsxyxnext", func = "SxyxNext"}

--历史记录
PacketCode[5823] = {server = 5823, client = 5824, des = "cgsxyxhistory", func = "SxyxHistory"}
PacketCode[5824] = {server = 5823, client = 5824, des = "gcsxyxhistory", func = "SxyxHistory"}


--获得房间列表
PacketCode[5825] = {server = 5825, client = 5826, des = "cgsxyxtablelist", func = "SxyxTableList"}
PacketCode[5826] = {server = 5825, client = 5826, des = "gcsxyxtablelist", func = "SxyxTableList"}

--创建房间
PacketCode[5827] = {server = 5827, client = 5828, des = "cgsxyxcreatetable", func = "SxyxCreateTable"}
PacketCode[5828] = {server = 5827, client = 5828, des = "gcsxyxcreatetable", func = "SxyxCreateTable"}

--上庄
PacketCode[5829] = {server = 5829, client = 5830, des = "cgsxyxbeingbanker", func = "SxyxBeingBanker"}
PacketCode[5830] = {server = 5829, client = 5830, des = "gcsxyxbeingbanker", func = "SxyxBeingBanker"}

--下庄
PacketCode[5831] = {server = 5831, client = 5832, des = "cgsxyxdropbanker", func = "SxyxDropBanker"}
PacketCode[5832] = {server = 5831, client = 5832, des = "gcsxyxdropbanker", func = "SxyxDropBanker"}

--更新不知道什么
PacketCode[5834] = {server = 5833, client = 5834, des = "gcsxyxupdate", func = "SxyxUpdate"}

--聊天
PacketCode[5835] = {server = 5835, client = 5836, des = "cgsxyxchat", func = "SxyxChat"}
PacketCode[5836] = {server = 5835, client = 5836, des = "gcsxyxchat", func = "SxyxChat"}

--站立玩家列表
PacketCode[5837] = {server = 5837, client = 5838, des = "cgsxyxstandlist", func = "SxyxStandList"}
PacketCode[5838] = {server = 5837, client = 5838, des = "gcsxyxstandlist", func = "SxyxStandList"}

--上庄列表
PacketCode[5839] = {server = 5839, client = 5840, des = "cgsxyxbankerlist", func = "SxyxBankerList"}
PacketCode[5840] = {server = 5839, client = 5840, des = "gcsxyxbankerlist", func = "SxyxBankerList"}

--这个是欢乐赢豆的统计
PacketCode[5841] = {server = 5841, client = 5842, des = "cgsxyxstats", func = "SxyxStats"}
PacketCode[5842] = {server = 5841, client = 5842, des = "gcsxyxstats", func = "SxyxStats"}

PacketCode[5843] = {server = 5843, client = 5844, des = "cgsxyxallgamehistory", func = "SxyxAllgameHistory"}
PacketCode[5844] = {server = 5843, client = 5844, des = "gcsxyxallgamehistory", func = "SxyxAllgameHistory"}

PacketCode[5845] = {server = 5845, client = 5846, des = "cgsxyxgamehistoryupdata", func = "SxyxGameHistoryUpdata"}
PacketCode[5846] = {server = 5845, client = 5846, des = "gcsxyxgamehistoryupdata", func = "SxyxGameHistoryUpdata"}

PacketCode[5847] = {server = 5847, client = 5848, des = "cgsxyxlegalcheck", func = "SxyxLegalCheck"}
PacketCode[5848] = {server = 5847, client = 5848, des = "cgsxyxlegalcheck", func = "SxyxLegalCheck"}

PacketCode[5849] = {server = 5849, client = 5850, des = "cgsxyxlegallist", func = "SxyxLegalList"}
PacketCode[5850] = {server = 5849, client = 5850, des = "cgsxyxlegallist", func = "SxyxLegalList"}
--
ReturnCode["sxyx_not_exist"] = 5801
ReturnCode["sxyx_chair_notmatch"] = 5802
ReturnCode["sxyx_chair_isplyaer"] = 5803
ReturnCode["sxyx_is_sitdown"] = 5804
ReturnCode["sxyx_pwd_notmatch"] = 5805
ReturnCode["sxyx_banker_exist"] = 5806
ReturnCode["sxyx_banker_exist"] = 5807
ReturnCode["sxyx_state_notpour"] = 5808
ReturnCode["sxyx_jetton_notenough"] = 5809
ReturnCode["sxyx_pourtype_error"] = 5810
ReturnCode["sxyx_sit_isbanker"] = 5811
ReturnCode["sxyx_banker_leave"] = 5812
ReturnCode["sxyx_behanker_list"] = 5813  --加入上庄列表
ReturnCode["sxyx_behanker_drop"] = 5814  --申请下庄
ReturnCode["sxyx_pour_banker"] = 5815  --庄家不能下注
ReturnCode["sxyx_banker_notenough"] = 5816  --筹码不足，不能上庄
ReturnCode["sxyx_pour_lose"] = 5817  --庄家不够赔钱了，不能下注
ReturnCode["sxyx_is_pour"] = 5818  --你已经在本局游戏中下注，请等待游戏结束再离开
ReturnCode["sxyx_is_full"] = 5819  --人已经满了
ReturnCode["sxyx_drop_success"] = 5820
ReturnCode["sxyx_not_banker"] = 5821
ReturnCode["sxyx_is_drop"] = 5822  
ReturnCode["sxyx_is_bankerlist"] = 5823
ReturnCode["sxyx_not_bankerJetton"] = 5824
ReturnCode["sxyx_is_driopbankererror"] = 5825
ReturnCode["sxyx_banker_cancel"] = 5826  
ReturnCode["sxyx_banker_exist_2"] = 5827 
ReturnCode["sxyx_not_bankerJetton_2"] = 5828 
ReturnCode["sxyx_legal_not"] = 5829
ReturnCode["sxyx_legal_notcheck"] = 5830
ReturnCode["sxyx_legal_checking"] = 5831
ReturnCode["sxyx_register_error"] = 5832
ReturnCode["sxyx_syn_info"] = 5833


ReturnCode[5801] = "该牌桌不存在"
ReturnCode[5802] = "坐下的位置不匹配"
ReturnCode[5803] = "该座位已经有玩家"
ReturnCode[5804] = "你已经坐下了，请勿重复操作"
ReturnCode[5805] = "牌桌密码不匹配"
ReturnCode[5806] = "庄家已经存在，上庄失败"
ReturnCode[5808] = "请稍后再下注"
ReturnCode[5809] = "您的筹码不足"
ReturnCode[5810] = "下注失败，请选择正确的下注区域"
ReturnCode[5811] = "坐下失败，你已经是庄家了！"
ReturnCode[5812] = "请先下庄再离开牌桌"
ReturnCode[5813] = "已加入上庄列表"
ReturnCode[5814] = "你已经申请下庄"
ReturnCode[5815] = "庄家不能下注"
ReturnCode[5816] = "筹码不足，不能上庄"
ReturnCode[5817] = "已达下注上限，下注失败"
ReturnCode[5818] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[5819] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[5820] = "申请下庄成功"  
ReturnCode[5821] = "您还不是庄家呢" 
ReturnCode[5822] = "您已经申请下庄了"   
ReturnCode[5823] = "您已经加入上庄列表中，请耐心等着"  
ReturnCode[5824] = "押注额过大庄家不够赔付"  
ReturnCode[5825] = "申请下庄失败， 要当庄3局后才可下庄"
ReturnCode[5826] = "取消申请上庄成功"  
ReturnCode[5827] = "您已经是庄家了， 请不要重复申请上上庄"
ReturnCode[5828] = "下注金币已经达到上限"
----------------------

ReturnCode[5829] = "查询的结果不存在"
ReturnCode[5830] = "牌局还没结束，请稍后查询"
ReturnCode[5831] = "正常查询中,请稍后..."
ReturnCode[5832] = "玩家注册失败"
ReturnCode[5833] = "同步信息成功"