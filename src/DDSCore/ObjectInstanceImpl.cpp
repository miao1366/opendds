#include "ObjectInstanceImpl.h"
#include "ObjectInstancePrivate.h"

namespace Data_Exchange_Platform
{

	ObjectInstanceImpl::ObjectInstanceImpl(std::string instanceName, ObjectClass * objectClass)
		:m_private(new ObjectInstancePrivate(instanceName, objectClass,this))
	{
	}

	ObjectInstanceImpl::~ObjectInstanceImpl()
	{
		delete m_private;
	}

	bool ObjectInstanceImpl::updateAttribute(std::string attributeName)
	{
		return m_private->updateAttribute(attributeName);
	}

	void ObjectInstanceImpl::attributeChanged(std::string attributeName)
	{
	}

	bool ObjectInstanceImpl::setAttribute(std::string attributeName, Variant value)
	{
		return m_private->setAttribute(attributeName, value);
	}

	Variant ObjectInstanceImpl::attribute(std::string attributeName)
	{
		return m_private->attribute(attributeName);
	}

	std::string ObjectInstanceImpl::instanceName()
	{
		return m_private->instanceName();
	}

	ObjectClass * ObjectInstanceImpl::objectClass()
	{
		return m_private->objectClass();
	}

	void ObjectInstanceImpl::attachAmbassador(Ambassador * ambassador)
	{
		m_private->attachAmbassador(ambassador);
	}

	Ambassador * ObjectInstanceImpl::ambassador()
	{
		return m_private->ambassador();
	}

}


