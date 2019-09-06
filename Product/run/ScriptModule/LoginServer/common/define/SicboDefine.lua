g_sicboDefine = {}

g_sicboDefine.game_type = 5600

g_sicboDefine.roundtimes = 60*60*24 --多久轮数清零 24小时

g_sicboDefine.init_sit = 6    --初始化的6个座位

g_sicboDefine.table_common = 777777

g_sicboDefine.page_item = 8   --每一页显示的牌桌的数量

--下注区域
g_sicboDefine.pour_type = {}  
g_sicboDefine.pour_type[1]	=	1	--	小
g_sicboDefine.pour_type[2]	=	2	--	大
g_sicboDefine.pour_type[3]	=	3	--	双骰（1）
g_sicboDefine.pour_type[4]	=	4	--	双骰（2）
g_sicboDefine.pour_type[5]	=	5	--	双骰（3）
g_sicboDefine.pour_type[6]	=	6	--	双骰（4）
g_sicboDefine.pour_type[7]	=	7	--	双骰（5）
g_sicboDefine.pour_type[8]	=	8	--	双骰（6）
g_sicboDefine.pour_type[9]	=	9	--	全围
g_sicboDefine.pour_type[10]	=	10	--	围骰（1）
g_sicboDefine.pour_type[11]	=	11	--	围骰（2）
g_sicboDefine.pour_type[12]	=	12	--	围骰（3）
g_sicboDefine.pour_type[13]	=	13	--	围骰（4）
g_sicboDefine.pour_type[14]	=	14	--	围骰（5）
g_sicboDefine.pour_type[15]	=	15	--	围骰（6）
g_sicboDefine.pour_type[16]	=	16	--	4点
g_sicboDefine.pour_type[17]	=	17	--	5点
g_sicboDefine.pour_type[18]	=	18	--	6点
g_sicboDefine.pour_type[19]	=	19	--	7点
g_sicboDefine.pour_type[20]	=	20	--	8点
g_sicboDefine.pour_type[21]	=	21	--	9点
g_sicboDefine.pour_type[22]	=	22	--	10点
g_sicboDefine.pour_type[23]	=	23	--	11点
g_sicboDefine.pour_type[24]	=	24	--	12点
g_sicboDefine.pour_type[25]	=	25	--	13点
g_sicboDefine.pour_type[26]	=	26	--	14点
g_sicboDefine.pour_type[27]	=	27	--	15点
g_sicboDefine.pour_type[28]	=	28	--	16点
g_sicboDefine.pour_type[29]	=	29	--	17点
g_sicboDefine.pour_type[30]	=	30	--	一
g_sicboDefine.pour_type[31]	=	31	--	二
g_sicboDefine.pour_type[32]	=	32	--	三
g_sicboDefine.pour_type[33]	=	33	--	四
g_sicboDefine.pour_type[34]	=	34	--	五
g_sicboDefine.pour_type[35]	=	35	--	六


g_sicbocard_type = {}
g_sicbocard_type.xiao = 1    	        --小(4~10)
g_sicbocard_type.da = 2    		        --大(11~17)

g_sicbocard_type.win_rate = {}          --这个是赔率
g_sicbocard_type.win_rate[1]	=	1	--	小
g_sicbocard_type.win_rate[2]	=	1	--	大
g_sicbocard_type.win_rate[3]	=	10	--	双骰（1）
g_sicbocard_type.win_rate[4]	=	10	--	双骰（2）
g_sicbocard_type.win_rate[5]	=	10	--	双骰（3）
g_sicbocard_type.win_rate[6]	=	10	--	双骰（4）
g_sicbocard_type.win_rate[7]	=	10	--	双骰（5）
g_sicbocard_type.win_rate[8]	=	10	--	双骰（6）
g_sicbocard_type.win_rate[9]	=	30	--	全围
g_sicbocard_type.win_rate[10]	=	200	--	围骰（1）
g_sicbocard_type.win_rate[11]	=	200	--	围骰（2）
g_sicbocard_type.win_rate[12]	=	200	--	围骰（3）
g_sicbocard_type.win_rate[13]	=	200	--	围骰（4）
g_sicbocard_type.win_rate[14]	=	200	--	围骰（5）
g_sicbocard_type.win_rate[15]	=	200	--	围骰（6）
g_sicbocard_type.win_rate[16]	=	60	--	4点
g_sicbocard_type.win_rate[17]	=	30	--	5点
g_sicbocard_type.win_rate[18]	=	20	--	6点
g_sicbocard_type.win_rate[19]	=	12	--	7点
g_sicbocard_type.win_rate[20]	=	8	--	8点
g_sicbocard_type.win_rate[21]	=	6	--	9点
g_sicbocard_type.win_rate[22]	=	6	--	10点
g_sicbocard_type.win_rate[23]	=	6	--	11点
g_sicbocard_type.win_rate[24]	=	6	--	12点
g_sicbocard_type.win_rate[25]	=	8	--	13点
g_sicbocard_type.win_rate[26]	=	12	--	14点
g_sicbocard_type.win_rate[27]	=	20	--	15点
g_sicbocard_type.win_rate[28]	=	30	--	16点
g_sicbocard_type.win_rate[29]	=	60	--	17点
-- 根据结果计算 单骰 1：1    双骰1: 2     三骰1:3
g_sicbocard_type.win_rate[30]	=	0	--	一
g_sicbocard_type.win_rate[31]	=	0	--	二
g_sicbocard_type.win_rate[32]	=	0	--	三
g_sicbocard_type.win_rate[33]	=	0	--	四
g_sicbocard_type.win_rate[34]	=	0	--	五
g_sicbocard_type.win_rate[35]	=	0	--	六


