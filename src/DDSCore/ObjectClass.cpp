#include "ObjectClass.h"
#include "ObjectClassPrivate.h"

namespace Data_Exchange_Platform
{

	ObjectClass::ObjectClass(std::string objectName)
		:m_private(new ObjectClassPrivate(objectName,this))
	{
	}

	ObjectClass::~ObjectClass()
	{
		delete m_private;
	}

	std::string ObjectClass::objectName()
	{
		return m_private->objectName();
	}

	ObjectClass::Attribute * ObjectClass::addAttribute(std::string attributeName, DataType& dataType, ExchangeType exchangeType)
	{
		return m_private->addAttribute(attributeName, dataType, exchangeType);
	}

	ObjectClass::Attribute * ObjectClass::attribute(std::string attributeName)
	{
		return m_private->attribute(attributeName);
	}

	ObjectClass::Attribute * ObjectClass::attributeAt(int index)
	{
		return m_private->attributeAt(index);
	}

	int ObjectClass::attrbuteCount()
	{
		return m_private->attrbuteCount();
	}

}

