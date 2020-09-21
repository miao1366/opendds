#include "TimeServerAmbassador.h"
#include "TimerServerInstance.h"

namespace Data_Exchange_Platform
{

	TimeServerAmbassador::TimeServerAmbassador(long domain_ID)
		:Ambassador(domain_ID)
	{
		registerObjectClass(TimerClass::timerServerClass());

		declareInterationClass(TimerClass::registerRequestServerClass());
		declareInterationClass(TimerClass::unregisterRequestServerClass());
		declareInterationClass(TimerClass::advanceRequestServerClass());
		declareInterationClass(TimerClass::resetRequestServerClass());
		declareInterationClass(TimerClass::resetedServerClass());

		declareInterationClass(TimerClass::playServerClass());

		m_timer = new TimerServerInstance();
		ObjectInstanceImpl* obj = dynamic_cast<ObjectInstanceImpl*>(m_timer);
		registerObjectInstance(obj);

	}


	TimeServerAmbassador::~TimeServerAmbassador()
	{
		delete m_timer;
	}

	void TimeServerAmbassador::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == TimerClass::registerRequestServerClass())
		{
			registerRequestProc(interationInstance);
		}
		if (interationInstance->interationClass() == TimerClass::unregisterRequestServerClass())
		{
			unregisterRequestProc(interationInstance);
		}
		if (interationInstance->interationClass() == TimerClass::advanceRequestServerClass())
		{
			advanceRequestProc(interationInstance);
		}
		if (interationInstance->interationClass() == TimerClass::resetRequestServerClass())
		{
			resetRequestProc(interationInstance);
		}
		if (interationInstance->interationClass() == TimerClass::playServerClass())
		{
			Variant oper = interationInstance->parameterValue("operation");
			int value = boost::get<int>(oper);
			m_timer->playOperation(value);
			
		}

	}

	void TimeServerAmbassador::registerRequestProc(InterationInstanceImpl * inst)
	{
		Variant timerid = inst->parameterValue("timerid");
		if (m_timer)
		{
			std::string strtimerid = boost::get<std::string>(timerid);
			m_timer->registerClient(strtimerid);
			m_pubHandleMap.insert(std::pair<long,std::string>(inst->pubHandle(),strtimerid));
			m_timer->updateAttribute();
		}
		
	}

	void TimeServerAmbassador::unregisterRequestProc(InterationInstanceImpl * inst)
	{
		Variant timerid = inst->parameterValue("timerid");
		m_timer->unregisterClient(boost::get<std::string>(timerid));
	}

	void TimeServerAmbassador::advanceRequestProc(InterationInstanceImpl * inst)
	{
		Variant timerid = inst->parameterValue("timerid");
		Variant advance = inst->parameterValue("advance");
		Variant username = inst->parameterValue("username");

		{
			std::stringstream logbuf;
			logbuf << "timerid " << boost::get<std::string>(timerid) <<" username: "<< boost::get<std::string>(username) << " advance timeStamp: " << boost::get<__int64>(advance);
			log(logbuf.str());
		}

		m_timer->advanceTo(boost::get<std::string>(timerid), boost::get<__int64>(advance));
	}

	void TimeServerAmbassador::resetRequestProc(InterationInstanceImpl * inst)
	{
		m_timer->reset();
		sendReseted();
		m_timer->updateAttribute();
	}

	void TimeServerAmbassador::sendReseted()
	{
		InterationInstanceImpl inst(TimerClass::resetedServerClass());
		sendInterationInstance(&inst);
	}

	void TimeServerAmbassador::subscriptionLost(std::string classname, long handle)
	{
		std::map<long, std::string>::iterator it = m_pubHandleMap.find(handle);
		if (it != m_pubHandleMap.end())
		{
			m_timer->unregisterClient(it->second);
			m_pubHandleMap.erase(it);
		}
	}

}
