#pragma once

#include <common.h>
#include "ServiceResource.h"

namespace Data_Exchange_Platform {

	class TimeAmbassador;
	class TimerInstance;

	class ServiceResourcePrivate
	{
	public:
		ServiceResourcePrivate(ServiceResource* publicPtr);
		~ServiceResourcePrivate();

		std::string resname() { return m_resname; }
		std::string id() { return m_guid; }

		void permit() { m_online = true; }

		bool isOnline() { return m_online; }

		bool inTest() { return m_testAmbassador; }

		bool login(std::string resource_name, std::vector<std::string> exparam);
		bool logout();

		void setEnabled(bool enabled);

		std::string getLastError();
		void setLastError(std::string lastError);
		void errorClear();

		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		bool declareInterationClass(InterationClass* interationClass);

		bool sendInterationInstance(InterationInstanceImpl* interationInstance);

		bool sendInterationInstanceByTime(InterationInstanceImpl * interationInstance);
		bool sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance);

		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		void joinTestDomain(long domain);
		void leaveTestDomain();

		void registerTimer( TimerInstance* timer );

		void monitor(int delay, double lost, double speed);

		bool holdRequest(std::string testid);
		bool freeRequest(std::string testid);

	private:
		std::string m_guid;
		std::string m_resname;
		bool m_enabled;
		std::vector<std::string> m_exparam;
		TimeAmbassador* m_testAmbassador;
		ServiceResource * m_publicPtr;
		bool m_online;

		std::string m_lastError;
	};

}



