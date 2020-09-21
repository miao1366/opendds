#include "InterationClass.h"
#include "InterationClassPrivate.h"

namespace Data_Exchange_Platform
{

	InterationClass::InterationClass(std::string interationName, ExchangeType exchangeType)
		:m_private(new InterationClassPrivate(interationName, exchangeType))
	{
	}

	InterationClass::~InterationClass()
	{
		delete m_private;
	}

	std::string InterationClass::interationName()
	{
		return m_private->interationName();
	}

	ExchangeType InterationClass::exchangeType()
	{
		return m_private->exchangeType();
	}

	InterationClass::Parameter * InterationClass::addParameter(std::string parameterName, DataType& dataType)
	{
		return m_private->addParameter(parameterName, dataType);
	}

	InterationClass::Parameter * InterationClass::parameter(std::string parameterName)
	{
		return m_private->parameter(parameterName);
	}

	InterationClass::Parameter * InterationClass::parameterAt(int index)
	{
		return m_private->parameterAt(index);
	}

	int InterationClass::parameterCount()
	{
		return m_private->parameterCount();
	}

	bool InterationClass::persistent()
	{
		return m_private->persistent();
	}

	void InterationClass::setPersistent(bool persist)
	{
		m_private->setPersistent(persist);
	}

}