/**
* So基础类
*
*/

#ifndef __CTLIB_SOBASE_H__
#define __CTLIB_SOBASE_H__

#ifdef WIN32
#include "CTNull.h"
#else
#include <dlfcn.h>
#endif


/////////////////////////////动态加载应用/////////////////////////////
#include "CTMsgBase.h"
#include "CTAsyncCmdFactory.h"

/**
* so应用注册宏.
* 与server的接口，用于在so中注册应用
* @param[in] T 应用类型
*/
#define CTLIB_SO_FUNC(T) \
extern "C" CTSoAppInf* CreateSoApp() \
{ \
	return new T;  \
} \
\
extern "C" void DestroySoApp(CTSoAppInf* pstSoApp) \
{ \
	delete (T*)pstSoApp; \
}

namespace CTLib
{

/**
* so应用接口类.
* 用于mpsvr
*/
class CTSoAppInf
{
public:
	virtual ~CTSoAppInf(){};

	/**
	* so应用初始化.
	* @param[in] pData 用于mpsvr向应用传递初始化所需的参数
	* @return 0为正常，非0为错误
	*/
	virtual int Init(void * pData) = 0;
	
	/**
	* so应用处理函数.
	* 主要用于解析、拼装下层传递的enp头部
	* @param[in] pszReqBuf 请求buf
	* @param[in] iReqLen 请求长度
	* @param[in] pszAnsBuf 返回用buf
	* @param[in] iAnsBufLen 返回buf的最大长度
	* @param[in] iAnsLen 返回串的长度，需要在函数中设置
	* @return 0为正常，非0为错误
	*/
	virtual int Do(const char* pszReqBuf, int iReqLen,
		char* pszAnsBuf, int iAnsBufLen, int& iAnsLen)
	{
		if(iReqLen < (int)sizeof(CEnpNetHead))
		{
			return -1;
		}

		CEnpNetHead stHead;
		memcpy(&stHead, pszReqBuf, sizeof(CEnpNetHead));

		if(stHead.unDataLength != (iReqLen - sizeof(CEnpNetHead)))
		{
			return -2;
		}

		if(iAnsBufLen < (int)sizeof(CEnpNetHead))
		{
			return -3;
		}

		int iRet = Do(stHead, pszReqBuf + sizeof(CEnpNetHead), stHead.unDataLength, 
				pszAnsBuf + sizeof(CEnpNetHead), iAnsBufLen - sizeof(CEnpNetHead), iAnsLen);

		stHead.unDataLength = (unsigned short)(iAnsLen & 0xFFFF);
		memcpy(pszAnsBuf, &stHead, sizeof(CEnpNetHead));
		iAnsLen += sizeof(CEnpNetHead);
		return iRet;
	}

	/**
	* so应用退出函数.
	* @return 0为正常，非0为错误
	*/
	virtual int Exit() = 0;
protected:
	/**
	* so应用处理函数.
	* 用于执行应用处理逻辑
	* @param[in] stHead 请求包头部，包含用户ip等连接信息
	* @param[in] pszReqBuf 请求buf
	* @param[in] iReqLen 请求长度
	* @param[in] pszAnsBuf 返回用buf
	* @param[in] iAnsBufLen 返回buf的最大长度
	* @param[in] iAnsLen 返回串的长度，需要在函数中设置
	* @return 0为正常，非0为错误
	*/
	virtual int Do(const CEnpNetHead& stHead, const char* pszReqBuf, int iReqLen,
		char* pszAnsBuf, int iAnsBufLen, int& iAnsLen) = 0;
};

/**
* so加载类.
* 用于server加载so，每个加载器对应一个so，每个so对应一个app
*/
class CTSoLoader
{
public:
	typedef CTSoAppInf* CREATE_APP_FUNC();
	typedef void DESTROY_APP_FUNC(CTSoAppInf*);

	enum enumSoRetCode
	{
		SOAPP_OK = 0,					///<  成功
		SOAPP_LOAD_LIBRARY_ERROR = -1,   ///<  加载so失败
		SOAPP_LOAD_SYMBOL_ERROR = -2,	 ///<  加载so函数失败
	};

