#ifndef __INTERATIONINSTANCEIMPL_H__
#define __INTERATIONINSTANCEIMPL_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class InterationClass;
	class InterationInstancePrivate;
	
	//������ʵ��
	class DATA_EXCHANGE_PLATFORM_EXPORT InterationInstanceImpl
	{
	public:
		/*���캯��
		interationClass		������
		*/
		InterationInstanceImpl(InterationClass* interationClass);
		~InterationInstanceImpl();
		
		/*���ò���ֵ
		parameterName		��������
		value				����ֵ
		*/
		bool setParameterValue(std::string parameterName,Variant value);
		
		/*��ȡ����ֵ
		parameterName		��������
		*/
		Variant parameterValue(std::string parameterName);
		
		/*��ȡ������
		����ֵ				������
		*/
		InterationClass* interationClass();

		void setTimeStamp(long long timestamp) ;
		long long timeStamp() ;

		void setPubHandle(long handle);
		long pubHandle();
		
	private:
		InterationInstancePrivate* m_private;
		
	};
	
}

#endif //__INTERATIONINSTANCEIMPL_H__