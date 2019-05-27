
g_hljjDefine = {}


g_hljjDefine.pour_type = {1,2,3}


g_hljjDefine.table_common = 177777
g_hljjDefine.table_grade = 178888

g_hljjDefine.init_sit = 7      --初始化的8个座位
g_hljjDefine.page_item = 8     --每一页显示的牌桌的数量
g_hljjDefine.min_bankjetton = 10000000     --最小上庄的倍数
g_hljjDefine.min_pri_bankjetton = 5000000    --私人场最小上庄的倍数


g_hljjDefine.min_pour_common = 100
g_hljjDefine.min_pour_grade = 10000
g_hljjDefine.pour2_rate = 10
g_hljjDefine.pour3_rate = 100

if g_servername == "run_tyyd" then
	g_hljjDefine.min_bankjetton = 1000    --最小上庄的倍数
	g_hljjDefine.min_pri_bankjetton = 500    --私人场中最小上庄的倍数
	
	g_hljjDefine.pour2_rate = 10
	g_hljjDefine.pour3_rate = 50
	
	g_hljjDefine.min_pour_common = 100
	g_hljjDefine.min_pour_grade = 1000
	g_initData.jetton = 100
end




g_hljjDefine.state_ready = 1     --在这个状态，检查是否有庄家，如果没有庄家或者玩家，如果没有，则等待玩家进来，或者等待庄家上庄
g_hljjDefine.state_start = 2
g_hljjDefine.state_perflop = 3   --摇豆子时间
g_hljjDefine.state_pour = 4      --下注的时间
g_hljjDefine.state_count = 5     --结算时间


g_hljjDefine.perflop_time = 3    --开始的时候，摇杯子的时间
g_hljjDefine.pour_time = 15      --下注的时间
g_hljjDefine.count_time = 7

g_hljjDefine.chat_txt = 1
g_hljjDefine.chat_img = 2
g_hljjDefine.chat_voice = 3

g_hljjDefine.common_odds = 290


g_hljjDefine.leave_dismiss = 1   --房主解散房间
g_hljjDefine.leave_timeout = 2   --五轮没下注被踢出


g_hljjDefine.quit_count = 10     --经过几轮没下注后就被踢出

g_hljjDefine.time_public = 20
g_hljjDefine.time_private = 300  --私人房中，庄家离线后三百秒会自动解散