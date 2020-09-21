#include "ObjectClassPrivate.h"

namespace Data_Exchange_Platform
{
	ObjectClassPrivate::~ObjectClassPrivate()
	{
		for (int i = 0; i < m_vecAttribute.size(); i++)
		{
			delete m_vecAttribute[i];
		}
	}

	ObjectClass::Attribute * ObjectClassPrivate::addAttribute(std::string attributeName, DataType & dataType, ExchangeType exchangeType)
	{
		ObjectClass::Attribute * attr = new ObjectClass::Attribute(attributeName, dataType, exchangeType,m_publicPtr);
		m_vecAttribute.push_back(attr);
		return attr;
	}

	ObjectClass::Attribute * ObjectClassPrivate::attribute(std::string attributeName)
	{
		for (int i = 0; i < m_vecAttribute.size(); i++)
		{
			if (m_vecAttribute[i]->attributeName() == attributeName)
			{
				return m_vecAttribute[i];
			}
		}
		return nullptr;
	}

}
