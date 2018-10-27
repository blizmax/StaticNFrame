--静态常量， 临时放一些不便的值，方便修改

require "script/gxlua/unilight"

module('static_const', package.seeall)

--静态常量，好友系统，玩家离线后最大数据最大在线时间
Static_Const_Friend_MAX_ONLINE_TIME_AFTER_OFFLINE = 60

--静态常量，好友系统，最大好友数量
Static_Const_Friend_MAX_Friend_Count = 20

--静态常量，好友系统，系统自动推荐好友时间
Static_Const_Friend_System_Auto_Recommend_Time = 1200

--静态常量，好友系统，系统每隔一段时间查看数据
Static_Const_Friend_System_Check_Data_Time = 600

--静态常量，旅行团，雇佣CD时间
Static_Const_TRAVEL_Employ_CD_Time = 600

--静态常量，旅行团，团员跟团时间周期
Static_Const_TRAVEL_Employ_MAX_TIME = 600

--静态常量，旅行团，旅行团初始化抓捕次数
Static_Const_TRAVEL_INIT_MAX_CAPTURE_TIMES = 3

--静态常量，旅行团，旅行团初始化解锁的位置数目
Static_Const_TRAVEL_Init_UNLOCK_SLOT_COUNT = 3

--静态常量，旅行团，最大雇佣玩家数目
Static_Const_TRAVEL_MAX_EMPLOY_USER_COUNT = 10

--静态常量，旅行团，最大推荐雇佣好友数目
Static_Const_TRAVEL_MAX_RECOMMEND_COUNT = 5

