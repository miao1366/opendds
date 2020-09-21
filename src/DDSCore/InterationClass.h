#ifndef __INTERATIONCLASS_H__
#define __INTERATIONCLASS_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class Ambassador;
	class InterationClassPrivate;
	
	//交互类
	class DATA_EXCHANGE_PLATFORM_EXPORT InterationClass
	{
	public:
		/*构造函数
		interationName		交互类名称
		exchangeType		数据交互方式
		*/
		InterationClass(std::string interationName,ExchangeType exchangeType);
		~InterationClass();
		
		/*获得交互类名称
		返回值				交互类名称
		*/
		std::string interationName();

		/*获得交互类数据交互方式
		返回值				交互类数据交互方式
		*/
		ExchangeType exchangeType();

		//交互类参数
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
		
		/*添加交互类参数
		parameterName		参数名称
		dataType			数据类型
		返回值				交互类参数
		*/
		Parameter* addParameter(std::string parameterName,DataType& dataType);

		/*通过参数名称查询交互类参数
		parameterName		参数名称
		返回值				交互类参数
		*/
		Parameter* parameter(std::string parameterName);

		/*通过索引值查询交互类参数
		index				索引值
		返回值				交互类参数
		*/
		Parameter* parameterAt(int index);

		/*获取交互类参数个数
		返回值				参数个数
		*/
		int parameterCount();
		
		bool persistent() ;
		void setPersistent(bool persist) ;

	private:
		InterationClassPrivate* m_private;
	};
}

#endif //__INTERATIONCLASS_H__