g_caipiaoDefine = {}
g_caipiaoDefine.game_type = 6000
--牌桌状态
g_caipiaoDefine.state_maintenance = 1   --维护中状态
g_caipiaoDefine.state_pour =        2   --下注状态
g_caipiaoDefine.state_fengpan =     3   --封盘状态

g_caipiaoDefine.init_data = {}                 --初始化的数据，一开始初始化三个类型的牌桌，先初始化一个
g_caipiaoDefine.init_data[1] = {}              --幸运飞艇
g_caipiaoDefine.init_data[2] = {}              --香港六合彩
g_caipiaoDefine.init_data[3] = {}              --北京赛车
--g_caipiaoDefine.init_data[4] = {}
--g_caipiaoDefine.init_data[5] = {}
--g_caipiaoDefine.init_data[6] = {}

--幸运飞艇
g_caipiaoDefine.init_data[1]['tableid'] = {2241001}             -- 对应的tableid
g_caipiaoDefine.init_data[1]['maxuser'] = 999                   -- 最大人数
g_caipiaoDefine.init_data[1]['tabletype'] = 1                   -- 桌子类型
g_caipiaoDefine.init_data[1]['tablename'] = "幸运飞艇"          -- 桌子名称
g_caipiaoDefine.init_data[1]['fengpantime'] =  5 * 60           -- 封盘运行时间(5分钟开奖 对应api开奖时间)
g_caipiaoDefine.init_data[1]['openawardtime'] =  5 * 60 + 15    -- 开奖时间(s)(这边延迟15s开奖)
g_caipiaoDefine.init_data[1]['gamekey'] =  1                    --彩种

--香港六合彩
g_caipiaoDefine.init_data[2]['tableid'] = {2242001}             -- 对应的tableid
g_caipiaoDefine.init_data[2]['maxuser'] = 999                   -- 最大人数
g_caipiaoDefine.init_data[2]['tabletype'] = 1                   -- 桌子类型
g_caipiaoDefine.init_data[2]['tablename'] = "香港六合彩"        -- 桌子名称
g_caipiaoDefine.init_data[2]['fengpantime'] =  2 * 60 *60          -- 封盘运行时间(2天开奖 对应api开奖时间)此处特殊处理
g_caipiaoDefine.init_data[2]['openawardtime'] =  30 * 60        -- 开奖时间(s)(这边延迟15s开奖)
g_caipiaoDefine.init_data[2]['gamekey'] =  2                    --彩种

--北京赛车
g_caipiaoDefine.init_data[3]['tableid'] = {2243001}             -- 对应的tableid
g_caipiaoDefine.init_data[3]['maxuser'] = 999                   -- 最大人数
g_caipiaoDefine.init_data[3]['tabletype'] = 1                   -- 桌子类型
g_caipiaoDefine.init_data[3]['tablename'] = "北京赛车"        -- 桌子名称
g_caipiaoDefine.init_data[3]['fengpantime'] =  60          -- 封盘运行时间
g_caipiaoDefine.init_data[3]['openawardtime'] = 60 + 15     -- 开奖时间(s)(这边延迟15s开奖)
g_caipiaoDefine.init_data[3]['gamekey'] =  3                --彩种

