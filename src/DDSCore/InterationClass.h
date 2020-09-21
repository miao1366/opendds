#ifndef __INTERATIONCLASS_H__
#define __INTERATIONCLASS_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class Ambassador;
	class InterationClassPrivate;
	
	//������
	class DATA_EXCHANGE_PLATFORM_EXPORT InterationClass
	{
	public:
		/*���캯��
		interationName		����������
		exchangeType		���ݽ�����ʽ
		*/
		InterationClass(std::string interationName,ExchangeType exchangeType);
		~InterationClass();
		
		/*��ý���������
		����ֵ				����������
		*/
		std::string interationName();

		/*��ý��������ݽ�����ʽ
		����ֵ				���������ݽ�����ʽ
		*/
		ExchangeType exchangeType();

		//���������
		class Parameter
		{
		public:
			std::string parameterName() {return m_parameterName;}
			DataType& dataType() {return m_dataType;}
		private:
			Parameter(std::string parameterName,DataType& dataType)
			:m_parameterName(parameterName),m_dataType(dataType){}
			
			friend class InterationClassPrivate;
			std::string m_parameterName;
			DataType& m_dataType;
		};
		
		/*��ӽ��������
		parameterName		��������
		dataType			��������
		����ֵ				���������
		*/
		Parameter* addParameter(std::string parameterName,DataType& dataType);

		/*ͨ���������Ʋ�ѯ���������
		parameterName		��������
		����ֵ				���������
		*/
		Parameter* parameter(std::string parameterName);

		/*ͨ������ֵ��ѯ���������
		index				����ֵ
		����ֵ				���������
		*/
		Parameter* parameterAt(int index);

		/*��ȡ�������������
		����ֵ				��������
		*/
		int parameterCount();
		
		bool persistent() ;
		void setPersistent(bool persist) ;

	private:
		InterationClassPrivate* m_private;
	};
}

#endif //__INTERATIONCLASS_H__