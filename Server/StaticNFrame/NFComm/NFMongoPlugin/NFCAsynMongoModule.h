// -------------------------------------------------------------------------
//    @FileName         :    NFCAsynMongoModule.h
//    @Author           :    Yi.Gao
//    @Date             :    2019-1-1
//    @Module           :    NFCAsynMongoModule
//
//
// -------------------------------------------------------------------------

#pragma once

#include "NFComm/NFPluginModule/NFIAsynMongoModule.h"
#include "NFComm/NFPluginModule/NFITaskModule.h"
#include "NFCMongoDriverManager.h"
#include "NFComm/NFPluginModule/NFTask.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFMongoInsertJsonTask : public NFTask
{
public:
	NFMongoInsertJsonTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json_query)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json_query = _json_query;
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->InsertOne(nServerID, collectionName, json_query);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json_query;
};

class NFMongoInsertMessageTask : public NFTask
{
public:
	NFMongoInsertMessageTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
	}

	virtual ~NFMongoInsertMessageTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->InsertOne(nServerID, collectionName, *message);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
};

class NFMongoUpdateOneByStringKeyTask : public NFTask
{
public:
	NFMongoUpdateOneByStringKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json, const std::string& _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json = _json;
		key = _key;
	}

	virtual ~NFMongoUpdateOneByStringKeyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, json, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json;
	std::string key;
};

class NFMongoUpdateOneByIntKeyTask : public NFTask
{
public:
	NFMongoUpdateOneByIntKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json, uint64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json = _json;
		key = _key;
	}

	virtual ~NFMongoUpdateOneByIntKeyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, json, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json;
	uint64_t key;
};

class NFMongoUpdateOneMessageByIntKeyTask : public NFTask
{
public:
	NFMongoUpdateOneMessageByIntKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message, uint64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
		key = _key;
	}

	virtual ~NFMongoUpdateOneMessageByIntKeyTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, *message, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
	uint64_t key;
};

class NFMongoUpdateOneMessageByStringKeyTask : public NFTask
{
public:
	NFMongoUpdateOneMessageByStringKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message, const std::string& _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
		key = _key;
	}

	virtual ~NFMongoUpdateOneMessageByStringKeyTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOneByKey(nServerID, collectionName, *message, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
	std::string key;
};

class NFMongoUpdateOneMessageTask : public NFTask
{
public:
	NFMongoUpdateOneMessageTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
	}

	virtual ~NFMongoUpdateOneMessageTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOne(nServerID, collectionName, *message);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
};

class NFMongoUpdateOneJsonTask : public NFTask
{
public:
	NFMongoUpdateOneJsonTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json = _json;
	}

	virtual ~NFMongoUpdateOneJsonTask()
	{
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateOne(nServerID, collectionName, json);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json;
};

class NFMongoFindOneJsonTask : public NFTask
{
public:
	NFMongoFindOneJsonTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json_query)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json_query = _json_query;
	}

	virtual ~NFMongoFindOneJsonTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindOne(nServerID, collectionName, json_query);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json_query;
	std::string ret;
};

class NFMongoFindAllTask : public NFTask
{
public:
	NFMongoFindAllTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
	}

	virtual ~NFMongoFindAllTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindAll(nServerID, collectionName);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::vector<std::string> ret;
};

class NFMongoFindAllToJsonTask : public NFTask
{
public:
	NFMongoFindAllToJsonTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
	}

	virtual ~NFMongoFindAllToJsonTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			std::vector<std::string> vec = pMongoDriverManager->FindAll(nServerID, collectionName);
			ret += "[";
			for (size_t i = 0; i < vec.size(); i++)
			{
				ret += vec[i];
				if (i != vec.size() - 1)
					ret += ",";
			}
			ret += "]";
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string ret;
};

class FindOneByIntKey : public NFTask
{
public:
	FindOneByIntKey(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, int64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		key = _key;
	}

	virtual ~FindOneByIntKey()
	{
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindOneByKey(nServerID, collectionName, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	int64_t key;
	std::string ret;
};

class FindOneByStringKey : public NFTask
{
public:
	FindOneByStringKey(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		key = _key;
	}

	virtual ~FindOneByStringKey()
	{
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindOneByKey(nServerID, collectionName, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string key;
	std::string ret;
};

class NFMongoFindManyTask : public NFTask
{
public:
	NFMongoFindManyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json_query, const std::string& _json_opts)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json_query = _json_query;
		json_opts = _json_opts;
	}

	virtual ~NFMongoFindManyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindMany(nServerID, collectionName, json_query, json_opts);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json_query;
	std::string json_opts;
	std::vector<std::string> ret;
};

class NFMongoUpdateFieldByStringKeyTask : public NFTask
{
public:
	NFMongoUpdateFieldByStringKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json, const std::string _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json = _json;
		key = _key;
	}

	virtual ~NFMongoUpdateFieldByStringKeyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, json, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json;
	std::string key;
};

class NFMongoUpdateFieldByIntKeyTask : public NFTask
{
public:
	NFMongoUpdateFieldByIntKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _json, uint64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		json = _json;
		key = _key;
	}

	virtual ~NFMongoUpdateFieldByIntKeyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, json, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string json;
	uint64_t key;
};

class NFMongoUpdateFieldMessageByIntKeyTask : public NFTask
{
public:
	NFMongoUpdateFieldMessageByIntKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message, uint64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
		key = _key;
	}

	virtual ~NFMongoUpdateFieldMessageByIntKeyTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, *message, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
	uint64_t key;
};

