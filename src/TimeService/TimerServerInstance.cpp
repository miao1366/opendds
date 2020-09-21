#include "TimerServerInstance.h"
#include "TimerClass.h"
#include <boost/bind.hpp>

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 

#include "TimeAmbassador.h"

namespace Data_Exchange_Platform
{

	TimerServerInstance::TimerServerInstance(std::string timerName)
		:ObjectInstanceImpl(timerName,TimerClass::timerServerClass()),
		m_timer(m_io)
	{
		setAttribute("currentTime", __int64(0));

		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("./rtps.ini", pt);

		int fasterthan = pt.get<int>("TIMESERVICE.Fasterthan", 1);
		setAttribute("fasterthan", fasterthan!=0);

		setState(Wait);
		m_nextTime = 0;
		m_thread = nullptr;
		m_isPause = false;
		m_speed = 1.0;
	}

	TimerServerInstance::~TimerServerInstance(void)
	{
		if (m_thread)
		{
			m_thread->interrupt();
			m_thread->join();
			delete m_thread;
			m_thread = nullptr;
		}
	}

	bool TimerServerInstance::registerClient(std::string id)
	{
		boost::mutex::scoped_lock tlock(m_timerMutex);
		if (m_advanceQueue.find(id) == m_advanceQueue.end())
		{
			std::queue<__int64> advque;
			advque.push(currentTime());
			m_advanceQueue.insert(std::pair<std::string, std::queue<__int64>>(id, advque));
			return true;
		}
		return false;
	}

	bool TimerServerInstance::unregisterClient(std::string id)
	{
		boost::mutex::scoped_lock tlock(m_timerMutex);
		if (m_advanceQueue.erase(id) > 0)
		{
			if (advanceQueue())
			{
				m_timerCondition.notify_one();
			}
			return true;
		}
		return false;
	}

	bool TimerServerInstance::advanceTo(std::string id, __int64 msec)
	{
		boost::mutex::scoped_lock tlock(m_timerMutex);

		if (m_advanceQueue.empty())
		{
			return false;
		}
		if ((m_advanceQueue[id].empty()&& msec>currentTime())
			|| (!m_advanceQueue[id].empty() && msec > m_advanceQueue[id].back()))
		{
			m_advanceQueue[id].push(msec);
			if(advanceQueue())
			{
				m_timerCondition.notify_one();
			}
			return true;
		}
		return false;
	}

	void TimerServerInstance::reset()
	{
		setAttribute("currentTime", __int64(0));
		setState(Wait);
		m_nextTime = 0;
		m_timer.cancel();

		std::map<std::string, std::queue<boost::int64_t>>::iterator it = m_advanceQueue.begin();

		for (; it != m_advanceQueue.end(); it++)
		{
			while (!it->second.empty())
			{
				it->second.pop();
			}
		}
	}

	boost::int64_t TimerServerInstance::currentTime()
	{
		return boost::get<boost::int64_t>(attribute("currentTime"));
	}

	TimerServerInstance::State TimerServerInstance::getState()
	{
		return State(boost::get<int>(attribute("state")));
	}

	bool TimerServerInstance::isFasterthan()
	{
		return boost::get<bool>(attribute("fasterthan"));
	}

	void TimerServerInstance::playOperation(int operation)
	{
		if (operation == 0)
		{
			m_isPause = true;
		}
		if (operation == 1)
		{
			m_playCondition.notify_one();
		}
		if (operation == 3)
		{
			m_speed += 0.25;
		}
		if (operation == 4)
		{
			m_speed -= 0.25;
		}
	}

	void TimerServerInstance::setState(State state)
	{
		setAttribute("state", int(state));
	}

	bool TimerServerInstance::advanceQueue()
	{
		if (getState() == Wait)
		{
			std::map<std::string, std::queue<boost::int64_t>>::iterator it = m_advanceQueue.begin();
			int count = 0;
			boost::int64_t nextTime = 0;
			for (; it != m_advanceQueue.end(); it++)
			{
				while (!it->second.empty() && it->second.front() <= currentTime())
				{
					it->second.pop();
				}
				if (!it->second.empty() && it->second.front() > currentTime())
				{
					count++;
					if (it == m_advanceQueue.begin())
					{
						nextTime = it->second.front();
					}
					else
					{
						nextTime = std::min(nextTime, it->second.front());
					}
				}
				
			}
			if (m_advanceQueue.size() == count)
			{
				if (m_isPause)
				{
					boost::mutex::scoped_lock lock(m_playMutex);
					m_playCondition.wait(lock);
					m_isPause = false;
				}
				m_nextTime = nextTime;			
					
				if (isFasterthan())
				{
					if (m_thread)
					{
						m_thread->interrupt();
						m_thread->join();
						delete m_thread;
						m_thread = nullptr;
					}
					timeout();
				}
				else
				{
					if (m_thread == nullptr)
					{
						m_timer.expires_from_now((boost::posix_time::milliseconds(m_nextTime - currentTime()))*m_speed);
						m_timer.async_wait(boost::bind(&TimerServerInstance::timeout, this));
						m_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
					}
					else
					{
						m_timer.expires_at(m_timer.expires_at() + (boost::posix_time::milliseconds(m_nextTime - currentTime()))*m_speed);
						m_timer.async_wait(boost::bind(&TimerServerInstance::timeout, this));
					}
					setState(Run);
				}

				return true;
			}
			else
			{
				
				return false;
			}
		}
		return false;
	}

	void TimerServerInstance::timeout()
	{
		{
			std::stringstream logbuf;
			logbuf << "timeout: " << m_nextTime;
			ambassador()->log(logbuf.str());
		}
		if (!isFasterthan())
		{
			boost::mutex::scoped_lock lock(m_timerMutex);
			setAttribute("currentTime", m_nextTime);
			setState(Wait);
			updateAttribute();
			if (!advanceQueue())
			{
				m_timerCondition.wait(lock);
			}
		}
		else
		{
			setAttribute("currentTime", m_nextTime);
			updateAttribute();
			advanceQueue();
		}
	}

}