#pragma once

#include <Ambassador.h>
#include "UserApplicationPrivate.h"
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <list>

using namespace Data_Exchange_Platform;

class UserAmbassador :public Ambassador
{
public:
	~UserAmbassador();

	static UserAmbassador* instance();

	bool login(std::string username, std::string password);
	void logout(std::string username);
	bool createTest(std::string username, std::string testname, std::string testpassword, std::string description, std::vector<std::string> resid, std::vector<std::string> exparam);
	bool enterTest(std::string username,std::string testid, std::string testpassword, std::vector<std::string> exparam);
	void exitTest(std::string username, std::string testid);
	void closeTest(std::string username, std::string testid);

	void attachUserApplication(UserApplicationPrivate*  userApplication) { m_vecUserApplication.push_back(userApplication); }
	void detachUserApplication(UserApplicationPrivate*  userApplication) {
		std::vector<UserApplicationPrivate*>::iterator it = std::find(m_vecUserApplication.begin(),m_vecUserApplication.end(),userApplication);
		if (it!=m_vecUserApplication.end())
		{
			m_vecUserApplication.erase(it); 
		}
	}
	
	void enableSpeedMonitor(bool bSpeed) {
		m_monitorSpeed = bSpeed;
	}

	void enableSyncTime(bool bSync) {
		m_syncTime = bSync;
	}

private:
	UserAmbassador();

	void createUserLoginClass();
	void createUserAccessClass();
	void createUserLogoutClass();

	void createEnterTestClass();
	void createExitTestClass();
	void createTestLicenceClass();

	void createResUpdateClass();

	void createNewTestClass();
	void createTestAccessClass();
	void createCloseTestClass();
	void createTestUpdateClass();

	void createUserMonitorClass();

	void createExitTestReally();

	static void userAccessProc(InterationInstanceImpl* interationInstance);
	static void testLicenceProc(InterationInstanceImpl* interationInstance);

	static void resUpdateProc(InterationInstanceImpl* interationInstance);

	static void userMonitorProc(InterationInstanceImpl* interationInstance);

	static void testAccessProc(InterationInstanceImpl* interationInstance);
	static void testUpdateProc(InterationInstanceImpl* interationInstance);

	static void exitTestReallyProc(InterationInstanceImpl* interationInstance);

	void receiveInterationInstance(InterationInstanceImpl* interationInstance);

	void monitorTimeout();

private:
	static UserAmbassador* m_instance;
	std::vector<UserApplicationPrivate*>  m_vecUserApplication;

	boost::mutex m_userMutex;
	boost::condition_variable m_userCondition;

	boost::mutex m_testLicenceMutex;
	boost::condition_variable m_testLicenceCondition;

	boost::mutex m_testAccessMutex;
	boost::condition_variable m_testAccessCondition;

	boost::thread m_monitor;
	boost::asio::io_service m_io;
	boost::asio::deadline_timer m_timer;

	bool m_monitorSpeed;
	bool m_syncTime;

	static __int64  pretime;
	static double lost;
	static __int64 num;
};

