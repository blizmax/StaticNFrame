

--牛牛的牌型是三位数，第一位是花色后面两位是牌大小
--小王是：515，大王是525


g_sscaiDefine = {}

g_sscaiDefine.pour_type = {1,2,3,4}

g_sscaiDefine.table_type = 1


g_sscaiDefine.table_common = 2038888

g_sscaiDefine.init_sit = 6      --初始化的6个座位
g_sscaiDefine.page_item = 8   --每一页显示的牌桌的数量
g_sscaiDefine.min_bankjetton = 100000    --最小上庄的倍数
g_sscaiDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_sscaiDefine.min_pour_common = 100   --普通场最小下注
g_sscaiDefine.min_pour_grade = 10000  --高级场最小下注
g_sscaiDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_sscaiDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_sscaiDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_sscaiDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_sscaiDefine.pour2_rate = 10
	g_sscaiDefine.pour3_rate = 50
	
	g_sscaiDefine.min_pour_common = 100
	g_sscaiDefine.min_pour_grade = 1000
end


g_sscaiDefine.state_ready = 1
g_sscaiDefine.state_start = 2
g_sscaiDefine.state_play = 3
g_sscaiDefine.state_count = 4
g_sscaiDefine.state_end = 5


g_sscaiDefine.time_start = 10
g_sscaiDefine.time_play = 15
g_sscaiDefine.time_count = 30
g_sscaiDefine.time_end = 15

g_sscaiDefine.chat_txt = 1
g_sscaiDefine.chat_img = 2
g_sscaiDefine.chat_voice = 3


g_sscaiDefine.type_ydc = 1       --欢乐赢
g_sscaiDefine.type_lxc = 2

g_sscaiDefine.leave_dismiss = 1   --房主解散房间
g_sscaiDefine.leave_timeout = 2   --五轮没下注被踢出

g_sscaiDefine.quit_count  = 10     --几轮没下注后就被踢出

g_sscaiDefine.time_public = 20  --如果是大众长，就有20秒的掉线时间
g_sscaiDefine.time_private = 300  --如果是私人场，则有5分钟的时间

g_sscaiDefine.day_init_gold = 10000  --每天第一次登录的时候，只在竞技场中给10000分的奖励


g_sscaicard_type = {}

g_sscaicard_type.san_pai = 1     --散牌
g_sscaicard_type.dui_zi = 2     --对子
g_sscaicard_type.shun_zi = 3    --顺子
g_sscaicard_type.jin_hua = 4     --金花
g_sscaicard_type.shun_jin = 5     --顺金
g_sscaicard_type.bao_zi = 6    --豹子



g_sscaicard_type.win_rate = {3,4,5,6,10,100}  --这个是赔率

--牌定义
g_sscaiDefine.pokerList = {
101,102,103,104,105,106,107,108,109,110,111,112,113,
201,202,203,204,205,206,207,208,209,210,211,212,213,
301,302,303,304,305,306,307,308,309,310,311,312,313,
401,402,403,404,405,406,407,408,409,410,411,412,413
}
