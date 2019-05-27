
ShopService = {}

function ShopService.Init(cgmsg, gcmsg)
	
	local pInfo = PlayerModel.GetPlayerInfo(cgmsg.userid)
	if pInfo == nil then
		return nil
	end	

	gcmsg.paytype:append(g_payType.pay_trdpay)

		gcmsg.paylist:append(1501)
		gcmsg.paylist:append(1502)
		gcmsg.paylist:append(1503)
		gcmsg.paylist:append(1504)
		gcmsg.paylist:append(1505)
		gcmsg.paylist:append(1506)
	
end

function ShopService.GetPayItem(payID)
	
	if g_servername == "run_test" then
		
		
	elseif g_servername == "run_gdmj"  or g_servername == "run_ylmj" then
		return db_paygdmj[payID]
	
	elseif g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		return db_payhlyd[payID]   --hyld和zsmj是同一个服务器
	
	elseif g_servername == "run_hlyd" then
		return db_payhlyd[payID]
	elseif g_servername == "run_tyyd" then
		return db_paytyyd[payID]
	elseif g_servername == "run_hubmj" then
		return db_payxymj[payID]
	end
	
end

function ShopService.GetShopItem(shopID)
	
	if g_servername == "run_test" then

	elseif g_servername == "run_gdmj" or g_servername == "run_ylmj" then
		
		return db_shopgdmj[shopID]
	elseif g_servername == "run_zsmj" or g_servername == "run_xwpk" then
		return db_shophlyd[shopID]    --hlyd和zsmj是同一个服务器
	elseif g_servername == "run_hlyd" then
		return db_shophlyd[shopID]
	elseif g_servername == "run_tyyd" then
		return db_shoptyyd[shopID]
	elseif g_servername == "run_hubmj" then
		return db_shopxymj[shopID]
	end	
	
end