g_sicboDefine.min_bankjetton = 100000    --最小上庄的倍数
g_sicboDefine.min_pri_bankjetton = 50000    --私人场中最小上庄的倍数

g_sicboDefine.min_pour_common = 100  --普通场最小下注
g_sicboDefine.min_pour_grade = 10000  --高级场最小下注
g_sicboDefine.pour2_rate = 10         --第二个按钮和第一个的倍数
g_sicboDefine.pour3_rate = 100        --第三个和第一个的倍数

if g_servername == "run_tyyd" then
	g_sicboDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_sicboDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_sicboDefine.pour2_rate = 10
	g_sicboDefine.pour3_rate = 50
	
	g_sicboDefine.min_pour_common = 100
	g_sicboDefine.min_pour_grade = 1000
end


--现在，我们初始化的牌桌数量有我们自己指定创建，没有必要动态创建，后期看需要加入到数据库中动态创建
--所有牌桌都一样，暂时不区分类型

g_sicboDefine.init_data = {}                 --初始化的数据，一开始初始化六个类型的牌桌，先初始化一个
g_sicboDefine.init_data[1] = {}
g_sicboDefine.init_data[2] = {}
--g_sicboDefine.init_data[3] = {}
--g_sicboDefine.init_data[4] = {}
--g_sicboDefine.init_data[5] = {}
--g_sicboDefine.init_data[6] = {}


