#pragma once

#include <ace/Global_Macros.h>

#include <dds/DdsDcpsSubscriptionExtC.h>
#include <dds/DCPS/LocalObject.h>
#include <dds/DCPS/Definitions.h>

#include <queue>
#include <boost/thread.hpp>

namespace Data_Exchange_Platform {

	class InterationInstanceImpl;
	class AmbassadorPrivate;
	class InterationClass;

	class DataExchangeReaderListener 
		: public virtual OpenDDS::DCPS::LocalObject<OpenDDS::DCPS::DataReaderListener>
	{
	public:
		DataExchangeReaderListener(AmbassadorPrivate* ambassador);

		DataExchangeReaderListener(InterationClass* interationClass, AmbassadorPrivate* amb);

		~DataExchangeReaderListener() {}

		void interrupt();

		virtual void on_requested_deadline_missed(
			DDS::DataReader_ptr reader,
			const DDS::RequestedDeadlineMissedStatus& status);

		virtual void on_requested_incompatible_qos(
			DDS::DataReader_ptr reader,
			const DDS::RequestedIncompatibleQosStatus& status);

		virtual void on_sample_rejected(
			DDS::DataReader_ptr reader,
			const DDS::SampleRejectedStatus& status);

		virtual void on_liveliness_changed(
			DDS::DataReader_ptr reader,
			const DDS::LivelinessChangedStatus& status);

		virtual void on_data_available(
			DDS::DataReader_ptr reader);

		virtual void on_subscription_matched(
			DDS::DataReader_ptr reader,
			const DDS::SubscriptionMatchedStatus& status);

		virtual void on_sample_lost(
			DDS::DataReader_ptr reader,
			const DDS::SampleLostStatus& status);

		virtual void on_subscription_disconnected (
			::DDS::DataReader_ptr reader,
			const ::OpenDDS::DCPS::SubscriptionDisconnectedStatus & status) ;

		virtual void on_subscription_reconnected (
			::DDS::DataReader_ptr reader,
			const ::OpenDDS::DCPS::SubscriptionReconnectedStatus & status) ;

		virtual void on_subscription_lost (
			::DDS::DataReader_ptr reader,
			const ::OpenDDS::DCPS::SubscriptionLostStatus & status) ;

		virtual void on_connection_deleted (
			::DDS::DataReader_ptr reader) ;

		virtual void on_budget_exceeded (
			::DDS::DataReader_ptr reader,
			const ::OpenDDS::DCPS::BudgetExceededStatus & status) ;

		void receiveInteration();

	private:
		AmbassadorPrivate* m_ambassador;
		//DDS::DataReader_ptr reader;
		//AmbassadorPrivate* m_amb;
		InterationClass* inter;
		boost::thread m_recvThread;
		boost::mutex m_mutex;
		boost::condition_variable m_condition;
		std::queue<InterationInstanceImpl*> m_queInstance;
		bool m_loop;
		int m_max;

		long long m_num_samples;
	};

}