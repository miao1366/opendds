#pragma once

#include "ObjectClass.h"
#include <vector>

namespace Data_Exchange_Platform
{

	class ObjectClassPrivate
	{
	public:
		ObjectClassPrivate(std::string objectName, ObjectClass* publicPtr)
			:m_objectName(objectName) , m_publicPtr(publicPtr){}
		~ObjectClassPrivate();

		std::string objectName() { return m_objectName; }

		ObjectClass::Attribute* addAttribute(std::string attributeName, DataType& dataType, ExchangeType exchangeType);
		ObjectClass::Attribute* attribute(std::string attributeName);

		ObjectClass::Attribute* attributeAt(int index) { return m_vecAttribute.at(index); }
		int attrbuteCount() { return m_vecAttribute.size(); }

	private:
		std::string m_objectName;
		ObjectClass* m_publicPtr;
		std::vector<ObjectClass::Attribute*> m_vecAttribute;
	};

}

