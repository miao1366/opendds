#pragma once

#include <Ambassador.h>
#include "ServiceResourcePrivate.h"
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <list>

using namespace Data_Exchange_Platform;

class ResourceAmbassador :public Ambassador
{
public:
	~ResourceAmbassador();

	static ResourceAmbassador* instance();

	bool login(std::string resource_name, std::string id, std::vector<std::string> exparam);
	void updateState(std::string resource_name, std::string id, std::vector<std::string> exparam, bool enabled, bool exist);

	bool logout(std::string resource_name, std::string id);

	void attachServiceResource(ServiceResourcePrivate*  serviceResource) { m_vecServiceResource.push_back(serviceResource); }
	void detachUserApplication(ServiceResourcePrivate*  serviceResource) {
		std::vector<ServiceResourcePrivate*>::iterator it = std::find(m_vecServiceResource.begin(),m_vecServiceResource.end(),serviceResource);
		if (it!=m_vecServiceResource.end())
		{
			m_vecServiceResource.erase(it); 
		}
	}

	void enableSpeedMonitor(bool bSpeed) {
		m_monitorSpeed = bSpeed;
	}

	void enableSyncTime(bool bSync) {
		m_syncTime = bSync;
	}

private:
	ResourceAmbassador();

	void createResLoginClass();
	void createResAccessClass();
	void createResLogoutClass();
	//void createTestLicenceClass();
	void createResUpdateClass();

	void createResRequestClass();
	void createResReplyClass();

	//void createResQueryClass();
	//void createResOnlineClass();

	void createResMonitorClass();

	static void resAccessProc(InterationInstanceImpl* interationInstance);
	//static void testLicenceProc(InterationInstanceImpl* interationInstance);
	static void resRequestProc(InterationInstanceImpl* interationInstance);

	static void resMonitorProc(InterationInstanceImpl* interationInstance);

	void monitorTimeout();

private:
	static ResourceAmbassador* m_instance;
	std::vector<ServiceResourcePrivate*>  m_vecServiceResource;

	boost::mutex m_resMutex;
	boost::condition_variable m_resCondition;

	boost::thread m_monitor;
	boost::asio::io_service m_io;
	boost::asio::deadline_timer m_timer;

	bool m_monitorSpeed;
	bool m_syncTime;
	
	static __int64  prets ;
	static double lost ;
	static __int64 num ;
};

