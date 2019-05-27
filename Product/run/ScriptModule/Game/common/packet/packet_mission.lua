
--1000~1100 是人物信息的ID区间
PacketCode[1201] = {server = 1201, client = 1202, des = "cgmissionshare", func = "MissionShare"}
PacketCode[1202] = {server = 1201, client = 1202, des = "gcmissionshare", func = "MissionShare"}



ReturnCode["hlyd_share_fail"] = 1201

ReturnCode[1201] = "分享失败"

ReturnCode["jlb_name_nil"] = 3201   --名称不能为空