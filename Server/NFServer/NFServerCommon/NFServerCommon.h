// -------------------------------------------------------------------------
//    @FileName         :    NFServerCommon.h
//    @Author           :    GaoYi
//    @Date             :    2018/06/11
//    @Email			:    445267987@qq.com
//    @Module           :    NFServerCommon
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFConfigMgr.h"

class NFServerCommon
{
public:
	static NFServerConfig* GetServerConfig(NFIPluginManager* pPluginManager, NF_SERVER_TYPES eServerType);
};

class NFProxyData
{
public:
	NFProxyData()
	{
		mServerId = 0;
		mLinkId = 0;
	}

	uint32_t mServerId;
	uint32_t mLinkId;

	virtual void print() { }
};

enum NFMsgIdSection
{
	emsgid_net_begin = 1,
	emsgid_net_end = 100,

	emsgid_login_begin = 100,
	emsgid_login_end = 200,

	emsgid_center_begin = 200,
	emsgid_center_end = 300,

	emsgid_gateway_begin = 300,
	eMap2Gateway_BuildConncet		= 351, //建立连接
	eMap2Gateway_WorldMsg			= 352, //建立连接
	eMap2Gateway_NotifyOffline	= 353, //通知玩家下线

	eMap2Gateway_AOISync			= 354, 
	eMap2Gateway_UpdateLink		= 355,
    eMap2Gateway_Broadcast        = 356, //广播消息
	emsgid_gateway_end = 400,

	emsgid_db_begin = 400,
	emsgid_db_end = 900,

	emsgid_platform_begin = 900,
	emsgid_platform_end = 1100,

	emsgid_chat_begin = 1100,
	emsgid_chat_end = 1200,

	emsgid_match_begin = 1200,
	emsgid_match_end = 1300,

	emsgid_pvp_begin = 1300,
	emsgid_pvp_end = 1400,

	emsgid_map_begin = 1400,
	//这两条协议，在网关需要获取IP地址
	eLogin_FGW_QueryPlayerData			= 2001,					//获取玩家数据
	eLogin_FGW_CreateChar				= 2002,

	emsgid_map_end = 5000,

