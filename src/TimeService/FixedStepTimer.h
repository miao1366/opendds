#pragma once

#include "TimerInstance.h"

namespace Data_Exchange_Platform
{
	class DATA_EXCHANGE_PLATFORM_EXPORT FixedStepTimer : public TimerInstance
	{
	public:
		FixedStepTimer(__int64 interval, __int64 lookahead=0);
		~FixedStepTimer();

		__int64 interval();
		void setInterval(__int64 n);

		virtual void timeout(__int64 msec);

		bool advance();

	private:
		__int64 m_interval;
	};
}

