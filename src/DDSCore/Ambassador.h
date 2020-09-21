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
	
	//通信代表
	class DATA_EXCHANGE_PLATFORM_EXPORT Ambassador
	{
	public:
		/*构造函数
		domain_ID			域ID
		*/
		Ambassador(long domain_ID);
		virtual ~Ambassador();
		
		/*注册对象类实例
		objectInstance		对象类实例
		返回值				是否注册成功
		*/
		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		/*注册对象类
		objectClass			对象类
		返回值				是否注册成功
		*/
		bool registerObjectClass(ObjectClass* objectClass);

		/*通过对象类名称查询已注册对象类
		objectName			对象类名称
		返回值				对象类
		*/
		ObjectClass* objectClass(std::string objectName);

		/*通过对象类名称和实例名称查询已注册对象类实例
		instanceName		对象类实例名称
		objectName			对象类名称
		返回值				对象类实例
		*/
		ObjectInstanceImpl* objectInstance(std::string instanceName, std::string objectName);
		
		/*声明对象类属性
		attribute			对象类属性
		返回值				是否声明成功
		*/
		bool declareObjectAttribute(ObjectClass::Attribute* attribute);
		
		/*发送对象类属性
		objectInstance		对象类实例
		attribute			对象类属性
		value				属性值
		返回值				是否发送成功
		*/
		bool sendObjectAttribute(ObjectInstanceImpl* objectInstance,ObjectClass::Attribute* attribute,Variant value);
		
		/*接收对象类属性
		objectInstance		对象类实例
		attribute			对象类属性
		value				属性值
		*/
		virtual void receiveObjectAttribute(ObjectInstanceImpl* objectInstance,ObjectClass::Attribute* attribute,Variant value);
		

		/*声明交互类
		interationClass		交互类
		返回值				是否声明成功
		*/
		bool declareInterationClass(InterationClass* interationClass);

		/*通过交互类名称查询交互类
		interationName		交互类名称
		返回值				交互类
		*/
		InterationClass*  interationClass(std::string interationName);
		
		/*发送交互类实例
		interationInstance	交互类实例
		返回值				是否发送成功
		*/
		bool sendInterationInstance(InterationInstanceImpl* interationInstance, bool bwait = true);
		bool sendInterationInstance(std::vector<InterationInstanceImpl*> vec_interationInstance, bool bwait = true);

		/*接收交互类实例
		interationInstance	交互类实例
		*/
		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);
		
		/*等待交互类实例（未实现）
		*/
		InterationInstanceImpl* waitInterationInstance(InterationClass* interationClass,int timeout=60);

		/*注册交互类接收过程函数
		recvfunc			接收过程函数
		*/
		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		/*等待连接（暂定）
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