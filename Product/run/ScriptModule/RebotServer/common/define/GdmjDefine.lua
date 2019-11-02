g_gdmjDefine = {}
g_gdmjDefine.game_type = 8600
g_gdmjDefine.maxUser = 2
g_gdmjTableType = {}
g_gdmjTableType["junior"] = 1
g_gdmjTableType["medium"] = 2
g_gdmjTableType["serior"] = 3

g_gdmjNeedJetton = {2000,20000,100000}

g_gdmjPourList = {}
g_gdmjPourList[1] = 1
g_gdmjPourList[2] = 100
g_gdmjPourList[3] = 500
g_gdmjPourList[4] = 1000

g_gdmjJbcMaNum = {2,4,4}

g_gdmjDefine.game_lockstate = {}
g_gdmjDefine.game_lockstate.lock_no = 1
g_gdmjDefine.game_lockstate.lock_yes = 2

g_gdmjAction = {}
g_gdmjAction.type_play = 0   --出牌
g_gdmjAction.type_guo = 1    --过牌
g_gdmjAction.type_chi = 2    --吃
g_gdmjAction.type_peng = 3   --碰
g_gdmjAction.type_gang = 4   --杠
g_gdmjAction.type_hu = 5     --胡
g_gdmjAction.type_qiangganghu = 6   --抢杠胡
g_gdmjAction.type_chihu = 7         --吃胡
g_gdmjAction.type_genzhuang = 8    --跟庄，这个操作只是在客户端有显示，服务器判断后主动发给客户端
g_gdmjAction.type_mingpai = 9    --卡五星中，明牌的操作
g_gdmjAction.type_koupai = 10    --卡五星中，扣牌的牌型
g_gdmjAction.type_buhua = 11     --补花
g_gdmjAction.type_gang_bucard = 120  --杠完补牌的标识

g_gdmjType = {}  --广东麻将的类型
g_gdmjType.type_tdh = 211    --推倒胡
g_gdmjType.type_ztdh = 212   --做牌推倒胡
g_gdmjType.type_jph = 213    --鸡平胡
g_gdmjType.type_hzz = 214    --惠州庄麻将
g_gdmjType.type_hdz = 215    --惠东庄
g_gdmjType.type_hbz = 216    --惠博庄
g_gdmjType.type_hzmj = 217    --红中麻将
g_gdmjType.type_rpmj = 218    --饶平麻将
g_gdmjType.type_ermj = 219    --二人麻将

g_gdmjType.type_jbc = 211   --金币场,金币场中，默认是推倒胡

g_gdmjStatus = {}   --麻将牌桌的状态
g_gdmjStatus.status_none = 0  --空闲的状态，不用处理
g_gdmjStatus.status_ready = 1  --玩家正在等待，还没有全部点准备
g_gdmjStatus.status_waiting = 2  --玩家已经全部点准备了，可以开始了
g_gdmjStatus.status_playing = 3  --正在玩家，这时候，玩家不能退出
g_gdmjStatus.status_counting = 4  --结算的时候
g_gdmjStatus.status_end = 5  --每一个牌局结束后都会有10秒的动画时间
g_gdmjStatus.status_dissolve = 6  --解散房间
g_gdmjStatus.status_delete = 7  --在循环中，如果碰到这种状态的牌桌，就把该牌桌删除
g_gdmjStatus.status_deleteed = 8 --已经删除的状态

g_gdmjPlayerState = {}  --麻将玩家的状态,针对玩家个人
g_gdmjPlayerState.play_none = 0  --空的状态
g_gdmjPlayerState.play_ready = 1  --玩家已经坐下，还没点击准备的按钮
g_gdmjPlayerState.play_waiting = 2   --等待行动的状态
g_gdmjPlayerState.play_action = 3 --轮到自己行动,这个是行动，会闪亮的
g_gdmjPlayerState.play_optional = 4 --轮到自己选择了，碰和杠的情况，下面的灯是不会亮的

g_gdmjUserState = {}        --这个是针对整桌玩家来说的状态
g_gdmjUserState.state_waiting = 1
g_gdmjUserState.state_playing = 2

g_gdmjPokerList = {}  --麻将的基础列表
g_gdmjPokerList[1] = {11,12,13,14,15,16,17,18,19}   --九个万字
g_gdmjPokerList[2] = {21,22,23,24,25,26,27,28,29}   --九个筒子
g_gdmjPokerList[3] = {31,32,33,34,35,36,37,38,39}   --九个锁子
g_gdmjPokerList[4] = {41,42,43,44}   --东南西北
g_gdmjPokerList[5] = {51,52,53}      --中发白
g_gdmjPokerList[6] = {61,62,63,64}   --梅兰竹菊
g_gdmjPokerList[7] = {71,72,73,74}   --春夏秋冬


