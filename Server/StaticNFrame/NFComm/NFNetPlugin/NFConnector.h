// -------------------------------------------------------------------------
//    @FileName         :    NFConnector.h
//    @Author           :    GaoYi
//    @Date             :    2018/05/26
//    @Email			:    445267987@qq.com
//    @Module           :    NFNetPlugin
//
// -------------------------------------------------------------------------
#pragma once
#include <memory>
#include <functional>
#include <vector>

#include "NFLibEvent.h"
#include <NFComm/NFCore/NFDuration.h>
#include "NFEventWatcher.h"

class TCPClient;
class NFEventLoop;
class FdChannel;
class DNSResolver;

class Connector : public std::enable_shared_from_this<Connector>
{
public:
	typedef std::function<void(evutil_socket_t sockfd, const std::string& /*local addr*/)> NewConnectionCallback;
	Connector(NFEventLoop* loop, TCPClient* client);
	~Connector();
	void Start();
	void Cancel();
public:
	void SetNewConnectionCallback(NewConnectionCallback cb)
	{
		conn_fn_ = cb;
	}

	bool IsConnecting() const
	{
		return status_ == kConnecting;
	}

	bool IsConnected() const
	{
		return status_ == kConnected;
	}

	bool IsDisconnected() const
	{
		return status_ == kDisconnected;
	}

	int status() const
	{
		return status_;
	}

private:
	void Connect();
	void HandleWrite();
	void HandleError();
	void OnConnectTimeout();
	void OnDNSResolved(const std::vector<struct in_addr>& addrs);
	std::string StatusToString() const;
private:
	enum Status
	{
		kDisconnected,
		kDNSResolving,
		kDNSResolved,
		kConnecting,
		kConnected
	};

	Status status_;
	NFEventLoop* loop_;
	TCPClient* owner_tcp_client_;

	std::string remote_addr_; // host:port
	std::string remote_host_; // host
	int remote_port_ = 0; // port
	struct sockaddr_storage raddr_;

	NFDuration timeout_;

	evutil_socket_t fd_ = -1;

	// A flag indicate whether the Connector owns this fd.
	// If the Connector owns this fd, the Connector has responsibility to close this fd.
	bool own_fd_ = false;

	std::unique_ptr<FdChannel> chan_;
	std::unique_ptr<NFTimerEventWatcher> timer_;
	std::shared_ptr<DNSResolver> dns_resolver_;
	NewConnectionCallback conn_fn_;
};