	//game ->  match -> battle
	emsgid_battle_begin = emsgid_map_end,
	eGame2Battle_EnterScene = 5000,
	eGame2Battle_SynSkill_Info = 5001,			//技能升级信息以及伤害同步
	eGame2Battle_SynSkill_Del = 5002,				//删除技能(副手技能更换)
	eGame2Battle_SynSkill_Pos = 5003,				//只能位置更换
	eGame2Battle_SynPlayer_Data = 5004,		//玩家基础数据同步
	eGame2Battle_SyncTower_Data = 5005,			//玩家请求进入爬塔
	eGame2Battle_SyncRelive = 5006,    					//死亡复活
	eGame2Battle_PlayerDisconnect = 5007,		//玩家断开连接
    eGame2Battle_SyncGuild  = 5008,                 //同步玩家公会信息
	eGame2Battle_SyncFacade = 5009,				//同步玩家成长系统外观
	eGame2Battle_ChangeTttle = 5010,				//同步称号变更
	eGame2Battle_ChangeEquip = 5011,				//同步装备变更
	eGame2Battle_JoinGuildBossSecen = 5012,		//创建公会boss场景
    eGame2Battle_ModPlayerData = 5013,            //修改玩家数据(GM命令用)
	eGame2Battle_EnterDungeon = 5020,
	eGame2Battle_ChangeMap = 5021,
	eGame2Battle_UpdatePlayerValue = 5022,	//更新玩家数据
	eGame2Battle_CheckTrigger = 5023,	//检查触发器
	eGame2Battle_AddQuestTrigger = 5024,	//增加触发器
	eGame2Battle_UpdateTask = 5025,	//更新任务
	eGame2Battle_QuestNPCAddBuff = 5026,	//任务触发  给指定NPC加buff
	eGame2Battle_WorldBossStart = 5027,	//通知世界boss开始
	eGame2Battle_WorldBossBid = 5028,	//通知世界boss出价
	eGame2Battle_ChangeAwaken = 5029,   //开始变身
	eGame2Battle_ChangePet = 5030, //
	eGame2Battle_ChangeRide = 5031, //
	eGame2Battle_AddCurHp = 5032, //
	eGame2Battle_AddBuff = 5033, //
	eGame2Battle_BornMercenary = 5034, // 创建佣兵
	eGame2Battle_TicketBossChallenge = 5035, //挑战门票boss
	eGame2Battle_TakeDropItem = 5036, //拾取掉落物品
	eGame2Battle_CreateDungeon = 5037, //多人副本用 创建副本
	eGame2Battle_CreateMonster = 5038, //game上召唤怪物
	eGame2Battle_ChangeAvatar = 5039, //玩家时装同步
	eGame2Battle_FollowLeader = 5040,//切换到队长分线
	eGame2Battle_ChangeMapPendingBranch= 5041,//预定切换分线
	eGame2Battle_BackOnline = 5042,
	eGame2Battle_UpdateHeadInfo = 5043,				//同步称号变更
	eGame2Battle_UpdateMercenaryScript = 5044, //同步佣兵技能
	eGame2Battle_UpdateRideScript = 5045, //同步佣兵技能
	eGame2Battle_UpdateAwakenScript = 5046, //同步觉醒技能
	eGame2Battle_UpdateMyCombat = 5047,//同步自己的战斗力
    eGame2Battle_CaptureFlag = 5049,		//拔旗子
	eGame2Battle_LeaveScene = 5050,		//离开之前的场景
	eGame2Battle_QuietFlagStart = 5051, //通知沉静之旗开始
	eGame2Battle_NotifyGuild = 5052,	//通知公会改变
	eGame2Battle_UpdateGodArmScript = 5053,	//通知神兵技能
	eGame2Battle_UpdateWingScript = 5054,	//通知翅膀技能
	eGame2Battle_CreateGBPVP = 5055,	//通知创建工会战PVP战场
	eGame2Battle_GuildBattle_Strengthen = 5056,	//公会争霸玩家购买强化功能
	eGame2Battle_DeleteBuff = 5057,	//删除buff
	eGame2Battle_ReqOpenTicketBox = 5058,	//打开门票BOSS宝箱
	eGame2Battle_ModelChange = 5059, //玩家模型切换(幻化)
    
	//client->  battle
	emsgid_client2battle_begin = 5200,
	eClient2Battle_ClientLoadOK = 5200,
	eClient2Battle_SyncMove = 5201,
	eClient2Battle_SpeelBegin = 5202, //切换地图层
	eClient2Battle_SpeelEnd = 5203,     //撤换地图层结束
	eClient2Battle_GetMapBranch = 5204,	    //获取分线列表
	eClient2Battle_ChangeBranch = 5205,      //跳分线
	eClient2Battle_TriggerEvent = 5207,	//触发事件(信仰之跃、进出草丛、草车等)
	eClient2Battle_GameStoryStart = 5208, 	//剧情副本开始
	eClient2Battle_GameStoryEnd = 5209, 	//剧情副本结束
	eClient2Battle_NPCStopAI = 5210, 	//停止NPC AI
	eClient2Battle_NPCActiveAI = 5211, 	//激活NPC AI
	eClient2Battle_Dungeon_PassStory = 5213,	//跳过剧情
	eClient2Battle_Dungeon_Event = 5214, //触发事件
	eClient2Battle_Tower_EnterOk = 5216, // 进入爬塔加载成功
	eClient2Battle_Tower_UserBuffer = 5217, // 使用Buff
	eClient2Battle_Tower_LeaveTower = 5218, //离开爬塔 
	eClient2Battle_Tower_ReqEnter = 5219, // 请求进入爬塔
	eClient2Battle_SummonMercenary = 5220,// 召唤佣兵来自己身边
	eClient2Battle_Transfer = 5221,// 触发传送
	eClient2Battle_ReloadFov = 5222,// 重新加载视野
    eClient2Battle_ReqTBPlayerInfo = 5223, //领地战个人信息(NullMessage) 
	eClient2Battle_Skill_ExcuteSkill = 5300,			//玩家释放技能
	eClient2Battle_Skill_RequestHurt = 5301,			//玩家请求伤害
	eClient2Battle_Skill_BlockSkillStart = 5302,		//玩家放格挡技能开始
	eClient2Battle_Skill_BlockSkillEnd = 5303,			//玩家放格挡技能结束
	
