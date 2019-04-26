className = className or {}

NF_DT_TYPE = {
	NF_DT_UNKNOWN = 0, --unknown type
	NF_DT_BOOLEAN = 1, --bool
	NF_DT_INT = 2, --int64_t
	NF_DT_DOUBLE = 3, --doubl
	NF_DT_STRING = 4, --string(char*)
	NF_DT_ARRAY = 5, --std::vector<NFIData>
	NF_DT_LIST = 6, --std::list<NFIData>
	NF_DT_MAPSTRING = 7, --std::map<std::string,NFIData>
	NF_DT_MAPINT = 8, --std::map<int,NFIData>
	NF_DT_MAX = 9, --max

}