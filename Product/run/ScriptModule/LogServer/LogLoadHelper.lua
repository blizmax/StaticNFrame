LoadHelper = {}

require("trdlib.Init")  --先加载第三方库
require("conf.serverConf")
require("conf.redisConf")
require("Public.Init")
-------------------------------------------------------------
require("Human.Model.PlayerModel")
require("Human.Model.GoodsModel")
require("Human.Model.OnlineModel")
require("Human.Model.NoticeModel")
require("Human.Model.BroadCastModel")
require("Human.Model.ChatModel")
require("Human.Model.MailModel")
require("Human.Model.AgencyModel")
require("Human.Model.AwardModel")
require("Human.Model.BankModel")
require("Human.Model.RosterModel")
require("Human.Model.GameModel")
require("Human.Model.PlayerStatsModel")
require("Human.Model.HttpSendModel")
require("common.st_shop_pb")
require("common.st_human_pb")
require("common.msg_human_pb")
require("common.define.HumanDefine")
require("Human.Utils.GameUtils")
require("Human.Utils.GameHelp")
-------------------------------------------------------------
require("common.st_user_pb")
-------------------------------------------------------------
require("Agent.Model.AgentModel")
require("Barccatat.Model.BarccatatModel")
require("Bcbm.Model.BcbmModel")
require("Brnn.Model.BrnnModel")
require("Cjmp.Model.CjmpModel")
require("Ddz.Model.DdzModel")
require("DouNiu.Model.DouNiuModel")
require("Ernn.Model.ErnnModel")
require("Ersh.Model.ErshModel")
require("Forest.Model.ForestModel")
require("Fqzs.Model.FqzsModel")
require("Fruit.Model.FruitModel")
require("HongHei.Model.HongHeiModel")
require("CaiPiao.Model.CaiPiaoModel")
require("CaiPiao.Model.CaiPiaoXyftModel")
require("CaiPiao.Model.CaiPiaoLhcModel")
require("CaiPiao.Model.CaiPiaoBjscModel")
require("Shop.Model.ShopModel")
require("FenFenCai.Model.FenFenCaiModel")
require("Lhdb.Model.LhdbModel")
require("Lhd.Model.LhdModel")
require("Lkpy.Model.LkpyModel")
require("Lkpy.Model.LkpyPond")
require("PSZ.Model.PszModel")
require("Sicbo.Model.SicboModel")
require("Texas.Model.TexasModel")
require("Tgpd.Model.TgpdModel")
require("Yqs.Model.YqsModel")
require("Yqs.Model.YqsPond")
require("Slhb.Model.SlhbModel")
require("Sscai.Model.SscaiModel")
require("Ttz.Model.TtzModel")
require("Rank.Model.Flowingwater")
require("DouNiuTz.Model.DouNiuTzModel")
require("Gdmj.Model.GdmjModel")
require("peiwang.Services.peiwangService")

require("common.define.AgentDefine")
require("common.define.YqsDefine")
require("common.define.BarccatatDefine")
require("common.define.BcbmDefine")
require("common.define.BrnnDefine")
require("common.define.CaiPiaoDefine")
require("common.define.CaipiaoBjscDefine")
require("common.define.CaipiaoLhcDefine")
require("common.define.CaipiaoXyftDefine")
require("common.define.CjmpDefine")
require("common.define.DdzDefine")
require("common.define.DouNiuDefine")
require("common.define.ErnnDefine")
require("common.define.ErshDefine")
require("common.define.ForestDefine")
require("common.define.FqzsDefine")
require("common.define.FruitDefine")
require("common.define.GdmjDefine")
require("common.define.HongHeiDefine")
require("common.define.LhdDefine")
require("common.define.PszDefine")
require("common.define.SicboDefine")
require("common.define.TexasDefine")
require("common.define.TgpdDefine")
require("common.define.LhdbDefine")
require("common.define.LkpyDefine")
require("common.define.SlhbDefine")
require("common.define.ShopDefine")
require("common.define.FenFenCaiDefine")
require("common.define.TtzDefine")
require("common.define.DouNiuTzDefine")

require("common.st_agent_pb")
require("common.st_human_pb")
require("common.st_barccatat_pb")
require("common.st_bcbm_pb")
require("common.st_brnn_pb")
require("common.st_caipiao_pb")
require("common.st_cjmp_pb")
require("common.st_ddz_pb")
require("common.st_douniu_pb")
require("common.st_forest_pb")
require("common.st_fqzs_pb")
require("common.st_honghei_pb")
require("common.st_lhd_pb")
require("common.st_psz_pb")
require("common.st_sicbo_pb")
require("common.st_texas_pb")
require("common.st_yqs_pb")
require("common.st_slhb_pb")
require("common.st_utils_pond_pb")
require("common.st_shop_pb")
require("common.msg_shop_pb")
require("common.st_ernn_pb")
require("common.st_douniutz_pb")
require("common.st_fenfencai_pb")
require("common.st_ttz_pb")
require("common.st_gdmj_pb")
require("common.st_gdmj2_pb")
require("common.st_yqs_pb")

