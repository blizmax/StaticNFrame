


--3800~3900 是时时猜的玩法
PacketCode[3801] = {server = 3801, client = 3802, des = "cgsscaienter", func = "SscaiEnter"}
PacketCode[3802] = {server = 3801, client = 3802, des = "gcsscaienter", func = "SscaiEnter"}

PacketCode[3803] = {server = 3803, client = 3804, des = "cgsscaileave", func = "SscaiLeave"}
PacketCode[3804] = {server = 3803, client = 3804, des = "gcsscaileave", func = "SscaiLeave"}

PacketCode[3805] = {server = 3805, client = 3806, des = "cgsscaisitdown", func = "SscaiSitDown"}
PacketCode[3806] = {server = 3805, client = 3806, des = "gcsscaisitdown", func = "SscaiSitDown"}

PacketCode[3807] = {server = 3807, client = 3808, des = "cgsscaipourjetton", func = "SscaiPourJetton"}
PacketCode[3808] = {server = 3807, client = 3808, des = "gcsscaipourjetton", func = "SscaiPourJetton"}

PacketCode[3810] = {server = 3809, client = 3810, des = "gcsscaicount", func = "SscaiCount"}

PacketCode[3811] = {server = 3811, client = 3812, des = "cgsscaitablelist", func = "SscaiTableList"}
PacketCode[3812] = {server = 3811, client = 3812, des = "gcsscaitablelist", func = "SscaiTableList"}

PacketCode[3813] = {server = 3813, client = 3814, des = "cgsscaicreatetable", func = "SscaiCreateTable"}
PacketCode[3814] = {server = 3813, client = 3814, des = "gcsscaicreatetable", func = "SscaiCreateTable"}

PacketCode[3815] = {server = 3815, client = 3816, des = "cgsscaibeingbanker", func = "SscaiBeingBanker"}
PacketCode[3816] = {server = 3815, client = 3816, des = "gcsscaibeingbanker", func = "SscaiBeingBanker"}

PacketCode[3817] = {server = 3817, client = 3818, des = "cgsscaidropbanker", func = "SscaiDropBanker"}
PacketCode[3818] = {server = 3817, client = 3818, des = "gcsscaidropbanker", func = "SscaiDropBanker"}

PacketCode[3820] = {server = 3819, client = 3820, des = "gcsscaistart", func = "SscaiStart"}

PacketCode[3822] = {server = 3821, client = 3822, des = "gcsscaiupdate", func = "SscaiUpdate"}

PacketCode[3824] = {server = 3823, client = 3824, des = "gcsscaistandup", func = "SscaiStandUp"}

PacketCode[3826] = {server = 3825, client = 3826, des = "gcsscaigroupjetton", func = "SscaiGroupJetton"}

PacketCode[3827] = {server = 3827, client = 3828, des = "cgsscaihistory", func = "SscaiHistory"}
PacketCode[3828] = {server = 3827, client = 3828, des = "gcsscaihistory", func = "SscaiHistory"}

PacketCode[3829] = {server = 3829, client = 3830, des = "cgsscaichat", func = "SscaiChat"}
PacketCode[3830] = {server = 3829, client = 3830, des = "gcsscaichat", func = "SscaiChat"}

PacketCode[3831] = {server = 3831, client = 3832, des = "cgsscaistandlist", func = "SscaiStandList"}
PacketCode[3832] = {server = 3831, client = 3832, des = "gcsscaistandlist", func = "SscaiStandList"}

PacketCode[3833] = {server = 3833, client = 3834, des = "cgsscaiplayerlist", func = "SscaiPlayerList"}
PacketCode[3834] = {server = 3833, client = 3834, des = "gcsscaiplayerlist", func = "SscaiPlayerList"}

PacketCode[3835] = {server = 3835, client = 3836, des = "cgsscaistats", func = "SscaiStats"}
PacketCode[3836] = {server = 3835, client = 3836, des = "gcsscaistats", func = "SscaiStats"}

--PacketCode[3837] = {server = 3837, client = 3838, des = "cgfruitupdatepour", func = "FruitUpdatePour"}
PacketCode[3838] = {server = 3837, client = 3838, des = "gcsscaiupdatepour", func = "SscaiUpdatePour"}



PacketCode[3839] = {server = 3839, client = 3840, des = "cgsscaibiglist", func = "SscaiBigList"}
PacketCode[3840] = {server = 3839, client = 3840, des = "gcsscaibiglist", func = "SscaiBigList"}

--
ReturnCode["sscai_not_exist"] = 3801
ReturnCode["sscai_chair_notmatch"] = 3802
ReturnCode["sscai_chair_isplyaer"] = 3803
ReturnCode["sscai_is_sitdown"] = 3804
ReturnCode["sscai_pwd_notmatch"] = 3805
ReturnCode["sscai_banker_exist"] = 3806
ReturnCode["sscai_banker_exist"] = 3807
ReturnCode["sscai_state_notpour"] = 3808
ReturnCode["sscai_jetton_notenough"] = 3809
ReturnCode["sscai_pourtype_error"] = 3810
ReturnCode["sscai_sit_isbanker"] = 3811
ReturnCode["sscai_banker_leave"] = 3812
ReturnCode["sscai_behanker_list"] = 3813  --加入上庄列表
ReturnCode["sscai_behanker_drop"] = 3814  --申请下庄
ReturnCode["sscai_pour_banker"] = 3815  --庄家不能下注
ReturnCode["sscai_banker_notenough"] = 3816  --筹码不足，不能上庄
ReturnCode["sscai_pour_lose"] = 3817  --庄家不够赔钱了，不能下注
ReturnCode["sscai_is_pour"] = 3818  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["sscai_is_full"] = 3819  --人已经满了
ReturnCode["sscai_state_error"] = 3820  --人已经满了
ReturnCode["sscai_not_banker"] = 3621  
ReturnCode["sscai_is_drop"] = 3622  --已经申请下庄
ReturnCode["sscai_drop_success"] = 3623  --申请下庄成功
ReturnCode["sscai_banker_limit"] = 3624



ReturnCode[3801] = "该牌桌不存在"
ReturnCode[3802] = "坐下的位置不匹配"
ReturnCode[3803] = "该座位已经有玩家"
ReturnCode[3804] = "你已经坐下了，请勿重复操作"
ReturnCode[3805] = "牌桌密码不匹配"
ReturnCode[3806] = "庄家已经存在，上庄失败"
ReturnCode[3808] = "请稍后再下注"
ReturnCode[3809] = "您的筹码不足"
ReturnCode[3810] = "下注失败，请选择正确的下注区域"
ReturnCode[3811] = "坐下失败，你已经是庄家了！"
ReturnCode[3812] = "请先下庄再离开牌桌"
ReturnCode[3813] = "已加入上庄列表"
ReturnCode[3814] = "你已经申请下庄"
ReturnCode[3815] = "庄家不能下注"
ReturnCode[3816] = "筹码不足，不能上庄"
ReturnCode[3817] = "已达下注上限，下注失败"
ReturnCode[3818] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[3819] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[3820] = "现在不是下注时间，请稍后在下注哦"
ReturnCode[3621] = "你还不是庄家呢"
ReturnCode[3622] = "你已经申请下庄了，请勿重复操作"
ReturnCode[3623] = "申请下庄成功"
ReturnCode[3624] = "金币还差一点，请先到商城中购买然后再过来吧"
----------------------