g_sicboDefine.init_data[1]['tableid'] = {2201001}     --这里用table，防止后面有需要搭建多个同样类型的
g_sicboDefine.init_data[1]['maxuser'] = 999
g_sicboDefine.init_data[1]['tablename'] = "荣耀厅 01"
g_sicboDefine.init_data[1]['bankerid'] = 1
g_sicboDefine.init_data[1]['bankername'] = '青醉'
g_sicboDefine.init_data[1]['bankernamelist'] = {'青醉','狂想曲','wx100','江山美人','财神爷','赢他一个亿','发发发','百万梦想'}
g_sicboDefine.init_data[1]['faceid'] = "90001_1001"
g_sicboDefine.init_data[1]['banklimit'] = 200000
g_sicboDefine.init_data[1]['enterlimit'] = 5000
g_sicboDefine.init_data[1]['entermax'] = -1
g_sicboDefine.init_data[1]['tabletype'] = 1
g_sicboDefine.init_data[1]['bankjetton'] = 10000000
g_sicboDefine.init_data[1]['jiangchibalance'] = 1000 --奖池临界调控值
g_sicboDefine.init_data[1]['userleave'] = 5 --真人多少次没下注离开
g_sicboDefine.init_data[1]['limitred'] = 2000000 --限红
--下注区域最大投注限制，最小为1
g_sicboDefine.init_data[1]['pourlimit'] = {}
g_sicboDefine.init_data[1]['pourlimit'][1]	=	5000	--	小
g_sicboDefine.init_data[1]['pourlimit'][2]	=	5000	--	大
g_sicboDefine.init_data[1]['pourlimit'][3]	=	750	    --	双骰（1）
g_sicboDefine.init_data[1]['pourlimit'][4]	=	750	    --	双骰（2）
g_sicboDefine.init_data[1]['pourlimit'][5]	=	750	    --	双骰（3）
g_sicboDefine.init_data[1]['pourlimit'][6]	=	750	    --	双骰（4）
g_sicboDefine.init_data[1]['pourlimit'][7]	=	750	    --	双骰（5）
g_sicboDefine.init_data[1]['pourlimit'][8]	=	750	    --	双骰（6）
g_sicboDefine.init_data[1]['pourlimit'][9]	=	250	    --	全围
g_sicboDefine.init_data[1]['pourlimit'][10]	=	100	    --	围骰（1）
g_sicboDefine.init_data[1]['pourlimit'][11]	=	100	    --	围骰（2）
g_sicboDefine.init_data[1]['pourlimit'][12]	=	100	    --	围骰（3）
g_sicboDefine.init_data[1]['pourlimit'][13]	=	100	    --	围骰（4）
g_sicboDefine.init_data[1]['pourlimit'][14]	=	100	    --	围骰（5）
g_sicboDefine.init_data[1]['pourlimit'][15]	=	100	    --	围骰（6）
g_sicboDefine.init_data[1]['pourlimit'][16]	=	150	    --	4点
g_sicboDefine.init_data[1]['pourlimit'][17]	=	500	    --	5点
g_sicboDefine.init_data[1]['pourlimit'][18]	=	500	    --	6点
g_sicboDefine.init_data[1]['pourlimit'][19]	=	500	    --	7点
g_sicboDefine.init_data[1]['pourlimit'][20]	=	750	    --	8点
g_sicboDefine.init_data[1]['pourlimit'][21]	=	1000	--	9点
g_sicboDefine.init_data[1]['pourlimit'][22]	=	1000	--	10点
g_sicboDefine.init_data[1]['pourlimit'][23]	=	1000	--	11点
g_sicboDefine.init_data[1]['pourlimit'][24]	=	1000	--	12点
g_sicboDefine.init_data[1]['pourlimit'][25]	=	750	    --	13点
g_sicboDefine.init_data[1]['pourlimit'][26]	=	500	    --	14点
g_sicboDefine.init_data[1]['pourlimit'][27]	=	500	    --	15点
g_sicboDefine.init_data[1]['pourlimit'][28]	=	500	    --	16点
g_sicboDefine.init_data[1]['pourlimit'][29]	=	150	    --	17点
g_sicboDefine.init_data[1]['pourlimit'][30]	=	2500	--	一
g_sicboDefine.init_data[1]['pourlimit'][31]	=	2500	--	二
g_sicboDefine.init_data[1]['pourlimit'][32]	=	2500	--	三
g_sicboDefine.init_data[1]['pourlimit'][33]	=	2500	--	四
g_sicboDefine.init_data[1]['pourlimit'][34]	=	2500	--	五
g_sicboDefine.init_data[1]['pourlimit'][35]	=	2500	--	六


