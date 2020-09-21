#pragma once

#include <dds/DCPS/Service_Participant.h>

#include <vector>
#include <map>
#include <boost/threadpool.hpp>
#include "common.h"
#include "InterationInstanceImpl.h"
#include "ObjectClass.h"

#include "DataExchangeTypeSupportImpl.h"
#include "DataWriterListenerImpl.h"

namespace Data_Exchange_Platform {

	class Ambassador;
	class InterationClass;
	class ObjectInstanceImpl;
	class DataExchangeReaderListener;

	class AmbassadorPrivate
	{
	public:
		AmbassadorPrivate(long domain_ID, Ambassador* publicPtr);
		//AmbassadorPrivate(){};
		~AmbassadorPrivate();

		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);
		bool registerObjectClass(ObjectClass* objectClass);
		ObjectClass* objectClass(std::string objectName);
		ObjectInstanceImpl* objectInstance(std::string instanceName, std::string objectName);

		bool declareObjectAttribute(ObjectClass::Attribute* attribute);

		bool sendObjectAttribute(ObjectInstanceImpl* objectInstance, ObjectClass::Attribute* attribute, Variant value);
		void receiveObjectAttribute(ObjectInstanceImpl* objectInstance, ObjectClass::Attribute* attribute, Variant value);

		//InterationClass
		bool declareInterationClass(InterationClass* interationClass);
		InterationClass*  interationClass(std::string interationName);

		bool sendInterationInstance(std::vector<InterationInstanceImpl*> vec_interationInstance, bool bwait = true);
		bool sendInterationInstanceProc(std::vector<InterationInstanceImpl*> vec_interationInstance, bool release = false );

		void receiveInterationInstance(InterationInstanceImpl* interationInstance);

		InterationInstanceImpl* waitInterationInstance(InterationClass* interationClass, int timeout = 60);
		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		void waitConnection();

		void subscriptionLost(std::string classname,long handle);

		void log(std::string msg);

		bool pushCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue);

		bool popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue);

		void setInterationQueueMax(int max);

		int interationQueueMax();

		static double runtime();

	private:
		bool createParticipant(long domain_ID);
		bool createPublisher();
		bool createSubscriber();

		std::string topicName(ObjectClass::Attribute* attribute);
		std::string topicName(InterationClass* interation);

		DDS::Topic_var createTopic(ObjectClass::Attribute* attribute);
		DDS::Topic_var createTopic(InterationClass* interation);

		void cleanup();

		std::vector<ObjectClass*>			m_vecObjCls;
		std::vector<ObjectInstanceImpl*>	m_vecObjInst;
		std::vector<InterationClass*>		m_vecInterCls;
		std::vector<InterationRecvFunc>		m_vecRecvFunc;

		DDS::DomainParticipant_var			m_participant;
		DDS::Publisher_var					m_publisher;
		DDS::Subscriber_var					m_subscriber;

		Ambassador *						m_publicPtr;

		std::vector<DDS::DataWriter_var> m_vecDataWriter;
		std::vector<DDS::DataReader_var> m_vecDataReader;

		std::map<ObjectClass::Attribute*, ObjectAttributeDataWriter_var> m_attrDataWriterMap;
		std::map<InterationClass*, InterationsDataWriter_var> m_interDataWriterMap;

		friend class DataExchangeReaderListener;

		std::map<DataExchangeReaderListener*,ObjectClass::Attribute*> m_attrDataListenerMap;
		std::map<DataExchangeReaderListener*,InterationClass*> m_interDataListenerMap;

		std::vector<DataExchangeReaderListener*> m_vecListener;

		std::map<InterationClass*, DDS::DataReader_var> m_interDataReaderMap;

		InterationsTypeSupport_var m_interTypeSupport;
		ObjectAttributeTypeSupport_var m_attrTypeSupport;

		int m_interationQueueMax;

		struct InitDDDSEnv{
			InitDDDSEnv();
			~InitDDDSEnv();
			ACE_OS_Object_Manager ace_os_object_manager;
			ACE_Object_Manager ace_object_manager;
		};
		static InitDDDSEnv initDDSEnv;

		boost::threadpool::pool m_threadpool;

		static LARGE_INTEGER m_performanceFrequency;
		static LARGE_INTEGER m_setupCounter;
		static LARGE_INTEGER m_markCounter;

	};

}
