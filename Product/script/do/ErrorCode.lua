ERROR_CODE = {
	--Common
	SUCCESS = 0,
	ARGUMENT_ERROR = 1,							--参数格式错误
	LOGICAL_ERROR = 2,							--逻辑错误
	TABLE_ERROR = 3,							--表中数据有错
	MONEY_NOT_ENOUGH = 4,						--金币不足
	DIAMOND_NOT_ENOUGH = 5,						--钻石不足
	OPEATE_AGAIN = 6,							--不能重复操作

	--Building
	BUILDING_REBUILD_MAX = 10001,					--建筑物已达到最大改造等级
	BUILDING_LEVEL_MAX = 10002,						--建筑物已达到最大等级
	BUILDING_BUY_AGAIN = 10003,						--建筑物重复购买
	BUILDING_NOT_BUY = 10004,						--建筑尚未购买
	BUILDING_LEVEL_NOT_ENOUGH = 10005,				--建筑等级不足
	BUILDING_STATE_NOT_OPEN = 10006,				--该地图尚未开启
}
