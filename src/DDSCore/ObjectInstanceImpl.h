#ifndef __OBJECTINSTANCEIMPL_H__
#define __OBJECTINSTANCEIMPL_H__

#include "common.h"
#include "ObjectClass.h"

namespace Data_Exchange_Platform
{
	class ObjectInstancePrivate;
	
	//对象类实例
	class DATA_EXCHANGE_PLATFORM_EXPORT ObjectInstanceImpl
	{
	public:
		/*构造函数
		instanceName		对象类实例名称
		objectClass			对象类
		*/
		ObjectInstanceImpl(std::string instanceName,ObjectClass* objectClass); 
		~ObjectInstanceImpl();

		/*更新对象类属性
		attributeName		对象类属性名称
		*/
		bool updateAttribute(std::string attributeName="");
		
		/*对象类属性更新回调
		attributeName		对象类属性名称
		*/
		virtual void attributeChanged(std::string attributeName);
		
		/*设置对象类属性值
		attributeName		对象类属性名称
		返回值				是否成功设置
		*/
		bool setAttribute(std::string attributeName,Variant value);
		
		/*获取对象类属性值
		attributeName		对象类属性名称
		返回值				对象类属性值
		*/
		Variant attribute(std::string attributeName);
		
		/*获取对象类实例名称
		返回值				对象类实例名称
		*/
		std::string instanceName();

		/*获取对象类实例名称
		返回值				对象类
		*/
		ObjectClass* objectClass();
		
		/*管理通信代表（暂定）
		*/
		void attachAmbassador(Ambassador* ambassador);

		Ambassador* ambassador();
		
	private:
		ObjectInstancePrivate* m_private;
	};
	
}

#endif //__OBJECTINSTANCEIMPL_H__