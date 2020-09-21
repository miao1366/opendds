#ifndef __INTERATIONINSTANCEIMPL_H__
#define __INTERATIONINSTANCEIMPL_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class InterationClass;
	class InterationInstancePrivate;
	
	//交互类实例
	class DATA_EXCHANGE_PLATFORM_EXPORT InterationInstanceImpl
	{
	public:
		/*构造函数
		interationClass		交互类
		*/
		InterationInstanceImpl(InterationClass* interationClass);
		~InterationInstanceImpl();
		
		/*设置参数值
		parameterName		参数名称
		value				参数值
		*/
		bool setParameterValue(std::string parameterName,Variant value);
		
		/*获取参数值
		parameterName		参数名称
		*/
		Variant parameterValue(std::string parameterName);
		
		/*获取交互类
		返回值				交互类
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