#include "DataExchangeReaderListener.h"
#include "AmbassadorPrivate.h"
#include "DataExchangeTypeSupportC.h"
#include "DataExchangeTypeSupportImpl.h"
#include "InterationClass.h"
#include "InterationInstanceImpl.h"
#include "ObjectClass.h"
#include "ObjectInstanceImpl.h"
#include <iostream>
#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/variant.hpp>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <stdlib.h>
#include <exception>

 #include <boost/log/core.hpp>
 #include <boost/log/trivial.hpp>
 //#include <boost/log/expressions.hpp>
 //#include <boost/log/sinks/text_file_backend.hpp>
 //#include <boost/log/utility/setup/file.hpp>
 //#include <boost/log/utility/setup/common_attributes.hpp>
 //#include <boost/log/sources/severity_logger.hpp>
 //#include <boost/log/sources/record_ostream.hpp>


#include "AmbassadorPrivate.h"
#include "Ambassador.h"
#include "InterationClass.h"
#include "ObjectInstanceImpl.h"
#include "DataExchangeReaderListener.h"

#include <dds\DCPS\Time_helper.h>
 
 //namespace logging = boost::log;
 //namespace src = boost::log::sources;
 //namespace sinks = boost::log::sinks;
 //namespace keywords = boost::log::keywords;

using namespace std;

namespace Data_Exchange_Platform {

	DataExchangeReaderListener::DataExchangeReaderListener(AmbassadorPrivate* ambassador)
		:m_ambassador(ambassador), m_recvThread(boost::bind(&DataExchangeReaderListener::receiveInteration, this)), m_loop(true), m_max(100), m_num_samples(0)
	{
		inter = NULL;
	}

	DataExchangeReaderListener::DataExchangeReaderListener(InterationClass* interationClass, AmbassadorPrivate* amb)
		:inter(interationClass), m_ambassador(amb), m_recvThread(boost::bind(&DataExchangeReaderListener::receiveInteration, this)), m_loop(true), m_num_samples(0)
	{
	}

	void DataExchangeReaderListener::interrupt()
	{
		m_loop = false;
		m_recvThread.interrupt();
		m_recvThread.join();
	}

	void DataExchangeReaderListener::on_requested_deadline_missed(DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_requested_deadline_missed" << endl;
	}

	void DataExchangeReaderListener::on_requested_incompatible_qos(DDS::DataReader_ptr reader, const DDS::RequestedIncompatibleQosStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_requested_incompatible_qos" << endl;
	}

