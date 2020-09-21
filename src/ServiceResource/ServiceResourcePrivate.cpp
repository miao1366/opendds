#include "ServiceResourcePrivate.h"
#include "ResourceAmbassador.h"
#include <iostream>
#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\uuid\uuid_generators.hpp>
#include <Ambassador.h>
#include <TimeAmbassador.h>
#include <TimerInstance.h>

namespace Data_Exchange_Platform {

	class TestAmbassador: public TimeAmbassador
	{
	public:
		TestAmbassador(long domain_ID,ServiceResource* serviceResource)
			:TimeAmbassador(domain_ID),m_serviceResource(serviceResource){}

		void receiveInterationInstance(InterationInstanceImpl* interationInstance){
			TimeAmbassador::receiveInterationInstance(interationInstance);
			m_serviceResource->receiveInterationInstance(interationInstance);
		}
	private:
		ServiceResource* m_serviceResource;
	};

	ServiceResourcePrivate::ServiceResourcePrivate(ServiceResource * publicPtr)
		:m_testAmbassador(nullptr),m_publicPtr(publicPtr), m_online(false), m_enabled(true)
	{
		boost::uuids::random_generator rgen;
		boost::uuids::uuid uuid = rgen();
		m_guid = boost::lexical_cast<std::string>(uuid);
		ResourceAmbassador::instance()->attachServiceResource(this);
	}

	ServiceResourcePrivate::~ServiceResourcePrivate()
	{
		logout();
		ResourceAmbassador::instance()->detachUserApplication(this);
		if (m_testAmbassador)
		{
			delete m_testAmbassador;
		}
	}

	bool ServiceResourcePrivate::login(std::string resource_name, std::vector<std::string> exparam)
	{
		m_resname = resource_name;
		m_exparam = exparam;
		bool bret = ResourceAmbassador::instance()->login(m_resname, m_guid, exparam);
		ResourceAmbassador::instance()->updateState(m_resname, m_guid, m_exparam, m_enabled, true);
		return m_online&&bret;
	}

	bool ServiceResourcePrivate::logout()
	{
		m_online = false;
		ResourceAmbassador::instance()->updateState(m_resname, m_guid, m_exparam, m_enabled, false);
		return ResourceAmbassador::instance()->logout(m_resname, m_guid);
	}

	void ServiceResourcePrivate::setEnabled(bool enabled)
	{
		m_enabled = enabled;
		ResourceAmbassador::instance()->updateState(m_resname, m_guid, m_exparam , m_enabled, true);
	}

	std::string ServiceResourcePrivate::getLastError()
	{
		return m_lastError;
	}

	void ServiceResourcePrivate::setLastError(std::string lastError)
	{
		m_lastError = lastError;
	}

	void ServiceResourcePrivate::errorClear()
	{
		m_lastError.clear();
	}

	bool ServiceResourcePrivate::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->registerObjectInstance(objectInstance);
		}
		return false;
	}

	bool ServiceResourcePrivate::declareInterationClass(InterationClass * interationClass)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->declareInterationClass(interationClass);
		}
		return false;
	}

	bool ServiceResourcePrivate::sendInterationInstance(InterationInstanceImpl * interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstance(interationInstance);
		}
		return false;
	}

	bool ServiceResourcePrivate::sendInterationInstanceByTime(InterationInstanceImpl * interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstanceByTime(interationInstance);
		}
		return false;
	}

	bool ServiceResourcePrivate::sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstanceByTime(vec_interationInstance);
		}
		return false;
	}

	void ServiceResourcePrivate::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		if (m_testAmbassador)
		{
			m_testAmbassador->registerInterationReceiveProc(recvfunc);
		}
	}

	void ServiceResourcePrivate::joinTestDomain(long domain)
	{
		m_testAmbassador = new TestAmbassador(domain,m_publicPtr);
		m_publicPtr->joinTest();
	}

	void ServiceResourcePrivate::leaveTestDomain()
	{
		if (m_testAmbassador)
		{
			delete m_testAmbassador;
			m_testAmbassador = nullptr;
			m_publicPtr->leaveTest();
		}
	}

	void ServiceResourcePrivate::registerTimer( TimerInstance* timer )
	{
		m_testAmbassador->registerTimer(timer);
	}

	void ServiceResourcePrivate::monitor(int delay, double lost, double speed)
	{
		m_publicPtr->monitor(delay,lost,speed);
	}

	bool ServiceResourcePrivate::holdRequest(std::string testid)
	{
		return m_publicPtr->holdRequest(testid);
	}

	bool ServiceResourcePrivate::freeRequest(std::string testid)
	{
		return m_publicPtr->freeRequest(testid);
	}

}
