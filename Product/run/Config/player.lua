require 'class'

dbTableName["dy_player"] = {
	{colName="userid", colType="int",  colTypeNum = NF_DT_TYPE.NF_DT_INT, colTypeLength=0, primaryKey=true, index=true, autoincrement=true, },
}

className["player"] = {
	{nodeName="userid", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="玩家ID"},
	---------------------------------------------------------------玩家基本信息------------------------------------------------------------------------------------
	{nodeName="cid", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="用作判断手机标识的唯一字符串"},
	{nodeName="account", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="账号"},
	{nodeName="password", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="密码"},
	{nodeName="nickname", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="玩家昵称"},
	{nodeName="regdate", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="注册日期"},
	
	{nodeName="face_1", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="头像1"},
	{nodeName="face_2", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="头像2"},
	{nodeName="face_3", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="头像3"},
	{nodeName="face_4", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="头像4"},
	
	{nodeName="description", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="个性签名"},
	
	-------------------------------------------------------玩家详情--------------------------------------------------------------------------------------------
	{nodeName="email", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="邮件"},
	{nodeName="phonenum", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="电话号码"},
	{nodeName="channel", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="平台标识"},
	{nodeName="province", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="省份"},
	{nodeName="city", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="城市"},
	
	{nodeName="imei", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="imei"},
	{nodeName="devname", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="设备号"},
	{nodeName="macname", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="macname"},
	
	-----------------------------------------------------------玩家详情----------------------------------------------------------------------------------------
	{nodeName="bindnick", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="实名认证消息"},
	{nodeName="platformid", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="平台返回的标识ID"},
	
	
	-----------------------------------------------------------玩家基本信息----------------------------------------------------------------------------------------
	{nodeName="sex", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="性别"},
	{nodeName="age", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="年龄"},
	{nodeName="isrobot", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="是否是机器人"},
	
	{nodeName="exp", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="经验"},
	{nodeName="level", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="等级"},
	{nodeName="viplevel", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="vip等级"},
	{nodeName="jetton", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="筹码必须用64位的"},
	{nodeName="bank_jetton", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="暂存在银行的筹码"},
	{nodeName="gold", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="金币，暂时先不用这个"},
	{nodeName="money", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="钻石"},
	{nodeName="popularity", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="魅力值"},
	{nodeName="lasttime", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="上次登录时间"},
	
	-------------------------------------------------------玩家运营信息--------------------------------------------------------------------------------------------

	{nodeName="idenstatus", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="头像认证状态"},
	{nodeName="bindtype", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="绑定类型"},
	
	{nodeName="mobiletype", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="运营商"},

	
	{nodeName="penulttime", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="倒数第二次登录"},
	{nodeName="silent", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="是否禁言"},
	{nodeName="isban", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="是否禁号，列入黑名单"},
	
	---------------------------------------------------------------------------------------------------------------------------------------------------
	
	{nodeName="playtype", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="玩家玩牌的类型：1:一般型，没有特点；2：小白型：全凭喜好。3：保守型，一定要拿到好牌才跟或者加注；4：激进型：一般有好牌就加注或者跟了。5：冒险型：非常喜欢诈唬，有好牌一定allin或者跟"},

	{nodeName="blacklist", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="黑名单"},
	{nodeName="propertyid", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="玩家佩戴的物品ID"},
	{nodeName="agencystatus", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="代理的状态，最高两位数，个尾数是0：表示没有绑定代理，1：已经绑定代理；十位数：0：没申请代理，1：已经申请代理，2：成为了代理"},
	
	---------------------------------------------------------------玩家签到和分享------------------------------------------------------------------------------------
	{nodeName="registration1", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="签到详情 上旬"},
	{nodeName="registration2", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="签到详情	中旬"},
	{nodeName="registration3", nodeType=NF_DT_TYPE.NF_DT_STRING, public=false, private=false, save=true, dbTable = "dy_player", desc="签到详情	下旬"},
	{nodeName="signinmonth", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="签到月份"},
	{nodeName="bsignin", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="今天是否已签到   //未签到:0  已签到:签到时间"},
	{nodeName="turntablenum", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="转盘次数"},
	{nodeName="tengames", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="十局是否完成"},
	{nodeName="todayshare", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="今天是否分享"},
	{nodeName="caninvite", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=true, dbTable = "dy_player", desc="是否能被邀请	"},
	
	---------------------------------------------------------------玩家线上信息------------------------------------------------------------------------------------
	{nodeName="online", nodeType=NF_DT_TYPE.NF_DT_BOOLEAN, public=false, private=false, save=false, desc="是否在线"},
	{nodeName="loginTime", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=false, desc="登陆时间"},
	{nodeName="logoutTime", nodeType=NF_DT_TYPE.NF_DT_INT, public=false, private=false, save=false, desc="下线时间"},
}