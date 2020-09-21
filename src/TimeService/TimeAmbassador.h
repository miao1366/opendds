#pragma once

#include <common.h>
#include <Ambassador.h>
#include <boost/thread.hpp>

namespace Data_Exchange_Platform
{
	class TimerInstance;
	

	class DATA_EXCHANGE_PLATFORM_EXPORT TimeAmbassador : public Ambassador
	{
	public:	
		TimeAmbassador(long domain_ID);
		
		~TimeAmbassador(void);

		void registerTimer(TimerInstance* timer);

		void sendAdvanceRequest(std::string timerid,__int64 msec);

		bool sendRegisterRequest(std::string timerid);

		void sendUnregisterRequest(std::string timerid);

		bool sendResetRequest();

		bool sendInterationInstanceByTime(InterationInstanceImpl * interationInstance);

		bool sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance);

		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);

		virtual bool popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue);

	private:
		
		void resetedProc(InterationInstanceImpl* inst);

		TimerInstance* m_timer;

		boost::mutex m_mutex;
		boost::condition_variable m_condition;

	};

}
