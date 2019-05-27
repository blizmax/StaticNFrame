


g_hlnnPlayer = {}  --红方ID
g_hlnnPlayer.red = 1
g_hlnnPlayer.blue = 2
g_hlnnPourType = {11,12,13,21,22,23,24,25,31,32,33,34,35}
g_hlnnState = {}
g_hlnnState.state_start = 1     --等待,已经发好牌了，进入发牌的等待时间
g_hlnnState.state_perflop = 2  --发牌，4秒时间
g_hlnnState.state_pour = 3     --下注，16秒时间
g_hlnnState.state_count = 4    --结算，9秒时间