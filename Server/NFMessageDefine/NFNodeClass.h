#pragma once

////////////////////////////// player node name /////////////////////////////
#define NF_PLAYER_NODE_INT_USERID                            "userid"                                //玩家ID
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
#define NF_PLAYER_NODE_INT_SEX                               "sex"                                   //性别
#define NF_PLAYER_NODE_INT_AGE                               "age"                                   //年龄
#define NF_PLAYER_NODE_INT_ISROBOT                           "isrobot"                               //是否是机器人
#define NF_PLAYER_NODE_INT_EXP                               "exp"                                   //经验
#define NF_PLAYER_NODE_INT_LEVEL                             "level"                                 //等级
#define NF_PLAYER_NODE_INT_VIPLEVEL                          "viplevel"                              //vip等级
#define NF_PLAYER_NODE_INT_JETTON                            "jetton"                                //筹码必须用64位的
#define NF_PLAYER_NODE_INT_BANK_JETTON                       "bank_jetton"                           //暂存在银行的筹码
#define NF_PLAYER_NODE_INT_GOLD                              "gold"                                  //金币，暂时先不用这个
#define NF_PLAYER_NODE_INT_MONEY                             "money"                                 //钻石
#define NF_PLAYER_NODE_INT_POPULARITY                        "popularity"                            //魅力值
#define NF_PLAYER_NODE_INT_LASTTIME                          "lasttime"                              //上次登录时间
#define NF_PLAYER_NODE_INT_IDENSTATUS                        "idenstatus"                            //头像认证状态
#define NF_PLAYER_NODE_INT_BINDTYPE                          "bindtype"                              //绑定类型
#define NF_PLAYER_NODE_INT_MOBILETYPE                        "mobiletype"                            //运营商
#define NF_PLAYER_NODE_INT_PENULTTIME                        "penulttime"                            //倒数第二次登录
#define NF_PLAYER_NODE_INT_SILENT                            "silent"                                //是否禁言
#define NF_PLAYER_NODE_INT_ISBAN                             "isban"                                 //是否禁号，列入黑名单
#define NF_PLAYER_NODE_INT_PLAYTYPE                          "playtype"                              //玩家玩牌的类型：1:一般型，没有特点；2：小白型：全凭喜好。3：保守型，一定要拿到好牌才跟或者加注；4：激进型：一般有好牌就加注或者跟了。5：冒险型：非常喜欢诈唬，有好牌一定allin或者跟
#define NF_PLAYER_NODE_INT_BLACKLIST                         "blacklist"                             //黑名单
#define NF_PLAYER_NODE_INT_PROPERTYID                        "propertyid"                            //玩家佩戴的物品ID
#define NF_PLAYER_NODE_INT_AGENCYSTATUS                      "agencystatus"                          //代理的状态，最高两位数，个尾数是0：表示没有绑定代理，1：已经绑定代理；十位数：0：没申请代理，1：已经申请代理，2：成为了代理
#define NF_PLAYER_NODE_STRING_REGISTRATION1                  "registration1"                         //签到详情 上旬
#define NF_PLAYER_NODE_STRING_REGISTRATION2                  "registration2"                         //签到详情	中旬
#define NF_PLAYER_NODE_STRING_REGISTRATION3                  "registration3"                         //签到详情	下旬
#define NF_PLAYER_NODE_INT_SIGNINMONTH                       "signinmonth"                           //签到月份
#define NF_PLAYER_NODE_INT_BSIGNIN                           "bsignin"                               //今天是否已签到   //未签到:0  已签到:签到时间
#define NF_PLAYER_NODE_INT_TURNTABLENUM                      "turntablenum"                          //转盘次数
#define NF_PLAYER_NODE_INT_TENGAMES                          "tengames"                              //十局是否完成
#define NF_PLAYER_NODE_INT_TODAYSHARE                        "todayshare"                            //今天是否分享
#define NF_PLAYER_NODE_INT_CANINVITE                         "caninvite"                             //是否能被邀请	
#define NF_PLAYER_NODE_BOOL_ONLINE                           "online"                                //是否在线
#define NF_PLAYER_NODE_INT_LOGINTIME                         "loginTime"                             //登陆时间
#define NF_PLAYER_NODE_INT_LOGOUTTIME                        "logoutTime"                            //下线时间