	eClient2Battle_Skill_End = 5350,							//技能相关协议ID段

	//=====pvpstart=====
	ePVP_CP_NoticeConnectOK = 5400,
	ePVP_CP_LeaveAttack = 5401,
	ePVP_CP_LoadBattleOk = 5402,					//通知加载场景OK
	ePVP_CP_DoPVPBuffer = 5403,					//触发PVP buffer
	ePVP_CP_PVPSurrender = 5406,					//投降

	//=====pvpend=====

	eClient2Battle_WorldBoss_Challenge = 5450,	//挑战世界boss
	eClient2Battle_WorldBoss_SaledItem = 5451,	//查询单个已拍卖物品信息
	eClient2Battle_WorldBoss_SaleList = 5452,	//请求拍卖信息
	eClient2Battle_WorldBoss_HurtRankList = 5453,	//请求伤害排行
	eClient2Battle_Ping = 5454,  //请求ping
	eClient2Battle_ChangeAttackMode = 5455,  //切换攻击模式
	eClient2Battle_Anchor = 5456,  //抛锚

	//=====collect start=======
	eClient2Battle_Collect_Operate = 5460, //采集
	eClient2Battle_Qte = 5461, 	   //提交采集
	
	
	//=====collect end=========

	eClient2Battle_PlayerCaptureFlag = 5462,	//拔旗

	emsgid_client2battle_end = 6000,
	emsgid_battle_end = 6000,


	//battle -> match
	emsgid_battle2match_begin = 6000,
	eBattle2Match_ServerInfo = 6000,
	eBattle2Match_UpdatePlayerNum = 6001,
	eBattle2Match_CreateDungeon = 6002,
	eBattle2Match_BloodSeaCreateScene = 6003, //献血海域创建场景
	eBattle2Match_BloodSeaCloseScene = 6004, //鲜血海域结束战斗
	emsgid_battle2match_end = 6200,


	//battle -> match -> game
	emsgid_battle2game_begin = 6200,
	eBattle2Game_EnterScene = 6200,
	eBattle2Game_EnterDungeon = 6201,
	eBattle2Game_AddDropItem = 6203,		//掉落物品
	eBattle2Game_SynSKill_Cost = 6204, //技能消耗同步
	eBattle2Game_DungeonResult = 6205,		//副本结果
	eBattle2Game_SyncMove = 6206,		//同步坐标
	eBattle2Game_UpdateQuest = 6207,	//更新任务
	eBattle2Game_TriggerQuest = 6208,	//触发任务
	eBattle2Game_MonsterDie = 6209,	//怪物死亡通知
	eBattle2Game_LeaveDungeon = 6210,	//离开副本
	eBattle2Game_SyncTower_Data = 6211, //爬塔数据
	eBattle2Game_DungeonLoadOk = 6212, //副本加载完成
	eBattle2Game_WolrdBoss_HpChange = 6213, //
	eBattle2Game_WolrdBoss_SaleResult = 6214,	//拍卖结果
	eBattle2Game_ChangeMapForceAwakenOff = 6215, //切换副本强制变身结束
	eBattle2Game_SceneLoadOk = 6216,//玩家进入场景完成
	eBattle2Game_CreateDungeon = 6217,
	eBattle2Game_SyncData = 6218,  //同步更改的数据
	eBattle2Game_Notice = 6219,  //广播通知
	eBattle2Game_DieToChangeAwakenAndRide = 6220, //切换副本强制变身结束
	eBattle2Game_UpdateBossDamage = 6222,	//同步boss伤害
	eBattle2Game_MercenaryDieInfo =  6223, // 佣兵死亡信息
    eBattle2Game_SyncGuildData = 6224,      //同步公会信息
    eBattle2Game_GuildBossOver = 6225,      //公会boss战结束
	eBattle2Game_SendMail = 6226,       //发送奖励邮件
	eBattle2Game_BackToRecordMap = 6227,       //回之前的场景
	eBattle2Game_SaveMercenaryPer = 6228,  //保存佣兵血量
	eBattle2Game_UpdatePlayerStatus = 6229, //同步玩家状态
	eBattle2Game_GetRoomList = 6230,//返回房间信息
    eBattle2Game_BackToCity = 6231, //回城
    eBattle2Game_TBKillReward = 6232,  //领土争夺战 杀人奖励
    eBattle2Game_TBResult = 6233,           //领土争夺战结算
    eBattle2Game_IsLoadOK = 6234,           //加载是否完成(断线重连相关)
	eBattle2Game_KickPlayer = 6235,		//加载场景异常 踢下线 防止无法正常登陆到场景
	eBattle2Game_SurvivalOver = 6236,	//大逃杀结束
	eBattle2Game_PlayerCaptureFlag = 6237,	//领取争霸拔旗
    eBattle2Game_GoldBattleKillReward = 6240,  //黄金争霸战 杀人奖励
    eBattle2Game_GoldBattleResult = 6241,           //黄金争霸战结算
	eBattle2Game_GoldBattle_PlayerCaptureFlag = 6242,	//黄金争霸战拔旗
	eBattle2Game_AttackWorldBoss = 6243,	//初次对世界boss造成伤害
	eBattle2Game_GuildBattle_Leave = 6244,						//公会争霸战玩家离开入口
	eBattle2Game_GuildBattle_Over = 6245,						//公会争霸结算
	eBattle2Game_BloodSea_Over = 6246,					//鲜血海域更新积分
	eBattle2Game_TicketBossRank = 6247,					//门票BOSS排名
	emsgid_battle2game_end = 6500,

