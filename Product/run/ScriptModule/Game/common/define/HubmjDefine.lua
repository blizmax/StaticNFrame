
g_hubmjTableType = {}
g_hubmjTableType["junior"] = 1
g_hubmjTableType["medium"] = 2
g_hubmjTableType["serior"] = 3

g_hubmjNeedJetton = {2000,20000,100000}

g_hubmjPourList = {400,1200,4000}
g_hubmjPourList[1] = 400
g_hubmjPourList[2] = 1200
g_hubmjPourList[3] = 4000

g_hubmjJbcMaNum = {2,4,4}



g_hubmjAction = {}
g_hubmjAction.type_play = 0   --出牌
g_hubmjAction.type_guo = 1    --过牌
g_hubmjAction.type_chi = 2    --吃
g_hubmjAction.type_peng = 3   --碰
g_hubmjAction.type_gang = 4   --杠
g_hubmjAction.type_hu = 5     --胡
g_hubmjAction.type_qiangganghu = 6   --抢杠胡
g_hubmjAction.type_chihu = 7         --吃胡
g_hubmjAction.type_genzhuang = 8    --跟庄，这个操作只是在客户端有显示，服务器判断后主动发给客户端
g_hubmjAction.type_mingpai = 9    --卡五星中，明牌的操作
g_hubmjAction.type_koupai = 10    --卡五星中，扣牌的牌型

g_hubmjType = {}  --广东麻将的类型
g_hubmjType.type_jzhh = 2811    --荆州晃晃
g_hubmjType.type_yjly = 2812   --一脚癞油
g_hubmjType.type_yldd = 2813    --一癞到底
g_hubmjType.type_jhmj = 2814    --监洪麻将

--g_hubmjType.type_jzhh_jbc = 311    --金币场,金币场中，默认是推倒胡
--g_hubmjType.type_ = 312    --金币场,金币场中，默认是推倒胡



g_hubmjStatus = {}   --麻将牌桌的状态
g_hubmjStatus.status_none = 0  --空闲的状态，不用处理
g_hubmjStatus.status_ready = 1  --玩家正在等待，还没有全部点准备
g_hubmjStatus.status_waiting = 2  --玩家已经全部点准备了，可以开始了
g_hubmjStatus.status_playing = 3  --正在玩家，这时候，玩家不能退出
g_hubmjStatus.status_counting = 4  --结算的时候
g_hubmjStatus.status_end = 5  --每一个牌局结束后都会有10秒的动画时间
g_hubmjStatus.status_dissolve = 6  --解散房间
g_hubmjStatus.status_delete = 7  --在循环中，如果碰到这种状态的牌桌，就把该牌桌删除

g_hubmjPlayerState = {}  --麻将玩家的状态,针对玩家个人
g_hubmjPlayerState.play_none = 0  --空的状态
g_hubmjPlayerState.play_ready = 1  --玩家已经坐下，还没点击准备的按钮
g_hubmjPlayerState.play_waiting = 2   --等待行动的状态
g_hubmjPlayerState.play_action = 3 --轮到自己行动,这个是行动，会闪亮的
g_hubmjPlayerState.play_optional = 4 --轮到自己选择了，碰和杠的情况，下面的灯是不会亮的

g_hubmjUserState = {}        --这个是针对整桌玩家来说的状态
g_hubmjUserState.state_waiting = 1
g_hubmjUserState.state_playing = 2

g_hubmjPokerList = {}  --麻将的基础列表
g_hubmjPokerList[1] = {11,12,13,14,15,16,17,18,19}   --九个万字
g_hubmjPokerList[2] = {21,22,23,24,25,26,27,28,29}   --九个筒子
g_hubmjPokerList[3] = {31,32,33,34,35,36,37,38,39}   --九个锁子
g_hubmjPokerList[4] = {41,42,43,44}   --东南西北
g_hubmjPokerList[5] = {51,52,53}      --中发白
g_hubmjPokerList[6] = {61,62,63,64}   --梅兰竹菊
g_hubmjPokerList[7] = {71,72,73,74}   --春夏秋冬



g_hubmjTime = {}
g_hubmjTime.waiting_time = 3  --发完牌后，等待的时间
g_hubmjTime.counting_time = 0 --3  --计算的时间
g_hubmjTime.action_time = 15 --每轮行动的时间
g_hubmjTime.end_time = 5  --结算的时候
g_hubmjTime.autostart_time = 15  --没轮结束后，在结束界面，20，等待固定时间后即可开始
g_hubmjTime.viptable_break_time = 30  --超过三分钟了，如果贵宾房的还没有开始，就自动解散
g_hubmjTime.vip_tabletype_money = 888
g_hubmjTime.dissolve_time = 120  --vip申请解散的时候，最长的解散时间
g_hubmjTime.play_robot_time = 15  --每一次自动响应的时间，如果超过这个时间，系统就开始自动打，这个表现在金币场中
g_hubmjTime.ready_time = 600   --创建的每个房间，如果10分钟还没有开始，就解散
g_hubmjTime.over_time = -7200  --倒计时到这个就解散
g_hubmjWinType = {}  --广东麻将赢牌的牌型
g_hubmjWinType.type_none = 0   --没有赢牌

g_hubmjWinType.type_ruanzimo = 1   --软自摸是1番
g_hubmjWinType.type_zimo = 2   --自摸是1番，自摸是2番


g_hubmjGangDetail = {}
g_hubmjGangDetail.type_angang = 1      --暗杠 
g_hubmjGangDetail.type_minggang = 2    --明杠
g_hubmjGangDetail.type_bugang = 3      --补杠

g_hubmjGangDetail.type_laizigang = 11      --癞子杠
g_hubmjGangDetail.type_hongzhonggang = 12      --红中杠
g_hubmjGangDetail.type_chaotian = 13      --朝天杠
g_hubmjGangDetail.type_buhua = 14      --补花



g_hubmjDefine = {}

g_hubmjDefine.mark_alllist = {11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,33,34,35,36,37,38,39,41,42,43,44,51,52,53}



--表示风圈列表，东风东局，东风西局,1:东；2：南；3：西；4：北
--没当庄家发生变化的时候，这个值就会加1
g_hubmjFengQuan = {11,12,13,14,21,22,23,24,31,32,33,34,41,42,43,44}  


g_hubmjDefine.chat_txt = 1
g_hubmjDefine.chat_img = 2
g_hubmjDefine.chat_voice = 3