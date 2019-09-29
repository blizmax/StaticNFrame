

module("CheckIPLocation", package.seeall)

function work(buffer)
	
	--print("CpapiHttpGet=============")
	--
	
	--360的IP查询库  http://ip.360.cn/IPQuery/ipquery?ip=115.159.152.210
	--local addArr = {}
	--addArr['userid'] = 101962
	--addArr['ip'] = '113.87.13.30'
	--print(luajson.encode(addArr))
	for i=1,100 do
	
		local strGet = redisItem:lpop( LocationModel.location_list, LocationModel.redis_index)
		if strGet == nil then
			break
		end
		
		local dataArr = luajson.decode( strGet )
		
		local url = "http://ip.360.cn/IPQuery/ipquery?ip="..dataArr['ip']   --先用360的去获取，然后在用其他的。
		
		--print(url)
		local getData = HttpGet(url)
		

		local getArr = luajson.decode(getData)

		local pInfo = PlayerModel.GetPlayerInfo( dataArr['userid'] )
		
		if pInfo ~= nil and getArr['errno'] == 0 then
			
			local pos = string.find( getArr['data'], '\t' )

			if pos ~= nil and pos > 0 then
			
				pInfo.province = string.sub(getArr['data'], 1, pos - 1)
				pInfo.ip = dataArr['ip']
				local macname = string.sub( getArr['data'], pos + 1, string.len( getArr['data'] ) )
				
				local sqlCase = "update dy_player set province='"..pInfo.province.."',macname='"..macname.."',ip='"..pInfo.ip.."' where userid="..dataArr['userid']
				LogFile("error", sqlCase)
				mysqlItem:execute(sqlCase)
			
			else
				pInfo.province =getArr['data']
				pInfo.ip = dataArr['ip']
				local sqlCase = "update dy_player set province='"..pInfo.province.."',ip='"..pInfo.ip.."' where userid="..dataArr['userid']
				LogFile("error", sqlCase)
				mysqlItem:execute(sqlCase)
			end
			
			PlayerModel.SetPlayerInfo(pInfo)   --同时设置对应的playerinfo
		end
		
		--
	end	
	
end