class NFMongoUpdateFieldMessageByStringKeyTask : public NFTask
{
public:
	NFMongoUpdateFieldMessageByStringKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const google::protobuf::Message& _message, const std::string& _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		message = _message.New();
		message->CopyFrom(_message);
		key = _key;
	}

	virtual ~NFMongoUpdateFieldMessageByStringKeyTask()
	{
		NF_SAFE_DELETE(message);
	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			pMongoDriverManager->UpdateFieldByKey(nServerID, collectionName, *message, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	google::protobuf::Message* message;
	std::string key;
};

class FindFieldByIntKey : public NFTask
{
public:
	FindFieldByIntKey(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _fieldPath, int64_t _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		fieldPath = _fieldPath;
		key = _key;
	}

	virtual ~FindFieldByIntKey()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindFieldByKey(nServerID, collectionName, fieldPath, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string fieldPath;
	int64_t key;
	std::string ret;
};

class NFMongoFindFieldByStringKeyTask : public NFTask
{
public:
	NFMongoFindFieldByStringKeyTask(NFCMongoDriverManager* p, const int _nServerID, const std::string& _collectionName, const std::string& _fieldPath, const std::string& _key)
	{
		pMongoDriverManager = p;
		nServerID = _nServerID;
		collectionName = _collectionName;
		fieldPath = _fieldPath;
		key = _key;
	}

	virtual ~NFMongoFindFieldByStringKeyTask()
	{

	}

	/**
	**  �첽�̴߳�������������һ���߳�������
	*/
	virtual bool ThreadProcess()
	{
		if (pMongoDriverManager)
		{
			ret = pMongoDriverManager->FindFieldByKey(nServerID, collectionName, fieldPath, key);
		}
		return true;
	}
private:
	NFCMongoDriverManager* pMongoDriverManager;
	int nServerID;
	std::string collectionName;
	std::string fieldPath;
	std::string key;
	std::string ret;
};

class NFCAsynMongoModule : public NFIAsynMongoModule
{
public:
	NFCAsynMongoModule(NFIPluginManager* p);
	virtual ~NFCAsynMongoModule();

	virtual bool Awake();
	virtual bool Init();
	virtual bool AfterInit();
	virtual bool Execute();
	virtual bool BeforeShut();
	virtual bool Shut();

	virtual bool StartActorPool(const int nCount);
	virtual bool CloseActorPool();

	/**
	* @brief ͨ������Ķ�̬����id�����actor
	*		 Ϊ�˷�ֹ���ݿ���ң���ֹͬʱ�����ݿ���е�һ�����ݣ���ȡд�룬
	*		 ʹ�ö�̬����id, ʹ����ĳ��ʱ��ֻ��һ���̶߳Ա��е�һ�����ݣ���ȡ��д��
	* @param balanceId ��̬����id
	* @return	һ��actor����
	*/
	int GetBalanceActor(uint64_t balanceId);

	/**
	* @brief ������һ��actor
	*
	* @return actor����
	*/
	int GetRandActor();

	NFCMongoDriverManager* GetDriver(int actorId);
public:
	/**
	* @brief ���serverid������, һ��serverid��Ӧһ���������ݿ������
	*
	* @return bool
	*/
	virtual bool AddMongoServer(const int nServerID, const std::string& ip, uint32_t port, const std::string& dbname);

	/**
	* @brief ���serverid������, һ��serverid��Ӧһ���������ݿ������
	*
	* @return bool
	*/
	virtual bool AddMongoServer(const int nServerID, const std::string& uri, const std::string& dbname);

	/**
	* @brief �������
	*
	* @return bool
	*/
	virtual bool CreateCollection(const int nServerID, const std::string& collectionName, const std::string& key);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool InsertJson(const int nServerID, const std::string& collectionName, const std::string& json_query);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool InsertMessage(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOneByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string& key);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const std::string& json);

	/**
	* @brief ����һ������
	*
	* @return bool
	*/
	virtual bool UpdateOne(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindOne(const int nServerID, const std::string& collectionName, const std::string& json_query);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindMany(const int nServerID, const std::string& collectionName, const std::string& json_query, const std::string& json_opts = "");

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindAll(const int nServerID, const std::string& collectionName);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindAllToJson(const int nServerID, const std::string& collectionName);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindOneByKey(const int nServerID, const std::string& collectionName, int64_t key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindOneByKey(const int nServerID, const std::string& collectionName, const std::string& key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, const std::string key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool UpdateFieldByKey(const int nServerID, const std::string& collectionName, const std::string& json, uint64_t key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, uint64_t key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool UpdateFieldByKey(const int nServerID, const std::string& collectionName, const google::protobuf::Message& message, const std::string& key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, int64_t key);

	/**
	* @brief ��������
	*
	* @return bool
	*/
	virtual bool FindFieldByKey(const int nServerID, const std::string& collectionName, const std::string& fieldPath, const std::string& key);
private:
	std::map<int, NFCMongoDriverManager*> m_mongoDriverMap;
	uint64_t mnLastCheckTime;

	/**
	* @brief actor��������
	*/
	std::vector<int> m_vecActorPool;

	/**
	* @brief ����ƽ��������actor
	*/
	int mnSuitIndex = 0;
};