g_sicboDefine.init_data[2]['tableid'] = {2202001}    --对应的tableid
g_sicboDefine.init_data[2]['maxuser'] = 999
g_sicboDefine.init_data[2]['tablename'] = "荣耀厅 02"
g_sicboDefine.init_data[2]['bankerid'] = 1
g_sicboDefine.init_data[2]['bankername'] = '王者、巅峰'
g_sicboDefine.init_data[2]['bankernamelist'] = {'echo','why me','o-o','千年三梦','后悔无期','长生不老','梦想家','来一发'}
g_sicboDefine.init_data[2]['faceid'] = "90002_1001"
g_sicboDefine.init_data[2]['banklimit'] = 200000
g_sicboDefine.init_data[2]['enterlimit'] = 5000
g_sicboDefine.init_data[2]['entermax'] = -1
g_sicboDefine.init_data[2]['tabletype'] = 2
g_sicboDefine.init_data[2]['bankjetton'] = 10000000
g_sicboDefine.init_data[2]['jiangchibalance'] = 1000 --奖池临界调控值
g_sicboDefine.init_data[2]['userleave'] = 5 --真人多少次没下注离开
g_sicboDefine.init_data[2]['limitred'] = 2000000 --限红
--下注区域最大投注限制，最小为1
g_sicboDefine.init_data[2]['pourlimit'] = {}
g_sicboDefine.init_data[2]['pourlimit'][1]	=	5000	--	小
g_sicboDefine.init_data[2]['pourlimit'][2]	=	5000	--	大
g_sicboDefine.init_data[2]['pourlimit'][3]	=	750	    --	双骰（1）
g_sicboDefine.init_data[2]['pourlimit'][4]	=	750	    --	双骰（2）
g_sicboDefine.init_data[2]['pourlimit'][5]	=	750	    --	双骰（3）
g_sicboDefine.init_data[2]['pourlimit'][6]	=	750	    --	双骰（4）
g_sicboDefine.init_data[2]['pourlimit'][7]	=	750	    --	双骰（5）
g_sicboDefine.init_data[2]['pourlimit'][8]	=	750	    --	双骰（6）
g_sicboDefine.init_data[2]['pourlimit'][9]	=	250	    --	全围
g_sicboDefine.init_data[2]['pourlimit'][10]	=	100	    --	围骰（1）
g_sicboDefine.init_data[2]['pourlimit'][11]	=	100	    --	围骰（2）
g_sicboDefine.init_data[2]['pourlimit'][12]	=	100	    --	围骰（3）
g_sicboDefine.init_data[2]['pourlimit'][13]	=	100	    --	围骰（4）
g_sicboDefine.init_data[2]['pourlimit'][14]	=	100	    --	围骰（5）
g_sicboDefine.init_data[2]['pourlimit'][15]	=	100	    --	围骰（6）
g_sicboDefine.init_data[2]['pourlimit'][16]	=	150	    --	4点
g_sicboDefine.init_data[2]['pourlimit'][17]	=	500	    --	5点
g_sicboDefine.init_data[2]['pourlimit'][18]	=	500	    --	6点
g_sicboDefine.init_data[2]['pourlimit'][19]	=	500	    --	7点
g_sicboDefine.init_data[2]['pourlimit'][20]	=	750	    --	8点
g_sicboDefine.init_data[2]['pourlimit'][21]	=	1000	--	9点
g_sicboDefine.init_data[2]['pourlimit'][22]	=	1000	--	10点
g_sicboDefine.init_data[2]['pourlimit'][23]	=	1000	--	11点
g_sicboDefine.init_data[2]['pourlimit'][24]	=	1000	--	12点
g_sicboDefine.init_data[2]['pourlimit'][25]	=	750	    --	13点
g_sicboDefine.init_data[2]['pourlimit'][26]	=	500	    --	14点
g_sicboDefine.init_data[2]['pourlimit'][27]	=	500	    --	15点
g_sicboDefine.init_data[2]['pourlimit'][28]	=	500	    --	16点
g_sicboDefine.init_data[2]['pourlimit'][29]	=	150	    --	17点
g_sicboDefine.init_data[2]['pourlimit'][30]	=	2500	--	一
g_sicboDefine.init_data[2]['pourlimit'][31]	=	2500	--	二
g_sicboDefine.init_data[2]['pourlimit'][32]	=	2500	--	三
g_sicboDefine.init_data[2]['pourlimit'][33]	=	2500	--	四
g_sicboDefine.init_data[2]['pourlimit'][34]	=	2500	--	五
g_sicboDefine.init_data[2]['pourlimit'][35]	=	2500	--	六

