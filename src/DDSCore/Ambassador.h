#ifndef __AMBASSADOR_H__
#define __AMBASSADOR_H__

#include "common.h"
#include "ObjectClass.h"
#include "InterationInstanceImpl.h"
#include <queue>

namespace Data_Exchange_Platform
{
	class ObjectClass;
	class ObjectInstanceImpl;
	class InterationClass;
	
	class AmbassadorPrivate;
	
	//ͨ�Ŵ���
	class DATA_EXCHANGE_PLATFORM_EXPORT Ambassador
	{
	public:
		/*���캯��
		domain_ID			��ID
		*/
		Ambassador(long domain_ID);
		virtual ~Ambassador();
		
		/*ע�������ʵ��
		objectInstance		������ʵ��
		����ֵ				�Ƿ�ע��ɹ�
		*/
		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		/*ע�������
		objectClass			������
		����ֵ				�Ƿ�ע��ɹ�
		*/
		bool registerObjectClass(ObjectClass* objectClass);

		/*ͨ�����������Ʋ�ѯ��ע�������
		objectName			����������
		����ֵ				������
		*/
		ObjectClass* objectClass(std::string objectName);

		/*ͨ�����������ƺ�ʵ�����Ʋ�ѯ��ע�������ʵ��
		instanceName		������ʵ������
		objectName			����������
		����ֵ				������ʵ��
		*/
		ObjectInstanceImpl* objectInstance(std::string instanceName, std::string objectName);
		
		/*��������������
		attribute			����������
		����ֵ				�Ƿ������ɹ�
		*/
		bool declareObjectAttribute(ObjectClass::Attribute* attribute);
		
		/*���Ͷ���������
		objectInstance		������ʵ��
		attribute			����������
		value				����ֵ
		����ֵ				�Ƿ��ͳɹ�
		*/
		bool sendObjectAttribute(ObjectInstanceImpl* objectInstance,ObjectClass::Attribute* attribute,Variant value);
		
		/*���ն���������
		objectInstance		������ʵ��
		attribute			����������
		value				����ֵ
		*/
		virtual void receiveObjectAttribute(ObjectInstanceImpl* objectInstance,ObjectClass::Attribute* attribute,Variant value);
		

		/*����������
		interationClass		������
		����ֵ				�Ƿ������ɹ�
		*/
		bool declareInterationClass(InterationClass* interationClass);

		/*ͨ�����������Ʋ�ѯ������
		interationName		����������
		����ֵ				������
		*/
		InterationClass*  interationClass(std::string interationName);
		
		/*���ͽ�����ʵ��
		interationInstance	������ʵ��
		����ֵ				�Ƿ��ͳɹ�
		*/
		bool sendInterationInstance(InterationInstanceImpl* interationInstance, bool bwait = true);
		bool sendInterationInstance(std::vector<InterationInstanceImpl*> vec_interationInstance, bool bwait = true);

		/*���ս�����ʵ��
		interationInstance	������ʵ��
		*/
		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);
		
		/*�ȴ�������ʵ����δʵ�֣�
		*/
		InterationInstanceImpl* waitInterationInstance(InterationClass* interationClass,int timeout=60);

		/*ע�ύ������չ��̺���
		recvfunc			���չ��̺���
		*/
		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		/*�ȴ����ӣ��ݶ���
		*/
		void waitConnection();

		virtual void subscriptionLost(std::string classname, long handle);

		void log(std::string msg);

		virtual bool pushCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue);

		virtual bool popCheckInterationQueue(const std::queue<InterationInstanceImpl*>& instanceQueue);

		void setInterationQueueMax(int max);

		int interationQueueMax();

		static double runtime();

	private:
		AmbassadorPrivate* m_private;
	};

}

#endif //__AMBASSADOR_H__