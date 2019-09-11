
--3900~4000 是斗地主的协议

--获取玩家的自己创建的房间列表
PacketCode[3901] = {server = 3901, client = 3902, des = "cgddztablelist", func = "DdzTableList"}
PacketCode[3902] = {server = 3901, client = 3902, des = "gcddztablelist", func = "DdzTableList"}

--创建房间
PacketCode[3903] = {server = 3903, client = 3904, des = "cgddzcreate", func = "DdzCreate"}
PacketCode[3904] = {server = 3903, client = 3904, des = "gcddzcreate", func = "DdzCreate"}

--加入房间
PacketCode[3905] = {server = 3905, client = 3906, des = "cgddzenter", func = "DdzEnter"}
PacketCode[3906] = {server = 3905, client = 3906, des = "gcddzenter", func = "DdzEnter"}

--坐下
PacketCode[3907] = {server = 3907, client = 3908, des = "cgddzsitdown", func = "DdzSitDown"}
PacketCode[3908] = {server = 3907, client = 3908, des = "gcddzsitdown", func = "DszSitDown"}

--离开
PacketCode[3909] = {server = 3909, client = 3910, des = "cgddzleave", func = "DdzLeave"}
PacketCode[3910] = {server = 3909, client = 3910, des = "gcddzleave", func = "DdzLeave"}

--站起
PacketCode[3911] = {server = 3911, client = 3912, des = "cgddzstandup", func = "DdzStandUp"}
PacketCode[3912] = {server = 3911, client = 3912, des = "gcddzstandup", func = "DdzStandUp"}

--申请解散
PacketCode[3913] = {server = 3913, client = 3914, des = "cgddzdissolve", func = "DdzDissolve"}
PacketCode[3914] = {server = 3913, client = 3914, des = "gcddzdissolve", func = "DdzDissolve"}

--回应申请解散
PacketCode[3915] = {server = 3915, client = 3916, des = "cgddzdissolveopt", func = "DdzDissolveOpt"}
PacketCode[3916] = {server = 3915, client = 3916, des = "gcddzdissolveopt", func = "DdzDissolveOpt"}

--聊天
PacketCode[3917] = {server = 3917, client = 3918, des = "cgddzchat", func = "DdzChat"}
PacketCode[3918] = {server = 3917, client = 3918, des = "gcddzchat", func = "DdzChat"}

--开始
PacketCode[3919] = {server = 3919, client = 3920, des = "cgddzstart", func = "DdzStart"}
PacketCode[3920] = {server = 3919, client = 3920, des = "gcddzstart", func = "DdzStart"}

--通知行动玩家
PacketCode[3922] = {server = 3921, client = 3922, des = "gcddzinformaction", func = "DdzInformAction"}

--玩家的行动
PacketCode[3923] = {server = 3923, client = 3924, des = "cgddzaction", func = "DdzAction"}
PacketCode[3924] = {server = 3923, client = 3924, des = "gcddzaction", func = "DdzAction"}

--通知客户端隐藏按钮
PacketCode[3926] = {server = 3925, client = 3926, des = "gcddzconcealbutton", func = "DdzConcealButton"}

--确定地主
PacketCode[3928] = {server = 3927, client = 3928, des = "gcddzlandlordout", func = "DdzLandlordOut"}

--出牌
PacketCode[3929] = {server = 3929, client = 3930, des = "cgddzplay", func = "DdzPlay"}
PacketCode[3930] = {server = 3929, client = 3930, des = "gcddzplay", func = "DdzPlay"}

--结算
PacketCode[3932] = {server = 3931, client = 3932, des = "gcddzcount", func = "DdzCount"}

--准备
PacketCode[3933] = {server = 3933, client = 3934, des = "cgddzready", func = "DdzReady"}
PacketCode[3934] = {server = 3933, client = 3934, des = "gcddzready", func = "DdzReady"}

--战绩
PacketCode[3935] = {server = 3935, client = 3936, des = "cgddzhistory", func = "DdzHistory"}
PacketCode[3936] = {server = 3935, client = 3936, des = "gcddzhistory", func = "DdzHistory"}

