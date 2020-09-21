#ifndef __OBJECTCLASS_H__
#define __OBJECTCLASS_H__

#include "common.h"

namespace Data_Exchange_Platform
{
	class Ambassador;
	class ObjectClassPrivate;

	//对象类
	class DATA_EXCHANGE_PLATFORM_EXPORT ObjectClass
	{
	public:
		/*构造函数
		objectName		对象类名称
		*/
		ObjectClass(std::string objectName);
		~ObjectClass();
		
		/*获取对象类名称
		返回值			对象类名称
		*/
		std::string objectName();

		//对象类属性
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
		
		/*添加对象类属性
		attributeName		对象类属性
		dataType			数据类型
		exchangeType		数据交互方式
		返回值				对象类属性
		*/
		Attribute* addAttribute(std::string attributeName,DataType& dataType,ExchangeType exchangeType);

		/*通过属性名称查询对象类属性
		attributeName		属性名称
		返回值				对象类属性
		*/
		Attribute* attribute(std::string attributeName);

		/*通过索引值查询对象类属性
		index				索引值
		返回值				对象类属性
		*/
		Attribute* attributeAt(int index);

		/*获取对象类属性个数
		返回值				属性个数
		*/
		int attrbuteCount();
		
	private:
		ObjectClassPrivate* m_private;
	};
	
}

#endif //__OBJECTCLASS_H__