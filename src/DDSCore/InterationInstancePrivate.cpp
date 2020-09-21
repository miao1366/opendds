#include "InterationInstancePrivate.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

namespace Data_Exchange_Platform
{

	bool InterationInstancePrivate::setParameterValue(std::string parameterName, Variant value)
	{
		InterationClass::Parameter* param = m_interationClass->parameter(parameterName);
		if (param/*&&value.type()==param->dataType()*/)
		{
			if (value.type() != param->dataType())
			{
				BOOST_LOG_TRIVIAL(error) << m_interationClass->interationName() << "." << parameterName << " setParameterValue error.";
			}
			m_ParameterMap[param] = value;
			return true;
		}
		return false;
	}

	Variant InterationInstancePrivate::parameterValue(std::string parameterName)
	{
		InterationClass::Parameter* param = m_interationClass->parameter(parameterName);
		if (param&&m_ParameterMap.find(param)!= m_ParameterMap.end())
		{
			return m_ParameterMap.at(param);
		}
		return Variant();
	}

}
