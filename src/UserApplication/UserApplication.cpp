#include "UserApplication.h"
#include "UserApplicationPrivate.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include "UserAmbassador.h"
using namespace std;

namespace Data_Exchange_Platform {

	UserApplication::UserApplication()
		:m_private(new UserApplicationPrivate(this))
	{		
	}

	UserApplication::~UserApplication()
	{
		delete m_private;
	}

	bool UserApplication::login(std::string username, std::string password)
	{
		return m_private->login(username, password);
	}

	bool UserApplication::logout()
	{
		return m_private->logout();
	}

	std::string UserApplication::createTest(std::string testname, std::string testpassword, std::string description, std::vector<std::string> resid, std::vector<std::string> exparam)
	{
		return m_private->createTest(testname, testpassword, description, resid, exparam);
	}

	bool UserApplication::enterTest(std::string testid, std::string testpassword, std::vector<std::string> exparam )
	{
		return m_private->enterTest(testid, testpassword, exparam);
	}

	bool UserApplication::enterOrCreateTest(std::string testname, std::string testpassword, std::vector<std::string> resid)
	{
		TestInfoList tl = testList();
		for (int i = 0; i < tl.size() ; i++)
		{
			if (tl[i].testname == testname)
			{
				return enterTest(tl[i].guid, testpassword);
			}
		}
		std::string testid = createTest(testname, testpassword, "", resid);
		if (!testid.empty() || testid != "")
		{
			return enterTest(testid, testpassword);
		}
		return false;
	}

	bool UserApplication::exitTest()
	{
		return m_private->exitTest();
	}

	bool UserApplication::closeTest()
	{
		return m_private->closeTest();
	}

	UserApplication::TestInfoList UserApplication::testList(std::string filtername)
	{
		return m_private->testList(filtername);
	}

	UserApplication::TestInfo UserApplication::testInfo(std::string testid)
	{
		return m_private->testInfo(testid);
	}

	void UserApplication::testUpdated(TestInfo testinfo, UDATE_OPERATION)
	{
	}

	std::string UserApplication::getLastError()
	{
		return m_private->getLastError();
	}

	void UserApplication::updateTestUserExtendedParameter(int index, std::string param)
	{
	}

	void UserApplication::updateTestExtendedParameter(int index, std::string param)
	{
	}

	void UserApplication::joinTest()
	{

	}

	void UserApplication::leaveTest()
	{

	}

	bool UserApplication::registerObjectInstance(ObjectInstanceImpl * objectInstance)
	{
		return m_private->registerObjectInstance(objectInstance);
	}

	bool UserApplication::declareInterationClass(InterationClass * interationClass)
	{
		return m_private->declareInterationClass(interationClass);
	}

	bool UserApplication::sendInterationInstance(InterationInstanceImpl * interationInstance)
	{
		return m_private->sendInterationInstance(interationInstance);
	}

	bool UserApplication::sendInterationInstanceByTime(InterationInstanceImpl * interationInstance)
	{
		return m_private->sendInterationInstanceByTime(interationInstance);
	}

	bool UserApplication::sendInterationInstanceByTime(std::vector<InterationInstanceImpl*> vec_interationInstance)
	{
		return m_private->sendInterationInstanceByTime(vec_interationInstance);
	}

	void UserApplication::registerInterationReceiveProc(InterationRecvFunc recvfunc)
	{
		m_private->registerInterationReceiveProc(recvfunc);
	}

	void UserApplication::log(std::string msg)
	{
		m_private->log(msg);
	}

	UserApplication::ResourceInfoList UserApplication::resourceList( std::string filtername/*=""*/ )
	{
		return m_private->resourceList(filtername);
	}

	UserApplication::ResourceInfo UserApplication::resourceInfo(std::string resid)
	{
		return m_private->resourceInfo(resid);
	}

	void UserApplication::resourceUpdated(ResourceInfo resinfo, UDATE_OPERATION)
	{
	}

	void UserApplication::disconnected(std::string classname)
	{

	}

	void UserApplication::registerTimer( TimerInstance* timer )
	{
		m_private->registerTimer(timer);
	}

	void UserApplication::monitor(int delay, double lost, double speed)
	{
	}

	void UserApplication::enableSpeedMonitor(bool bSpeed)
	{
		UserAmbassador::instance()->enableSpeedMonitor(bSpeed);
	}

	void UserApplication::enableSyncTime(bool bSync)
	{
		UserAmbassador::instance()->enableSyncTime(bSync);
	}

	void UserApplication::receiveInterationInstance(InterationInstanceImpl * interationInstance)
	{
	}

	void UserApplication::testLicenceProc(InterationInstanceImpl * interationInstance)
	{

	}
	void UserApplication::userAccessProc(InterationInstanceImpl * interationInstance)
	{

	}
	void UserApplication::userMonitor(InterationInstanceImpl * interationInstance)
	{

	}

}
