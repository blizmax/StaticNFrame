
--定位服务器

LocationService = {}


function LocationService.Init()
	--定位服务器的初始化。
	
end


function LocationService.ServerLoop()
	if false == ThreadManager.IsLock(LocationModel.location_lock) and LocationModel.GetListLen() > 0 then
		processWork( "CheckIPLocation", "location" )
	end	
end



