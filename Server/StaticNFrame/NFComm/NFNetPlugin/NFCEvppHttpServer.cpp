// -------------------------------------------------------------------------
//    @FileName         :    NFEvppHttpServer.cpp
//    @Author           :    GaoYi
//    @Date             :    2019-6-9
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
//                    _ooOoo_
//                   o8888888o
//                   88" . "88
//                   (| -_- |)
//                    O\ = /O
//                ____/`---'\____
//              .   ' \\| |// `.
//               / \\||| : |||// \
//             / _||||| -:- |||||- \
//               | | \\\ - /// | |
//            | \_| ''\---/'' | |
//              \ .-\__ `-` ___/-. /
//           ___`. .' /--.--\ `. . __
//        ."" '< `.___\_<|>_/___.' >'"".
//       | | : `- \`.;`\ _ /`;.`/ - ` : | |
//         \ \ `-. \_ __\ /__ _/ .-` / /
// ======`-.____`-.___\_____/___.-`____.-'======
//                    `=---='
//
// .............................................
//          ·ð×æ±£ÓÓ             ÓÀÎÞBUG
//
// -------------------------------------------------------------------------

#include "NFCEvppHttpServer.h"
#include "NFComm/NFPluginModule/NFLogMgr.h"
#include "NFComm/NFCore/NFStringUtility.h"
#include "common/rapidjson/rapidjson.h"
#include "common/rapidjson/document.h"
#include "common/rapidjson/stringbuffer.h"
#include "common/rapidjson/writer.h"
#include "NFComm/NFCore/NFCommon.h"

NFCHttpEvppServer::~NFCHttpEvppServer()
{

}

bool NFCHttpEvppServer::Execute()
{
	return true;
}

bool NFCHttpEvppServer::InitServer(uint32_t listen_port)
{
	if (m_pHttpServer->Init(listen_port))
	{
		return true;
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Listen Port:{} Failed!", listen_port);
	return false;
}

bool NFCHttpEvppServer::InitServer(const std::vector<uint32_t>& listen_ports)
{
	if (m_pHttpServer->Init((const std::vector<int>&)listen_ports))
	{
		return true;
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Listen Port:{} Failed!", listen_ports);
	return false;
}

bool NFCHttpEvppServer::InitServer(const std::string& listen_ports/*like "80,8080,443"*/)
{
	if (m_pHttpServer->Init(listen_ports))
	{
		return true;
	}

	NFLogError(NF_LOG_NET_PLUGIN, 0, "Listen Port:{} Failed!", listen_ports);
	return false;
}