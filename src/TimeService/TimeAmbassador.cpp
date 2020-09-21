#include "TimeAmbassador.h"
#include "TimerClass.h"
#include "TimerInstance.h"
#include "TimerServerInstance.h"

#include <iostream>

namespace Data_Exchange_Platform
{

	TimeAmbassador::TimeAmbassador(long domain_ID)
		:Ambassador(domain_ID)
	{
		registerObjectClass(TimerClass::timerClientClass());

		declareInterationClass(TimerClass::registerRequestClientClass());
		declareInterationClass(TimerClass::unregisterRequestClientClass());
		declareInterationClass(TimerClass::advanceRequestClientClass());
		declareInterationClass(TimerClass::resetRequestClientClass());
		declareInterationClass(TimerClass::resetedClientClass());

		declareInterationClass(TimerClass::playClientClass());

		//waitConnection();

		m_timer = nullptr;
	}

	TimeAmbassador::~TimeAmbassador(void)
	{
		//if (m_timer)
		//{
		//	delete m_timer;
		//}
		sendUnregisterRequest(m_timer->timerid());
	}

	void TimeAmbassador::registerTimer( TimerInstance* timer )
	{
		if(m_timer==nullptr)
		{
			ObjectInstanceImpl* obj = dynamic_cast<ObjectInstanceImpl*>(timer);
			registerObjectInstance(obj);
			m_timer = timer;
			m_timer->init();
		}
	}

	void TimeAmbassador::sendAdvanceRequest(std::string timerid, __int64 msec)
	{
		InterationInstanceImpl inst(TimerClass::advanceRequestClientClass());

		inst.setParameterValue("timerid", timerid);
		inst.setParameterValue("username", m_timer->userName());
		inst.setParameterValue("testid", m_timer->testId());
		inst.setParameterValue("advance", msec);

		sendInterationInstance(&inst);
	}

	bool TimeAmbassador::sendRegisterRequest(std::string timerid)
	{
		InterationInstanceImpl inst(TimerClass::registerRequestClientClass());
		inst.setParameterValue("timerid", timerid);
		sendInterationInstance(&inst);
		log("timerid " + timerid);
		return m_timer->waitInit();
	}

	void TimeAmbassador::sendUnregisterRequest(std::string timerid)
	{
		InterationInstanceImpl inst(TimerClass::unregisterRequestClientClass());
		inst.setParameterValue("timerid", timerid);
		sendInterationInstance(&inst);
	}

	bool TimeAmbassador::sendResetRequest()
	{
		boost::mutex::scoped_lock lock(m_mutex);
		InterationInstanceImpl inst(TimerClass::resetRequestClientClass());
		sendInterationInstance(&inst);
		return m_condition.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(30));
	}

	bool TimeAmbassador::sendInterationInstanceByTime(InterationInstanceImpl * interationInstance)
	{
		interationInstance->setTimeStamp(m_timer->localTime());
		return sendInterationInstance(interationInstance);
	}

	bool TimeAmbassador::sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance)
	{
		for (int i = 0;i< vec_interationInstance.size();i++)
		{
			vec_interationInstance[i]->setTimeStamp(m_timer->localTime());
		}
		return sendInterationInstance(vec_interationInstance);
	}

	void TimeAmbassador::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{
		resetedProc(interationInstance);
		if (interationInstance->interationClass() == TimerClass::advanceRequestClientClass())
		{
			m_timer->receiveInterationInstance(interationInstance);
		}
		
	}

	bool TimeAmbassador::popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue)
	{
		return instanceQueue.front()->timeStamp() < m_timer->localTime();
	}

	void TimeAmbassador::resetedProc(InterationInstanceImpl* inst)
	{
		if (inst->interationClass()==TimerClass::resetedClientClass())
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_timer->resetedBegin();
			m_timer->reseted();
			m_timer->resetedEnd();
			m_condition.notify_one();
		}
	}

}
