#pragma once

////////////////////////////// LogInfo node name /////////////////////////////
#define NF_LOGINFO_NODE_TABLE_DETAIL_TABLE		"detail_table"
enum NF_ENUM_LOGINFO_TABLE_DETAIL_TABLE
{
	NF_LOGINFO_DETAIL_TABLE_COL_UINT32_LOGID                    = 0,                                    //日志ID
	NF_LOGINFO_DETAIL_TABLE_COL_BOOL_DISPLAY                    = 1,                                    //是否显示日志
	NF_LOGINFO_DETAIL_TABLE_COL_UINT32_LEVEL                    = 2,                                    //日志等级
	NF_LOGINFO_DETAIL_TABLE_COL_STRING_LOGNAME                  = 3,                                    //日志名字
	NF_LOGINFO_DETAIL_TABLE_COL_ARRAY_GUID                      = 4,                                    //guid
};
#define NF_LOGINFO_NODE_UINT32_LOGLEVEL                      "LogLevel"                              //日志等级
#define NF_LOGINFO_NODE_UINT32_LOGFLUSHLEVEL                 "LogFlushLevel"                         //日志刷新
////////////////////////////// player node name /////////////////////////////
#define NF_PLAYER_NODE_UINT64_USERID                         "userid"                                //玩家ID
#define NF_PLAYER_NODE_STRING_CID                            "cid"                                   //用作判断手机标识的唯一字符串
#define NF_PLAYER_NODE_STRING_ACCOUNT                        "account"                               //账号
#define NF_PLAYER_NODE_STRING_PASSWORD                       "password"                              //密码
#define NF_PLAYER_NODE_STRING_NICKNAME                       "nickname"                              //玩家昵称
#define NF_PLAYER_NODE_STRING_REGDATE                        "regdate"                               //注册日期
#define NF_PLAYER_NODE_STRING_FACE_1                         "face_1"                                //头像1
#define NF_PLAYER_NODE_STRING_FACE_2                         "face_2"                                //头像2
#define NF_PLAYER_NODE_STRING_FACE_3                         "face_3"                                //头像3
#define NF_PLAYER_NODE_STRING_FACE_4                         "face_4"                                //头像4
#define NF_PLAYER_NODE_STRING_DESCRIPTION                    "description"                           //个性签名
#define NF_PLAYER_NODE_STRING_EMAIL                          "email"                                 //邮件
#define NF_PLAYER_NODE_STRING_PHONENUM                       "phonenum"                              //电话号码
#define NF_PLAYER_NODE_STRING_CHANNEL                        "channel"                               //平台标识
#define NF_PLAYER_NODE_STRING_PROVINCE                       "province"                              //省份
#define NF_PLAYER_NODE_STRING_CITY                           "city"                                  //城市
#define NF_PLAYER_NODE_STRING_IMEI                           "imei"                                  //imei
#define NF_PLAYER_NODE_STRING_DEVNAME                        "devname"                               //设备号
#define NF_PLAYER_NODE_STRING_MACNAME                        "macname"                               //macname
#define NF_PLAYER_NODE_STRING_BINDNICK                       "bindnick"                              //实名认证消息
#define NF_PLAYER_NODE_STRING_PLATFORMID                     "platformid"                            //平台返回的标识ID
#define NF_PLAYER_NODE_UINT32_SEX                            "sex"                                   //性别
#define NF_PLAYER_NODE_UINT32_AGE                            "age"                                   //年龄
#define NF_PLAYER_NODE_UINT32_ISROBOT                        "isrobot"                               //是否是机器人
#define NF_PLAYER_NODE_UINT32_EXP                            "exp"                                   //经验
#define NF_PLAYER_NODE_UINT32_LEVEL                          "level"                                 //等级
#define NF_PLAYER_NODE_UINT64_VIPLEVEL                       "viplevel"                              //vip等级
#define NF_PLAYER_NODE_UINT64_JETTON                         "jetton"                                //筹码必须用64位的
#define NF_PLAYER_NODE_UINT64_BANK_JETTON                    "bank_jetton"                           //暂存在银行的筹码
#define NF_PLAYER_NODE_UINT64_GOLD                           "gold"                                  //金币，暂时先不用这个
#define NF_PLAYER_NODE_UINT64_MONEY                          "money"                                 //钻石
#define NF_PLAYER_NODE_UINT64_POPULARITY                     "popularity"                            //魅力值
#define NF_PLAYER_NODE_UINT64_LASTTIME                       "lasttime"                              //上次登录时间
#define NF_PLAYER_NODE_UINT32_IDENSTATUS                     "idenstatus"                            //头像认证状态
#define NF_PLAYER_NODE_UINT32_BINDTYPE                       "bindtype"                              //绑定类型
#define NF_PLAYER_NODE_UINT32_MOBILETYPE                     "mobiletype"                            //运营商
#define NF_PLAYER_NODE_UINT64_PENULTTIME                     "penulttime"                            //倒数第二次登录
#define NF_PLAYER_NODE_UINT32_SILENT                         "silent"                                //是否禁言
#define NF_PLAYER_NODE_UINT32_ISBAN                          "isban"                                 //是否禁号，列入黑名单
#define NF_PLAYER_NODE_UINT32_PLAYTYPE                       "playtype"                              //玩家玩牌的类型：1:一般型，没有特点；2：小白型：全凭喜好。3：保守型，一定要拿到好牌才跟或者加注；4：激进型：一般有好牌就加注或者跟了。5：冒险型：非常喜欢诈唬，有好牌一定allin或者跟
#define NF_PLAYER_NODE_UINT32_BLACKLIST                      "blacklist"                             //黑名单
#define NF_PLAYER_NODE_UINT32_PROPERTYID                     "propertyid"                            //玩家佩戴的物品ID
#define NF_PLAYER_NODE_UINT32_AGENCYSTATUS                   "agencystatus"                          //代理的状态，最高两位数，个尾数是0：表示没有绑定代理，1：已经绑定代理；十位数：0：没申请代理，1：已经申请代理，2：成为了代理
#define NF_PLAYER_NODE_STRING_REGISTRATION1                  "registration1"                         //签到详情 上旬
#define NF_PLAYER_NODE_STRING_REGISTRATION2                  "registration2"                         //签到详情	中旬
#define NF_PLAYER_NODE_STRING_REGISTRATION3                  "registration3"                         //签到详情	下旬
#define NF_PLAYER_NODE_UINT32_SIGNINMONTH                    "signinmonth"                           //签到月份
#define NF_PLAYER_NODE_UINT32_BSIGNIN                        "bsignin"                               //今天是否已签到   //未签到:0  已签到:签到时间
#define NF_PLAYER_NODE_UINT32_TURNTABLENUM                   "turntablenum"                          //转盘次数
#define NF_PLAYER_NODE_UINT32_TENGAMES                       "tengames"                              //十局是否完成
#define NF_PLAYER_NODE_UINT32_TODAYSHARE                     "todayshare"                            //今天是否分享
#define NF_PLAYER_NODE_UINT32_CANINVITE                      "caninvite"                             //是否能被邀请	
#define NF_PLAYER_NODE_BOOL_ONLINE                           "online"                                //是否在线
#define NF_PLAYER_NODE_UINT64_LOGINTIME                      "loginTime"                             //登陆时间
#define NF_PLAYER_NODE_UINT64_LOGOUTTIME                     "logoutTime"                            //下线时间