--g_sicboDefine.init_data[3]['tableid'] = {2203001}    --对应的tableid
--g_sicboDefine.init_data[3]['maxuser'] = 999
--g_sicboDefine.init_data[3]['tablename'] = "荣耀厅 03"
--g_sicboDefine.init_data[3]['bankerid'] = 1
--g_sicboDefine.init_data[3]['bankername'] = '心有阳光'
--g_sicboDefine.init_data[3]['bankernamelist'] = {'赌&神','pwd2000','TECH ME','出入江湖','三贱客','可儿','100cc','九姑娘'}
--g_sicboDefine.init_data[3]['faceid'] = "90003_1001"
--g_sicboDefine.init_data[3]['banklimit'] = 500000
--g_sicboDefine.init_data[3]['enterlimit'] = 5000
--g_sicboDefine.init_data[3]['entermax'] = -1
--g_sicboDefine.init_data[3]['tabletype'] = 3
--g_sicboDefine.init_data[3]['bankjetton'] = 10000000
--g_sicboDefine.init_data[3]['jiangchibalance'] = 1000 --奖池临界调控值
--g_sicboDefine.init_data[3]['userleave'] = 5 --真人多少次没下注离开
--g_sicboDefine.init_data[3]['limitred'] = 2000000 --限红
----下注区域最大投注限制，最小为1
--g_sicboDefine.init_data[3]['pourlimit'] = {}
--g_sicboDefine.init_data[3]['pourlimit'][1]	=	5000	--	小
--g_sicboDefine.init_data[3]['pourlimit'][2]	=	5000	--	大
--g_sicboDefine.init_data[3]['pourlimit'][3]	=	750	    --	双骰（1）
--g_sicboDefine.init_data[3]['pourlimit'][4]	=	750	    --	双骰（2）
--g_sicboDefine.init_data[3]['pourlimit'][5]	=	750	    --	双骰（3）
--g_sicboDefine.init_data[3]['pourlimit'][6]	=	750	    --	双骰（4）
--g_sicboDefine.init_data[3]['pourlimit'][7]	=	750	    --	双骰（5）
--g_sicboDefine.init_data[3]['pourlimit'][8]	=	750	    --	双骰（6）
--g_sicboDefine.init_data[3]['pourlimit'][9]	=	250	    --	全围
--g_sicboDefine.init_data[3]['pourlimit'][10]	=	100	    --	围骰（1）
--g_sicboDefine.init_data[3]['pourlimit'][11]	=	100	    --	围骰（2）
--g_sicboDefine.init_data[3]['pourlimit'][12]	=	100	    --	围骰（3）
--g_sicboDefine.init_data[3]['pourlimit'][13]	=	100	    --	围骰（4）
--g_sicboDefine.init_data[3]['pourlimit'][14]	=	100	    --	围骰（5）
--g_sicboDefine.init_data[3]['pourlimit'][15]	=	100	    --	围骰（6）
--g_sicboDefine.init_data[3]['pourlimit'][16]	=	150	    --	4点
--g_sicboDefine.init_data[3]['pourlimit'][17]	=	500	    --	5点
--g_sicboDefine.init_data[3]['pourlimit'][18]	=	500	    --	6点
--g_sicboDefine.init_data[3]['pourlimit'][19]	=	500	    --	7点
--g_sicboDefine.init_data[3]['pourlimit'][20]	=	750	    --	8点
--g_sicboDefine.init_data[3]['pourlimit'][21]	=	1000	--	9点
--g_sicboDefine.init_data[3]['pourlimit'][22]	=	1000	--	10点
--g_sicboDefine.init_data[3]['pourlimit'][23]	=	1000	--	11点
--g_sicboDefine.init_data[3]['pourlimit'][24]	=	1000	--	12点
--g_sicboDefine.init_data[3]['pourlimit'][25]	=	750	    --	13点
--g_sicboDefine.init_data[3]['pourlimit'][26]	=	500	    --	14点
--g_sicboDefine.init_data[3]['pourlimit'][27]	=	500	    --	15点
--g_sicboDefine.init_data[3]['pourlimit'][28]	=	500	    --	16点
--g_sicboDefine.init_data[3]['pourlimit'][29]	=	150	    --	17点
--g_sicboDefine.init_data[3]['pourlimit'][30]	=	2500	--	一
--g_sicboDefine.init_data[3]['pourlimit'][31]	=	2500	--	二
--g_sicboDefine.init_data[3]['pourlimit'][32]	=	2500	--	三
--g_sicboDefine.init_data[3]['pourlimit'][33]	=	2500	--	四
--g_sicboDefine.init_data[3]['pourlimit'][34]	=	2500	--	五
--g_sicboDefine.init_data[3]['pourlimit'][35]	=	2500	--	六

