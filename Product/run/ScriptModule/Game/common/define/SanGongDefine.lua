
g_sangongDefine = {}   --火锅英雄

g_sangongDefine.state_ready = 0   --等待庄家开始
g_sangongDefine.state_qiangzhuang = 1   --抢庄的流程
g_sangongDefine.state_start = 2   --等待每个玩家
g_sangongDefine.state_wait = 3    --等待每个玩家
g_sangongDefine.state_play = 4    --等待每个玩家
g_sangongDefine.state_count = 5   --等待每个玩家
g_sangongDefine.state_end = 6     --每一个牌局结束后都会有10秒的动画时间
g_sangongDefine.state_dissolve = 7  --解散房间
g_sangongDefine.state_delete = 8  --在循环中，如果碰到这种状态的牌桌，就把该牌桌删除

g_sangongDefine.time_dissolve = 120   --300  --300秒没开始，房间自动解散
g_sangongDefine.time_user_play = 0   --用户行动的时间
g_sangongDefine.time_count = 12        --结算的时间
g_sangongDefine.time_qiangzhuang = 15
g_sangongDefine.time_select = 10
g_sangongDefine.time_over = -7200  --倒计时到这个就解散
g_sangongDefine.time_ready = 600  --倒计时到这个就解散
g_sangongDefine.max_user = 6
g_sangongDefine.time_end = 3  --倒计时到这个就解散


g_sangongDefine.table_srf = 101          --这个是私人房
g_sangongDefine.table_jbc = {}           --
g_sangongDefine.table_jbc['junior'] = 1
g_sangongDefine.table_jbc['medium'] = 2
g_sangongDefine.table_jbc['serior'] = 3

g_sangongDefine.pokerList = {101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413}

g_sangongDefine.pokerListAll = {101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413,
515,525}   --包括大小王的


g_sangongDefine.pourList = {}
g_sangongDefine.pourList[1] = 400
g_sangongDefine.pourList[2] = 1200
g_sangongDefine.pourList[3] = 3000

g_sangongDefine.play_status_none = 0  --还没有操作
g_sangongDefine.play_status_guo = 1   --过牌
g_sangongDefine.play_status_yao = 2   --要牌

g_sangongCardType = {}


g_sangongCardType.type_lingdian = 100      --零点
g_sangongCardType.type_yidian = 101        --一点
g_sangongCardType.type_liangdian = 102     --两点
g_sangongCardType.type_sandian = 103       --三点
g_sangongCardType.type_sidian = 104        --四点
g_sangongCardType.type_wudian = 105        --五点
g_sangongCardType.type_liudian = 106       --六点
g_sangongCardType.type_qidian = 107        --七点
g_sangongCardType.type_badian = 108        --八点
g_sangongCardType.type_jiudian = 109       --九点



g_sangongCardType.type_hunsangong = 211     --混三公
g_sangongCardType.type_xiaosangong = 221    --小三公
g_sangongCardType.type_dasangong = 231     --大三公
g_sangongCardType.type_baojiu = 241        --爆九


g_sangongCardType.type_fjsanggong = 341      --三公    --这个是福建三公的玩法,4倍
g_sangongCardType.type_fjbaozi = 342         --豹子    --这个是福建三公的玩法，5倍
g_sangongCardType.type_fjdaxiangwang = 343      --大小王    --这个是福建三公的玩法，6倍


g_sangongCardType[0] = g_sangongCardType.type_lingdian
g_sangongCardType[1] = g_sangongCardType.type_yidian
g_sangongCardType[2] = g_sangongCardType.type_liangdian
g_sangongCardType[3] = g_sangongCardType.type_sandian
g_sangongCardType[4] = g_sangongCardType.type_sidian
g_sangongCardType[5] = g_sangongCardType.type_wudian
g_sangongCardType[6] = g_sangongCardType.type_liudian
g_sangongCardType[7] = g_sangongCardType.type_qidian
g_sangongCardType[8] = g_sangongCardType.type_badian
g_sangongCardType[9] = g_sangongCardType.type_jiudian


g_sangongPlayStatus = {}
g_sangongPlayStatus.status_ready = 101   --没次开局前初始化成这个数值
g_sangongPlayStatus.status_wait = 102    --准备好了就等待，等待牌局开始
g_sangongPlayStatus.status_select = 103  --牌局开始后，庄家需要等待玩家选择倍数
g_sangongPlayStatus.status_play = 104
g_sangongPlayStatus.status_end = 105


g_sangongDefine.zhuang_ding = 101   --固定庄家,只能是房主当庄
g_sangongDefine.zhuang_lun = 102    --轮流庄家，按顺序轮流当庄
g_sangongDefine.zhuang_qiang = 103  --抢庄，如果有多个人抢庄，则随机给抢的人
g_sangongDefine.zhuang_tongbi = 104 --通比，谁大谁当庄
g_sangongDefine.zhuang_kanqiang = 105 --看牌抢庄


g_sangongDefine.sgtype_guangdong = 1001  --广东三公
g_sangongDefine.sgtype_fujian = 1002  --福建三公



