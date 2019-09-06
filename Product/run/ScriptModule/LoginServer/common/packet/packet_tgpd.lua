
PacketCode[4601] = {server = 4601, client = 4602, des = "cgtgpdenter", func = "TgpdEnter"}
PacketCode[4602] = {server = 4601, client = 4602, des = "gctgpdenter", func = "TgpdEnter"}

PacketCode[4603] = {server = 4603, client = 4604, des = "cgtgpdleave", func = "TgpdLeave"}
PacketCode[4604] = {server = 4603, client = 4604, des = "gctgpdleave", func = "TgpdLeave"}

PacketCode[4605] = {server = 4605, client = 4606, des = "cgtgpdstart", func = "TgpdStart"}
PacketCode[4606] = {server = 4605, client = 4606, des = "gctgpdstart", func = "TgpdStart"}

ReturnCode["tgpd_not_exist"] = 4601
ReturnCode[4601] = "该牌桌不存在"

ReturnCode["tgpd_jetton_not_enough"] = 4602
ReturnCode[4602] = "筹码不足"