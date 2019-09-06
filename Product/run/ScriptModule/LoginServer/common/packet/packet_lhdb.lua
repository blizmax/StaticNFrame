
PacketCode[4501] = {server = 4501, client = 4502, des = "cglhdbenter", func = "LhdbEnter"}
PacketCode[4502] = {server = 4501, client = 4502, des = "gclhdbenter", func = "LhdbEnter"}

PacketCode[4503] = {server = 4503, client = 4504, des = "cglhdbleave", func = "LhdbLeave"}
PacketCode[4504] = {server = 4503, client = 4504, des = "gclhdbleave", func = "LhdbLeave"}

PacketCode[4505] = {server = 4505, client = 4506, des = "cglhdbstart", func = "LhdbStart"}
PacketCode[4506] = {server = 4505, client = 4506, des = "gclhdbstart", func = "LhdbStart"}

ReturnCode["lhdb_not_exist"] = 4501
ReturnCode[4501] = "该牌桌不存在"

ReturnCode["lhdb_jetton_not_enough"] = 4502
ReturnCode[4502] = "筹码不足"