g_gdmjTime = {}
g_gdmjTime.waiting_time = 3  --发完牌后，等待的时间
g_gdmjTime.counting_time = 0 --3  --计算的时间
g_gdmjTime.action_time = 15 --每轮行动的时间
g_gdmjTime.end_time = 5  --结算的时候
g_gdmjTime.autostart_time = 15  --没轮结束后，在结束界面，20，等待固定时间后即可开始
g_gdmjTime.viptable_break_time = 180  --超过三分钟了，如果贵宾房的还没有开始，就自动解散
g_gdmjTime.vip_tabletype_money = 888
g_gdmjTime.dissolve_time = 120  --vip申请解散的时候，最长的解散时间
g_gdmjTime.play_robot_time = 15  --每一次自动响应的时间，如果超过这个时间，系统就开始自动打，这个表现在金币场中
g_gdmjTime.ready_time = 3600   --
g_gdmjTime.over_time = -7200  --倒计时到这个就解散

g_gdmjWinType = {}  --广东麻将赢牌的牌型
g_gdmjWinType.type_none = 0   --没有赢牌

g_gdmjWinType.type_jihu = 101  --鸡胡
g_gdmjWinType.type_pinghu = 102  --平胡：全是顺子，没有刻子
g_gdmjWinType.type_pengpenghu = 103  --碰碰胡：全是刻子，没有顺子
g_gdmjWinType.type_hunyise = 104  --混一色：由字牌和另外单一花色（筒、万或条）组成
g_gdmjWinType.type_qingyise = 105  --清一色：整副牌由同一花色组成
g_gdmjWinType.type_hunpeng = 106  --混碰：混一色+碰碰胡
g_gdmjWinType.type_qingpeng = 107  --清碰：清一色+碰碰胡
g_gdmjWinType.type_hunyaojiu = 108  --混幺九：由幺九牌和字牌组成的牌型
g_gdmjWinType.type_xiaosanyuan = 109  --小三元：拿齐中、发、白三种三元牌，其中一种是将
g_gdmjWinType.type_xiaosixi = 110  --小四喜：拿齐东、南、西、北四种字牌，其中一种是将
g_gdmjWinType.type_qixiaodui = 111  --七小对:
g_gdmjWinType.type_longqidui = 112  --龙七对:
g_gdmjWinType.type_ziyise = 113  --字一色：由字牌组成的牌型
g_gdmjWinType.type_qingyaojiu = 114  --清幺九：只有幺九两种牌组成的牌型
g_gdmjWinType.type_dasanyuan = 115  --大三元：有中发白三组刻子
g_gdmjWinType.type_dasixi = 116  --大四喜：有东、南、西、北四组刻子
g_gdmjWinType.type_jiulianbaodeng = 117  --九连宝灯：牌型组成1112345678999组成
g_gdmjWinType.type_shisanyao = 118  --十三幺，1,9，筒，索，东，南，西，北，中，白，发组成的任意一张牌
g_gdmjWinType.type_tianhe = 131  --天和
g_gdmjWinType.type_dihe = 132  --地和
g_gdmjWinType.type_renhe = 133  --人和



--g_gdmjWinType.type_hzz_quanyaojiu = 201   --惠州庄的全幺九：全是由幺九牌组成
--g_gdmjWinType.type_hzz_qingyaojiu = 202   --惠州庄的清幺九：清一色幺九牌+字牌
--g_gdmjWinType.type_hzz_zayaojiu = 203     --惠州庄的杂幺九：幺九牌+字牌

g_gdmjWinType.type_hzz_jihu = 201     --惠州庄的鸡胡
g_gdmjWinType.type_hzz_zase = 202     --惠州庄的杂色
g_gdmjWinType.type_hzz_duiduihu = 203    --惠州庄的对对胡，门清需要加分
g_gdmjWinType.type_hzz_qingyise = 204    --惠州庄的清一色，包括：全一色的鸡胡，清碰+字牌，杂幺九,七小对  门清需要加分
g_gdmjWinType.type_hzz_dage = 205    --惠州庄的大哥，包括：清一色+碰碰胡，清幺九，杂幺九  门清需要加分
g_gdmjWinType.type_hzz_shisanyao = 206    --惠州庄的十三幺，包括：十三幺，字一色，全幺九  门清需要加分
g_gdmjWinType.type_hzz_qidui = 207    --惠州庄的七对

