#include "TimerClass.h"

namespace Data_Exchange_Platform
{

	ObjectClass* TimerClass::m_timerServerClass = nullptr;
	ObjectClass* TimerClass::m_timerClientClass = nullptr;

	InterationClass* TimerClass::m_registerRequestServerClass = nullptr;
	InterationClass* TimerClass::m_registerRequestClientClass = nullptr;

	InterationClass* TimerClass::m_unregisterRequestServerClass = nullptr;
	InterationClass* TimerClass::m_unregisterRequestClientClass = nullptr;

	InterationClass* TimerClass::m_advanceRequestServerClass = nullptr;
	InterationClass* TimerClass::m_advanceRequestClientClass = nullptr;

	InterationClass* TimerClass::m_resetRequestServerClass = nullptr;
	InterationClass* TimerClass::m_resetRequestClientClass = nullptr;

	InterationClass* TimerClass::m_resetedServerClass = nullptr;
	InterationClass* TimerClass::m_resetedClientClass = nullptr;

	InterationClass* TimerClass::m_playeServerClass = nullptr;
	InterationClass* TimerClass::m_playClientClass = nullptr;

	ObjectClass * TimerClass::timerServerClass()
	{
		if (!m_timerServerClass)
		{
			m_timerServerClass = new ObjectClass("_Timer_");

			m_timerServerClass->addAttribute("state", DATATYPE(int), Publish_Only);
			m_timerServerClass->addAttribute("currentTime", DATATYPE(__int64), Publish_Only);
			m_timerServerClass->addAttribute("fasterthan", DATATYPE(bool), Subscribe_Only);

		}

		return m_timerServerClass;
	}

	ObjectClass * TimerClass::timerClientClass()
	{
		if (!m_timerClientClass)
		{
			m_timerClientClass = new ObjectClass("_Timer_");

			m_timerClientClass->addAttribute("state", DATATYPE(int), Subscribe_Only);
			m_timerClientClass->addAttribute("currentTime", DATATYPE(__int64), Subscribe_Only);
			m_timerClientClass->addAttribute("fasterthan", DATATYPE(bool), Publish_Only);

		}
		return m_timerClientClass;
	}

	InterationClass * TimerClass::registerRequestServerClass()
	{
		if (!m_registerRequestServerClass)
		{
			m_registerRequestServerClass = new InterationClass("_Timer_Register_Request_", Subscribe_Only);

			m_registerRequestServerClass->addParameter("timerid", DATATYPE(std::string));
		}
		return m_registerRequestServerClass;
	}

	InterationClass * TimerClass::registerRequestClientClass()
	{
		if (!m_registerRequestClientClass)
		{
			m_registerRequestClientClass = new InterationClass("_Timer_Register_Request_", Publish_Only);

			m_registerRequestClientClass->addParameter("timerid", DATATYPE(std::string));
		}
		return m_registerRequestClientClass;
	}

	InterationClass * TimerClass::unregisterRequestServerClass()
	{
		if (!m_unregisterRequestServerClass)
		{
			m_unregisterRequestServerClass = new InterationClass("_Timer_Unregister_Request_", Subscribe_Only);

			m_unregisterRequestServerClass->addParameter("timerid", DATATYPE(std::string));
		}
		return m_unregisterRequestServerClass;
	}

	InterationClass * TimerClass::unregisterRequestClientClass()
	{
		if (!m_unregisterRequestClientClass)
		{
			m_unregisterRequestClientClass = new InterationClass("_Timer_Unregister_Request_", Publish_Only);

			m_unregisterRequestClientClass->addParameter("timerid", DATATYPE(std::string));
		}
		return m_unregisterRequestClientClass;
	}

	InterationClass * TimerClass::advanceRequestServerClass()
	{
		if (!m_advanceRequestServerClass)
		{
			m_advanceRequestServerClass = new InterationClass("_Timer_Advance_Request_", Subscribe_Only);

			m_advanceRequestServerClass->addParameter("timerid", DATATYPE(std::string));
			m_advanceRequestServerClass->addParameter("username", DATATYPE(std::string));
			m_advanceRequestServerClass->addParameter("testid", DATATYPE(std::string));
			m_advanceRequestServerClass->addParameter("advance", DATATYPE(__int64));
		}
		return m_advanceRequestServerClass;
	}

	InterationClass * TimerClass::advanceRequestClientClass()
	{
		if (!m_advanceRequestClientClass)
		{
			m_advanceRequestClientClass = new InterationClass("_Timer_Advance_Request_", Publish_Subscribe);

			m_advanceRequestClientClass->addParameter("timerid", DATATYPE(std::string));
			m_advanceRequestClientClass->addParameter("username", DATATYPE(std::string));
			m_advanceRequestClientClass->addParameter("testid", DATATYPE(std::string));
			m_advanceRequestClientClass->addParameter("advance", DATATYPE(__int64));
		}
		return m_advanceRequestClientClass;
	}

	InterationClass * TimerClass::resetRequestServerClass()
	{
		if (!m_resetRequestServerClass)
		{
			m_resetRequestServerClass = new InterationClass("_Timer_Reset_Request_", Subscribe_Only);

		}
		return m_resetRequestServerClass;
	}

	InterationClass * TimerClass::resetRequestClientClass()
	{
		if (!m_resetRequestClientClass)
		{
			m_resetRequestClientClass = new InterationClass("_Timer_Reset_Request_", Publish_Only);

		}
		return m_resetRequestClientClass;
	}

	InterationClass * TimerClass::resetedServerClass()
	{
		if (!m_resetedServerClass)
		{
			m_resetedServerClass = new InterationClass("_Timer_Reseted_", Publish_Only);

		}
		return m_resetedServerClass;
	}

	InterationClass * TimerClass::resetedClientClass()
	{
		if (!m_resetedClientClass)
		{
			m_resetedClientClass = new InterationClass("_Timer_Reseted_", Subscribe_Only);

		}
		return m_resetedClientClass;
	}

	InterationClass * TimerClass::playServerClass()
	{
		if (!m_playeServerClass)
		{
			m_playeServerClass = new InterationClass("__PLAY__",Subscribe_Only);
			m_playeServerClass->addParameter("operation",DATATYPE(int));
		}
		return m_playeServerClass;
	}

	InterationClass * TimerClass::playClientClass()
	{
		if (!m_playClientClass)
		{
			m_playClientClass = new InterationClass("__PLAY__", Publish_Only);
			m_playClientClass->addParameter("operation",DATATYPE(int));
		}
		return m_playClientClass;
	}

}