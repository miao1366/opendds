#ifndef __USERAPPLICATION_H__
#define __USERAPPLICATION_H__

#include "common.h"

namespace Data_Exchange_Platform{
	
	class ObjectInstanceImpl;
	class InterationClass;
	class InterationInstanceImpl;
	class TimerInstance;

	class UserApplicationPrivate;

	//用户应用
	class DATA_EXCHANGE_PLATFORM_EXPORT UserApplication
	{
	public:
		UserApplication();
		~UserApplication();
	
		/*用户登陆
		username		用户名
		password		密码
		返回值			是否成功登陆
		*/
		bool login(std::string username, std::string password);
		
		/*用户退出
		*/
		bool logout();

		/*创建仿真试验
		testname		仿真试验名称
		description		仿真试验描述
		resid			选择资源ID
		testpassword	仿真试验密码
		exparam			扩展参数列表
		返回值			成功则返回仿真试验ID，否则返回字符串为空
		*/
		std::string createTest(std::string testname, 
			std::string testpassword, 
			std::string description, 
			std::vector<std::string> resid, 
			std::vector<std::string> exparam = std::vector<std::string>() );
		
		/*加入仿真试验
		testid			仿真试验ID
		testpassword	仿真试验密码
		exparam			扩展参数列表
		返回值			是否成功加入仿真试验
		*/
		bool enterTest(std::string testid, 
			std::string testpassword, 
			std::vector<std::string> exparam = std::vector<std::string>() );

		bool enterOrCreateTest(std::string testname, 
			std::string testpassword, 
			std::vector<std::string> resid = std::vector<std::string>());
		
		/*退出仿真试验
		*/
		bool exitTest();

		/*关闭仿真试验
		*/
		bool closeTest();

		enum UDATE_OPERATION {
			Insert,
			Delete,
			Update
		};

		struct ResourceInfo{
			std::string guid;  					//服务资源ID
			std::string resname;				//服务资源类型
			bool enabled;						//是否可用
			std::vector<std::string> exparam;	//扩展参数列表
		}; //服务资源信息 

		typedef std::vector<ResourceInfo> ResourceInfoList; //服务资源信息列表 

		//获取服务资源信息列表
		ResourceInfoList resourceList(std::string filtername="");
		//根据资源ID查找服务资源信息
		ResourceInfo resourceInfo(std::string resid);
		//服务资源信息更新虚函数
		virtual void resourceUpdated(ResourceInfo resinfo, UDATE_OPERATION);

		struct UserInfo{
			std::string name;					//用户名
			std::vector<std::string> exparam;	//扩展参数列表
		};

		typedef std::vector<UserInfo> UserInfoList; //用户信息列表 

		struct TestInfo{
			std::string guid;						//仿真试验ID
			std::string testname;					//仿真试验名
			std::string description;				//仿真试验描述
			UserInfoList users;			//仿真试验用户列表
			ResourceInfoList resources;	//仿真试验资源列表
			std::vector<std::string> exparam;		//扩展参数列表
		};

		typedef std::vector<TestInfo> TestInfoList; //仿真试验信息列表 

		

		//获取服务资源信息列表
		TestInfoList testList(std::string filtername="");
		//根据试验ID查找仿真试验信息
		TestInfo testInfo(std::string testid);
		//仿真试验信息更新虚函数
		virtual void testUpdated(TestInfo testinfo, UDATE_OPERATION);

		/*获得最近一次错误信息
		返回值			错误信息
		*/
		std::string getLastError();

////////////////////////////////////////////////////////////////////////////////

		/*试验用户扩展参数更新
		index 	序号索引
		param 	参数值
		*/
		void updateTestUserExtendedParameter(int index,std::string param);

		/*试验扩展参数更新
		index 	序号索引
		param 	参数值
		*/
		void updateTestExtendedParameter(int index,std::string param);

/////////////////////////////////////////////////////////////////////////////////

		/*加入仿真试验回调虚函数
		*/
		virtual void joinTest();

		/*离开仿真试验回调虚函数
		*/
		virtual void leaveTest();

		/*接收交互类实例回调虚函数
		interationInstance	交互类实例
		*/
		virtual void receiveInterationInstance(InterationInstanceImpl* interationInstance);

		/*注册对象类实例
		objectInstance		对象类实例
		返回值				是否注册成功
		*/
		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		/*声明交互类
		interationClass		交互类
		返回值				是否声明成功
		*/
		bool declareInterationClass(InterationClass* interationClass);

		/*发送交互类实例
		interationInstance	交互类实例
		返回值				是否发送成功
		*/
		bool sendInterationInstance(InterationInstanceImpl* interationInstance);

		bool sendInterationInstanceByTime(InterationInstanceImpl * interationInstance);

		bool sendInterationInstanceByTime(std::vector<InterationInstanceImpl*> vec_interationInstance);

		virtual void disconnected(std::string classname);

		void registerTimer(TimerInstance* timer);

		virtual void monitor(int delay, double lost, double speed);

		static void enableSpeedMonitor(bool bSpeed);

		static void enableSyncTime(bool bSync);

		/*注册交互类接收过程函数
		recvfunc			接收过程函数
		*/
		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		void log(std::string msg);

		virtual void testLicenceProc(InterationInstanceImpl * interationInstance);
		virtual void userAccessProc(InterationInstanceImpl * interationInstance);
		virtual void userMonitor(InterationInstanceImpl * interationInstance);
	private:
		UserApplicationPrivate* m_private;
	};
}

#endif //__USERAPPLICATION_H__