	/**
	* so载入函数.
	* @param[in] sSoFile so文件绝对路径
	* @return 0为正常，非0为错误
	*/
	int LoadSoApp(const char* sSoFile)
	{
		void* pstSo = dlopen(sSoFile, RTLD_LAZY|RTLD_NODELETE|RTLD_GLOBAL);
		if (!pstSo)
		{
            CT_WARNING(("dlopen, %s", dlerror()));
			return SOAPP_LOAD_LIBRARY_ERROR;
		}

		m_pCreateFunc = (CREATE_APP_FUNC*) dlsym(pstSo, "CreateSoApp");
		if (dlerror())
		{
            CT_WARNING(("dlsym CreateSoApp, %s", dlerror()));
			return SOAPP_LOAD_SYMBOL_ERROR;
		}

		m_pDestroyFunc = (DESTROY_APP_FUNC*) dlsym(pstSo, "DestroySoApp");
		if (dlerror())
		{
            CT_WARNING(("dlsym DestroySoApp, %s", dlerror()));
			return SOAPP_LOAD_SYMBOL_ERROR;
		}

		dlclose(pstSo);
		return SOAPP_OK;
	}

	/**
	* so应用创建函数.
	* @return 应用对象指针
	* 函数定义请参考CTLIB_SO_FUNC宏
	*/
	CTSoAppInf* CreateSoApp()
	{
		return m_pCreateFunc();
	}

	/**
	* so应用销毁函数.
	* @param[in] pstSoApp 应用对象指针
	* 函数定义请参考CTLIB_SO_FUNC宏
	*/
	void DestroySoApp(CTSoAppInf* pstSoApp)
	{
		m_pDestroyFunc(pstSoApp);
	}

private:
	CREATE_APP_FUNC* m_pCreateFunc;
	DESTROY_APP_FUNC* m_pDestroyFunc;
};

/////////////////////////////动态加载命令/////////////////////////////
/**
* 动态命令创建函数.
* so与server的接口
*/
typedef CTAsyncCmdRegisterInf* CTLIB_CREATE_CMD_FUNC();

#define CTLIB_CREATE_CMD_FUNC_NAME create_dynamic_cmd
#define CTLIB_CREATE_CMD_FUNC_NAME_STRING "create_dynamic_cmd"

/**
* 加载动态命令宏.
* 用于在server中加载动态命令
* @param[in] Path so文件绝对路径
* @param[out] CmdRegister 命令注册器，需要在server中保存，在server退出前销毁
*/
#define CTLIB_LOAD_DYNAMIC_CMD(Path, CmdRegister) \
	CmdRegister = NULL;\
	void* pstCmd = dlopen(Path, RTLD_LAZY|RTLD_NODELETE);\
	if (!pstCmd) \
{\
	CT_ERROR(("Cannot load library: %s, %s",dlerror(), Path));\
}\
else\
{\
	CTLIB_CREATE_CMD_FUNC* fCreateFunc = (CTLIB_CREATE_CMD_FUNC*) dlsym(pstCmd, CTLIB_CREATE_CMD_FUNC_NAME_STRING);\
	const char* dlsym_error = dlerror();\
	if (dlsym_error) \
{\
	CT_ERROR(("Cannot load symbol %s: %s",CTLIB_CREATE_CMD_FUNC_NAME_STRING, dlsym_error));\
}\
	else\
{\
	dlclose(pstCmd);\
	CmdRegister = fCreateFunc();\
}\
}

/**
* 动态命令注册宏.
* so与server的接口，用于在so中注册命令用
* @param[in] CmdID cmdid
* @param[in] CLASS 命令类型
*/
#ifdef WIN32
#define CTLIB_REGISTER_DYNAMIC_CMD(CmdID, CLASS) \
static CTAsyncCmdRegisterInf* CTLIB_CREATE_CMD_FUNC_NAME()\
{\
    return new CTAsyncCmdRegister<CLASS>(CmdID);\
}
#else
#define CTLIB_REGISTER_DYNAMIC_CMD(CmdID, CLASS) \
extern "C" CTAsyncCmdRegisterInf* CTLIB_CREATE_CMD_FUNC_NAME()\
{\
	return new CTAsyncCmdRegister<CLASS>(CmdID);\
}
#endif

} //namespace CTLib
#endif
