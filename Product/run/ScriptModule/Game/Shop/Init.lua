require("Shop.Controller.BuyGoods")

require("common.define.ShopDefine")
require("common.packet.packet_shop")

require("common.data.db_pay")
require("common.data.db_shop")

if g_servername == "run_test" then

elseif g_servername == "run_gdmj" or g_servername == "run_ylmj" then
	require("common.data.db_paygdmj")
	require("common.data.db_shopgdmj")
	--require("Http.HttpHlyd")
elseif g_servername == "run_zsmj" or g_servername == "run_xwpk" then
	require("common.data.db_payzsmj")
	require("common.data.db_shopzsmj")
	require("common.data.db_payhlyd")
	require("common.data.db_shophlyd")	
	require("Http.HttpHlyd")
elseif g_servername == "run_hlyd" then
	require("common.data.db_payhlyd")
	require("common.data.db_shophlyd")
elseif g_servername == "run_tyyd" then
	require("common.data.db_paytyyd")
	require("common.data.db_shoptyyd")
elseif g_servername == "run_hubmj" then
	require("common.data.db_payxymj")
	require("common.data.db_shopxymj")
end	


require("Shop.Model.ShopModel")
require("Shop.Services.ShopService")



g_redisIndex[ShopModel.buy_money] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}
g_redisIndex[ShopModel.redis_index] = {index = g_redisInfo.redis_two, key = ShopModel.buy_money, link = 1}

