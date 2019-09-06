
--1000~1100 是人物信息的ID区间
PacketCode[1201] = {server = 1201, client = 1202, des = "cgmissionshare", func = "MissionShare"}
PacketCode[1202] = {server = 1201, client = 1202, des = "gcmissionshare", func = "MissionShare"}

PacketCode[1203] = {server = 1203, client = 1204, des = "cggetmissionlist", func = "GetMissionList"}
PacketCode[1204] = {server = 1203, client = 1204, des = "gcgetmissionlist", func = "GetMissionList"}

PacketCode[1205] = {server = 1205, client = 1206, des = "cggetmissionaward", func = "GetMissionAward"}
PacketCode[1206] = {server = 1205, client = 1206, des = "gcgetmissionaward", func = "GetMissionAward"}


ReturnCode["hlyd_share_fail"] = 1201

ReturnCode[1201] = "分享失败"

