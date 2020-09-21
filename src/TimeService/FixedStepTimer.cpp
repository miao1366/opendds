#include "FixedStepTimer.h"
#include "TimeAmbassador.h"

namespace Data_Exchange_Platform
{

	FixedStepTimer::FixedStepTimer(__int64 interval, __int64 lookahead)
		:m_interval(interval)
	{
		setLookahead(lookahead);
	}


	FixedStepTimer::~FixedStepTimer()
	{
	}

	__int64 FixedStepTimer::interval()
	{
		return m_interval;
	}

	void FixedStepTimer::setInterval(__int64 n)
	{
		m_interval = n;
	}

	void FixedStepTimer::timeout(__int64 msec)
	{
		TimerInstance::timeout(msec);
		//if (currentTime() % 1000 == 0 || currentTime() % 1000 == m_interval)
		{
			std::stringstream logbuf;
			logbuf << "timeout timeStamp: " << msec;
			timeAmbassador()->log(logbuf.str());
		}
	}

	bool FixedStepTimer::advance()
	{
		return advanceTo(localTime()+ interval());
	}

}