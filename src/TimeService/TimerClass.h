#pragma once

#include <ObjectClass.h>
#include <InterationClass.h>

namespace Data_Exchange_Platform
{
	enum TimerType {Server,Client};

	class DATA_EXCHANGE_PLATFORM_EXPORT TimerClass{
	public:
		static ObjectClass* timerServerClass();
		static ObjectClass* timerClientClass();

		static InterationClass* registerRequestServerClass();
		static InterationClass* registerRequestClientClass();

		static InterationClass* unregisterRequestServerClass();
		static InterationClass* unregisterRequestClientClass();

		static InterationClass* advanceRequestServerClass();
		static InterationClass* advanceRequestClientClass();

		static InterationClass* resetRequestServerClass();
		static InterationClass* resetRequestClientClass();

		static InterationClass* resetedServerClass();
		static InterationClass* resetedClientClass();

		static InterationClass* playServerClass();
		static InterationClass* playClientClass();

	private:
		static ObjectClass* m_timerServerClass;
		static ObjectClass* m_timerClientClass;

		static InterationClass* m_registerRequestServerClass;
		static InterationClass* m_registerRequestClientClass;

		static InterationClass* m_unregisterRequestServerClass;
		static InterationClass* m_unregisterRequestClientClass;

		static InterationClass* m_advanceRequestServerClass;
		static InterationClass* m_advanceRequestClientClass;

		static InterationClass* m_resetRequestServerClass;
		static InterationClass* m_resetRequestClientClass;

		static InterationClass* m_resetedServerClass;
		static InterationClass* m_resetedClientClass;

		static InterationClass* m_playeServerClass;
		static InterationClass* m_playClientClass;
	};

}