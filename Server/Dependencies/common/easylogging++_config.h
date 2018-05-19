// -------------------------------------------------------------------------
//    @FileName         :    logconfig.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-12
//    @Email			:    445267987@qq.com
//    @Module           :    NFCore
//
// -------------------------------------------------------------------------

#pragma once

//用来配置easylogging, 比在编译时加方便

//#define ELPP_THREAD_SAFE                              //
#define ELPP_NO_DEFAULT_LOG_FILE						//不要默认的那个LOG文件
#define ELPP_FEATURE_ALL								//崩溃时方便打印LOG
//#define ELPP_STACKTRACE


/** 
 * @brief 由于功能需要，特定在easylogging中加入了"日志按时间滚动"功能
 * 改动精确到second， 所以配置文件的时候必须文件名必须精确的second， 比如:all_server_fatal_%datetime{%Y%M%d%H%m%s}.log
 * 不然无法实现分割
*/