	//battle -> client
	emsgid_battle2client_begin = 6500,
	eBattle2Client_EnterScene = 6500,
	eBattle2Client_SyncPlayerAppear = 6501,
	eBattle2Client_SyncObjDisappear = 6502,
	eBattle2Client_SyncObjMove = 6503,
	eBattle2Client_SpeelBegin = 6504, //切换地图层

	eBattle2Client_GetMapBranch = 6505,		//获取分线列表返回
	eBattle2Client_ChangeBranch = 6506,      //跳线结果


	eBattle2Client_CheckPosition = 6507,      //检查坐标
	eBattle2Client_MonsterStopMove = 6508,      //停止移动
	eBattle2Client_TriggerEvent = 6509,		//触发事件
	eBattle2Client_SyncMonsterAppear = 6510,	//怪物出现

	eBattle2Client_ChangeTitle = 6511,			//同步称号变更
	eBattle2Client_ChangeEquip = 6512,		//同步装备变更
	eBattle2Client_ClientLoadOK = 6513,		//发送当前分线ID
	
	eBattle2Client_Story = 6514,		//剧情播放
	eBattle2Client_Customs = 6515,		//关卡
	eBattle2Client_ChangeFacade = 6516, //同步成长系统外观变化
	eBattle2Client_MonsterTalk = 6517,		//NPC喊话

	eBattle2Client_MonsterNum = 6518,		//怪物波数
	eBattle2Client_ChangeCamp = 6519,		//改阵容
	eBattle2Client_PretendStart = 6520,		//伪装开始
	eBattle2Client_PretendEnd = 6521,		//伪装结束
	eBattle2Client_SyncAttInBat = 6522,  //同步战斗属性数据
	eBattle2Client_DropItem = 6523,			//物品掉落

