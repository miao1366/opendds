#pragma once

#include "common.h"
#include "InterationClass.h"
#include <map>
#include <time.h>

namespace Data_Exchange_Platform
{

	class InterationInstancePrivate
	{
	public:
		InterationInstancePrivate(InterationClass* interationClass)
			:m_interationClass(interationClass), m_pubHandle(0), m_timeStamp(0){

		}

		bool setParameterValue(std::string parameterName, Variant value);

		Variant parameterValue(std::string parameterName);

		InterationClass* interationClass() { return m_interationClass; }

		void setTimeStamp(long long timestamp) {m_timeStamp = timestamp;}
		long long timeStamp() {return m_timeStamp;} 

		void setPubHandle(long handle) { m_pubHandle = handle; }
		long pubHandle() { return m_pubHandle; }

	private:
		InterationClass* m_interationClass;
		std::map<InterationClass::Parameter*, Variant> m_ParameterMap;

		time_t m_timeStamp;
		long m_pubHandle;
	};

}