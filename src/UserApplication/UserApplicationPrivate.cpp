#include "UserApplicationPrivate.h"
#include "UserAmbassador.h"
#include <Ambassador.h>
#include <TimeAmbassador.h>
#include <TimerInstance.h>

namespace Data_Exchange_Platform {

	class TestAmbassador: public TimeAmbassador
	{
	public:
		TestAmbassador(long domain_ID,UserApplication* userApplication)
			:TimeAmbassador(domain_ID),m_userApplication(userApplication){}

		void receiveInterationInstance(InterationInstanceImpl* interationInstance){
			TimeAmbassador::receiveInterationInstance(interationInstance);
			m_userApplication->receiveInterationInstance(interationInstance);
		}

		void disconnected(std::string classname){
			m_userApplication->disconnected(classname);
		}
	private:
		UserApplication* m_userApplication;
	};

	UserApplicationPrivate::UserApplicationPrivate(UserApplication* publicPtr)
		:m_testAmbassador(nullptr) , m_publicPtr(publicPtr), m_online(false), m_ownTest(false)
	{
		UserAmbassador::instance()->attachUserApplication(this);
	}


	UserApplicationPrivate::~UserApplicationPrivate()
	{
		UserAmbassador::instance()->detachUserApplication(this);
		if (m_testAmbassador)
		{
			delete m_testAmbassador;
		}
	}

	bool UserApplicationPrivate::login(std::string username, std::string password)
	{
		m_username = username;
		m_password = password;
		bool bret = UserAmbassador::instance()->login(m_username, m_password);
		if (!bret)
		{
			setLastError("login timeout!");
		}
		return isOnline();
	}

	bool UserApplicationPrivate::logout()
	{
		if (!isOnline())
		{
			setLastError("user is not online.");
			return false;
		}
		UserAmbassador::instance()->logout(m_username);
		errorClear();
		return true;
	}

	std::string UserApplicationPrivate::createTest(std::string testname, std::string testpassword, std::string description, std::vector<std::string> resid, std::vector<std::string> exparam)
	{
		if (isOnline())
		{
			if(ownTest())
			{
				setLastError("user created a test already.");
				return std::string();
			}
			bool bret = UserAmbassador::instance()->createTest(m_username, testname, testpassword, description, resid, exparam);
			if (!bret)
			{
				setLastError("createTest timeout!");
			}
			if (!m_testid.empty())
			{
				m_ownTest = true;
			}
			return m_testid;
		}
		setLastError("user is not online.");
		return std::string();
	}

	bool UserApplicationPrivate::enterTest(std::string testid, std::string testpassword, std::vector<std::string> exparam )
	{
		if (isOnline())
		{
			if (inTest())
			{
				setLastError("user is already in test.");
				return false;
			}
			
			if (!ownTest() || (ownTest() && m_testid == testid))
			{
				m_testid = testid;
				m_testpassword = testpassword;
				bool bret = UserAmbassador::instance()->enterTest(m_username, m_testid, m_testpassword, exparam);
				if (!bret)
				{
					setLastError("enterTest timeout!");
				}
				return inTest();
			}
			else
			{
				setLastError("user created a test already.");
				return false;
			}
		}
		setLastError("user is not online.");
		return false;
	}

	bool UserApplicationPrivate::exitTest()
	{
		if (inTest())
		{
			UserAmbassador::instance()->exitTest(m_username, m_testid);
			leaveTestDomain();
			return true;
		}
		setLastError("user is not in test.");
		return false;
	}

	bool UserApplicationPrivate::closeTest()
	{
		if (ownTest())
		{
			UserAmbassador::instance()->closeTest(m_username, m_testid);
			leaveTestDomain();
			 
			return true;
		}
		setLastError("user do not own test.");
		return false;
	}