	void DataExchangeReaderListener::on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_sample_rejected" << endl;
	}

	void DataExchangeReaderListener::on_liveliness_changed(DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_liveliness_changed" << endl;
	}

	void DataExchangeReaderListener::on_data_available(DDS::DataReader_ptr reader)
	{

		DDS::SampleInfo info;
		try{
			if (m_ambassador->m_attrDataListenerMap.find(this) != m_ambassador->m_attrDataListenerMap.end())
			{
				ObjectClass::Attribute* attr = m_ambassador->m_attrDataListenerMap.at(this);
				ObjectAttributeDataReader_var attrReader =
					ObjectAttributeDataReader::_narrow(reader);
				if (!attrReader) {
					return;
				}
				ObjectAttribute objattr;

				DDS::ReturnCode_t error = attrReader->take_next_sample(objattr, info);
				
				if (error != DDS::RETCODE_OK) {
					return;
				}

				Variant value;
				std::istringstream is(std::string(objattr.attributeValue.get_buffer(), objattr.attributeValue.length()));
				boost::archive::binary_iarchive ia(is, boost::archive::no_header);
				ia >> value;
				ObjectInstanceImpl* instance = m_ambassador->objectInstance(objattr.instanceName.in(), objattr.className.in());

				if (instance)
				{
					try
					{
						BOOST_LOG_TRIVIAL(trace) << "recv obj beg" << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";
						m_ambassador->receiveObjectAttribute(instance, attr, value);
						BOOST_LOG_TRIVIAL(trace) << "recv obj end" << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";
					}
					catch (...)
					{
						BOOST_LOG_TRIVIAL(error) << objattr.className << "." <<attr->attributeName() << "对象类处理异常" << endl;
						return;
					}

				}
				
			}

			if (m_ambassador->m_interDataListenerMap.find(this) != m_ambassador->m_interDataListenerMap.end())
			{
				InterationClass* interClass = m_ambassador->m_interDataListenerMap.at(this);
				InterationsDataReader_var interReader =
					InterationsDataReader::_narrow(reader);
				if (!interReader) {
					return;
				}
				Interations interlst;
				
				DDS::ReturnCode_t error = interReader->take_next_sample(interlst, info);

				if (error != DDS::RETCODE_OK) {
					return ;
				}
		
					BOOST_LOG_TRIVIAL(trace) << "recv beg " << interClass->interationName() << " timeStamp: " << interlst.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

				std::istringstream is;
				boost::archive::binary_iarchive ia(is, boost::archive::no_header);

				for (int i = 0; i < interlst.interationlst.length(); i++)
				{
					Interation& inter = interlst.interationlst[i];

					InterationInstanceImpl* instance = new InterationInstanceImpl(interClass);
					instance->setPubHandle(info.publication_handle);

					try {
						instance->setTimeStamp(inter.timeStamp);					

						for (size_t i = 0; i < inter.parameterList.length()/*+1*/; i++)
						{
							Variant value;
							is.str(std::string(inter.parameterList[i].parameterValue.get_buffer(), inter.parameterList[i].parameterValue.length()));
							ia >> value;
							instance->setParameterValue(inter.parameterList[i].parameterName.in(), value);
							is.clear();

						}

					}
					catch (...)
					{
						BOOST_LOG_TRIVIAL(error) << "接收序列化异常" << endl;
						return;
					}


					if (!interClass->persistent())
					{

							BOOST_LOG_TRIVIAL(trace) << "mutex beg " << interClass->interationName() << " timeStamp: " << inter.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

						boost::mutex::scoped_lock lock(m_mutex);
						if (!m_queInstance.empty())
						{
							while (m_ambassador->pushCheckInterationQueue(m_queInstance))
							{
								delete m_queInstance.front();
								m_queInstance.pop();
							}
						}
						m_queInstance.push(instance);
						m_condition.notify_one();

							BOOST_LOG_TRIVIAL(trace) << "mutex end " << interClass->interationName() << " timeStamp: " << inter.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

					}
					else
					{

							BOOST_LOG_TRIVIAL(trace) << "cu_op beg " << interClass->interationName() << " timeStamp: " << inter.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

						m_ambassador->receiveInterationInstance(instance);
						delete instance;

							BOOST_LOG_TRIVIAL(trace) << "cu_op end " << interClass->interationName() << " timeStamp: " << inter.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

					}
				}


					BOOST_LOG_TRIVIAL(trace) << "recv end " << interClass->interationName() << " timeStamp: " << interlst.timeStamp << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

				
			}
		}
		catch(...)
		{
			BOOST_LOG_TRIVIAL(error) << "接收函数on_data_available异常" << endl;
			return;
		}

	}

	void DataExchangeReaderListener::on_subscription_matched(DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_subscription_matched" << endl;
	}

	void DataExchangeReaderListener::on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_sample_lost" << endl;
	}

	void DataExchangeReaderListener::receiveInteration()
	{	
		try
		{
			std::string className;
			if (m_ambassador->m_interDataListenerMap.find(this) != m_ambassador->m_interDataListenerMap.end())
			{
				InterationClass* interClass = m_ambassador->m_interDataListenerMap.at(this);
				className = interClass->interationName();
			}
			while(this->m_loop)
			{	
				boost::mutex::scoped_lock lock(this->m_mutex);
				InterationInstanceImpl* instance = nullptr;
				try
				{
					if (!this->m_queInstance.empty())
					{
						if (this->m_ambassador->popCheckInterationQueue(this->m_queInstance))
						{

							instance = this->m_queInstance.front();

								BOOST_LOG_TRIVIAL(trace) << "th_op beg " << instance->interationClass()->interationName() << " timeStamp: " << instance->timeStamp() << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

							this->m_queInstance.pop();

							this->m_ambassador->receiveInterationInstance(instance);

								BOOST_LOG_TRIVIAL(trace) << "th_op end " << instance->interationClass()->interationName() << " timeStamp: " << instance->timeStamp() << " runtime:" << std::setprecision(16) << Ambassador::runtime() << "ms";

							delete instance;
	
						}
						else
						{
							this->m_condition.wait(lock);
						}
						
					}
					else
					{
						this->m_condition.wait(lock);
					}
				}
				catch (...)
				{
					BOOST_LOG_TRIVIAL(error) << className << " 接收交互类处理函数异常" << endl;
					//if (instance)
					//{
					//	delete instance;
					//}
					//this->m_condition.wait(lock);
				}
			}
		}
		catch (...)
		{
			BOOST_LOG_TRIVIAL(error) << "接收交互类线程异常" << endl;
		}
		
	}

	void DataExchangeReaderListener::on_subscription_disconnected(::DDS::DataReader_ptr reader, const ::OpenDDS::DCPS::SubscriptionDisconnectedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_subscription_disconnected" << endl;
	}

	void DataExchangeReaderListener::on_subscription_reconnected(::DDS::DataReader_ptr reader, const ::OpenDDS::DCPS::SubscriptionReconnectedStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_subscription_reconnected" << endl;
	}

	void DataExchangeReaderListener::on_subscription_lost(::DDS::DataReader_ptr reader, const ::OpenDDS::DCPS::SubscriptionLostStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << "reader on_subscription_lost" << endl;
		try
		{
			CORBA::ULong len = status.publication_handles.length();
			for (CORBA::ULong i = 0; i < len; ++i)
			{
				BOOST_LOG_TRIVIAL(info) << "publication handles:" << status.publication_handles[i] << endl;
				if (m_loop&&m_ambassador->m_interDataListenerMap.find(this) != m_ambassador->m_interDataListenerMap.end())
				{
					InterationClass* interClass = m_ambassador->m_interDataListenerMap.at(this);
					m_ambassador->subscriptionLost(interClass->interationName(), status.publication_handles[i]);
				}
				
			}
		}
		catch (...)
		{
			BOOST_LOG_TRIVIAL(error) << "subscription lost 异常" << endl;
		}
	}

	void DataExchangeReaderListener::on_connection_deleted(::DDS::DataReader_ptr reader)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_connection_deleted" << endl;
		
	}

	void DataExchangeReaderListener::on_budget_exceeded(::DDS::DataReader_ptr reader, const ::OpenDDS::DCPS::BudgetExceededStatus & status)
	{
		BOOST_LOG_TRIVIAL(info) << reader->get_topicdescription()->get_name() << " reader on_budget_exceeded" << endl;
	}

}
