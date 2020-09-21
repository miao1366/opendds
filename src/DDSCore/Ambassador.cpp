#include "Ambassador.h"
#include "AmbassadorPrivate.h"

namespace Data_Exchange_Platform
{

	Ambassador::Ambassador(long domain_ID)
		:m_private(new AmbassadorPrivate(domain_ID,this))
	{
	}

	Ambassador::~Ambassador()
	{
		delete m_private;
	}

	bool Ambassador::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		return m_private->registerObjectInstance(objectInstance);
	}

	bool Ambassador::registerObjectClass(ObjectClass * objectClass)
	{
		return m_private->registerObjectClass(objectClass);
	}

	ObjectClass * Ambassador::objectClass(std::string objectName)
	{
		return m_private->objectClass(objectName);
	}

	ObjectInstanceImpl * Ambassador::objectInstance(std::string instanceName, std::string objectName)
	{
		return m_private->objectInstance(instanceName, objectName);
	}

	bool Ambassador::declareObjectAttribute(ObjectClass::Attribute * attribute)
	{
		return m_private->declareObjectAttribute(attribute);
	}

	bool Ambassador::sendObjectAttribute(ObjectInstanceImpl * objectInstance, ObjectClass::Attribute * attribute, Variant value)
	{
		return m_private->sendObjectAttribute(objectInstance, attribute, value);
	}

	void Ambassador::receiveObjectAttribute(ObjectInstanceImpl * objectInstance, ObjectClass::Attribute * attribute, Variant value)
	{
	}

	bool Ambassador::declareInterationClass(InterationClass * interationClass)
	{
		return m_private->declareInterationClass(interationClass);
	}

	InterationClass * Ambassador::interationClass(std::string interationName)
	{
		return m_private->interationClass(interationName);
	}

	bool Ambassador::sendInterationInstance(InterationInstanceImpl * interationInstance, bool bwait)
	{
		std::vector<InterationInstanceImpl *> vec_interationInstance;
		vec_interationInstance.push_back(interationInstance);
		return m_private->sendInterationInstance(vec_interationInstance, bwait);
	}

	bool Ambassador::sendInterationInstance(std::vector<InterationInstanceImpl*> vec_interationInstance, bool bwait )
	{
		return m_private->sendInterationInstance(vec_interationInstance, bwait);
	}

	void Ambassador::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{
	}

	InterationInstanceImpl* Ambassador::waitInterationInstance(InterationClass * interationClass, int timeout)
	{
		return m_private->waitInterationInstance(interationClass, timeout);
	}

	void Ambassador::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		m_private->registerInterationReceiveProc(recvfunc);
	}

	void Ambassador::waitConnection()
	{
		m_private->waitConnection();
	}

	void Ambassador::subscriptionLost(std::string classname, long handle)
	{
	}

	void Ambassador::log(std::string msg)
	{
		m_private->log(msg);
	}

	bool Ambassador::pushCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue)
	{
		return m_private->pushCheckInterationQueue(instanceQueue);
	}

	bool Ambassador::popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue)
	{
		return m_private->popCheckInterationQueue(instanceQueue);
	}

	void Ambassador::setInterationQueueMax(int max)
	{
		m_private->setInterationQueueMax(max);
	}

	int Ambassador::interationQueueMax()
	{
		return m_private->interationQueueMax();
	}

	double Ambassador::runtime()
	{
		return AmbassadorPrivate::runtime();
	}

}