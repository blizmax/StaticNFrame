// -------------------------------------------------------------------------
//    @FileName         :    NFEvppHttpServer.h
//    @Author           :    GaoYi
//    @Date             :    2019-6-9
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//                    .::::.
//                  .::::::::.
//                 :::::::::::  FUCK YOU
//             ..:::::::::::'
//           '::::::::::::'
//             .::::::::::
//        '::::::::::::::..
//             ..::::::::::::.
//           ``::::::::::::::::
//            ::::``:::::::::'        .:::.
//           ::::'   ':::::'       .::::::::.
//         .::::'      ::::     .:::::::'::::.
//        .:::'       :::::  .:::::::::' ':::::.
//       .::'        :::::.:::::::::'      ':::::.
//      .::'         ::::::::::::::'         ``::::.
//  ...:::           ::::::::::::'              ``::.
// ```` ':.          ':::::::::'                  ::::..
//                    '.:::::'                    ':'````..
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIHttpServer.h"
#include "evpp/http/http_server.h"
#include <list>
#include <map>

class NFCHttpEvppServer : public NFIHttpServer
{
public:
	NFCHttpEvppServer()
	{
		mPort = 0;
		m_pHttpServer = new evpp::http::Server(1);
	}

	template<typename BaseType>
	NFCHttpEvppServer(uint32_t unLinkId, BaseType* pBaseType, bool (BaseType::*handleRecieve)(uint32_t, const NFHttpHandle& req), NFWebStatus(BaseType::*handleFilter)(uint32_t, const NFHttpHandle& req))
	{
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1, std::placeholders::_2);
		mPort = 0;
		m_pHttpServer = new evpp::http::Server(1);
	}

	virtual ~NFCHttpEvppServer();

	virtual bool Execute();

	virtual bool InitServer(uint32_t listen_port);
	virtual bool InitServer(const std::vector<uint32_t>& listen_ports);
	virtual bool InitServer(const std::string& listen_ports/*like "80,8080,443"*/);
private:
	evpp::http::Server* m_pHttpServer;
private:
	uint32_t mPort;
	std::vector<uint32_t> mVecPort;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
};