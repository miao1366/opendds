#ifndef __OBJECTINSTANCEIMPL_H__
#define __OBJECTINSTANCEIMPL_H__

#include "common.h"
#include "ObjectClass.h"

namespace Data_Exchange_Platform
{
	class ObjectInstancePrivate;
	
	//������ʵ��
	class DATA_EXCHANGE_PLATFORM_EXPORT ObjectInstanceImpl
	{
	public:
		/*���캯��
		instanceName		������ʵ������
		objectClass			������
		*/
		ObjectInstanceImpl(std::string instanceName,ObjectClass* objectClass); 
		~ObjectInstanceImpl();

		/*���¶���������
		attributeName		��������������
		*/
		bool updateAttribute(std::string attributeName="");
		
		/*���������Ը��»ص�
		attributeName		��������������
		*/
		virtual void attributeChanged(std::string attributeName);
		
		/*���ö���������ֵ
		attributeName		��������������
		����ֵ				�Ƿ�ɹ�����
		*/
		bool setAttribute(std::string attributeName,Variant value);
		
		/*��ȡ����������ֵ
		attributeName		��������������
		����ֵ				����������ֵ
		*/
		Variant attribute(std::string attributeName);
		
		/*��ȡ������ʵ������
		����ֵ				������ʵ������
		*/
		std::string instanceName();

		/*��ȡ������ʵ������
		����ֵ				������
		*/
		ObjectClass* objectClass();
		
		/*����ͨ�Ŵ����ݶ���
		*/
		void attachAmbassador(Ambassador* ambassador);

		Ambassador* ambassador();
		
	private:
		ObjectInstancePrivate* m_private;
	};
	
}

#endif //__OBJECTINSTANCEIMPL_H__