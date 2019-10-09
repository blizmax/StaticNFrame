
require("trdlib.Init")  --先加载第三方库

require("conf.serverConf")
require("conf.redisConf")
require("Public.Init")

require("common.define.HumanDefine")
require("common.packet.packet_human")

require("common.define.AgentDefine")
require("common.define.BarccatatDefine")
require("common.define.BcbmDefine")
require("common.define.BrnnDefine")
require("common.define.FenFenCaiDefine")
require("common.define.FqzsDefine")
require("common.define.HongHeiDefine")
require("common.define.LhdDefine")
require("common.define.SicboDefine")
require("common.define.SlhbDefine")
require("common.define.TtzDefine")

require("common.packet.packet_agent")
require("common.packet.packet_barccatat")
require("common.packet.packet_bcbm")
require("common.packet.packet_brnn")
require("common.packet.packet_fenfencai")
require("common.packet.packet_fqzs")
require("common.packet.packet_honghei")
require("common.packet.packet_lhd")
require("common.packet.packet_slhb")
require("common.packet.packet_ttz")

require("common.st_human_pb")
require("common.st_shop_pb")
require("common.st_agent_pb")
require("common.st_barccatat_pb")
require("common.st_bcbm_pb")
require("common.st_brnn_pb")
require("common.st_fenfencai_pb")
require("common.st_fqzs_pb")
require("common.st_honghei_pb")
require("common.st_fqzs_pb")
require("common.st_honghei_pb")

require("common.st_lhd_pb")
require("common.st_slhb_pb")
require("common.st_ttz_pb")

require("common.msg_agent_pb")
require("common.msg_agent2_pb")
require("common.msg_barccatat_pb")
require("common.msg_barccatat2_pb")

require("common.msg_bcbm_pb")
require("common.msg_brnn_pb")
require("common.msg_brnn2_pb")

require("common.msg_fenfencai_pb")
require("common.msg_fqzs_pb")

require("common.msg_honghei_pb")
require("common.msg_honghei2_pb")

require("common.msg_human_pb")
require("common.msg_human2_pb")

require("common.msg_lhd_pb")
require("common.msg_lhd2_pb")

require("common.msg_slhb_pb")

require("common.msg_ttz_pb")
require("common.msg_ttz2_pb")

require("RebotExecute")
require("RebotAccount")
require("RebotModule")
require("AccountLogin")
require("BarccatatService")
require("HongheiService")
require("LhdService")
require("BcbmService")
require("BrnnService")
require("FqzsService")
require("SciboService")