g_redisIndex[ShopModel.buy_money] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}
g_redisIndex[ShopModel.redis_index] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}
g_redisIndex[FruitModel.redis_index] = {index = g_redisInfo.redis_three, key = FruitModel.redis_index, link = 1}
g_redisIndex[BarccatatModel.redis_index] = {index = g_redisInfo.redis_four, des = "barccatat_info"}
g_redisIndex[BarccatatModel.redis_index] = {index = g_redisInfo.redis_four, des = "barccatat_info"}
g_redisIndex[BcbmModel.redis_index] = {index = g_redisInfo.redis_three, key = BcbmModel.redis_index, link = 1}
g_redisIndex[BrnnModel.redis_index] = {index = g_redisInfo.redis_four, des = "brnn_info"}
g_redisIndex[BrnnModel.redis_index] = {index = g_redisInfo.redis_four, des = "brnn_info"}
g_redisIndex[CaiPiaoModel.redis_index] = {index = g_redisInfo.redis_four, des = "caipiao_info"}
g_redisIndex[CaiPiaoModel.redis_index] = {index = g_redisInfo.redis_four, des = "caipiao_info"}
g_redisIndex[CjmpModel.redis_index] = {index = g_redisInfo.redis_four, des = "cjmp_info"}
g_redisIndex[g_ddzRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ddz_info"}
g_redisIndex[g_douniuRedis.redis_index] = {index = g_redisInfo.redis_three, des = "douniu_info"}
g_redisIndex[g_douniuTzRedis.redis_index] = {index = g_redisInfo.redis_three, des = "douniuTz_info"}
g_redisIndex[g_ernnRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ernn_info"}
g_redisIndex[g_ershRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ersh_info"}
g_redisIndex[FenFenCaiModel.redis_index] = {index = g_redisInfo.redis_four, des = "fenfencai_info"}
g_redisIndex[ForestModel.redis_index] = {index = g_redisInfo.redis_three, key = ForestModel.redis_index, link = 1}
g_redisIndex[FqzsModel.redis_index] = {index = g_redisInfo.redis_three, key = FqzsModel.redis_index, link = 1}
g_redisIndex[g_gdmjRedis.redis_index] = {index = g_redisInfo.redis_four, des = "gdmj"}  --一定要记得在这里设置
g_redisIndex[HongHeiModel.redis_index] = {index = g_redisInfo.redis_four, des = "honghei_info"}
g_redisIndex[LhdModel.redis_index] = {index = g_redisInfo.redis_four, des = "lhd_info"}
g_redisIndex[LhdbModel.redis_index] = {index = g_redisInfo.redis_three, key = LhdbModel.redis_index, link = 1}
g_redisIndex[LkpyModel.redis_index] = {index = g_redisInfo.redis_three, key = LkpyModel.redis_index, link = 1}
g_redisIndex[g_peiwangRedis.redis_index] = {index = g_redisInfo.redis_three, des = "peiwang", link = 0}
g_redisIndex[g_pszRedis.redis_index] = {index = g_redisInfo.redis_three, key = g_pszRedis.redis_index, link = 1}
g_redisIndex[SicboModel.redis_index] = {index = g_redisInfo.redis_four, des = "sicbo_info"}
g_redisIndex[SlhbModel.redis_index] = {index = g_redisInfo.redis_four, des = "slhb_info"}
g_redisIndex[SscaiModel.redis_index] = {index = g_redisInfo.redis_three, key = SscaiModel.redis_index, link = 1}
g_redisIndex[g_texasRedis.redis_index] = {index = g_redisInfo.redis_four, key = g_texasRedis.redis_index, link = 1}
g_redisIndex[TgpdModel.redis_index] = {index = g_redisInfo.redis_three, key = TgpdModel.redis_index, link = 1}
g_redisIndex[TtzModel.redis_index] = {index = g_redisInfo.redis_four, des = "Ttz_info"}
g_redisIndex[TtzModel.redis_index] = {index = g_redisInfo.redis_four, des = "Ttz_info"}
g_redisIndex[YqsModel.redis_index] = {index = g_redisInfo.redis_three, key = YqsModel.redis_index, link = 1}

require("common.packet.packet_human")
require("Rank.Init")
require("Robot.Init")
require("LogServer.Init")


--鉴于代理系统的复杂性。把代理系统单独出来作为一个模块来进行处理

require("common.st_agent_pb")
require("common.msg_agent_pb")
require("common.msg_agent2_pb")
require("common.define.AgentDefine")
require("common.packet.packet_agent")
require("Agent.Model.AgentModel")
require("Agent.Model.UnlimitedModel")
require("Agent.Model.BonusModule")
require("Agent.Services.UnlimitedAgentService")
require("Agent.Services.AgentService")
require("Agent.Services.UnlimitedAward")
require("Agent.Worker.UnlimitedCount")
require("Agent.Worker.AgentCount")
require("Agent.Worker.UnlimitedIncrUserNum")
require("Agent.Worker.UnlimitedIncrUserActivity")
require("Agent.Worker.UnlimitedDayCheck")
require("Agent.Worker.UnlimitedWeekCheck")
require("Agent.Worker.UnlimitedPersonCheck")
require("Agent.Worker.CommissionCheck")

g_redisIndex[AgentModel.redis_index] = {index = g_redisInfo.redis_two, key = AgentModel.redis_index, link = 1}

