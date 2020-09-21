#pragma once

#include "InterationClass.h"
#include <vector>

namespace Data_Exchange_Platform
{

	class InterationClassPrivate
	{
	public:
		InterationClassPrivate(std::string interationName, ExchangeType exchangeType)
			:m_interationName(interationName), m_exchangeType(exchangeType),m_persistent(false){}
		~InterationClassPrivate();

		std::string interationName() { return m_interationName; }
		ExchangeType exchangeType() { return m_exchangeType; }

		InterationClass::Parameter* addParameter(std::string parameterName, DataType& dataType);
		InterationClass::Parameter* parameter(std::string parameterName);

		InterationClass::Parameter* parameterAt(int index) { return m_vecParameter.at(index); }
		int parameterCount() { return m_vecParameter.size(); }

		bool persistent() {return m_persistent;}
		void setPersistent(bool persist) {m_persistent=persist;}

	private:
		std::string m_interationName;
		ExchangeType m_exchangeType;
		std::vector<InterationClass::Parameter*> m_vecParameter;

		bool m_persistent;
	};

}

