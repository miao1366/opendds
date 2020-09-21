#include "ObjectInstancePrivate.h"
#include "Ambassador.h"

namespace Data_Exchange_Platform
{

	bool ObjectInstancePrivate::updateAttribute(std::string attributeName)
	{
		if (m_ambassador)
		{
			if (attributeName == "")
			{
				for (size_t i = 0; i < m_objectClass->attrbuteCount(); i++)
				{
					ObjectClass::Attribute* attr = m_objectClass->attributeAt(i);
					if (attr&&attr->exchangeType()&Publish_Only)
					{
						Variant value = attribute(attr->attributeName());
						m_ambassador->sendObjectAttribute(m_publicPtr, attr, value);
					}
				}
				return true;
			}
			else
			{
				ObjectClass::Attribute* attr = m_objectClass->attribute(attributeName);
				if (attr&&attr->exchangeType()&Publish_Only)
				{
					Variant value = attribute(attributeName);
					m_ambassador->sendObjectAttribute(m_publicPtr, attr, value);
					return true;
				}
			}
		}
		return false;
	}

	bool ObjectInstancePrivate::setAttribute(std::string attributeName, Variant value)
	{
		ObjectClass::Attribute* attr = m_objectClass->attribute(attributeName);//"Dx"
		if (attr)
		{
			boost::mutex::scoped_lock lock(m_mutex);
			m_attributeMap[attr] = value;
			return true;
		}
		return false;
	}

	Variant ObjectInstancePrivate::attribute(std::string attributeName)
	{
		ObjectClass::Attribute* attr = m_objectClass->attribute(attributeName);
		if (attr)
		{
			return m_attributeMap.at(attr);
		}
		return Variant();
	}

	void ObjectInstancePrivate::attachAmbassador(Ambassador * ambassador)
	{
		m_ambassador = ambassador;
		m_ambassador->registerObjectInstance(m_publicPtr);
	}

	Ambassador * ObjectInstancePrivate::ambassador()
	{
		return m_ambassador;
	}

}
