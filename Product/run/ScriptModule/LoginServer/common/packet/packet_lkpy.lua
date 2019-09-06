
PacketCode[4701] = {server = 4701, client = 4702, des = "cglkpyenter", func = "LkpyEnter"}
PacketCode[4702] = {server = 4701, client = 4702, des = "gclkpyenter", func = "LkpyEnter"}

PacketCode[4703] = {server = 4703, client = 4704, des = "cglkpyleave", func = "LkpyLeave"}
PacketCode[4704] = {server = 4703, client = 4704, des = "gclkpyleave", func = "LkpyLeave"}

PacketCode[4705] = {server = 4705, client = 4706, des = "cglkpystart", func = "LkpyStart"}
PacketCode[4706] = {server = 4705, client = 4706, des = "gclkpystart", func = "LkpyStart"}

PacketCode[4707] = {server = 4707, client = 4708, des = "cglkpycreatefish", func = "LkpyCreateFish"}
PacketCode[4708] = {server = 4707, client = 4708, des = "gclkpycreatefish", func = "LkpyCreateFish"}

PacketCode[4709] = {server = 4709, client = 4710, des = "cglkpycreatebullet", func = "LkpyCreateBullet"}
PacketCode[4710] = {server = 4709, client = 4710, des = "gclkpycreatebullet", func = "LkpyCreateBullet"}

PacketCode[4711] = {server = 4711, client = 4712, des = "cglkpychangecannon", func = "LkpyChangeCannon"}
PacketCode[4712] = {server = 4711, client = 4712, des = "gclkpychangecannon", func = "LkpyChangeCannon"}

PacketCode[4713] = {server = 4713, client = 4714, des = "cglkpycatchfish", func = "LkpyCatchFish"}
PacketCode[4714] = {server = 4713, client = 4714, des = "gclkpycatchfish", func = "LkpyCatchFish"}

-- 广播玩家进入房间
PacketCode[4715] = {server = 4715, client = 4716, des = "cglkpyenterbroadcast", func = "LkpyEnterBroadcast"}
PacketCode[4716] = {server = 4715, client = 4716, des = "gclkpyenterbroadcast", func = "LkpyEnterBroadcast"}

-- 回收
PacketCode[4717] = {server = 4717, client = 4718, des = "cglkpyreclaims", func = "LkpyReclaims"}
PacketCode[4718] = {server = 4717, client = 4718, des = "gclkpyreclaims", func = "LkpyReclaims"}

-- 潮汐
PacketCode[4719] = {server = 4719, client = 4720, des = "cglkpytide", func = "LkpyCastTide"}
PacketCode[4720] = {server = 4719, client = 4720, des = "gclkpytide", func = "LkpyCastTide"}

-- 使用物品
PacketCode[4721] = {server = 4721, client = 4722, des = "cglkpyuseitem", func = "LkpyUseItem"}
PacketCode[4722] = {server = 4721, client = 4722, des = "gclkpyuseitem", func = "LkpyUseItem"}

-- 获取信息
PacketCode[4723] = {server = 4723, client = 4724, des = "cglkpygetinfo", func = "LkpyGetInfo"}
PacketCode[4724] = {server = 4723, client = 4724, des = "gclkpygetinfo", func = "LkpyGetInfo"}

-- 潮汐结束
PacketCode[4725] = {server = 4725, client = 4726, des = "cglkpytideend", func = "LkpyTideEnd"}
PacketCode[4726] = {server = 4725, client = 4726, des = "gclkpytideend", func = "LkpyTideEnd"}

-- 鱼潮结束
PacketCode[4727] = {server = 4727, client = 4728, des = "cglkpyshoalend", func = "LkpyShoalEnd"}
PacketCode[4728] = {server = 4727, client = 4728, des = "gclkpyshoalend", func = "LkpyShoalEnd"}

ReturnCode["lkpy_not_exist"] = 4701
ReturnCode[4701] = "该牌桌不存在"

ReturnCode["lkpy_jetton_not_enough"] = 4702
ReturnCode[4702] = "金币不足"

ReturnCode["lkpy_not_sit_user"] = 4703
ReturnCode[4703] = "观战玩家不能进行此操作"

ReturnCode["lkpy_invalid_bullet_id"] = 4704
ReturnCode[4704] = "无效的子弹"

ReturnCode["lkpy_cannon_min"] = 4705
ReturnCode[4705] = "炮台等级已达到最低等级"

ReturnCode["lkpy_cannon_max"] = 4706
ReturnCode[4706] = "炮台等级已达到最高等级"

ReturnCode["lkpy_invalid_item"] = 4707
ReturnCode[4707] = "无效的物品"

ReturnCode["lkpy_invalid_cannon_type"] = 4708
ReturnCode[4708] = "切换炮台失败"

ReturnCode["lkpy_table_type_error"] = 4709
ReturnCode[4709] = "牌桌类型错误"

ReturnCode["lkpy_jetton_more"] = 4710
ReturnCode[4710] = "金币超出限制了，请到更高的场次游戏"
