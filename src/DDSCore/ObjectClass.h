#ifndef __OBJECTCLASS_H__
#define __OBJECTCLASS_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class Ambassador;
	class ObjectClassPrivate;

	//������
	class DATA_EXCHANGE_PLATFORM_EXPORT ObjectClass
	{
	public:
		/*���캯��
		objectName		����������
		*/
		ObjectClass(std::string objectName);
		~ObjectClass();
		
		/*��ȡ����������
		����ֵ			����������
		*/
		std::string objectName();

		//����������
		class Attribute
		{
		public:	
			std::string attributeName() {return m_attributeName;}
			DataType& dataType() {return m_dataType;}
			ExchangeType exchangeType() {return m_exchangeType;}
			ObjectClass* objectClass() {return m_objectClass;}
		private:
			Attribute(std::string attributeName,DataType& dataType,ExchangeType exchangeType,ObjectClass* objectClass)
			:m_attributeName(attributeName),m_dataType(dataType),m_exchangeType(exchangeType),m_objectClass(objectClass){}
			
			friend class ObjectClassPrivate;
			std::string m_attributeName;
			DataType& m_dataType;
			ExchangeType m_exchangeType;
			ObjectClass* m_objectClass;
		};
		
		/*��Ӷ���������
		attributeName		����������
		dataType			��������
		exchangeType		���ݽ�����ʽ
		����ֵ				����������
		*/
		Attribute* addAttribute(std::string attributeName,DataType& dataType,ExchangeType exchangeType);

		/*ͨ���������Ʋ�ѯ����������
		attributeName		��������
		����ֵ				����������
		*/
		Attribute* attribute(std::string attributeName);

		/*ͨ������ֵ��ѯ����������
		index				����ֵ
		����ֵ				����������
		*/
		Attribute* attributeAt(int index);

		/*��ȡ���������Ը���
		����ֵ				���Ը���
		*/
		int attrbuteCount();
		
	private:
		ObjectClassPrivate* m_private;
	};
	
}

#endif //__OBJECTCLASS_H__