	eBattle2Client_Tower_EnterScene = 6524, //可以进入爬塔区域
	eBattle2Client_Tower_AddBuffer = 6525,//增加Buffer
	eBattle2Client_Tower_DelBuffer = 6526,//删除Buffer
	eBattle2Client_Tower_NewFloor = 6527,//进入下一层
	eBattle2Client_Tower_EndTower = 6528,//结束爬塔
	//新成长系统相关
	eBattle2Client_ChangePet = 6529,			// 同步宠物变更
	eBattle2Client_ChangeRide = 6530,			// 同步坐骑变更
	eBattle2Client_ChangeAwaken = 6531,			// 同步觉醒变更
	eBattle2Client_SyncMercenaryAppear = 6532,			// 佣兵出现在视野
	eBattle2Client_ChangeAvatar = 6533,			// 同步Avatar
	eBattle2Client_ChangeSuit = 6534,			// 同步suit
	eBattle2Client_ReconnectUnDoTrigger = 6535,
	eBattle2Client_TriggerServerTips = 6536, //触发servertips
	eBattle2Client_SafeAreaStatus = 6537,		//进出安全区通知
	eBattle2Client_ServerMove = 6538,    //服务器控制移动
	eBattle2Client_PlayerDie = 6539,		//死亡通知
	eBattle2Client_SyncSceneObjAppear = 6540, //场景中的生物出现在视野中，包过人，怪物，佣兵.......
	eBattle2Client_SyncSceneObjDisappear = 6541,  //场景中的生物消失在视野中
	eBattle2Client_SyncModelChange = 6542,		//同步玩家幻化模型切换
	eBattle2Client_Skill_ExcuteSelf = 6600,    // 释放技能返回给自己的消息
	eBattle2Client_Skill_ExcuteOther = 6601,    	// 释放技能返回给同步区域其他玩家的消息
	eBattle2Client_Skill_UpdateHurtData = 6602,    	// 升级技能
	eBattle2Client_Skill_AttackBack = 6603,    	// 击退
	eBattle2Client_Skill_AddBuffer = 6604,    	//加buffer
	eBattle2Client_Skill_DelBuffer = 6605,   	//减buffer
	eBattle2Client_Skill_UpdateBuffer = 6606,		//更新buffer
	eBattle2Client_Skill_AttackSprint = 6607,    	//冲刺
	eBattle2Client_Skill_AttackDie = 6608,    	//非正常死亡
	eBattle2Client_Skill_BlockBack = 6609,     //玩家放格挡技能返回
	eBattle2Client_Skill_BlockStart = 6609,     //玩家放格挡技能开始
	eBattle2Client_Skill_BlockEnd = 6610,     //玩家放格挡技能结束
	eBattle2Client_Skill_UpdateAttacking = 6611,     //是否是战斗状态
	eBattle2Client_Skill_Strike = 6612,     //反击
	eBattle2Client_Die = 6613,     //死亡
	eBattle2Client_Skill_Shout = 6614,     //喊话
	eBattle2Client_Skill_ClearCD = 6615,     //清空技能CD	
	eBattle2Client_Skill_UpdateAttackStatus = 6616,     //更新战斗状态
	eBattle2Client_Relive = 6617, 		//复活状态回复	
	eBattle2Client_UpdateMonsterAttackStatus = 6618,     //更新怪物战斗状态
	eBattle2Client_Skill_Block = 6619,     //格挡住
	eBattle2Client_Skill_UpdateTP = 6620,    	//更新TP值
	eBattle2Client_Skill_UpdateFlagCamp = 6621,    	//更新旗子阵营
	eBattle2Client_RelivePos = 6622,		//复活点位置同步
	eBattle2Client_UpdateSkillGroupCD = 6623,		//更新组技能ID
	eBattle2Client_Skill_Skill_End = 6650,		//技能相关协议ID段


	//世界boss
	eBattle2Client_WolrdBoss_DorpItemList = 6651,		//世界boss掉落通知
	eBattle2Client_WolrdBoss_SaleStart = 6652,		//世界boss掉落物品开始拍卖
	eBattle2Client_WolrdBoss_SaleBid = 6653,		//世界boss  拍卖出价通知
	eBattle2Client_WolrdBoss_SaleResult = 6654,		//世界boss  拍卖结果通知
	eBattle2Client_WolrdBoss_HpChange = 6655,		//世界boss  HP
	eBattle2Client_WolrdBoss_SaledItem = 6656,		//世界boss  查询单个已拍卖物品信息
	eBattle2Client_WolrdBoss_HurtRankList = 6657,		//伤害列表

    eBattle2Client_TicketBoss_DropItem = 6658,

    eBattle2Client_SyncDropItemAppear = 6659,		//场景掉落物品出现视野
    eBattle2Client_SyncDropItemChange = 6660,		//场景掉落物品更新
    eBattle2Client_Ping = 6661, 
    eBattle2Client_AttackMode = 6662,	//攻击模式变更通知
    eBattle2Client_Transfer = 6663,// 触发传送
    eBattle2Client_GuildBossDie=6664, //通知客户端公会boss死亡 以及 结束
    eBattle2Client_Anchor = 6665,  //抛锚
    eBattle2Client_UpdateHeadInfo = 6666,//玩家头顶信息更新
    eBattle2Client_UpdateMercenaryRank = 6667, //玩家佣兵排名更新
    eBattle2Client_TriggerRadius = 6668,//触发器判断范围通知
    eBattle2Client_ScenePlayerNum = 6669,//当前场景的人数
    eBattle2Client_GradeDataChanged = 6670, //副本评级相关数据变更
    eBattle2Client_Boss_HurtRankList = 6671,		//自己的积分排名
    eBattle2Client_SyncMonsterOnwer = 6672, //同步所有权变化信息
    eBattle2Client_MonsterCountChanged = 6673, //同步死亡怪物数量变更
    eBattle2Client_AckTBPlayerInfo = 6674, //领地战个人信息
	eBattle2Client_SurvivalStartFight = 6675, //大逃杀开始战斗


