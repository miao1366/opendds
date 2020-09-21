#include "TimerInstance.h"
#include <boost/bind.hpp>
#include "TimeAmbassador.h"

#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\uuid\uuid_generators.hpp>

namespace Data_Exchange_Platform
{

	TimerInstance::TimerInstance(std::string timerName)
		:ObjectInstanceImpl(timerName,TimerClass::timerClientClass())
	{
		setAttribute("state",0);
		setAttribute("currentTime", __int64(0));
		setAttribute("fasterthan", true);

		m_lookahead = 0;

		m_sig.connect(boost::bind(&TimerInstance::timeout,this,_1));
		m_bInit = false;

		boost::uuids::random_generator rgen;
		boost::uuids::uuid uuid = rgen();
		m_timerid = boost::lexical_cast<std::string>(uuid);
	}

	TimerInstance::~TimerInstance(void)
	{
		if (m_bInit)
		{
			m_bInit = false;
			while (!m_advanceQueue.empty())
			{
				m_advanceQueue.pop();
			}
			if (timeAmbassador())
			{
				timeAmbassador()->sendUnregisterRequest(m_timerid);
			}
		}
	}

	bool TimerInstance::isVaild()
	{
		return m_bInit;
	}

	__int64 TimerInstance::lookahead()
	{
		return m_lookahead;
	}

	void TimerInstance::setLookahead(__int64 msec )
	{
		if(msec>0)
			m_lookahead = msec;
	}

	bool TimerInstance::isRunning()
	{
		return 1==boost::get<int>(attribute("state"));
	}

	bool TimerInstance::isWaiting()
	{
		return 0==boost::get<int>(attribute("state"));
	}

	__int64 TimerInstance::currentTime()
	{
		return boost::get<__int64>(attribute("currentTime"));
	}

	void TimerInstance::timeout(__int64 msec )
	{
		
	}

	bool TimerInstance::advanceTo(__int64 msec)
	{
		if (m_bInit)
		{
			if ((m_advanceQueue.empty() && msec>currentTime())
				|| (!m_advanceQueue.empty()
					&& msec>m_advanceQueue.back()
					&& m_advanceQueue.back() - currentTime() <= m_lookahead)
				)
			{
				m_advanceQueue.push(msec);
				timeAmbassador()->sendAdvanceRequest(m_timerid,msec);
				if (msec - currentTime() < m_lookahead)
				{
					m_sig(m_advanceQueue.back());
				}

				//if (currentTime() % 1000 == 0)
				{
					std::stringstream logbuf;
					logbuf << "advance timeStamp: " << msec;
					timeAmbassador()->log(logbuf.str());
				}
				return true;

			}
		}
		return false;
	}

	bool TimerInstance::init()
	{
		if (!m_bInit)
		{
			while (!m_advanceQueue.empty())
			{
				m_advanceQueue.pop();
			}
			return timeAmbassador()->sendRegisterRequest(m_timerid);
		}
		return false;
	}

	bool TimerInstance::reset()
	{
		return timeAmbassador()->sendResetRequest();
	}

	void TimerInstance::resetedBegin()
	{
		m_mutex.lock();
		m_bInit = false;
		while (!m_advanceQueue.empty())
		{
			m_advanceQueue.pop();
		}
	}

	void TimerInstance::resetedEnd()
	{
		m_mutex.unlock();
	}

	void TimerInstance::reseted()
	{
	}

	bool TimerInstance::waitInit()
	{
		boost::mutex::scoped_lock lock(m_mutex);
		return m_condition.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(30));
	}

	__int64 TimerInstance::localTime()
	{
		if (!m_advanceQueue.empty())
		{
			return m_advanceQueue.back();
		}
		return currentTime();
	}

	std::string TimerInstance::timerid()
	{
		return m_timerid;
	}

	void TimerInstance::attributeChanged( std::string attributeName )
	{
		if (attributeName=="currentTime")
		{
			boost::mutex::scoped_lock lock(m_mutex);
			try {
				if (m_bInit==false)
				{
					m_bInit = true;
				
						//m_sig(currentTime());
						timeout(currentTime());
				
					m_condition.notify_one();
				}
				else if (m_bInit&&!m_advanceQueue.empty() && m_advanceQueue.front() <= currentTime())
				{	
					std::stringstream logbuf;
					logbuf << "advanceQueueSize:" << m_advanceQueue.size() << " front:"<< m_advanceQueue.front() 
						<<" back:" << m_advanceQueue.back() << " curtime: " << currentTime();

					if (m_advanceQueue.back() - currentTime() <= m_lookahead)
					{
						//m_sig(m_advanceQueue.back());
						timeout(m_advanceQueue.back());
						logbuf << " over";
					}
					m_advanceQueue.pop();

					timeAmbassador()->log(logbuf.str());
				}
			}
			catch (...)
			{
				timeAmbassador()->log("timeout error");
			}
		}

	}

	TimeAmbassador * TimerInstance::timeAmbassador()
	{
		return dynamic_cast<TimeAmbassador*>(ObjectInstanceImpl::ambassador());
	}

	void TimerInstance::setUser(std::string user, std::string test)
	{
		m_userName = user;
		m_testId = test;
	}

	std::string TimerInstance::userName(void)
	{
		return m_userName;
	}
	std::string TimerInstance::testId(void)
	{
		return m_testId;
	}
	void TimerInstance::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{

	}
}
