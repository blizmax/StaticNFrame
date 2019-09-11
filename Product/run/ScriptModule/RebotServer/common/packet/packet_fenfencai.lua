--7200~7300 是分分彩

--进入房间
PacketCode[7201] = {server = 7201, client = 7202, des = "cgfenfencaienter", func = "FenFenCaiEnter"}
PacketCode[7202] = {server = 7201, client = 7202, des = "gcfenfencaienter", func = "FenFenCaiEnter"}

--坐下
PacketCode[7203] = {server = 7203, client = 7204, des = "cgfenfencaisitdown", func = "FenFenCaiSitDown"}
PacketCode[7204] = {server = 7203, client = 7204, des = "gcfenfencaisitdown", func = "FenFenCaiSitDown"}

--站起
PacketCode[7206] = {server = 7205, client = 7206, des = "gcfenfencaistandup", func = "FenFenCaiStandUp"}

--离开
PacketCode[7207] = {server = 7207, client = 7208, des = "cgfenfencaileave", func = "FenFenCaiLeave"}
PacketCode[7208] = {server = 7207, client = 7208, des = "gcfenfencaileave", func = "FenFenCaiLeave"}

--下注
PacketCode[7209] = {server = 7209, client = 7210, des = "cgfenfencaipourjetton", func = "FenFenCaiPourJetton"}
PacketCode[7210] = {server = 7209, client = 7210, des = "gcfenfencaipourjetton", func = "FenFenCaiPourJetton"}

--无座玩家下注
PacketCode[7212] = {server = 7211, client = 7212, des = "gcfenfencaigroupjetton", func = "FenFenCaiGroupJetton"}

--续投
PacketCode[7213] = {server = 7213, client = 7214, des = "cgfenfencaipourjettonxt", func = "FenFenCaiPourJettonXt"}
PacketCode[7214] = {server = 7213, client = 7214, des = "gcfenfencaipourjettonxt", func = "FenFenCaiPourJettonXt"}

--结算
PacketCode[7216] = {server = 7215, client = 7216, des = "gcfenfencaicount", func = "FenFenCaiCount"}

--上庄
PacketCode[7217] = {server = 7217, client = 7218, des = "cgfenfencaibeingbanker", func = "FenFenCaiBeingBanker"}
PacketCode[7218] = {server = 7217, client = 7218, des = "gcfenfencaibeingbanker", func = "FenFenCaiBeingBanker"}

--下庄
PacketCode[7219] = {server = 7219, client = 7220, des = "cgfenfencaidropbanker", func = "FenFenCaiDropBanker"}
PacketCode[7220] = {server = 7219, client = 7220, des = "gcfenfencaidropbanker", func = "FenFenCaiDropBanker"}

--开始
PacketCode[7222] = {server = 7221, client = 7222, des = "gcfenfencaistart", func = "FenFenCaiStart"}

--开奖记录
PacketCode[7223] = {server = 7223, client = 7224, des = "cgfenfencaihistory", func = "FenFenCaiHistory"}
PacketCode[7224] = {server = 7223, client = 7224, des = "gcfenfencaihistory", func = "FenFenCaiHistory"}

--聊天
PacketCode[7225] = {server = 7225, client = 7226, des = "cgfenfencaichat", func = "FenFenCaiChat"}
PacketCode[7226] = {server = 7225, client = 7226, des = "gcfenfencaichat", func = "FenFenCaiChat"}

--玩家列表
PacketCode[7227] = {server = 7227, client = 7228, des = "cgfenfencaistandlist", func = "FenFenCaiStandList"}
PacketCode[7228] = {server = 7227, client = 7228, des = "gcfenfencaistandlist", func = "FenFenCaiStandList"}

--上庄列表
PacketCode[7229] = {server = 7229, client = 7230, des = "cgfenfencaibankerlist", func = "FenFenCaiBankerList"}
PacketCode[7230] = {server = 7229, client = 7230, des = "gcfenfencaibankerlist", func = "FenFenCaiBankerList"}

--发送正在获取开奖结果
PacketCode[7232] = {server = 7231, client = 7232, des = "gcfenfencaisendgetresult", func = "FenFenCaiSendgetResult"}

--更新状态
PacketCode[7234] = {server = 7233, client = 7234, des = "gcfenfencaiupdate", func = "FenFenCaiUpdate"}

--玩家下注记录
PacketCode[7235] = {server = 7235, client = 7236, des = "cgfenfencaiuserhistory", func = "FenFenCaiUserHistory"}
PacketCode[7236] = {server = 7235, client = 7236, des = "gcfenfencaiuserhistory", func = "FenFenCaiUserHistory"}

