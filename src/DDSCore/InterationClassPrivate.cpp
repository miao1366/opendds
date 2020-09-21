#include "InterationClassPrivate.h"

namespace Data_Exchange_Platform
{
	InterationClassPrivate::~InterationClassPrivate()
	{
		for (int i = 0; i < m_vecParameter.size(); i++)
		{
			delete m_vecParameter[i];
		}
	}

	InterationClass::Parameter * InterationClassPrivate::addParameter(std::string parameterName, DataType & dataType)
	{
		InterationClass::Parameter * param = new InterationClass::Parameter(parameterName, dataType);
		m_vecParameter.push_back(param);
		return param;
	}

	InterationClass::Parameter * InterationClassPrivate::parameter(std::string parameterName)
	{
		for (int i = 0; i < m_vecParameter.size(); i++)
		{
			if (m_vecParameter[i]->parameterName() == parameterName)
			{
				return m_vecParameter[i];
			}
		}
		return nullptr;
	}

}