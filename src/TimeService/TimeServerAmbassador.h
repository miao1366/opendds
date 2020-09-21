#pragma once

#include <common.h>
#include <Ambassador.h>
#include <map>

namespace Data_Exchange_Platform
{

	class TimerServerInstance;

	class DATA_EXCHANGE_PLATFORM_EXPORT TimeServerAmbassador : public Ambassador
	{
	public:
		TimeServerAmbassador(long domain_ID);
		~TimeServerAmbassador();

		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);

		void registerRequestProc(InterationInstanceImpl* inst);

		void unregisterRequestProc(InterationInstanceImpl* inst);

		void advanceRequestProc(InterationInstanceImpl* inst);

		void resetRequestProc(InterationInstanceImpl* inst);

		void sendReseted();

		virtual void subscriptionLost(std::string classname, long handle);

	private:
		TimerServerInstance * m_timer;

		std::map<long,std::string> m_pubHandleMap;

	};
}