g_gdmjWinType.type_siguihupai = 141  --四鬼胡牌
g_gdmjWinType.type_quanfeng = 142  --四鬼胡牌
g_gdmjWinMa = {}
g_gdmjWinMa[1] = {1,5,9,41,61,71}
g_gdmjWinMa[2] = {2,6,42,51,62,72}
g_gdmjWinMa[3] = {3,7,43,52,63,73}
g_gdmjWinMa[4] = {4,8,44,53,64,74}

g_gdmjHzzWinMa = {}  --惠州庄算马的规则
g_gdmjHzzWinMa[1] = {1,5,9,41,51,61,71}
g_gdmjHzzWinMa[2] = {2,6,42,52,62,72}
g_gdmjHzzWinMa[3] = {3,7,43,53,63,73}
g_gdmjHzzWinMa[4] = {4,8,44,64,74}

g_gdmjGangDetail = {}
g_gdmjGangDetail.type_angang = 1      --暗杠 
g_gdmjGangDetail.type_minggang = 2    --明杠
g_gdmjGangDetail.type_bugang = 3      --补杠


g_gdmjDefine.mark_shisanyao = {11,19,21,29,31,39,41,42,43,44,51,52,53}  --十三幺的判断列表，统一放到这里
g_gdmjDefine.mark_jiulanbaodeng = {1,1,1,2,3,4,5,6,7,8,9,9,9}
g_gdmjDefine.mark_alllist = {11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53}

g_gdmjDefine.mark_jiangHuArrNum = {2,5,8,11,14} --有将情况下需要胡的类型
g_gdmjDefine.mark_huArrNum = {0,3,9,12}      --无将情况下需要的数量


g_gdmjGuiType = {}
g_gdmjGuiType.type_wugui = 1
g_gdmjGuiType.type_baiban = 2
g_gdmjGuiType.type_fangui = 3

--表示风圈列表，东风东局，东风西局,1:东；2：南；3：西；4：北
--没当庄家发生变化的时候，这个值就会加1
g_gdmjFengQuan = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44}  


g_gdmjDefine.chat_txt = 1
g_gdmjDefine.chat_img = 2
g_gdmjDefine.chat_voice = 3


g_gdmjDefine.table_jbc = {}
g_gdmjDefine.table_jbc['free']   = 1
g_gdmjDefine.table_jbc['junior'] = 2
g_gdmjDefine.table_jbc['medium'] = 3
g_gdmjDefine.table_jbc['serior'] = 4
--进入金币车的最低分数
g_gdmjDefine.minJettonJBC = {}
g_gdmjDefine.minJettonJBC[g_gdmjDefine.table_jbc['free']] = 50
g_gdmjDefine.minJettonJBC[g_gdmjDefine.table_jbc['junior']] = 5000
g_gdmjDefine.minJettonJBC[g_gdmjDefine.table_jbc['medium']] = 30000
g_gdmjDefine.minJettonJBC[g_gdmjDefine.table_jbc['serior']] = 60000

g_gdmjDefine.maxJettonJBC = {}
g_gdmjDefine.maxJettonJBC[g_gdmjDefine.table_jbc['free']] = -1
g_gdmjDefine.maxJettonJBC[g_gdmjDefine.table_jbc['junior']] = -1
g_gdmjDefine.maxJettonJBC[g_gdmjDefine.table_jbc['medium']] = -1
g_gdmjDefine.maxJettonJBC[g_gdmjDefine.table_jbc['serior']] = -1


g_gdmjHzzWinJetton = {}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_jihu] = {3,"鸡胡",3}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_zase] = {6,"杂色",6}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_duiduihu] = {9,"对对胡",15}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_qingyise] = {15,"清一色",24}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_dage] = {24,"幺九",30}
g_gdmjHzzWinJetton[g_gdmjWinType.type_hzz_shisanyao] = {30, "十三幺",30}


g_gdmjActionNotice = {}
g_gdmjActionNotice.type_baodage = 101  --包大哥


g_gdmjDefine.pay_money = 1001   --g_goodsID.money
g_gdmjDefine.pay_jetton = 1002   --g_goodsID.jetton
g_gdmjDefine.pay_gold = 1003   --对应开发是的掌上币    --g_goodsID.gold