	eBattle2Client_QuietFlagBossInfo = 6680, //沉静之旗BOSS信息
	eBattle2Client_QuietFlagBossRefresh = 6681, //沉静之旗BOSS刷新
	eBattle2Client_QuietFlagBossDied = 6682, //沉静之旗死亡
	eBattle2Client_QuietFlagBossReward = 6683, //击杀神秘BOSS，给同公会的发奖励

	eBattle2Client_CollectNotifyError = 6690, //采集出错，移动，被攻击，攻击别人，打断采集
	eBattle2Client_CollectNotifyState = 6691, //通知采集状态
	eBattle2Client_Collect_Action_BroadCast = 6692, //广播告诉其他人自己的当前采集状态
	eBattle2Client_Collect_Operate = 6693,	//客户端操作采集
	eBattle2Client_Collect_Qte = 6694,		//客户端提交采集完成

	eBattle2Client_TreasureSeaPlayerInfo = 6697, //巡海探宝玩家信息
	eBattle2Client_TreasureSeaPlayerExpInfo = 6698, //巡海探宝玩家经验信息

	eBattle2Client_Died_In_Relive_Position = 6699,	//玩家是否在复活区死亡

    //TB PVP start 6700
    ePVP_PC_SyncOnePlayer = 6700,               	//同步多个玩家
    ePVP_PC_SyncMultiPlayer = 6701,             	//同步一个玩家
    ePVP_PC_RemovePlayer = 6702,					//删除一个玩家
    ePVP_PC_LoadBattle = 6703,					//通知加载场景
    ePVP_PC_LoadPVPBuffer = 6704,                //加载事件
    ePVP_PC_AddPVPBuffer = 6705,                //加载事件
    ePVP_PC_DelPVPBuffer = 6706,                //加载事件
    ePVP_PC_LoadPVPFlagEvent = 6707,					//加载旗子
    ePVP_PC_AddFlag = 6708,					//增加一个旗子
    ePVP_PC_DelFlag = 6709,					//删除旗子
    ePVP_PC_DesFlag = 6710,					//旗子的目标
    ePVP_PC_AttackEnd = 6711,					//战斗结束
    ePVP_PC_KillRanking = 6712,					//击杀排行榜
    ePVP_PC_IntegralRanking = 6713,					//积分排行榜
    ePVP_PC_BattleBegin = 6714,					//战斗开始
    eBattle2Client_TB_SyncFlagPos = 6715,       //同步旗帜位置
    eBattle2Client_GoldBattle_SyncFlagPos = 6715,       //黄金争霸同步旗帜位置
	eBattle2Client_GuildBattle_OfficeKilled = 6717,			//公会争霸战会长副会长被击杀
	eBattle2Client_GuildBattle_Action = 6718,			//公会争霸战玩家触发行为事件


	eBattle2Client_BloodSeaBattleResult = 6730,			//鲜血海域结算
	eBattle2Client_BloodSeaSynHp = 6731,				//同步HP
	eBattle2Client_BloodSeaSynKill = 6732,				//同步击杀
	eBattle2Client_BloodSeaTime = 6733,					//倒计时协议
	eBattle2Client_BroadPlayerBattleInfo = 6734,		//同步玩家信息
    //pvpend

	eBattle2Client_NoticePlayerCaptureFlag = 6735,		//通知玩家拔旗

	eBattle2Client_NoticeChangeBoxStatus = 6736,		//门票BOSS宝箱状态

    emsgid_battle2client_end = 7000,
    emsgid_errorcode_begin = 20000,
    emsgid_errorcode_end = 20000,

    emsg_id_Num,	
};