--g_sicboDefine.init_data[4]['tableid'] = {2204001}    --对应的tableid
--g_sicboDefine.init_data[4]['maxuser'] = 999
--g_sicboDefine.init_data[4]['tablename'] = "荣耀厅 04"
--g_sicboDefine.init_data[4]['bankerid'] = 1
--g_sicboDefine.init_data[4]['bankername'] = '初夏'
--g_sicboDefine.init_data[4]['bankernamelist'] = {'mygod','北京男孩','剩蛋哥','芝加哥不夜城','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_sicboDefine.init_data[4]['faceid'] = "90004_1001"
--g_sicboDefine.init_data[4]['banklimit'] = 500000
--g_sicboDefine.init_data[4]['enterlimit'] = 5000
--g_sicboDefine.init_data[4]['entermax'] = -1
--g_sicboDefine.init_data[4]['tabletype'] = 4
--g_sicboDefine.init_data[4]['bankjetton'] = 10000000
--g_sicboDefine.init_data[4]['jiangchibalance'] = 1000 --奖池临界调控值
--g_sicboDefine.init_data[4]['userleave'] = 5 --真人多少次没下注离开
--g_sicboDefine.init_data[4]['limitred'] = 2000000 --限红
----下注区域最大投注限制，最小为1
--g_sicboDefine.init_data[4]['pourlimit'] = {}
--g_sicboDefine.init_data[4]['pourlimit'][1]	=	5000	--	小
--g_sicboDefine.init_data[4]['pourlimit'][2]	=	5000	--	大
--g_sicboDefine.init_data[4]['pourlimit'][3]	=	750	    --	双骰（1）
--g_sicboDefine.init_data[4]['pourlimit'][4]	=	750	    --	双骰（2）
--g_sicboDefine.init_data[4]['pourlimit'][5]	=	750	    --	双骰（3）
--g_sicboDefine.init_data[4]['pourlimit'][6]	=	750	    --	双骰（4）
--g_sicboDefine.init_data[4]['pourlimit'][7]	=	750	    --	双骰（5）
--g_sicboDefine.init_data[4]['pourlimit'][8]	=	750	    --	双骰（6）
--g_sicboDefine.init_data[4]['pourlimit'][9]	=	250	    --	全围
--g_sicboDefine.init_data[4]['pourlimit'][10]	=	100	    --	围骰（1）
--g_sicboDefine.init_data[4]['pourlimit'][11]	=	100	    --	围骰（2）
--g_sicboDefine.init_data[4]['pourlimit'][12]	=	100	    --	围骰（3）
--g_sicboDefine.init_data[4]['pourlimit'][13]	=	100	    --	围骰（4）
--g_sicboDefine.init_data[4]['pourlimit'][14]	=	100	    --	围骰（5）
--g_sicboDefine.init_data[4]['pourlimit'][15]	=	100	    --	围骰（6）
--g_sicboDefine.init_data[4]['pourlimit'][16]	=	150	    --	4点
--g_sicboDefine.init_data[4]['pourlimit'][17]	=	500	    --	5点
--g_sicboDefine.init_data[4]['pourlimit'][18]	=	500	    --	6点
--g_sicboDefine.init_data[4]['pourlimit'][19]	=	500	    --	7点
--g_sicboDefine.init_data[4]['pourlimit'][20]	=	750	    --	8点
--g_sicboDefine.init_data[4]['pourlimit'][21]	=	1000	--	9点
--g_sicboDefine.init_data[4]['pourlimit'][22]	=	1000	--	10点
--g_sicboDefine.init_data[4]['pourlimit'][23]	=	1000	--	11点
--g_sicboDefine.init_data[4]['pourlimit'][24]	=	1000	--	12点
--g_sicboDefine.init_data[4]['pourlimit'][25]	=	750	    --	13点
--g_sicboDefine.init_data[4]['pourlimit'][26]	=	500	    --	14点
--g_sicboDefine.init_data[4]['pourlimit'][27]	=	500	    --	15点
--g_sicboDefine.init_data[4]['pourlimit'][28]	=	500	    --	16点
--g_sicboDefine.init_data[4]['pourlimit'][29]	=	150	    --	17点
--g_sicboDefine.init_data[4]['pourlimit'][30]	=	2500	--	一
--g_sicboDefine.init_data[4]['pourlimit'][31]	=	2500	--	二
--g_sicboDefine.init_data[4]['pourlimit'][32]	=	2500	--	三
--g_sicboDefine.init_data[4]['pourlimit'][33]	=	2500	--	四
--g_sicboDefine.init_data[4]['pourlimit'][34]	=	2500	--	五
--g_sicboDefine.init_data[4]['pourlimit'][35]	=	2500	--	六

--g_sicboDefine.init_data[5]['tableid'] = {2205001}    --对应的tableid
--g_sicboDefine.init_data[5]['maxuser'] = 999
--g_sicboDefine.init_data[5]['tablename'] = "荣耀厅 05"
--g_sicboDefine.init_data[5]['bankerid'] = 1
--g_sicboDefine.init_data[5]['bankername'] = '初夏'
--g_sicboDefine.init_data[5]['bankernamelist'] = {'冷锋','撒男孩','阿三哥','奥斯特','天堂美不美','阿尔法','货拉拉','草莓味果酱'}
--g_sicboDefine.init_data[5]['faceid'] = "90004_1001"
--g_sicboDefine.init_data[5]['banklimit'] = 500000
--g_sicboDefine.init_data[5]['enterlimit'] = 5000
--g_sicboDefine.init_data[5]['entermax'] = -1
--g_sicboDefine.init_data[5]['tabletype'] = 5
--g_sicboDefine.init_data[5]['bankjetton'] = 10000000
--g_sicboDefine.init_data[5]['jiangchibalance'] = 1000 --奖池临界调控值
--g_sicboDefine.init_data[5]['userleave'] = 5 --真人多少次没下注离开
--g_sicboDefine.init_data[5]['limitred'] = 2000000 --限红
----下注区域最大投注限制，最小为1
--g_sicboDefine.init_data[5]['pourlimit'] = {}
--g_sicboDefine.init_data[5]['pourlimit'][1]	=	5000	--	小
--g_sicboDefine.init_data[5]['pourlimit'][2]	=	5000	--	大
--g_sicboDefine.init_data[5]['pourlimit'][3]	=	750	    --	双骰（1）
--g_sicboDefine.init_data[5]['pourlimit'][4]	=	750	    --	双骰（2）
--g_sicboDefine.init_data[5]['pourlimit'][5]	=	750	    --	双骰（3）
--g_sicboDefine.init_data[5]['pourlimit'][6]	=	750	    --	双骰（4）
--g_sicboDefine.init_data[5]['pourlimit'][7]	=	750	    --	双骰（5）
--g_sicboDefine.init_data[5]['pourlimit'][8]	=	750	    --	双骰（6）
--g_sicboDefine.init_data[5]['pourlimit'][9]	=	250	    --	全围
--g_sicboDefine.init_data[5]['pourlimit'][10]	=	100	    --	围骰（1）
--g_sicboDefine.init_data[5]['pourlimit'][11]	=	100	    --	围骰（2）
--g_sicboDefine.init_data[5]['pourlimit'][12]	=	100	    --	围骰（3）
--g_sicboDefine.init_data[5]['pourlimit'][13]	=	100	    --	围骰（4）
--g_sicboDefine.init_data[5]['pourlimit'][14]	=	100	    --	围骰（5）
--g_sicboDefine.init_data[5]['pourlimit'][15]	=	100	    --	围骰（6）
--g_sicboDefine.init_data[5]['pourlimit'][16]	=	150	    --	4点
--g_sicboDefine.init_data[5]['pourlimit'][17]	=	500	    --	5点
--g_sicboDefine.init_data[5]['pourlimit'][18]	=	500	    --	6点
--g_sicboDefine.init_data[5]['pourlimit'][19]	=	500	    --	7点
--g_sicboDefine.init_data[5]['pourlimit'][20]	=	750	    --	8点
--g_sicboDefine.init_data[5]['pourlimit'][21]	=	1000	--	9点
--g_sicboDefine.init_data[5]['pourlimit'][22]	=	1000	--	10点
--g_sicboDefine.init_data[5]['pourlimit'][23]	=	1000	--	11点
--g_sicboDefine.init_data[5]['pourlimit'][24]	=	1000	--	12点
--g_sicboDefine.init_data[5]['pourlimit'][25]	=	750	    --	13点
--g_sicboDefine.init_data[5]['pourlimit'][26]	=	500	    --	14点
--g_sicboDefine.init_data[5]['pourlimit'][27]	=	500	    --	15点
--g_sicboDefine.init_data[5]['pourlimit'][28]	=	500	    --	16点
--g_sicboDefine.init_data[5]['pourlimit'][29]	=	150	    --	17点
--g_sicboDefine.init_data[5]['pourlimit'][30]	=	2500	--	一
--g_sicboDefine.init_data[5]['pourlimit'][31]	=	2500	--	二
--g_sicboDefine.init_data[5]['pourlimit'][32]	=	2500	--	三
--g_sicboDefine.init_data[5]['pourlimit'][33]	=	2500	--	四
--g_sicboDefine.init_data[5]['pourlimit'][34]	=	2500	--	五
--g_sicboDefine.init_data[5]['pourlimit'][35]	=	2500	--	六

--g_sicboDefine.init_data[6]['tableid'] = {2206001}    --对应的tableid
--g_sicboDefine.init_data[6]['maxuser'] = 999
--g_sicboDefine.init_data[6]['tablename'] = "荣耀厅 06"
--g_sicboDefine.init_data[6]['bankerid'] = 1
--g_sicboDefine.init_data[6]['bankername'] = '初夏'
--g_sicboDefine.init_data[6]['bankernamelist'] = {'sass','xs京男孩','士大夫哥','芝加哥不夜城','天堂美不美','洒下','货拉拉','阿瑟斯'}
--g_sicboDefine.init_data[6]['faceid'] = "90004_1001"
--g_sicboDefine.init_data[6]['banklimit'] = 500000
--g_sicboDefine.init_data[6]['enterlimit'] = 5000
--g_sicboDefine.init_data[6]['entermax'] = -1
--g_sicboDefine.init_data[6]['tabletype'] = 6
--g_sicboDefine.init_data[6]['bankjetton'] = 10000000
--g_sicboDefine.init_data[6]['jiangchibalance'] = 1000 --奖池临界调控值
--g_sicboDefine.init_data[6]['userleave'] = 5 --真人多少次没下注离开
--g_sicboDefine.init_data[6]['limitred'] = 2000000 --限红
----下注区域最大投注限制，最小为1
--g_sicboDefine.init_data[6]['pourlimit'] = {}
--g_sicboDefine.init_data[6]['pourlimit'][1]	=	5000	--	小
--g_sicboDefine.init_data[6]['pourlimit'][2]	=	5000	--	大
--g_sicboDefine.init_data[6]['pourlimit'][3]	=	750	    --	双骰（1）
--g_sicboDefine.init_data[6]['pourlimit'][4]	=	750	    --	双骰（2）
--g_sicboDefine.init_data[6]['pourlimit'][5]	=	750	    --	双骰（3）
--g_sicboDefine.init_data[6]['pourlimit'][6]	=	750	    --	双骰（4）
--g_sicboDefine.init_data[6]['pourlimit'][7]	=	750	    --	双骰（5）
--g_sicboDefine.init_data[6]['pourlimit'][8]	=	750	    --	双骰（6）
--g_sicboDefine.init_data[6]['pourlimit'][9]	=	250	    --	全围
--g_sicboDefine.init_data[6]['pourlimit'][10]	=	100	    --	围骰（1）
--g_sicboDefine.init_data[6]['pourlimit'][11]	=	100	    --	围骰（2）
--g_sicboDefine.init_data[6]['pourlimit'][12]	=	100	    --	围骰（3）
--g_sicboDefine.init_data[6]['pourlimit'][13]	=	100	    --	围骰（4）
--g_sicboDefine.init_data[6]['pourlimit'][14]	=	100	    --	围骰（5）
--g_sicboDefine.init_data[6]['pourlimit'][15]	=	100	    --	围骰（6）
--g_sicboDefine.init_data[6]['pourlimit'][16]	=	150	    --	4点
--g_sicboDefine.init_data[6]['pourlimit'][17]	=	500	    --	5点
--g_sicboDefine.init_data[6]['pourlimit'][18]	=	500	    --	6点
--g_sicboDefine.init_data[6]['pourlimit'][19]	=	500	    --	7点
--g_sicboDefine.init_data[6]['pourlimit'][20]	=	750	    --	8点
--g_sicboDefine.init_data[6]['pourlimit'][21]	=	1000	--	9点
--g_sicboDefine.init_data[6]['pourlimit'][22]	=	1000	--	10点
--g_sicboDefine.init_data[6]['pourlimit'][23]	=	1000	--	11点
--g_sicboDefine.init_data[6]['pourlimit'][24]	=	1000	--	12点
--g_sicboDefine.init_data[6]['pourlimit'][25]	=	750	    --	13点
--g_sicboDefine.init_data[6]['pourlimit'][26]	=	500	    --	14点
--g_sicboDefine.init_data[6]['pourlimit'][27]	=	500	    --	15点
--g_sicboDefine.init_data[6]['pourlimit'][28]	=	500	    --	16点
--g_sicboDefine.init_data[6]['pourlimit'][29]	=	150	    --	17点
--g_sicboDefine.init_data[6]['pourlimit'][30]	=	2500	--	一
--g_sicboDefine.init_data[6]['pourlimit'][31]	=	2500	--	二
--g_sicboDefine.init_data[6]['pourlimit'][32]	=	2500	--	三
--g_sicboDefine.init_data[6]['pourlimit'][33]	=	2500	--	四
--g_sicboDefine.init_data[6]['pourlimit'][34]	=	2500	--	五
--g_sicboDefine.init_data[6]['pourlimit'][35]	=	2500	--	六


g_sicboDefine.table_type = {}    --
g_sicboDefine.table_type['junior'] = 1
g_sicboDefine.table_type['medium'] = 2
g_sicboDefine.table_type['serior'] = 3
g_sicboDefine.table_type['high'] = 4
g_sicboDefine.table_type['higher'] = 5
g_sicboDefine.table_type['highest'] = 6


g_sicboDefine.state_ready = 1    --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_sicboDefine.state_start = 2
g_sicboDefine.state_perflop = 3   --摇豆子时间
g_sicboDefine.state_pour = 4      --下注的时间
g_sicboDefine.state_count = 5      --结算时间
g_sicboDefine.state_end = 6      --结束的状态


g_sicboDefine.perflop_time = 5   --开始的时候，摇杯子的时间
g_sicboDefine.pour_time = 15      --下注的时间
g_sicboDefine.count_time = 12

g_sicboDefine.chat_txt = 1
g_sicboDefine.chat_img = 2
g_sicboDefine.chat_voice = 3


g_sicboDefine.type_ydc = 1       --欢乐赢
g_sicboDefine.type_lxc = 2

g_sicboDefine.leave_dismiss = 1   --房主解散房间
g_sicboDefine.leave_timeout = 2   --五轮没下注被踢出

g_sicboDefine.quit_count  = 10     --几轮没下注后就被踢出

g_sicboDefine.time_public = 12  --如果是大众长，就有20秒的掉线时间
g_sicboDefine.time_private = 300  --如果是私人场，则有5分钟的时间