	bool UserApplicationPrivate::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->registerObjectInstance(objectInstance);
		}
		return false;
	}

	bool UserApplicationPrivate::declareInterationClass(InterationClass * interationClass)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->declareInterationClass(interationClass);
		}
		return false;
	}

	bool UserApplicationPrivate::sendInterationInstance(InterationInstanceImpl * interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstance(interationInstance);
		}
		return false;
	}

	bool UserApplicationPrivate::sendInterationInstanceByTime(InterationInstanceImpl * interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstanceByTime(interationInstance);
		}
		return false;
	}

	bool UserApplicationPrivate::sendInterationInstanceByTime(std::vector<InterationInstanceImpl *> vec_interationInstance)
	{
		if (m_testAmbassador)
		{
			return m_testAmbassador->sendInterationInstanceByTime(vec_interationInstance);
		}
		return false;
	}

	void UserApplicationPrivate::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		if (m_testAmbassador)
		{
			m_testAmbassador->registerInterationReceiveProc(recvfunc);
		}
	}

	void UserApplicationPrivate::joinTestDomain(long domain)
	{
		m_testAmbassador = new TestAmbassador(domain,m_publicPtr);
		m_publicPtr->joinTest();
	}

	void UserApplicationPrivate::leaveTestDomain()
	{
		if (m_testAmbassador)
		{
			delete m_testAmbassador;
			m_testAmbassador = nullptr;
			m_publicPtr->leaveTest();
		}
	}

	void UserApplicationPrivate::registerTimer( TimerInstance* timer )
	{
		if ( timer != nullptr) 
		{
			m_testAmbassador->registerTimer(timer);
			timer->setUser(m_username, m_testid);
		}
	}

	UserApplication::ResourceInfoList UserApplicationPrivate::resourceList( std::string filtername/*=""*/ )
	{
		if (filtername=="")
		{
			return m_resourceList;
		}

		UserApplication::ResourceInfoList resLst;
		for (int i = 0 ; i < m_resourceList.size(); i++)
		{
			if(m_resourceList[i].resname==filtername)
			{
				resLst.push_back(m_resourceList[i]);
			}
		}
		
		return resLst;
	}

	UserApplication::ResourceInfo UserApplicationPrivate::resourceInfo(std::string resid)
	{
		for (int i = 0; i < m_resourceList.size(); i++)
		{
			if (m_resourceList[i].guid == resid)
			{
				return m_resourceList[i];
			}
		}

		return UserApplication::ResourceInfo();
	}

	void UserApplicationPrivate::addResource( UserApplication::ResourceInfo resInfo )
	{
		for (int i = 0; i < m_resourceList.size(); i++)
		{
			if(m_resourceList[i].resname==resInfo.resname&&
				m_resourceList[i].guid==resInfo.guid)
			{
				return;
			}
		}
		m_resourceList.push_back(resInfo);
	}

	void UserApplicationPrivate::delResource( UserApplication::ResourceInfo resInfo )
	{
		for (UserApplication::ResourceInfoList::iterator it = m_resourceList.begin();
			it != m_resourceList.end(); it++ )
		{
			if (it->guid==resInfo.guid&&it->resname==resInfo.resname)
			{
				m_resourceList.erase(it);
				m_publicPtr->resourceUpdated(resInfo, UserApplication::Delete);
				return;
			}
		}
	}

	void UserApplicationPrivate::updateResource( UserApplication::ResourceInfo resInfo )
	{
		int index = 0 ;
		for (; index < m_resourceList.size(); index++)
		{
			if(m_resourceList[index].resname==resInfo.resname&&
				m_resourceList[index].guid==resInfo.guid)
			{
				m_resourceList[index].enabled = resInfo.enabled;
				m_resourceList[index].exparam = resInfo.exparam;
				break;
			}
		}
		if (index==m_resourceList.size())
		{
			m_resourceList.push_back(resInfo);
			m_publicPtr->resourceUpdated(resInfo, UserApplication::Insert);
		}
		else
		{
			m_publicPtr->resourceUpdated(resInfo, UserApplication::Update);
		}
	}

	UserApplication::TestInfoList UserApplicationPrivate::testList(std::string filtername)
	{
		if (filtername == "")
		{
			return m_testList;
		}

		UserApplication::TestInfoList testLst;
		for (int i = 0; i < m_testList.size(); i++)
		{
			for (int j = 0; j < m_testList[i].resources.size(); j++)
			{
				if (m_testList[i].resources[j].resname == filtername)
				{
					testLst.push_back(m_testList[i]);
					break;
				}
			}
		}

		return testLst;
		
	}

	UserApplication::TestInfo UserApplicationPrivate::testInfo(std::string testid)
	{
		for (int i = 0; i < m_testList.size(); i++)
		{
			if (m_testList[i].guid == testid)
			{
				return m_testList[i];
			}
		}

		return UserApplication::TestInfo();
	}

	void UserApplicationPrivate::addTest(UserApplication::TestInfo testInfo)
	{
		for (int i = 0; i < m_testList.size(); i++)
		{
			if (m_testList[i].guid == testInfo.guid)
			{
				return;
			}
		}
		m_testList.push_back(testInfo);
	}

	void UserApplicationPrivate::delTest(UserApplication::TestInfo testInfo)
	{
		for (UserApplication::TestInfoList::iterator it = m_testList.begin();
			it != m_testList.end(); it++)
		{
			if (it->guid == testInfo.guid)
			{
				m_testList.erase(it);
				m_publicPtr->testUpdated(testInfo, UserApplication::Delete);
				return;
			}
		}
	}

	void UserApplicationPrivate::updateTest(UserApplication::TestInfo testInfo)
	{
		int index = 0;
		for (; index < m_testList.size(); index++)
		{
			if (m_testList[index].guid == testInfo.guid)
			{
				m_testList[index].description = testInfo.description;
				m_testList[index].users = testInfo.users;
				m_testList[index].resources = testInfo.resources;
				m_testList[index].exparam = testInfo.exparam;
				break;
			}
		}
		if (index == m_testList.size())
		{
			m_testList.push_back(testInfo);
			m_publicPtr->testUpdated(testInfo, UserApplication::Insert);
		}
		else
		{
			m_publicPtr->testUpdated(testInfo, UserApplication::Update);
		}
		
	}

	void UserApplicationPrivate::monitor(int delay, double lost, double speed)
	{
		m_publicPtr->monitor(delay, lost, speed);
	}

	std::string UserApplicationPrivate::getLastError()
	{
		return m_lastError;
	}

	void UserApplicationPrivate::setLastError(std::string lastError)
	{
		m_lastError = lastError;
	}

	void UserApplicationPrivate::errorClear()
	{
		m_lastError.clear();
	}

	void UserApplicationPrivate::log(std::string msg)
	{
		UserAmbassador::instance()->log(msg);
	}

	void UserApplicationPrivate::receiveInterationInstance(InterationInstanceImpl* interationInstance)
	{
		m_publicPtr->receiveInterationInstance(interationInstance);
	}

	void UserApplicationPrivate::testLicenceProc(InterationInstanceImpl * interationInstance)
	{
		m_publicPtr->testLicenceProc(interationInstance);
	}

	void UserApplicationPrivate::userAccessProc(InterationInstanceImpl * interationInstance)
	{
		m_publicPtr->userAccessProc(interationInstance);
	}

	void UserApplicationPrivate::userMonitor(InterationInstanceImpl * interationInstance)
	{
		m_publicPtr->userMonitor(interationInstance);
	}
}

