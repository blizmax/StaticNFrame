className = className or {}
dbTableName = dbTableName or {}

NF_DT_TYPE = {
	NF_DT_BOOLEAN = "bool", --bool
	NF_DT_INT32 = "int32", --int64_t
	NF_DT_UINT32 = "uint32", --int64_t
	NF_DT_INT64 = "int64", --int64_t
	NF_DT_UINT64 = "uint64", --int64_t
	NF_DT_DOUBLE = "double", --doubl
	NF_DT_STRING = "string", --string(char*)
	NF_DT_ARRAY = "array", --std::vector<NFIData>
	NF_DT_LIST = "list", --std::list<NFIData>
	NF_DT_MAPSTRING = "mapstring", --std::map<std::string,NFIData>
	NF_DT_MAPINT = "mapint", --std::map<int,NFIData>
}