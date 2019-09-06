
PacketCode[4901] = {server = 4901, client = 4902, des = "cgyqsenter", func = "YqsEnter"}
PacketCode[4902] = {server = 4901, client = 4902, des = "gcyqsenter", func = "YqsEnter"}

PacketCode[4903] = {server = 4903, client = 4904, des = "cgyqsleave", func = "YqsLeave"}
PacketCode[4904] = {server = 4903, client = 4904, des = "gcyqsleave", func = "YqsLeave"}

PacketCode[4905] = {server = 4905, client = 4906, des = "cgyqsstart", func = "YqsStart"}
PacketCode[4906] = {server = 4905, client = 4906, des = "gcyqsstart", func = "YqsStart"}

PacketCode[4907] = {server = 4907, client = 4908, des = "cgyqscreatefish", func = "YqsCreateFish"}
PacketCode[4908] = {server = 4907, client = 4908, des = "gcyqscreatefish", func = "YqsCreateFish"}

PacketCode[4909] = {server = 4909, client = 4910, des = "cgyqscreatebullet", func = "YqsCreateBullet"}
PacketCode[4910] = {server = 4909, client = 4910, des = "gcyqscreatebullet", func = "YqsCreateBullet"}

PacketCode[4911] = {server = 4911, client = 4912, des = "cgyqschangecannon", func = "YqsChangeCannon"}
PacketCode[4912] = {server = 4911, client = 4912, des = "gcyqschangecannon", func = "YqsChangeCannon"}

PacketCode[4913] = {server = 4913, client = 4914, des = "cgyqscatchfish", func = "YqsCatchFish"}
PacketCode[4914] = {server = 4913, client = 4914, des = "gcyqscatchfish", func = "YqsCatchFish"}

-- 广播玩家进入房间
PacketCode[4915] = {server = 4915, client = 4916, des = "cgyqsenterbroadcast", func = "YqsEnterBroadcast"}
PacketCode[4916] = {server = 4915, client = 4916, des = "gcyqsenterbroadcast", func = "YqsEnterBroadcast"}

-- 回收
PacketCode[4917] = {server = 4917, client = 4918, des = "cgyqsreclaims", func = "YqsReclaims"}
PacketCode[4918] = {server = 4917, client = 4918, des = "gcyqsreclaims", func = "YqsReclaims"}

-- 潮汐
PacketCode[4919] = {server = 4919, client = 4920, des = "cgyqstide", func = "YqsCastTide"}
PacketCode[4920] = {server = 4919, client = 4920, des = "gcyqstide", func = "YqsCastTide"}

-- 使用物品
PacketCode[4921] = {server = 4921, client = 4922, des = "cgyqsuseitem", func = "YqsUseItem"}
PacketCode[4922] = {server = 4921, client = 4922, des = "gcyqsuseitem", func = "YqsUseItem"}

-- 获取信息
PacketCode[4923] = {server = 4923, client = 4924, des = "cgyqsgetinfo", func = "YqsGetInfo"}
PacketCode[4924] = {server = 4923, client = 4924, des = "gcyqsgetinfo", func = "YqsGetInfo"}

-- 潮汐结束
PacketCode[4925] = {server = 4925, client = 4926, des = "cgyqstideend", func = "YqsTideEnd"}
PacketCode[4926] = {server = 4925, client = 4926, des = "gcyqstideend", func = "YqsTideEnd"}

-- 鱼潮结束
PacketCode[4927] = {server = 4927, client = 4928, des = "cgyqsshoalend", func = "YqsShoalEnd"}
PacketCode[4928] = {server = 4927, client = 4928, des = "gcyqsshoalend", func = "YqsShoalEnd"}


ReturnCode["yqs_not_exist"] = 4901
ReturnCode[4901] = "该牌桌不存在"

ReturnCode["yqs_jetton_not_enough"] = 4902
ReturnCode[4902] = "金币不足"

ReturnCode["yqs_not_sit_user"] = 4903
ReturnCode[4903] = "观战玩家不能进行此操作"

ReturnCode["yqs_invalid_bullet_id"] = 4904
ReturnCode[4904] = "无效的子弹"

ReturnCode["yqs_cannon_min"] = 4905
ReturnCode[4905] = "炮台等级已达到最低等级"

ReturnCode["yqs_cannon_max"] = 4906
ReturnCode[4906] = "炮台等级已达到最高等级"

ReturnCode["yqs_invalid_item"] = 4907
ReturnCode[4907] = "无效的物品"

ReturnCode["yqs_invalid_cannon_type"] = 4908
ReturnCode[4908] = "切换炮台失败"

ReturnCode["yqs_table_type_error"] = 4909
ReturnCode[4909] = "牌桌类型错误"

ReturnCode["yqs_jetton_more"] = 4910
ReturnCode[4910] = "金币超出限制了，请到更高的场次游戏"
