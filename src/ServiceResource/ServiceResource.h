#ifndef __SERVICERESOURCE_H__
#define __SERVICERESOURCE_H__

#include <common.h>

namespace Data_Exchange_Platform{

	class ObjectInstanceImpl;
	class InterationClass;
	class InterationInstanceImpl;
	class TimerInstance;
	
	class ServiceResourcePrivate;

	//服务资源
	class DATA_EXCHANGE_PLATFORM_EXPORT ServiceResource
	{
	public:
		ServiceResource();
		~ServiceResource();
		
		/*服务资源注册
		resource_name	服务资源名称
		exparam			扩展参数列表
		返回值			是否成功注册
		*/
		bool login(std::string resource_name, std::vector<std::string> exparam = std::vector<std::string>() );
		
		/*服务资源注销
		返回值			是否成功注销
		*/
		bool logout();

		/*设置是否可用
		enabled			是否可用
		*/
		void setEnabled(bool enabled);

		/*获得最近一次错误信息
		返回值			错误信息
		*/
		std::string getLastError();

		//请求占用回调函数
		virtual bool holdRequest(std::string testid);

		//请求释放回调函数
		virtual bool freeRequest(std::string testid);

///////////////////////////////////////////////////////////////////////////////////////

		/*服务资源扩展参数更新
		index 	序号索引
		param 	参数值
		*/
		void updateResourceExtendedParameter(int index,std::string param);

		/*试验扩展参数更新
		index 	序号索引
		param 	参数值
		*/
		void updateTestExtendedParameter(int index,std::string param);

///////////////////////////////////////////////////////////////////////////////////////

		virtual void joinTest();

		virtual void leaveTest();

		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);

		bool sendInterationInstance(InterationInstanceImpl* interationInstance);

		bool sendInterationInstanceByTime(InterationInstanceImpl * interationInstance);
		bool sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance);

		void registerTimer( TimerInstance* timer );

		virtual void monitor(int delay, double lost, double speed);

		static void enableSpeedMonitor(bool bSpeed);

		static void enableSyncTime(bool bSync);

		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		bool declareInterationClass(InterationClass* interationClass);	

		void registerInterationReceiveProc(InterationRecvFunc recvfunc);
			
	private:
		ServiceResourcePrivate* m_private;
	};

}

#endif //__SERVICERESOURCE_H__