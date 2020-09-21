#ifndef DATAWRITER_LISTENER_IMPL
#define DATAWRITER_LISTENER_IMPL

#include "dds/DdsDcpsPublicationC.h"
#include "dds/DCPS/Definitions.h"
#include "dds/DCPS/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
namespace Data_Exchange_Platform{

	class InterationInstanceImpl;
	class AmbassadorPrivate;
	class InterationClass;

	class DataWriterListenerImpl
		: public virtual OpenDDS::DCPS::LocalObject<OpenDDS::DCPS::DataWriterListener>
	{

	public:
		DataWriterListenerImpl (void);
 		DataWriterListenerImpl(InterationClass* interationClass, AmbassadorPrivate* amb)
 			:inter(interationClass),m_amb(amb){a = 0;}
		virtual ~DataWriterListenerImpl (void);

		virtual void on_offered_deadline_missed (
			::DDS::DataWriter_ptr writer,
			const ::DDS::OfferedDeadlineMissedStatus & status
			);

		virtual void on_offered_incompatible_qos (
			::DDS::DataWriter_ptr writer,
			const ::DDS::OfferedIncompatibleQosStatus & status
			);

		virtual void on_liveliness_lost (
			::DDS::DataWriter_ptr writer,
			const ::DDS::LivelinessLostStatus & status
			);

		virtual void on_publication_matched (
			::DDS::DataWriter_ptr writer,
			const ::DDS::PublicationMatchedStatus & status
			);

		virtual void on_publication_disconnected (
			::DDS::DataWriter_ptr reader,
			const ::OpenDDS::DCPS::PublicationDisconnectedStatus & status
			);

		virtual void on_publication_reconnected (
			::DDS::DataWriter_ptr reader,
			const ::OpenDDS::DCPS::PublicationReconnectedStatus & status
			);

		virtual void on_publication_lost (
			::DDS::DataWriter_ptr writer,
			const ::OpenDDS::DCPS::PublicationLostStatus & status
			);


		virtual void on_connection_deleted (
			::DDS::DataWriter_ptr writer
			);

		bool publication_matched() const
		{
			return publication_matched_;
		}

	protected:
	private:
		bool publication_matched_;
		InterationClass* inter;
		AmbassadorPrivate* m_amb;
		int a;
	};
}

#endif /* DATAWRITER_LISTENER_IMPL  */
