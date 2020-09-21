#include "ServiceResource.h"
#include "ServiceResourcePrivate.h"
#include "ResourceAmbassador.h"

namespace Data_Exchange_Platform {

	ServiceResource::ServiceResource()
		:m_private(new ServiceResourcePrivate(this))
	{
	}

	ServiceResource::~ServiceResource()
	{
		delete m_private;
	}

	bool ServiceResource::login(std::string resource_name, std::vector<std::string> exparam)
	{
		return m_private->login(resource_name, exparam);
	}

	bool ServiceResource::logout()
	{
		return m_private->logout();
	}

	void ServiceResource::setEnabled(bool enabled)
	{
		m_private->setEnabled(enabled);
	}

	std::string ServiceResource::getLastError()
	{
		return m_private->getLastError();
	}

	bool ServiceResource::holdRequest(std::string testid)
	{
		return true;
	}

	bool ServiceResource::freeRequest(std::string testid)
	{
		return true;
	}

	void ServiceResource::updateResourceExtendedParameter(int index, std::string param)
	{
	}

	void ServiceResource::updateTestExtendedParameter(int index, std::string param)
	{
	}

	void ServiceResource::joinTest()
	{
	}

	void ServiceResource::leaveTest()
	{
	}

	void ServiceResource::receiveInterationInstance( InterationInstanceImpl* interationInstance )
	{

	}

	bool ServiceResource::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		return m_private->registerObjectInstance(objectInstance);
	}

	bool ServiceResource::declareInterationClass(InterationClass * interationClass)
	{
		return m_private->declareInterationClass(interationClass);
	}

	bool ServiceResource::sendInterationInstance(InterationInstanceImpl * interationInstance)
	{
		return m_private->sendInterationInstance(interationInstance);
	}

	bool ServiceResource::sendInterationInstanceByTime(InterationInstanceImpl * interationInstance)
	{
		return m_private->sendInterationInstanceByTime(interationInstance);
	}

	bool ServiceResource::sendInterationInstanceByTime(std::vector<InterationInstanceImpl*> vec_interationInstance)
	{
		return m_private->sendInterationInstanceByTime(vec_interationInstance);
	}

	void ServiceResource::registerTimer( TimerInstance* timer )
	{
		return m_private->registerTimer(timer);
	}

	void ServiceResource::monitor(int delay, double lost, double speed)
	{
	}

	void ServiceResource::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		m_private->registerInterationReceiveProc(recvfunc);
	}

	void ServiceResource::enableSpeedMonitor(bool bSpeed)
	{
		ResourceAmbassador::instance()->enableSpeedMonitor(bSpeed);
	}

	void ServiceResource::enableSyncTime(bool bSync)
	{
		ResourceAmbassador::instance()->enableSyncTime(bSync);
	}

}