require 'class'

className["loginfo"] = {
	{nodeName="loglevel", nodeType=NF_DT_TYPE.NF_DT_UINT32, desc="日志等级"},
	{nodeName="logflushlevel", nodeType=NF_DT_TYPE.NF_DT_UINT32, desc="日志刷新"},
	loginfo_table = {
		{nodeName="logid", nodeType=NF_DT_TYPE.NF_DT_UINT32, desc="日志ID"},
		{nodeName="display", nodeType=NF_DT_TYPE.NF_DT_BOOLEAN, desc="是否显示日志"},
		{nodeName="level", nodeType=NF_DT_TYPE.NF_DT_UINT32, desc="日志等级"},  
		{nodeName="logname", nodeType=NF_DT_TYPE.NF_DT_STRING, desc="日志名字"},
		{nodeName="guid", nodeType=NF_DT_TYPE.NF_DT_ARRAY, desc="guid"},
	} 
}