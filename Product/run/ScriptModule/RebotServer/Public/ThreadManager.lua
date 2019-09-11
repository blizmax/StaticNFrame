
--该类是用于处理C++传过来多线程的问题，
--包括共享内存锁的机制
--对于线程的锁，已经要记住，lock的时候一定要unlock
ThreadManager = {}
ThreadManager.redis_index = "redis_thread"

function ThreadManager.Lock(index)
	--这里防止进入死循环
	local redisKey = "lock_"..index
	for i = 1, 1000 do
		if false == redisItem:exists(redisKey,ThreadManager.redis_index) then
			break
		end
		sleep(1000) --稍后看看这里是秒还是毫秒
	end
	
	redisItem:setex(redisKey,3,ThreadManager.redis_index) --这里加上一个时间的期限，防止长期占用
end

function ThreadManager.UnLock(index)
	local redisKey = "lock_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.HlydLock(index)
	local redisKey = "lock_hlyd_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HlydUnLock(index)
	local redisKey = "lock_hlyd_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)	
end

function ThreadManager.PaohzLock(index)
	local redisKey = "lock_paohz_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.PaohzUnLock(index)
	local redisKey = "lock_paohz_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)	
end

function ThreadManager.HljjLock(index)
	local redisKey = "lock_hljj_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HljjUnLock(index)
	local redisKey = "lock_hljj_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)	
end

function ThreadManager.GdmjLock(index)
	local redisKey = "lock_gdmj_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.GdmjUnLock(index)
	local redisKey = "lock_gdmj_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.HlnnLock(index)
	local redisKey = "lock_hlnn_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HlnnUnLock(index)
	local redisKey = "lock_hlnn_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.BrnnLock(index)
	local redisKey = "lock_brnn_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.BrnnUnLock(index)
	local redisKey = "lock_brnn_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.HubmjLock(index)
	local redisKey = "lock_hlnn_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HubmjUnLock(index)
	local redisKey = "lock_hlnn_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.HgyxLock(index)
	local redisKey = "lock_hgyx_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HgyxUnLock(index)
	local redisKey = "lock_hgyx_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.SdbLock(index)
	local redisKey = "lock_Sdb_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.SdbUnLock(index)
	local redisKey = "lock_Sdb_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.DouNiuLock(index)
	local redisKey = "lock_DouNiu_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.DouNiuUnLock(index)
	local redisKey = "lock_DouNiu_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.SanGongLock(index)
	local redisKey = "lock_sangong_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.SanGongUnLock(index)
	local redisKey = "lock_sangong_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.PszLock(index)
	local redisKey = "lock_Psz_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.PszUnLock(index)
	local redisKey = "lock_Psz_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.ForestLock(index)
	local redisKey = "lock_forest_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.ForestUnLock(index)
	local redisKey = "lock_forest_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)	
end

function ThreadManager.FruitLock(index)
	local redisKey = "lock_fruit_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.FruitUnLock(index)
	local redisKey = "lock_fruit_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)	
end

function ThreadManager.SscaiLock(index)
	local redisKey = "lock_sscai_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.SscaiUnLock(index)
	local redisKey = "lock_sscai_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)		
end

function ThreadManager.ErshLock(index)
	local redisKey = "lock_ersh_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.ErshUnLock(index)
	local redisKey = "lock_ersh_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)		
end

function ThreadManager.BcbmLock(index)
	local redisKey = "lock_bcbm_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.BcbmUnLock(index)
	local redisKey = "lock_bcbm_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)		
end

function ThreadManager.FqzsLock(index)
	local redisKey = "lock_fqzs_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.FqzsUnLock(index)
	local redisKey = "lock_fqzs_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)		
end

function ThreadManager.RobotLock(index)
	local redisKey = "lock_robot_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,10, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.RobotUnLock(index)
	local redisKey = "lock_robot_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)		
end



function ThreadManager.BarccatatLock(index)
	local redisKey = "lock_barccatat_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.BarccatatUnLock(index)
	local redisKey = "lock_barccatat_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.LhdbLock(index)
	local redisKey = "lock_lhdb_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.LhdbUnLock(index)
	local redisKey = "lock_lhdb_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.TgpdLock(index)
	local redisKey = "lock_tgpd_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.TgpdUnLock(index)
	local redisKey = "lock_tgpd_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.LkpyLock(index)
	local redisKey = "lock_lkpy_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.LkpyUnLock(index)
	local redisKey = "lock_lkpy_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.YqsLock(index)
	local redisKey = "lock_yqs_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey, 30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.YqsUnLock(index)
	local redisKey = "lock_yqs_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.HongHeiLock(index)
	local redisKey = "lock_honghei_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.HongHeiUnLock(index)
	local redisKey = "lock_honghei_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.LhdLock(index)
	local redisKey = "lock_lhd_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.LhdUnLock(index)
	local redisKey = "lock_lhd_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.DdzLock(index)
	local redisKey = "lock_ddz_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.DdzUnLock(index)
	local redisKey = "lock_ddz_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.CjmpLock(index)
	local redisKey = "lock_cjmp_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.CjmpUnLock(index)
	local redisKey = "lock_cjmp_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.SicboLock(index)
	local redisKey = "lock_sicbo_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.SicboUnLock(index)
	local redisKey = "lock_sicbo_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.PlayerLock(index)
	local redisKey = "lock_pj_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,2, ThreadManager.redis_index) --设置超时是2秒
			break
		end
	end		
end

function ThreadManager.PlayerUnLock(index)
	local redisKey = "lock_pj_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.TexasLock(index)
	local redisKey = "lock_texas_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end
end

function ThreadManager.TexasUnLock(index)
	local redisKey = "lock_texas_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.CaiPiaoXyftLock(index)
	local redisKey = "lock_caipiaoxyft_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.CaiPiaoXyftUnLock(index)
	local redisKey = "lock_caipiaoxyft_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.CaiPiaoLhcLock(index)
	local redisKey = "lock_caipiaolhc_"..index
	while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end		
end

function ThreadManager.CaiPiaoLhcUnLock(index)
	local redisKey = "lock_caipiaolhc_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.CaiPiaoBjscLock(index)
	local redisKey = "lock_caipiaobjsc_"..index
    while true do
		local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
		if ok == true then
			redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
			break
		end
	end	
end

function ThreadManager.CaiPiaoBjscUnLock(index)
	local redisKey = "lock_caipiaobjsc_"..index
    redisItem:del(redisKey, ThreadManager.redis_index)
end

function ThreadManager.SlhbLock(index)
  local redisKey = "lock_slhb_"..index
  while true do
    local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
    if ok == true then
      redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
      break
    end
  end
end

function ThreadManager.SlhbUnLock(index)
  local redisKey = "lock_slhb_"..index
  redisItem:del(redisKey, ThreadManager.redis_index)
end


function ThreadManager.TtzLock(index)
	local redisKey = "lock_ttz_"..index
	while true do
	  local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
	  if ok == true then
		redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
		break
	  end
	end
end
  
function ThreadManager.TtzUnLock(index)
	local redisKey = "lock_ttz_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end
  
function ThreadManager.FenFenCaiLock(index)
	local redisKey = "lock_fenfencai_"..index
	while true do
	  local ok = redisItem:setnx(redisKey, "1", ThreadManager.redis_index)
	  if ok == true then
		redisItem:expire(redisKey,30, ThreadManager.redis_index) --设置超时是3秒
		break
	  end
	end
end
  
function ThreadManager.FenFenCaiUnLock(index)
	local redisKey = "lock_fenfencai_"..index
	redisItem:del(redisKey, ThreadManager.redis_index)
end
  