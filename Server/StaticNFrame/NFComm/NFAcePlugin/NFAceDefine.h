// -------------------------------------------------------------------------
//    @FileName         :    NFAceDefine.h
//    @Author           :    GaoYi
//    @Date             :    2018-05-24
//    @Email			:    445267987@qq.com
//    @Module           :    NFAcePlugin
//
// -------------------------------------------------------------------------

#pragma once

#include "ace/Reactor.h"
#include "ace/Synch.h"
#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

#define MAX_MSG_MASK          64000        //逻辑Mark的线程数
