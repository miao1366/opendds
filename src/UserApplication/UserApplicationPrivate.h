#pragma once

#include "UserApplication.h"

namespace Data_Exchange_Platform {

	class TimeAmbassador;
	class TimerInstance;

	class UserApplicationPrivate
	{
	public:
		UserApplicationPrivate(UserApplication* publicPtr);
		~UserApplicationPrivate();

		std::string username() { return m_username; }

		void permit() { m_online = true; }

		bool isOnline() { return m_online; }

		bool ownTest() { return m_ownTest; }

		bool inTest() { return m_testAmbassador; }

		std::string testID() { return m_testid; }
		void setTestID(std::string testid) { m_testid = testid; }

		bool login(std::string username, std::string password);

		bool logout();

		std::string createTest(std::string testname,
			std::string testpassword,
			std::string description,
			std::vector<std::string> resid,
			std::vector<std::string> exparam = std::vector<std::string>());

		bool enterTest(std::string testname, std::string testpassword, std::vector<std::string> exparam);

		bool exitTest();

		bool closeTest();

		bool registerObjectInstance(ObjectInstanceImpl* objectInstance);

		bool declareInterationClass(InterationClass* interationClass);

		bool sendInterationInstance(InterationInstanceImpl* interationInstance);

		bool sendInterationInstanceByTime(InterationInstanceImpl * interationInstance);

		bool sendInterationInstanceByTime(std::vector<InterationInstanceImpl*> vec_interationInstance);

		void registerInterationReceiveProc(InterationRecvFunc recvfunc);

		void joinTestDomain(long domain);

		void leaveTestDomain();

		void registerTimer(TimerInstance* timer);

		UserApplication::ResourceInfoList resourceList(std::string filtername="");
		UserApplication::ResourceInfo resourceInfo(std::string resid);
		void addResource(UserApplication::ResourceInfo resInfo);
		void delResource(UserApplication::ResourceInfo resInfo);
		void updateResource(UserApplication::ResourceInfo resInfo);

		UserApplication::TestInfoList testList(std::string filtername = "");
		UserApplication::TestInfo testInfo(std::string testid);
		void addTest(UserApplication::TestInfo testInfo);
		void delTest(UserApplication::TestInfo testInfo);
		void updateTest(UserApplication::TestInfo testInfo);

		void monitor(int delay, double lost, double speed);

		std::string getLastError();
		void setLastError(std::string lastError);
		void errorClear();

		void log(std::string msg);

		void testLicenceProc(InterationInstanceImpl * interationInstance);
		void userAccessProc(InterationInstanceImpl * interationInstance);
		void userMonitor(InterationInstanceImpl * interationInstance);


		void receiveInterationInstance(InterationInstanceImpl* interationInstance);
	private:
		std::string m_username;
		std::string m_password;

		std::string m_testid;
		std::string m_testpassword;
		bool m_online;
		bool m_ownTest;

		TimeAmbassador* m_testAmbassador;
		UserApplication* m_publicPtr;
		UserApplication::ResourceInfoList m_resourceList;

		UserApplication::TestInfoList m_testList;
		std::string m_lastError;

	};

}