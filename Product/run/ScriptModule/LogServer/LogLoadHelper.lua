LoadHelper = {}

require("trdlib.Init")  --先加载第三方库
require("conf.serverConf")
require("conf.redisConf")
require("Public.Init")

--------------------------------------------------------------
require("LogServer.Model.LogServer")
require("LogServer.Model.LogBehavior")
require("LogServer.Model.LogConstantly")
require("LogServer.Model.LogModel")
--------------------------------------------------------------
require("Robot.Model.RobotModel")
g_redisIndex[RobotModel.redis_index] = {index = g_redisInfo.redis_three, des = "robot", link = 0}
--------------------------------------------------------------
require("common.st_human_pb")
require("common.st_shop_pb")
require("common.msg_shop_pb")
require("common.msg_human_pb")
require("common.msg_human2_pb")
require("common.msg_user_pb")
require("common.st_user_pb")
require("common.define.HumanDefine")
require("common.packet.packet_human")

require("common.data.db_goods")
require("common.data.db_loginaward")
require("common.data.db_signaward")
require("Human.Utils.GameUtils")

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
require("Human.Model.LocationModel")
require("Human.Model.HttpSendModel")
require("Human.Model.ActivityModel")
require("Human.Model.SpecialModel")
g_redisIndex[NoticeModel.redis_index] = {index = g_redisInfo.redis_one, des="notice"}     --系统唯一的邮件
g_redisIndex[OnlineModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[GoodsModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[BroadCastModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[ChatModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[MailModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[PlayerModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[AgencyModel.redis_index] = {index = g_redisInfo.redis_one, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[RosterModel.redis_index] = {index = g_redisInfo.redis_three, key = MailModel.mail_list, link = 1}  --每个玩家自己的邮件
g_redisIndex[LocationModel.redis_index] = {index = g_redisInfo.redis_one, key = LocationModel.redis_index, link = 1}  --
g_redisIndex[HttpSendModel.redis_index] = {index = g_redisInfo.redis_one, key = HttpSendModel.redis_index, link = 1}  --
g_redisIndex[ActivityModel.redis_index] = {index = g_redisInfo.redis_three, key = HttpSendModel.redis_index, link = 1}  --
--------------------------------------------------------------
require("common.msg_rank_pb")
require("common.define.RankDefine")
require("common.packet.packet_rank")

require("Rank.Model.RankModel")
require("Rank.Model.PopularModel")
require("Rank.Model.JettonModel")
require("Rank.Model.Flowingwater")
g_redisIndex[PopularModel.popular_all] = {index = g_redisInfo.redis_three, key = PopularModel.popular_all, link = 0}
g_redisIndex[JettonModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}
g_redisIndex[PopularModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}
g_redisIndex[Flowingwater.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}
g_redisIndex[RankModel.redis_index] = {index = g_redisInfo.redis_three, des = "RankModel", link = 0}
--------------------------------------------------------------

require("common.st_mission_pb")
require("common.msg_mission_pb")
require("common.define.MissionDefine")
require("common.packet.packet_mission")

require("Mission.Model.MissionModel")
g_redisIndex[MissionModel.redis_index] = {index = g_redisInfo.redis_two, des="MissionModel"}  --每个玩家自己的邮件
--------------------------------------------------------------
require("common.define.ShopDefine")
require("common.packet.packet_shop")

require("common.data.db_pay")
require("common.data.db_shop")
require("common.data.db_paybygame")
require("common.data.db_shopbygame")
require("common.data.db_convert")

require("Shop.Model.ShopModel")
g_redisIndex[ShopModel.buy_money] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}
g_redisIndex[ShopModel.redis_index] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}
--------------------------------------------------------------
require("common.st_agent_pb")
require("common.msg_agent_pb")
require("common.msg_agent2_pb")
require("common.define.AgentDefine")
require("common.packet.packet_agent")

require("Agent.Model.AgentModel")
require("Agent.Model.UnlimitedModel")
require("Agent.Model.BonusModule")
g_redisIndex[AgentModel.redis_index] = {index = g_redisInfo.redis_two, key = AgentModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_fruit_pb")
require("common.msg_fruit_pb")
require("common.define.FruitDefine")
require("common.packet.packet_fruit")

require("Fruit.Model.FruitModel")
g_redisIndex[FruitModel.redis_index] = {index = g_redisInfo.redis_three, key = FruitModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_barccatat_pb")
require("common.msg_barccatat_pb")
require("common.msg_barccatat2_pb")
require("common.define.BarccatatDefine")
require("common.packet.packet_barccatat")

require("Barccatat.Model.BarccatatModel")
require("Barccatat.Model.BarccatatStatsModel")
g_redisIndex[BarccatatModel.redis_index] = {index = g_redisInfo.redis_four, des = "barccatat_info"}
g_redisIndex[BarccatatModel.redis_index] = {index = g_redisInfo.redis_four, des = "barccatat_info"}
--------------------------------------------------------------
require("common.st_bcbm_pb")
require("common.msg_bcbm_pb")
require("common.define.BcbmDefine")
require("common.packet.packet_bcbm")

require("Bcbm.Model.BcbmModel")
g_redisIndex[BcbmModel.redis_index] = {index = g_redisInfo.redis_three, key = BcbmModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_brnn_pb")
require("common.st_utils_pond_pb")
require("common.msg_brnn_pb")
require("common.define.BrnnDefine")
require("common.packet.packet_brnn")


require("Brnn.Model.BrnnModel")
require("Brnn.Model.BrnnStatsModel")
g_redisIndex[BrnnModel.redis_index] = {index = g_redisInfo.redis_four, des = "brnn_info"}
g_redisIndex[BrnnModel.redis_index] = {index = g_redisInfo.redis_four, des = "brnn_info"}
--------------------------------------------------------------
require("common.st_caipiao_pb")
require("common.msg_caipiao_pb")
require("common.msg_caipiao2_pb")
require("common.define.CaipiaoDefine")
require("common.define.CaipiaoXyftDefine")
require("common.define.CaipiaoLhcDefine")
require("common.define.CaipiaoBjscDefine")
require("common.packet.packet_caipiao")

require("CaiPiao.Model.CaiPiaoModel")
require("CaiPiao.Model.CaiPiaoXyftModel")
require("CaiPiao.Model.CaiPiaoLhcModel")
require("CaiPiao.Model.CaiPiaoBjscModel")
g_redisIndex[CaiPiaoModel.redis_index] = {index = g_redisInfo.redis_four, des = "caipiao_info"}
g_redisIndex[CaiPiaoModel.redis_index] = {index = g_redisInfo.redis_four, des = "caipiao_info"}
--------------------------------------------------------------
require("common.st_cjmp_pb")
require("common.st_utils_pond_pb")
require("common.msg_cjmp_pb")
require("common.msg_cjmp2_pb")
require("common.define.CjmpDefine")
require("common.packet.packet_cjmp")

require("Cjmp.Model.CjmpModel")
require("Cjmp.Model.CjmpStatsModel")
g_redisIndex[CjmpModel.redis_index] = {index = g_redisInfo.redis_four, des = "cjmp_info"}
--------------------------------------------------------------
require("common.packet.packet_ddz")
require("common.st_ddz_pb")
require("common.msg_ddz_pb")
require("common.msg_ddz2_pb")
require("common.define.DdzDefine")

require("Ddz.Model.DdzHistoryModel")
require("Ddz.Model.DdzModel")
g_redisIndex[g_ddzRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ddz_info"}
--------------------------------------------------------------
require("common.packet.packet_douniu")
require("common.st_douniu_pb")
require("common.msg_douniu_pb")
require("common.msg_douniu_2_pb")
require("common.define.DouNiuDefine")

require("DouNiu.Model.DouNiuModel")
require("DouNiu.Model.DouNiuHistoryModel")

g_redisIndex[g_douniuRedis.redis_index] = {index = g_redisInfo.redis_three, des = "douniu_info"}
--------------------------------------------------------------
require("common.packet.packet_douniuTz")
require("common.st_douniutz_pb")
require("common.msg_douniutz_pb")
require("common.msg_douniutz_2_pb")
require("common.define.DouNiuTzDefine")

require("DouNiuTz.Model.DouNiuTzModel")
require("DouNiuTz.Model.DouNiuTzHistoryModel")
g_redisIndex[g_douniuTzRedis.redis_index] = {index = g_redisInfo.redis_three, des = "douniuTz_info"}
--------------------------------------------------------------
require("common.packet.packet_ernn")
require("common.st_ernn_pb")
require("common.msg_ernn_pb")
require("common.msg_ernn_2_pb")
require("common.define.ErnnDefine")

require("Ernn.Model.ErnnModel")
require("Ernn.Model.ErnnHistoryModel")
g_redisIndex[g_ernnRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ernn_info"}
--------------------------------------------------------------
require("common.define.ErshDefine")
require("common.packet.packet_ersh")
require("common.st_ersh_pb")
require("common.msg_ersh_pb")

require("Ersh.Model.ErshModel")
g_redisIndex[g_ershRedis.redis_index] = {index = g_redisInfo.redis_three, des = "ersh_info"}
--------------------------------------------------------------
require("common.st_fenfencai_pb")
require("common.msg_fenfencai_pb")
require("common.define.FenFenCaiDefine")
require("common.packet.packet_fenfencai")

require("FenFenCai.Model.FenFenCaiModel") 
g_redisIndex[FenFenCaiModel.redis_index] = {index = g_redisInfo.redis_four, des = "fenfencai_info"}
--------------------------------------------------------------
require("common.st_forest_pb")
require("common.msg_forest_pb")
require("common.define.ForestDefine")
require("common.packet.packet_forest")

require("Forest.Model.ForestModel")
g_redisIndex[ForestModel.redis_index] = {index = g_redisInfo.redis_three, key = ForestModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_fqzs_pb")
require("common.msg_fqzs_pb")
require("common.define.FqzsDefine")
require("common.packet.packet_fqzs")

require("Fqzs.Model.FqzsModel")
g_redisIndex[FqzsModel.redis_index] = {index = g_redisInfo.redis_three, key = FqzsModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.packet.packet_gdmj")
require("common.st_gdmj_pb")
require("common.st_gdmj2_pb")
require("common.msg_gdmj_pb")
require("common.msg_gdmj2_pb")
require("common.define.GdmjDefine")
require("common.utils.MajiangHuUtils")
require("common.utils.MajiangUtils")

require("Gdmj.Model.GdmjModel")
require("Gdmj.Model.GdmjHistoryModel")
g_redisIndex[g_gdmjRedis.redis_index] = {index = g_redisInfo.redis_four, des = "gdmj"}  --一定要记得在这里设置
g_redisIndex[GdmjHistoryModel.redis_index] = {index = g_redisInfo.redis_four, des = "gdmj"}  --一定要记得在这里设置
--------------------------------------------------------------
require("common.st_honghei_pb")
require("common.st_utils_pond_pb")
require("common.msg_honghei_pb")
require("common.msg_honghei2_pb")
require("common.define.HongHeiDefine")
require("common.packet.packet_honghei")

require("HongHei.Model.HongHeiModel")
require("HongHei.Model.HongHeiStatsModel")
g_redisIndex[HongHeiModel.redis_index] = {index = g_redisInfo.redis_four, des = "honghei_info"}
--------------------------------------------------------------
require("common.msg_legal_pb")
require("common.packet.packet_legal")

require("Legal.Model.LegalModel")
g_redisIndex[LegalModel.redis_index] = {index = g_redisInfo.redis_three, des = "legal_info"}
--------------------------------------------------------------
require("common.st_lhd_pb")
require("common.st_utils_pond_pb")
require("common.msg_lhd_pb")
require("common.msg_lhd2_pb")
require("common.define.LhdDefine")

require("common.packet.packet_lhd")

require("common.st_lhdb_pb")
require("common.msg_lhdb_pb")
require("common.define.LhdbDefine")
require("common.packet.packet_lhdb")

require("Lhd.Model.LhdModel")
require("Lhd.Model.LhdStatsModel")
g_redisIndex[LhdModel.redis_index] = {index = g_redisInfo.redis_four, des = "lhd_info"}
--------------------------------------------------------------
require("Lhdb.Model.LhdbModel")
g_redisIndex[LhdbModel.redis_index] = {index = g_redisInfo.redis_three, key = LhdbModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_lkpy_pb")
require("common.msg_lkpy_pb")
require("common.define.LkpyDefine")
require("common.packet.packet_lkpy")

require("Lkpy.Model.LkpyModel")
require("Lkpy.Model.LkpyPond")
g_redisIndex[LkpyModel.redis_index] = {index = g_redisInfo.redis_three, key = LkpyModel.redis_index, link = 1}
--------------------------------------------------------------
require("peiwang.Services.peiwangService")
g_redisIndex[g_peiwangRedis.redis_index] = {index = g_redisInfo.redis_three, des = "peiwang", link = 0}
--------------------------------------------------------------
require("common.define.PszDefine")
require("common.packet.packet_psz")
require("common.st_psz_pb")
require("common.msg_psz_pb")
require("common.msg_psz_2_pb")

require("PSZ.Model.PszModel")
require("PSZ.Model.PszJinBiChang")
g_redisIndex[g_pszRedis.redis_index] = {index = g_redisInfo.redis_three, key = g_pszRedis.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_sicbo_pb")
require("common.st_utils_pond_pb")
require("common.msg_sicbo_pb")
require("common.msg_sicbo2_pb")
require("common.define.SicboDefine")

require("common.packet.packet_sicbo")

require("Sicbo.Model.SicboModel")
require("Sicbo.Model.SicboStatsModel")

g_redisIndex[SicboModel.redis_index] = {index = g_redisInfo.redis_four, des = "sicbo_info"}
--------------------------------------------------------------
require("common.st_slhb_pb")
require("common.msg_slhb_pb")
require("common.define.SlhbDefine")
require("common.packet.packet_slhb")

require("Slhb.Model.SlhbModel") 

g_redisIndex[SlhbModel.redis_index] = {index = g_redisInfo.redis_four, des = "slhb_info"}
--------------------------------------------------------------
require("common.data.db_slot")
require("common.st_slot_pb")
require("common.msg_slot_pb")

require("common.define.SlotDefine")
require("common.packet.packet_slot")
require("Slot.Model.SlotModel")

g_redisIndex[SlotModel.redis_index] = {index = g_redisInfo.redis_three, key = SlotModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_sscai_pb")
require("common.msg_sscai_pb")
require("common.define.SscaiDefine")
require("common.packet.packet_sscai")

require("Sscai.Model.SscaiModel")
g_redisIndex[SscaiModel.redis_index] = {index = g_redisInfo.redis_three, key = SscaiModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.define.TexasDefine")
require("common.packet.packet_texas")
require("common.st_texas_pb")
require("common.msg_texas_pb")

require("Texas.Model.TexasModel")
g_redisIndex[g_texasRedis.redis_index] = {index = g_redisInfo.redis_four, key = g_texasRedis.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_tgpd_pb")
require("common.msg_tgpd_pb")
require("common.define.TgpdDefine")
require("common.packet.packet_tgpd")

require("Tgpd.Model.TgpdModel")
g_redisIndex[TgpdModel.redis_index] = {index = g_redisInfo.redis_three, key = TgpdModel.redis_index, link = 1}
--------------------------------------------------------------
require("common.st_ttz_pb")
require("common.st_utils_pond_pb")
require("common.msg_ttz_pb")
require("common.msg_ttz2_pb")
require("common.define.TtzDefine")

require("common.packet.packet_ttz")

require("Ttz.Model.TtzModel")
require("Ttz.Model.TtzStatsModel")
g_redisIndex[TtzModel.redis_index] = {index = g_redisInfo.redis_four, des = "Ttz_info"}
g_redisIndex[TtzModel.redis_index] = {index = g_redisInfo.redis_four, des = "Ttz_info"}
--------------------------------------------------------------
require("common.st_yqs_pb")
require("common.msg_yqs_pb")
require("common.define.YqsDefine")
require("common.packet.packet_yqs")

require("Yqs.Model.YqsModel")
require("Yqs.Model.YqsPond")
g_redisIndex[YqsModel.redis_index] = {index = g_redisInfo.redis_three, key = YqsModel.redis_index, link = 1}
--------------------------------------------------------------
require("Http.HttpDebug")
require("Http.HttpGame")
require("Http.HttpPlay")
require("Http.HttpService")
require("Http.HttpLogin")
require("Http.HttpActivity")
require("Http.HttpUtils")
--------------------------------------------------------------