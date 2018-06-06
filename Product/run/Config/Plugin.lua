-- server type
NF_ST_NONE = 0;    		-- NONE
NF_ST_MASTER = 1;    	--
NF_ST_LOGIN = 2;    	--
NF_ST_WORLD = 3;    	--
NF_ST_GAME = 4;    		--
NF_ST_PROXY = 5;    	--
NF_ST_MATCH = 6;
NF_ST_BATTLE = 7;
NF_ST_UNION_MATCH = 8;
NF_ST_PUBLIC_MATCH = 9;
NF_ST_MAX = 10;    		--

LoadPlugin = 
{
	AllServer = {
		ServerPlugins = {
			"NFKernelPlugin",
			"NFActorPlugin",
			"NFNetPlugin",
			"NFTestPlugin",
			"NFMysqlPlugin"
		};
		ServerType = NF_ST_NONE;
	};
};