--请求挂机状态
PacketCode[7237] = {server = 7237, client = 7238, des = "cgfenfencaigethangupstate", func = "FenFenCaiGetHangUpState"}
PacketCode[7238] = {server = 7237, client = 7238, des = "gcfenfencaigethangupstate", func = "FenFenCaiGetHangUpState"}


--开始或取消挂机
PacketCode[7239] = {server = 7239, client = 7240, des = "cgfenfencaisethangupstate", func = "FenFenCaiSetHangUpState"}
PacketCode[7240] = {server = 7239, client = 7240, des = "gcfenfencaisethangupstate", func = "FenFenCaiSetHangUpState"}

--
ReturnCode["fenfencai_not_exist"] = 7201
ReturnCode["fenfencai_chair_notmatch"] = 7202
ReturnCode["fenfencai_chair_isplyaer"] = 7203
ReturnCode["fenfencai_is_sitdown"] = 7204
ReturnCode["fenfencai_pwd_notmatch"] = 7205
ReturnCode["fenfencai_banker_exist"] = 7206
ReturnCode["fenfencai_banker_exist"] = 7207
ReturnCode["fenfencai_state_notpour"] = 7208
ReturnCode["fenfencai_jetton_notenough"] = 7209
ReturnCode["fenfencai_pourtype_error"] = 7210
ReturnCode["fenfencai_sit_isbanker"] = 7211
ReturnCode["fenfencai_banker_leave"] = 7212
ReturnCode["fenfencai_behanker_list"] = 7213  --加入上庄列表
ReturnCode["fenfencai_behanker_drop"] = 7214  --申请下庄
ReturnCode["fenfencai_pour_banker"] = 7215  --庄家不能下注
ReturnCode["fenfencai_banker_notenough"] = 7216  --筹码不足，不能上庄
ReturnCode["fenfencai_pour_lose"] = 7217  --庄家不够赔钱了，不能下注
ReturnCode["fenfencai_is_pour"] = 7218  --你已经在本局游戏中下注，请等待游戏结束再离开

ReturnCode["fenfencai_is_full"] = 7219  --人已经满了
ReturnCode["fenfencai_pourlimit"] = 7220 --限红200000
ReturnCode["fenfencai_is_bankerlist"] = 7221  --已经在上庄列表中，请耐心等候
ReturnCode["fenfencai_is_drop"] = 7222  -- 您已经申请下庄了
ReturnCode["fenfencai_drop_success"] = 7223  --申请下庄成功
ReturnCode["fenfencai_not_banker"] = 7224  --你还不是庄家呢
ReturnCode["fenfencai_xt_success"] = 7225  --续投成功
ReturnCode["fenfencai_xt_fail"] = 7226  --续投失败
ReturnCode["fenfencai_pour_full"] = 7227  --该区域投注额度已满
ReturnCode["fenfencai_hangup_ban"] = 7228  --庄家不能挂机
ReturnCode["fenfencai_hangup_ing"] = 7229  --您已经在挂机中，请不要重复操作
ReturnCode["fenfencai_hangup_err"] = 7230  --请设置挂机的参数



ReturnCode[7201] = "该牌桌不存在"
ReturnCode[7202] = "坐下的位置不匹配"
ReturnCode[7203] = "该座位已经有玩家"
ReturnCode[7204] = "你已经坐下了，请勿重复操作"
ReturnCode[7205] = "牌桌密码不匹配"
ReturnCode[7206] = "庄家已经存在，上庄失败"
ReturnCode[7208] = "请稍后再下注"
ReturnCode[7209] = "您的筹码不足"
ReturnCode[7210] = "下注失败，请选择正确的下注区域"
ReturnCode[7211] = "坐下失败，你已经是庄家了！"
ReturnCode[7212] = "请先下庄再离开牌桌"
ReturnCode[7213] = "已加入上庄列表"
ReturnCode[7214] = "你已经申请下庄"
ReturnCode[7215] = "庄家不能下注"
ReturnCode[7216] = "金币不足，最低上庄需要3000金币"
ReturnCode[7217] = "已达下注上限，下注失败"
ReturnCode[7218] = "你已在本局游戏中下注，请等待游戏结束再离开"
ReturnCode[7219] = "该牌桌人数已满，请选择其他牌桌"
ReturnCode[7220] = "该玩家在该区域的下注额度大于最大限制"
ReturnCode[7221] = "已经在上庄列表中，请耐心等候"
ReturnCode[7222] = "您已经申请下庄了"  
ReturnCode[7223] = "申请下庄成功" 
ReturnCode[7224] = "你还不是庄家呢"

ReturnCode[7225] = "续投成功"
ReturnCode[7226] = "续投失败"
ReturnCode[7227] = "该区域投注额度已满!"
ReturnCode[7228] = "庄家不能挂机!"
ReturnCode[7229] = "您已经在挂机中，请不要重复操作!"
ReturnCode[7230] = "请设置挂机的参数"

----------------------