g_ErmjCardName = {}
g_ErmjCardName.dashixi = 1
g_ErmjCardName.dasanyuan = 2
g_ErmjCardName.jiulianbaoten = 3
g_ErmjCardName.xiaoyuwu = 4
g_ErmjCardName.dayuwu = 5
g_ErmjCardName.daqixing = 6
g_ErmjCardName.sigegang = 7
g_ErmjCardName.qiliandui = 8
g_ErmjCardName.tianhu = 9
g_ErmjCardName.dihu = 10

g_ErmjCardName.xiaoshixi = 11
g_ErmjCardName.xiaosanyuan = 12
g_ErmjCardName.shianke = 13
g_ErmjCardName.shuanglonghui = 14
g_ErmjCardName.ziyise = 15
g_ErmjCardName.renhu = 16
g_ErmjCardName.shitongshun = 17
g_ErmjCardName.sanyuanqidui = 18
g_ErmjCardName.shixiqidui = 19
g_ErmjCardName.shilianke = 20
g_ErmjCardName.shibugao = 21
g_ErmjCardName.hunyaojiu = 22
g_ErmjCardName.sangang = 23
g_ErmjCardName.tianting = 24
g_ErmjCardName.shizike = 25
g_ErmjCardName.dasanfeng = 26
g_ErmjCardName.santongshun = 27
g_ErmjCardName.qidui = 28
g_ErmjCardName.sanlianke = 29
g_ErmjCardName.qinglong = 30
g_ErmjCardName.sanbugao = 31
g_ErmjCardName.quanhua = 32
g_ErmjCardName.sananke = 33
g_ErmjCardName.qingyise = 34
g_ErmjCardName.miaoshohuichun = 35
g_ErmjCardName.haidilaoyuan = 36
g_ErmjCardName.ganshangkaihua = 37
g_ErmjCardName.qiangganghu = 38
g_ErmjCardName.xiaosanfeng = 39
g_ErmjCardName.shuangjianke = 40
g_ErmjCardName.pengpenghu = 41
g_ErmjCardName.shuangangang = 42
g_ErmjCardName.hunyise = 43
g_ErmjCardName.quanqiuren = 44
g_ErmjCardName.quandaiyao = 45
g_ErmjCardName.shuangminggang = 46
g_ErmjCardName.buqiuren = 47
g_ErmjCardName.hujuezhang = 48
g_ErmjCardName.menfengke = 49
g_ErmjCardName.quanfenke = 50
g_ErmjCardName.jianke    = 51
g_ErmjCardName.pinghu    = 52
g_ErmjCardName.shiguiyi  = 53
g_ErmjCardName.duanyao   = 54
g_ErmjCardName.shuanganke = 55
g_ErmjCardName.angang = 56
g_ErmjCardName.menqianqing  = 57
g_ErmjCardName.baoting      = 58
g_ErmjCardName.yibangao     = 59
g_ErmjCardName.lianliu      = 60
g_ErmjCardName.laoshaofu    = 61
g_ErmjCardName.huapai       = 62
g_ErmjCardName.minggang     = 63
g_ErmjCardName.bianzhang    = 64
g_ErmjCardName.kanzhang     = 65
g_ErmjCardName.dandiaojiang = 66
g_ErmjCardName.zimo         = 67

