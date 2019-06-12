// -------------------------------------------------------------------------
//    @FileName         :    NFITaskComponent.h
//    @Author           :    Gao.Yi
//    @Date             :    2019-06-12
//    @Module           :    NFITaskComponent
//
//
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

#include "NFComm/NFCore/NFPlatform.h"
#include "NFTask.h"

class NFTaskActor;

class NFITaskComponent
{
public:
	NFITaskComponent()
	{
		mTaskActor = nullptr;
	}

	virtual ~NFITaskComponent()
	{
	}

	virtual void SetTaskActor(NFTaskActor* taskActor)
	{
		mTaskActor = taskActor;
	}

	virtual NFTaskActor* GetActor()
	{
		return mTaskActor;
	}


	virtual void ProcessTaskStart(NFTask* pTask)
	{
	}

	virtual void ProcessTask(NFTask* pTask)
	{
		if (pTask)
		{
			//NFLogError("actor id:{}, threadid:{}", this->GetAddress().AsInteger(), ThreadId());
			pTask->ThreadProcess();
		}
	}

	virtual void ProcessTaskEnd(NFTask* pTask)
	{

	}
private:
	NFTaskActor* mTaskActor;
};


