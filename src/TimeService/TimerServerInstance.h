#pragma once

#include <ObjectInstanceImpl.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "TimerClass.h"
#include <queue>
#include <map>

namespace Data_Exchange_Platform
{

	class DATA_EXCHANGE_PLATFORM_EXPORT TimerServerInstance : public ObjectInstanceImpl
	{
	public:
		TimerServerInstance(std::string timerName="DEPTimer");
		~TimerServerInstance(void);

		bool registerClient(std::string id);
		bool unregisterClient(std::string id);
		bool advanceTo(std::string id, __int64 msec);

		void reset();

		__int64 currentTime();

		enum State{Wait = 0,Run};

		State getState();

		bool isFasterthan();

		void playOperation(int);

	private:

		std::map<std::string, std::queue<__int64>> m_advanceQueue;

		void setState(State state);
		bool advanceQueue();

		void timeout();

		boost::asio::io_service m_io;
		boost::asio::deadline_timer m_timer;

		boost::thread* m_thread;

		__int64 m_nextTime;
		boost::mutex m_timerMutex;
		boost::condition_variable m_timerCondition;

		bool						m_isPause;
		boost::mutex				m_playMutex;
		boost::condition_variable	m_playCondition;
		double						m_speed;
	};

}

