#include "DataWriterListenerImpl.h"

#include "dds/DdsDcpsPublicationC.h"
#include "dds/DCPS/Service_Participant.h"
using namespace std;
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


#include "AmbassadorPrivate.h"
#include "Ambassador.h"
#include "InterationClass.h"
#include "ObjectInstanceImpl.h"
#include "DataExchangeReaderListener.h"

namespace Data_Exchange_Platform {
	DataWriterListenerImpl::DataWriterListenerImpl ()
		: publication_matched_(false)
	{
		inter = NULL;
		m_amb = NULL;
	}

	DataWriterListenerImpl::~DataWriterListenerImpl ()
	{
	}

	void DataWriterListenerImpl::on_offered_deadline_missed (
		::DDS::DataWriter_ptr writer,
		const ::DDS::OfferedDeadlineMissedStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name() << "writer on_offered_deadline_missed" << endl;
	}

	void DataWriterListenerImpl::on_offered_incompatible_qos (
		::DDS::DataWriter_ptr writer,
		const ::DDS::OfferedIncompatibleQosStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< "writer on_offered_incompatible_qos" << endl;

	}

	void DataWriterListenerImpl::on_liveliness_lost (
		::DDS::DataWriter_ptr writer,
		const ::DDS::LivelinessLostStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< "writer on_liveliness_lost" << endl;
	}

	void DataWriterListenerImpl::on_publication_matched (
		::DDS::DataWriter_ptr writer,
		const ::DDS::PublicationMatchedStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;

		publication_matched_ = true;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< " writer on_publication_matched" << endl;

	}

	void DataWriterListenerImpl::on_publication_disconnected (
		::DDS::DataWriter_ptr writer,
		const ::OpenDDS::DCPS::PublicationDisconnectedStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< " writer on_publication_disconnected" << endl;
	}

	void DataWriterListenerImpl::on_publication_reconnected (
		::DDS::DataWriter_ptr writer,
		const ::OpenDDS::DCPS::PublicationReconnectedStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< " writer on_publication_reconnected" << endl;
	}

	void DataWriterListenerImpl::on_publication_lost (
		::DDS::DataWriter_ptr writer,
		const ::OpenDDS::DCPS::PublicationLostStatus & status
		)
	{
		ACE_UNUSED_ARG(writer) ;
		ACE_UNUSED_ARG(status) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name()<< " writer on_publication_lost" << endl;
	// 	AmbassadorPrivate* a = new AmbassadorPrivate;
	// 	a->UpdateInterWrite(writer);
		//if (a > 3)
		//{
		//	return;
		//}
		//m_amb->UpdateInterWrite(inter,writer, this);
		//a++;
	}

	void DataWriterListenerImpl::on_connection_deleted (
		::DDS::DataWriter_ptr writer
		)
	{
		ACE_UNUSED_ARG(writer) ;
		BOOST_LOG_TRIVIAL(info) << writer->get_topic()->get_name() << " writer on_connection_deleted" << endl;

	}
}
