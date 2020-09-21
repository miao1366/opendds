#include "AmbassadorPrivate.h"
#include "Ambassador.h"
#include "InterationClass.h"
#include "ObjectInstanceImpl.h"
#include "DataExchangeReaderListener.h"

#include <dds/DdsDcpsSubscriptionC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>
#include <dds/DCPS/WaitSet.h>

#include <sstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/variant.hpp>

#include <dds/DCPS/RTPS/RtpsDiscovery.h>
#include <dds/DCPS/transport/framework/TransportRegistry.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdpInst.h>
#include <dds/DCPS/transport/rtps_udp/RtpsUdpInst_rch.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <windows.h>
#include <fstream>
#include <stdlib.h>
#include <exception>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/ini_parser.hpp> 

#include "DataWriterListenerImpl.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

using namespace std;

namespace Data_Exchange_Platform {

	AmbassadorPrivate::AmbassadorPrivate(long domain_ID, Ambassador * publicPtr)
		:m_publicPtr(publicPtr), m_interationQueueMax(100), m_threadpool(32)
	{
		createParticipant(domain_ID);
		createPublisher();
		createSubscriber();
	}

	AmbassadorPrivate::~AmbassadorPrivate()
	{
		m_threadpool.wait();
		m_threadpool.clear();
		cleanup();
	}