--回放
PacketCode[3937] = {server = 3937, client = 3938, des = "cgddzhistorystep", func = "DdzHistoryStep"}
PacketCode[3938] = {server = 3937, client = 3938, des = "gcddzhistorystep", func = "DdzHistoryStep"}

--托管
PacketCode[3939] = {server = 3939, client = 3940, des = "cgddzautoplayer", func = "DdzAutoplayer"}
PacketCode[3940] = {server = 3939, client = 3940, des = "gcddzautoplayer", func = "DdzAutoplayer"}


--确定癞子
PacketCode[3942] = {server = 3941, client = 3942, des = "gcddzwildcard", func = "DdzWildCard"}

--牌型检测
PacketCode[3944] = {server = 3943, client = 3944, des = "gcddzplaycheckoutresult", func = "DdzPlayCheckoutResult"}

--等待下一局游戏开始
PacketCode[3946] = {server = 3945, client = 3946, des = "gcddznext", func = "DdzNext"}

--加倍
PacketCode[3947] = {server = 3947, client = 3948, des = "cgddzdouble", func = "DdzDouble"}
PacketCode[3948] = {server = 3947, client = 3948, des = "gcddzdouble", func = "DdzDouble"}

--换桌
PacketCode[3949] = {server = 3949, client = 3950, des = "cgddzvarytable", func = "DdzVaryTable"}
PacketCode[3950] = {server = 3949, client = 3950, des = "gcddzvarytable", func = "DdzVaryTable"}


ReturnCode["ddz_not_exist"] = 3901
ReturnCode["ddz_create_fail"] = 3902 
ReturnCode["ddz_full"] = 3903   
ReturnCode["ddz_sit_fail"] = 3905  
ReturnCode["ddz_leave_playing"] = 3906   
ReturnCode["ddz_leave_error"] = 3907 
ReturnCode["ddz_dissolve_fail"] = 3908   
ReturnCode["ddz_dissolve_fail2"] = 3909   
ReturnCode["ddz_dissolve_fail3"] = 3910  
ReturnCode["ddz_opt_error"] = 3911   
ReturnCode["ddz_action_no"] = 3912   
ReturnCode["ddz_poker_error"] = 3913     
ReturnCode["ddz_not_sit"] = 3914  
ReturnCode["ddz_sitdown_fail"] = 3915   
ReturnCode["ddz_stand_error"] = 3916   
ReturnCode["ddz_doNot_error"] = 3917   
ReturnCode["ddz_play_poker_error"] = 3918   
ReturnCode["ddz_poker_Type_error"] = 3919
ReturnCode["ddz_play_poker_error_2"] = 3920
ReturnCode["ddz_jetton_not_enough"] = 3921
ReturnCode["ddz_jetton_error_1"] = 3922		

ReturnCode[3901] = "房间已经解散"
ReturnCode[3902] = "创建房间失败"
ReturnCode[3903] = "该牌桌已经满桌，请选择其他牌桌"
ReturnCode[3905] = "钻石不足"
ReturnCode[3906] = "牌局正在进行中，请稍后退出"
ReturnCode[3907] = "请在游戏结束后离开房间"
ReturnCode[3908] = "解散失败"
ReturnCode[3909] = "只有房主才能解散房间"
ReturnCode[3910] = "只有座位上的玩家才能申请解散房间"
ReturnCode[3911] = "操作错误"
ReturnCode[3912] = "还没有轮到你，请稍后再操作"
ReturnCode[3913] = "出牌失败，请重新出牌"
ReturnCode[3914] = "只有坐下的玩家才能准备"
ReturnCode[3915] = "坐下失败"
ReturnCode[3916] = "请等待牌局结束再站起"
ReturnCode[3917] = "上一家就是您出的牌，您不能过"
ReturnCode[3918] = "出的牌不在你的手牌列表中"
ReturnCode[3919] = "牌型错误"
ReturnCode[3920] = "你的牌打不过对方"
ReturnCode[3921] = "金币不足"
ReturnCode[3922] = "金币太多了, 请到更高的场次去玩吧"	