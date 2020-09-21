#pragma once

#include <ObjectInstanceImpl.h>
#include "TimerClass.h"
#include <queue>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>

namespace Data_Exchange_Platform
{
	class TimeAmbassador;

	class DATA_EXCHANGE_PLATFORM_EXPORT TimerInstance : public ObjectInstanceImpl
	{
	public:
		TimerInstance(std::string timerName="DEPTimer");
		~TimerInstance(void);

		bool isVaild();

		__int64 lookahead();

		void setLookahead(__int64 msec);

		bool isRunning();

		bool isWaiting();

		__int64 currentTime();

		__int64 localTime();

		virtual void timeout(__int64 msec );

		bool advanceTo(__int64 msec);

		bool init();

		bool reset();

		void resetedBegin();
		void resetedEnd();

		virtual void reseted();

		bool waitInit();

		std::string timerid();
		virtual void receiveInterationInstance(InterationInstanceImpl * interationInstance);
		void setUser(std::string, std::string);
		std::string userName(void);
		std::string testId(void);
	protected:
		virtual void attributeChanged(std::string attributeName);

		TimeAmbassador* timeAmbassador();

		__int64 m_lookahead;
		std::queue<__int64> m_advanceQueue;

		boost::signals2::signal<void (__int64)> m_sig;

		bool m_bInit;

		std::string m_timerid;

		boost::mutex m_mutex;
		boost::condition_variable m_condition;

		std::string  m_userName;
		std::string  m_testId;

	};

}

