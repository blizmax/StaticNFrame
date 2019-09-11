

RateDefine = {}

RateDefine[g_logintable.type_hlyd]         =  0.05
RateDefine[g_logintable.type_hljj]         =  0.05
RateDefine[g_logintable.type_phz]          =  0.05       
RateDefine[g_logintable.type_gdmj]         =  0.05
RateDefine[g_logintable.type_brnn]         =  0.05
RateDefine[g_logintable.type_hubmj]        =  0.05
RateDefine[g_logintable.type_douniu]       =  0.05
RateDefine[g_logintable.type_ernn]         =  0.05
RateDefine[g_logintable.type_barccatat]    =  0.05
RateDefine[g_logintable.type_bcbm]         =  0.05
RateDefine[g_logintable.type_psz]          =  0.05
RateDefine[g_logintable.type_ddz]          =  0.05
RateDefine[g_logintable.type_honghei]      =  0.05
RateDefine[g_logintable.type_lhd]          =  0.05
RateDefine[g_logintable.type_cjmp]         =  0.05




function RateDefine.GetRate(gameType)
	return RateDefine[gameType] == nil and 5 or RateDefine[gameType]
end