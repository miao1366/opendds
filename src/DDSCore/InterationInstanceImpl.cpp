#include "InterationInstanceImpl.h"
#include "InterationInstancePrivate.h"

namespace Data_Exchange_Platform
{

	InterationInstanceImpl::InterationInstanceImpl(InterationClass * interationClass)
		:m_private(new InterationInstancePrivate(interationClass))
	{
	}

	InterationInstanceImpl::~InterationInstanceImpl()
	{
		delete m_private;
	}

	bool InterationInstanceImpl::setParameterValue(std::string parameterName, Variant value)
	{
		return m_private->setParameterValue(parameterName,value);
	}

	Variant InterationInstanceImpl::parameterValue(std::string parameterName)
	{
		return m_private->parameterValue(parameterName);
	}

	InterationClass * InterationInstanceImpl::interationClass()
	{
		return m_private->interationClass();
	}

	void InterationInstanceImpl::setTimeStamp(long long timestamp)
	{
		m_private->setTimeStamp(timestamp);
	}

	long long InterationInstanceImpl::timeStamp()
	{
		return m_private->timeStamp();
	}

	void InterationInstanceImpl::setPubHandle(long handle)
	{
		m_private->setPubHandle(handle);
	}

	long InterationInstanceImpl::pubHandle()
	{
		return m_private->pubHandle();
	}

}
