#pragma once

#include "common.h"
#include "ObjectInstanceImpl.h"
#include <map>
#include <boost/thread.hpp>

namespace Data_Exchange_Platform
{

	class ObjectInstancePrivate
	{
	public:
		ObjectInstancePrivate(std::string instanceName, ObjectClass* objectClass, ObjectInstanceImpl* publicPtr)
			:m_instanceName(instanceName), m_objectClass(objectClass), m_publicPtr(publicPtr),m_ambassador(nullptr){}

		bool updateAttribute(std::string attributeName = "");

		bool setAttribute(std::string attributeName, Variant value);

		Variant attribute(std::string attributeName);

		std::string instanceName() { return m_instanceName; }
		ObjectClass* objectClass() { return m_objectClass; }

		void attachAmbassador(Ambassador* ambassador);

		Ambassador* ambassador();

	private:
		std::string m_instanceName;
		ObjectClass* m_objectClass;
		Ambassador* m_ambassador;
		ObjectInstanceImpl* m_publicPtr;
		std::map<ObjectClass::Attribute*, Variant> m_attributeMap;
		boost::mutex m_mutex;
	};

}