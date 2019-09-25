
--鉴于代理系统的复杂性。把代理系统单独出来作为一个模块来进行处理


require("Activity.Model.ActivityModel")
require("Activity.Services.ActivityServices")

require("Activity.Worker.ActivityPlayernum")

g_redisIndex[ActivityModel.redis_index] = {index = g_redisInfo.redis_three, key = ActivityModel.redis_index, link = 1}