g_ErmjCardTypeIndex = {}
g_ErmjCardTypeIndex.name = 1         
g_ErmjCardTypeIndex.fanshu = 2
g_ErmjCardTypeIndex.switch = 3
g_ErmjCardType = {}
g_ErmjCardType.info = 
{
    [g_ErmjCardName.dashixi]        = {"大四喜",88,1},
    [g_ErmjCardName.dasanyuan]      = {"大三元",88,1},
    [g_ErmjCardName.jiulianbaoten]  = {"九莲宝灯",88,1},
    [g_ErmjCardName.xiaoyuwu]       = {"大于五",88,1},
    [g_ErmjCardName.dayuwu]         = {"小于五",88,1},
    [g_ErmjCardName.daqixing]       = {"大七星",88,1},
    [g_ErmjCardName.sigegang]       = {"四杠",88,1},
    [g_ErmjCardName.qiliandui]      = {"七连对",88,1},
    [g_ErmjCardName.tianhu]         = {"天胡",88,1},
    [g_ErmjCardName.dihu]           = {"地胡",88,1},

    [g_ErmjCardName.xiaoshixi]      = {"小四喜",64,1},
    [g_ErmjCardName.xiaosanyuan]    = {"小三元",64,1},
    [g_ErmjCardName.shianke]        = {"四暗刻",64,1},
    [g_ErmjCardName.shuanglonghui]  = {"双龙会",64,1},
    [g_ErmjCardName.ziyise]         = {"字一色",64,1},
    [g_ErmjCardName.renhu]          = {"人胡",64,1},

    [g_ErmjCardName.shitongshun]    = {"四同顺",48,1},
    [g_ErmjCardName.sanyuanqidui]   = {"三元七对子",48,1},
    [g_ErmjCardName.shixiqidui]     = {"四喜七对子",48,1},
    [g_ErmjCardName.shilianke]      = {"四连刻",48,1},

    [g_ErmjCardName.shibugao]       = {"四步高",32,1},
    [g_ErmjCardName.hunyaojiu]      = {"混幺九",32,1},
    [g_ErmjCardName.sangang]        = {"三杠",32,1},
    [g_ErmjCardName.tianting]       = {"天听",32,1},
    [g_ErmjCardName.shizike]        = {"四字刻",24,1},
    [g_ErmjCardName.dasanfeng]      = {"大三风",24,1},
    [g_ErmjCardName.santongshun]    = {"三同顺",24,1},
    [g_ErmjCardName.qidui]          = {"七对子",24,1},
    [g_ErmjCardName.sanlianke]      = {"三连刻",24,1},
    [g_ErmjCardName.qinglong]       = {"清龙",16,1},
    [g_ErmjCardName.sanbugao]       = {"三步高",16,1},
    [g_ErmjCardName.quanhua]        = {"全花",16,1},
    [g_ErmjCardName.sananke]        = {"三暗刻",16,1},
    [g_ErmjCardName.qingyise]       = {"清一色",16,1},

    [g_ErmjCardName.miaoshohuichun] = {"妙手回春",8,1},
    [g_ErmjCardName.haidilaoyuan]   = {"海底捞月",8,1},
    [g_ErmjCardName.ganshangkaihua] = {"杠上开花",8,1},
    [g_ErmjCardName.qiangganghu]    = {"抢杠胡",8,1},

    [g_ErmjCardName.xiaosanfeng]    = {"小三风",6,1},
    [g_ErmjCardName.shuangjianke]   = {"双箭刻",6,1},
    [g_ErmjCardName.pengpenghu]     = {"碰碰胡",6,1},
    [g_ErmjCardName.shuangangang]   = {"双暗杠",6,1},
    [g_ErmjCardName.hunyise]        = {"混一色",6,1},
    [g_ErmjCardName.quanqiuren]     = {"全求人",6,1},

    [g_ErmjCardName.quandaiyao]     = {"全带幺",4,1},
    [g_ErmjCardName.shuangminggang] = {"双明杠",4,1},
    [g_ErmjCardName.buqiuren]       = {"不求人",4,1},
    [g_ErmjCardName.hujuezhang]     = {"胡绝张",4,1},

    [g_ErmjCardName.menfengke]      = {"门风刻",2,1},
    [g_ErmjCardName.quanfenke]      = {"圈风刻",2,1},
    [g_ErmjCardName.jianke]         = {"箭刻",2,1},
    [g_ErmjCardName.pinghu]         = {"平胡",2,1},
    [g_ErmjCardName.shiguiyi]       = {"四归一",2,1},
    [g_ErmjCardName.duanyao]        = {"断幺",2,1},
    [g_ErmjCardName.shuanganke]     = {"双暗刻",2,1},
    [g_ErmjCardName.angang]         = {"暗杠",2,1},
    [g_ErmjCardName.menqianqing]    = {"门前清",2,1},
    [g_ErmjCardName.baoting]        = {"报听",2,1},

    [g_ErmjCardName.yibangao]       = {"一般高",1,1},
    [g_ErmjCardName.lianliu]        = {"连六",1,1},
    [g_ErmjCardName.laoshaofu]      = {"老少副",1,1},
    [g_ErmjCardName.huapai]         = {"花牌",1,0},       --花牌这里不处理
    [g_ErmjCardName.minggang]       = {"明杠",1,1},
    [g_ErmjCardName.bianzhang]      = {"边张",1,1},
    [g_ErmjCardName.kanzhang]       = {"坎张",1,1},
    [g_ErmjCardName.dandiaojiang]   = {"单钓将",1,1},
    [g_ErmjCardName.zimo]           = {"自摸",1,1}
}