	bool AmbassadorPrivate::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		if (this->objectInstance(objectInstance->instanceName(), objectInstance->objectClass()->objectName()))
		{
			return false;
		}
		if (!this->objectClass(objectInstance->objectClass()->objectName()))
		{
			registerObjectClass(objectInstance->objectClass());
		}
		m_vecObjInst.push_back(objectInstance);
		objectInstance->attachAmbassador(m_publicPtr);
		return true;
	}

	bool AmbassadorPrivate::registerObjectClass(ObjectClass * objectClass)
	{
		if (this->objectClass(objectClass->objectName()))
		{
			return false;
		}	
		for (int i = 0; i < objectClass->attrbuteCount(); i++)
		{
			declareObjectAttribute(objectClass->attributeAt(i));
		}
		m_vecObjCls.push_back(objectClass);
		return true;
	}

	ObjectClass * AmbassadorPrivate::objectClass(std::string objectName)
	{
		for (int i = 0; i < m_vecObjCls.size(); i++)
		{
			if(m_vecObjCls[i]->objectName()== objectName)
			{
				return m_vecObjCls[i];
			}
		}
		return nullptr;
	}

	ObjectInstanceImpl * AmbassadorPrivate::objectInstance(std::string instanceName, std::string objectName)
	{
		for (int i = 0; i < m_vecObjInst.size(); i++)
		{
			if (m_vecObjInst[i]->instanceName() == instanceName &&
				m_vecObjInst[i]->objectClass()->objectName() == objectName)
			{
				return m_vecObjInst[i];
			}
		}
		return nullptr;
	}

	bool AmbassadorPrivate::declareObjectAttribute(ObjectClass::Attribute * attribute)
	{
		DDS::Topic_var topic = createTopic(attribute);
		if (!topic)
		{
			return false;
		}
		if (attribute->exchangeType()&Publish_Only)
		{
			DDS::DataWriterListener_var listner(new DataWriterListenerImpl);
			DDS::DataWriter_var writer =
				m_publisher->create_datawriter(topic,
					DATAWRITER_QOS_DEFAULT,
					listner,
					OpenDDS::DCPS::DEFAULT_STATUS_MASK);
			if (!writer)
			{
				return false;
			}
			ObjectAttributeDataWriter_var attrWriter = ObjectAttributeDataWriter::_narrow(writer);
			if (!attrWriter)
			{
				return false;
			}
			m_attrDataWriterMap[attribute] = attrWriter;

			m_vecDataWriter.push_back(writer);

		}
		if (attribute->exchangeType()&Subscribe_Only)
		{
			DataExchangeReaderListener* listener = new DataExchangeReaderListener(this);

			DDS::DataReader_var reader =
				m_subscriber->create_datareader(topic,
					DATAREADER_QOS_DEFAULT,
					listener,
					OpenDDS::DCPS::DEFAULT_STATUS_MASK);

			if (!reader) {
				return false;
			}

			// To start collecting new data.
			dynamic_cast<OpenDDS::DCPS::DataReaderImpl*>(reader.in())->
				reset_latency_stats();
			dynamic_cast<OpenDDS::DCPS::DataReaderImpl*>(reader.in())->
				statistics_enabled(true);

			ObjectAttributeDataReader_var attrReader = ObjectAttributeDataReader::_narrow(reader);

			if (!attrReader) {
				return false;
			}

			m_attrDataListenerMap[listener] = attribute;

			m_vecListener.push_back(listener);
			m_vecDataReader.push_back(reader);
			
		}
		return true;
	}

	bool AmbassadorPrivate::sendObjectAttribute(ObjectInstanceImpl * objectInstance, ObjectClass::Attribute * attribute, Variant value)
	{
		ObjectAttributeDataWriter_var attrWriter = m_attrDataWriterMap.at(attribute);
		if (attrWriter)
		{
			ObjectAttribute attr;
			attr.instanceName = objectInstance->instanceName().c_str();
			attr.className = attribute->objectClass()->objectName().c_str();
			attr.attributeName = attribute->attributeName().c_str();
			attr.attributeType = attribute->dataType().name();

			std::ostringstream os;
			boost::archive::binary_oarchive oa(os, boost::archive::no_header);
			oa << value;
			//attr.attributeValue = os.str().c_str();

			int len = os.str().size();
			CORBA::Char* charArray = CORBA::CharSeq::allocbuf(len/sizeof(CORBA::Char));
			//CORBA::Char* charArray = new CORBA::Char[len / sizeof(CORBA::Char)];
			memset(charArray, 0, len);
			memcpy(charArray, os.str().c_str(), len);
			attr.attributeValue.replace(len, len, charArray,true);

			DDS::ReturnCode_t error = attrWriter->write(attr, DDS::HANDLE_NIL);
			if (error != DDS::RETCODE_OK) {
				return false;
			}
			return true;
		}
		return false;
	}

	void AmbassadorPrivate::receiveObjectAttribute(ObjectInstanceImpl * objectInstance, ObjectClass::Attribute * attribute, Variant value)
	{
		objectInstance->setAttribute(attribute->attributeName(), value);
		m_publicPtr->receiveObjectAttribute(objectInstance, attribute, value);
		objectInstance->attributeChanged(attribute->attributeName());
	}

	bool AmbassadorPrivate::declareInterationClass(InterationClass * interationClass)
	{
		if (this->interationClass(interationClass->interationName()))
		{
			return false;
		}
		DDS::Topic_var topic = createTopic(interationClass);
		if (!topic)
		{
			return false;
		}
		if (interationClass->exchangeType()&Publish_Only)
		{
			DDS::DataWriterListener_var listner(new DataWriterListenerImpl(interationClass,this));
			DDS::DataWriter_var writer =
				m_publisher->create_datawriter(topic,
					DATAWRITER_QOS_DEFAULT,
					listner,
					OpenDDS::DCPS::DEFAULT_STATUS_MASK);
			if (!writer)
			{
				std::cout << "create_datawriter failed:" << topicName(interationClass) << std::endl;
				return false;
			}
			InterationsDataWriter_var interWriter = InterationsDataWriter::_narrow(writer);
			if (!interWriter)
			{
				std::cout << "create_datawriter failed:" << topicName(interationClass) << std::endl;
				return false;
			}
			m_interDataWriterMap[interationClass] = interWriter;
			m_vecDataWriter.push_back(writer);

		}

		if (interationClass->exchangeType()&Subscribe_Only)
		{
			DataExchangeReaderListener* listener = new DataExchangeReaderListener(interationClass, this);

			DDS::DataReader_var reader =
				m_subscriber->create_datareader(topic,
					DATAREADER_QOS_DEFAULT,
					listener,
					OpenDDS::DCPS::DEFAULT_STATUS_MASK);

			if (!reader) {
				return false;
			}

			// To start collecting new data.
			dynamic_cast<OpenDDS::DCPS::DataReaderImpl*>(reader.in())->
				reset_latency_stats();
			dynamic_cast<OpenDDS::DCPS::DataReaderImpl*>(reader.in())->
				statistics_enabled(true);

			InterationsDataReader_var interReader = InterationsDataReader::_narrow(reader);

			if (!interReader) {
				return false;
			}

			m_interDataListenerMap[listener] = interationClass;
			m_interDataReaderMap[interationClass] = reader;

			m_vecListener.push_back(listener);
			m_vecDataReader.push_back(reader);
		}
		m_vecInterCls.push_back(interationClass);
		return true;
	}

	InterationClass * AmbassadorPrivate::interationClass(std::string interationName)
	{
		for (int i = 0; i < m_vecInterCls.size(); i++)
		{
			if (m_vecInterCls[i]->interationName() == interationName)
			{
				return m_vecInterCls[i];
			}
		}
		return nullptr;
	}

	bool AmbassadorPrivate::sendInterationInstance(std::vector<InterationInstanceImpl*> vec_interationInstance , bool bwait)
	{
		if (bwait)
		{
			return sendInterationInstanceProc(vec_interationInstance);
		}
		else
		{
			try {
				std::vector<InterationInstanceImpl*> vec_interationInstance_copy;
				for (int i = 0; i < vec_interationInstance.size(); i++)
				{
					InterationInstanceImpl* interationInstance = vec_interationInstance[i];
					InterationInstanceImpl* instanceCopy = new InterationInstanceImpl(interationInstance->interationClass());
					instanceCopy->setTimeStamp(interationInstance->timeStamp());
					for (int i = 0; i < interationInstance->interationClass()->parameterCount(); i++)
					{
						std::string parameterName = interationInstance->interationClass()->parameterAt(i)->parameterName();
						instanceCopy->setParameterValue(parameterName, interationInstance->parameterValue(parameterName));
					}
					vec_interationInstance_copy.push_back(instanceCopy);
				}
				m_threadpool.schedule(boost::bind(&AmbassadorPrivate::sendInterationInstanceProc, this, vec_interationInstance_copy, true));
				
			}
			catch (...)
			{
				BOOST_LOG_TRIVIAL(error) << "发送交互类函数异常" << endl;

			}
		}

		return true;
		
	}

	bool AmbassadorPrivate::sendInterationInstanceProc(std::vector<InterationInstanceImpl*> vec_interationInstance , bool release)
	{
		std::string interationName;
		try {
			
			long long timeStamp = 0;
			
			InterationClass* interationclass = nullptr;
			if (!vec_interationInstance.empty())
			{
				timeStamp = vec_interationInstance[0]->timeStamp();
				interationName = vec_interationInstance[0]->interationClass()->interationName();
				interationclass = vec_interationInstance[0]->interationClass();
			}
			else
			{
				return false;
			}

				BOOST_LOG_TRIVIAL(trace) << "send beg " << interationName << " timeStamp: " << timeStamp << " runtime:" << std::setprecision(16) << runtime() << "ms";

			InterationsDataWriter_var interWriter = m_interDataWriterMap.at(interationclass);
			if (!interWriter)
			{
				return false;
			}

			Interations interations;

			Interation* interationlst = Data_Exchange_Platform::InterationList::allocbuf(vec_interationInstance.size());

			std::ostringstream os;
			boost::archive::binary_oarchive oa(os, boost::archive::no_header);

			for (int v = 0;v< vec_interationInstance.size();v++)
			{
				InterationInstanceImpl* interationInstance = vec_interationInstance[v];

				Interation inter;
				inter.interationName = interationInstance->interationClass()->interationName().c_str();
				inter.timeStamp = interationInstance->timeStamp();

				//InterationParameter* paramArray = new InterationParameter[interationInstance->interationClass()->parameterCount()];

				InterationParameter* paramArray = Data_Exchange_Platform::paramlst::allocbuf(interationInstance->interationClass()->parameterCount());	

				for (size_t i = 0; i < interationInstance->interationClass()->parameterCount(); i++)
				{
					os.clear();

					InterationClass::Parameter* parameter = interationInstance->interationClass()->parameterAt(i);
					paramArray[i] = InterationParameter();
					paramArray[i].parameterName = parameter->parameterName().c_str();
					paramArray[i].parameterType = parameter->dataType().name();

					oa << interationInstance->parameterValue(parameter->parameterName());
					//paramArray[i].parameterValue = os.str().c_str();
					int len = os.str().size();				
					//CORBA::Char* charArray = CORBA::CharSeq::allocbuf(len/sizeof(CORBA::Char));
					CORBA::Char* charArray = new CORBA::Char[len / sizeof(CORBA::Char)];
					//memset(charArray, 0, len);
					memcpy(charArray, os.str().c_str(), len);
					paramArray[i].parameterValue.replace(len, len, charArray, true);

					os.str(std::string());
				}

				inter.parameterList.replace(interationInstance->interationClass()->parameterCount(),
					interationInstance->interationClass()->parameterCount(),
					paramArray, true);

				if (release)
				{
					delete interationInstance;
				}

				interationlst[v] = inter;
			}

			interations.timeStamp = timeStamp;
			interations.interationlst.replace(vec_interationInstance.size(),
				vec_interationInstance.size(), interationlst, true);

				BOOST_LOG_TRIVIAL(trace) << "wrt bef " << interationName << " timeStamp: " << timeStamp << " runtime:" << std::setprecision(16) << runtime() << "ms" ;

			DDS::ReturnCode_t error = interWriter->write(interations, DDS::HANDLE_NIL);

			if (error != DDS::RETCODE_OK) {

				BOOST_LOG_TRIVIAL(error) << interationName << "发送失败" << endl;

				return false;
			}


				BOOST_LOG_TRIVIAL(trace) << "send end " << interationName << " timeStamp: " << timeStamp << " runtime:" << std::setprecision(16) << runtime() << "ms";

			return true;

		}
		catch (...)
		{
			//std::cout << "发送交互类函数异常" << std::endl;
			BOOST_LOG_TRIVIAL(error) << interationName << " 发送交互类函数异常" << endl;

		}

		return false;
	}

	void AmbassadorPrivate::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{
		m_publicPtr->receiveInterationInstance(interationInstance);
		for (int i = 0; i < m_vecRecvFunc.size();i++)
		{
			m_vecRecvFunc[i](interationInstance);
		}
	}

	InterationInstanceImpl* AmbassadorPrivate::waitInterationInstance(InterationClass * interationClass, int timeout)
	{
		DDS::DataReader_var reader = m_interDataReaderMap.at(interationClass);
		if (reader)
		{
			/*DDS::StatusCondition_var condition = reader->get_statuscondition();
			condition->set_enabled_statuses(DDS::DATA_AVAILABLE_STATUS);

			DDS::WaitSet_var ws = new DDS::WaitSet;
			ws->attach_condition(condition);
			DDS::ConditionSeq conditions;
			DDS::Duration_t durtion = { timeout, 0 };
			if (ws->wait(conditions, durtion) == DDS::RETCODE_OK)
			{
				InterationsDataReader_var interReader =
					InterationDataReader::_narrow(reader);
				if (!interReader) {
					return nullptr;
				}
				Interation inter;
				DDS::SampleInfo info;

				DDS::ReturnCode_t error = interReader->read_next_sample(inter, info);
				if (error != DDS::RETCODE_OK) {
					return nullptr;
				}

				InterationInstanceImpl* instance = new InterationInstanceImpl(interationClass);
				for (size_t i = 0; i < inter.parameterList.length(); i++)
				{
					Variant value;
					std::istringstream is(inter.parameterList[i].parameterValue.in());
					boost::archive::text_iarchive ia(is);
					ia >> value;
					instance->setParameterValue(inter.parameterList[i].parameterName.in(), value);
				}
				ws->detach_condition(condition);
				return instance;
			}
			ws->detach_condition(condition);*/
		}
		return nullptr;
	}

	void AmbassadorPrivate::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		m_vecRecvFunc.push_back(recvfunc);
	}

	void AmbassadorPrivate::waitConnection()
	{
		BOOST_LOG_TRIVIAL(debug) << "wait connection beg";
		for (int i = 0; i < m_vecDataWriter.size();i++)
		{
			DDS::StatusCondition_var condition = m_vecDataWriter[i]->get_statuscondition();
			condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

			DDS::WaitSet_var ws = new DDS::WaitSet;
			ws->attach_condition(condition);

			DDS::ConditionSeq conditions;
			DDS::Duration_t timeout = { 30, 0 };
			if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
				return;
			}

			ws->detach_condition(condition);
		}

		for (int i = 0; i < m_vecDataReader.size(); i++)
		{
			DDS::StatusCondition_var condition = m_vecDataReader[i]->get_statuscondition();
			condition->set_enabled_statuses(DDS::SUBSCRIPTION_MATCHED_STATUS);

			DDS::WaitSet_var ws = new DDS::WaitSet;
			ws->attach_condition(condition);

			DDS::ConditionSeq conditions;
			DDS::Duration_t timeout = { 30, 0 };
			if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
				return;
			}

			ws->detach_condition(condition);
		}

		BOOST_LOG_TRIVIAL(debug) << "wait connection end";
	}

	void AmbassadorPrivate::subscriptionLost(std::string classname, long handle)
	{
		m_publicPtr->subscriptionLost(classname, handle);
	}

	void AmbassadorPrivate::log(std::string msg)
	{
		BOOST_LOG_TRIVIAL(debug) << msg << " runtime:" << std::setprecision(16) << runtime() <<"ms" ;
	}

	bool AmbassadorPrivate::pushCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue)
	{
		return instanceQueue.size() >= m_interationQueueMax;
	}

	bool AmbassadorPrivate::popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue)
	{
		return true;
	}

	void AmbassadorPrivate::setInterationQueueMax(int max)
	{
		m_interationQueueMax = max;
	}

	int AmbassadorPrivate::interationQueueMax()
	{
		return m_interationQueueMax;
	}

	bool AmbassadorPrivate::createParticipant(long domain_ID)
	{
		using namespace OpenDDS::RTPS;
		using namespace OpenDDS::DCPS;

		int argc = 3;
		char strPath[MAX_PATH] = {0};
		GetModuleFileName(NULL,strPath, MAX_PATH);
		char* argv[3] = { strPath,"-DCPSConfigFile","rtps.ini" };
		DDS::DomainParticipantFactory_var dpf = TheParticipantFactoryWithArgs(argc, argv);

		m_participant =
			dpf->create_participant(domain_ID,
				PARTICIPANT_QOS_DEFAULT,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!m_participant) {
			//TODO
			return false;
		}

		m_interTypeSupport = new InterationsTypeSupportImpl;
		if (m_interTypeSupport->register_type(m_participant, "") != DDS::RETCODE_OK)
		{
			return false;
		}

		m_attrTypeSupport = new ObjectAttributeTypeSupportImpl;
		if (m_attrTypeSupport->register_type(m_participant, "") != DDS::RETCODE_OK)
		{
			return false;
		}

		return true;
	}

	bool AmbassadorPrivate::createPublisher()
	{
		m_publisher =
			m_participant->create_publisher(PUBLISHER_QOS_DEFAULT,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!m_publisher) {
			//TODO
			return false;
		}
		return true;
	}

	bool AmbassadorPrivate::createSubscriber()
	{

		m_subscriber =
			m_participant->create_subscriber(SUBSCRIBER_QOS_DEFAULT,
				 0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!m_subscriber) {
			//TODO
			return false;
		}
		return true;
	}

	std::string AmbassadorPrivate::topicName(ObjectClass::Attribute * attribute)
	{
		return attribute->attributeName()+"_"+attribute->objectClass()->objectName();
	}

	std::string AmbassadorPrivate::topicName(InterationClass * interation)
	{
		return interation->interationName();
	}

	DDS::Topic_var AmbassadorPrivate::createTopic(ObjectClass::Attribute * attribute)
	{
		//TODO
		DDS::Topic_var topic =
			m_participant->create_topic(topicName(attribute).c_str(),
				m_attrTypeSupport->get_type_name(),
				TOPIC_QOS_DEFAULT,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!topic) {
			//TODO
			return nullptr;
		}
		return topic;
	}

	DDS::Topic_var AmbassadorPrivate::createTopic(InterationClass * interation)
	{
		//TODO
		DDS::Topic_var topic =
			m_participant->create_topic(topicName(interation).c_str(),
				m_interTypeSupport->get_type_name(),
				TOPIC_QOS_DEFAULT,
				0,
				OpenDDS::DCPS::DEFAULT_STATUS_MASK);

		if (!topic) {
			//TODO
			return nullptr;
		}
		return topic;
	}

	void AmbassadorPrivate::cleanup()
	{
		
		for (int i = 0; i < m_vecListener.size(); i++)
		{
			m_vecListener[i]->interrupt();
		}

		DDS::DomainParticipantFactory_var dpf = TheParticipantFactory;

		//m_participant->delete_contained_entities();
		dpf->delete_participant(m_participant);
		
	}

	AmbassadorPrivate::InitDDDSEnv AmbassadorPrivate::initDDSEnv;

	AmbassadorPrivate::InitDDDSEnv::InitDDDSEnv()
	{
		QueryPerformanceFrequency(&m_performanceFrequency);
		QueryPerformanceCounter(&m_setupCounter);

		ace_os_object_manager.init();
		ace_object_manager.init();

		int argc = 3;
		char strPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, strPath, MAX_PATH);

		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("./rtps.ini", pt);

		int dep_log_level = pt.get<int>("DEPLOG.Level", logging::trivial::info);

		char filePath[MAX_PATH];
		strcpy(filePath, strPath);
		char* ptr;
		char* p;
		int i = 0;
		ptr = strtok(filePath, "\\");
		if (ptr)
		{
			while (ptr != NULL)
			{
				p = ptr;
				ptr = strtok(NULL, "\\");
				i++;
			}

			string path = p;
			logging::add_file_log
			(
				keywords::file_name = path + "_%Y%m%d_%H%M%S.log",
				keywords::rotation_size = 10 * 1024 * 1024,
				keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
				keywords::format = "[%TimeStamp%]: %Message%"
			);
			logging::core::get()->set_filter(logging::trivial::severity >= dep_log_level);
			logging::add_common_attributes();
		}

	}

	AmbassadorPrivate::InitDDDSEnv::~InitDDDSEnv()
	{
		//TheServiceParticipant->shutdown();
		ace_os_object_manager.fini();
		ace_object_manager.fini();
	}

	double AmbassadorPrivate::runtime()
	{
		QueryPerformanceCounter(&m_markCounter);
		return (m_markCounter.QuadPart - m_setupCounter.QuadPart)*1000.0 / m_performanceFrequency.QuadPart;
	}

	LARGE_INTEGER AmbassadorPrivate::m_performanceFrequency;
	LARGE_INTEGER AmbassadorPrivate::m_setupCounter;
	LARGE_INTEGER AmbassadorPrivate::m_markCounter;

}