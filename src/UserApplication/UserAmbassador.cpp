#include "UserAmbassador.h"
#include <InterationClass.h>
#include <InterationInstanceImpl.h>
#include <common.h>
#include <iostream>
#include <boost\thread\thread_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

UserAmbassador* UserAmbassador::m_instance = nullptr;

__int64  UserAmbassador::pretime = 0;
double UserAmbassador::lost = 0;
__int64 UserAmbassador::num = 0;

UserAmbassador::UserAmbassador()
	:Ambassador(0x000000FF), m_monitorSpeed(false), m_syncTime(false),
	m_timer(m_io, boost::posix_time::seconds(3))
{
	createUserLoginClass();
	createUserAccessClass();
	createUserLogoutClass();
	createEnterTestClass();
	createExitTestClass();
	createTestLicenceClass();

	createResUpdateClass();

	createNewTestClass();
	createTestAccessClass();
	createCloseTestClass();
	createTestUpdateClass();

	createUserMonitorClass();

	createExitTestReally();

	registerInterationReceiveProc(userAccessProc);
	registerInterationReceiveProc(testLicenceProc);
	registerInterationReceiveProc(userMonitorProc);
	
	registerInterationReceiveProc(resUpdateProc);

	registerInterationReceiveProc(testAccessProc);
	registerInterationReceiveProc(testUpdateProc);

	registerInterationReceiveProc(exitTestReallyProc);

	//waitConnection();

	m_timer.async_wait(boost::bind(&UserAmbassador::monitorTimeout, this));
	m_monitor = boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));
}

void UserAmbassador::createUserLoginClass()
{
	InterationClass* userLogin = new InterationClass("__User_Login__", Publish_Only);
	userLogin->addParameter("username", DATATYPE(std::string));
	userLogin->addParameter("password", DATATYPE(std::string));
	declareInterationClass(userLogin);
}

void UserAmbassador::createUserAccessClass()
{
	InterationClass* userAccess = new InterationClass("__User_Access__", Subscribe_Only);
	userAccess->addParameter("username", DATATYPE(std::string));
	userAccess->addParameter("access", DATATYPE(bool));
	userAccess->addParameter("message", DATATYPE(std::string));

	userAccess->addParameter("res_vec_id", DATATYPE(std::vector<std::string>));
	userAccess->addParameter("res_vec_name", DATATYPE(std::vector<std::string>));
	userAccess->addParameter("res_vec_exparam", DATATYPE(std::vector<std::vector<std::string>>));

	userAccess->addParameter("test_vec_id", DATATYPE(std::vector<std::string>));
	userAccess->addParameter("test_vec_name", DATATYPE(std::vector<std::string>));
	userAccess->addParameter("test_vec_description", DATATYPE(std::vector<std::string>));
	userAccess->addParameter("test_vec_user", DATATYPE(std::vector<std::vector<std::string>>));
	userAccess->addParameter("test_vec_resid", DATATYPE(std::vector<std::vector<std::string>>));
	userAccess->addParameter("test_vec_exparam", DATATYPE(std::vector<std::vector<std::string>>));

	declareInterationClass(userAccess);
}

void UserAmbassador::createUserLogoutClass()
{
	InterationClass* userLogout = new InterationClass("__User_Logout__", Publish_Only);
	userLogout->addParameter("username", DATATYPE(std::string));
	declareInterationClass(userLogout);
}

void UserAmbassador::createEnterTestClass()
{
	InterationClass* enterTest = new InterationClass("__Enter_Test__", Publish_Only);
	enterTest->addParameter("username", DATATYPE(std::string));
	enterTest->addParameter("testid", DATATYPE(std::string));
	enterTest->addParameter("password", DATATYPE(std::string));
	enterTest->addParameter("exparam", DATATYPE(std::vector<std::string>));
	declareInterationClass(enterTest);
}

void UserAmbassador::createExitTestClass()
{
	InterationClass* exitTest = new InterationClass("__Exit_Test__", Publish_Only);
	exitTest->addParameter("username", DATATYPE(std::string));
	exitTest->addParameter("testid", DATATYPE(std::string));
	declareInterationClass(exitTest);
}

void UserAmbassador::createTestLicenceClass()
{
	InterationClass* testLicence = new InterationClass("__Test_Licence__", Subscribe_Only);
	testLicence->addParameter("username", DATATYPE(std::string));
	testLicence->addParameter("testid", DATATYPE(std::string));
	testLicence->addParameter("domain", DATATYPE(long));
	testLicence->addParameter("message", DATATYPE(std::string));
	declareInterationClass(testLicence);
}

void UserAmbassador::createResUpdateClass()
{
	InterationClass* resUpdate = new InterationClass("__Resource_Update__", Subscribe_Only);
	resUpdate->addParameter("id", DATATYPE(std::string));
	resUpdate->addParameter("resname", DATATYPE(std::string));
	resUpdate->addParameter("exparam", DATATYPE(std::vector<std::string>));
	resUpdate->addParameter("enable", DATATYPE(bool));
	resUpdate->addParameter("exist", DATATYPE(bool));
	resUpdate->addParameter("count", DATATYPE(int));
	declareInterationClass(resUpdate);
}

void UserAmbassador::createNewTestClass()
{
	InterationClass* newTest = new InterationClass("__New_Test__", Publish_Only);
	newTest->addParameter("username", DATATYPE(std::string));
	newTest->addParameter("testname", DATATYPE(std::string));
	newTest->addParameter("password", DATATYPE(std::string));
	newTest->addParameter("description", DATATYPE(std::string));
	newTest->addParameter("resid", DATATYPE(std::vector<std::string>));
	newTest->addParameter("exparam", DATATYPE(std::vector<std::string>));
	declareInterationClass(newTest);
}

void UserAmbassador::createTestAccessClass()
{
	InterationClass* testAccess = new InterationClass("__Test_Access__", Subscribe_Only);
	testAccess->addParameter("username", DATATYPE(std::string));
	testAccess->addParameter("testname", DATATYPE(std::string));
	testAccess->addParameter("testid", DATATYPE(std::string));
	testAccess->addParameter("access", DATATYPE(bool));
	testAccess->addParameter("message", DATATYPE(std::string));
	declareInterationClass(testAccess);
}

void UserAmbassador::createCloseTestClass()
{
	InterationClass* closeTest = new InterationClass("__Close_Test__", Publish_Only);
	closeTest->addParameter("username", DATATYPE(std::string));
	closeTest->addParameter("testid", DATATYPE(std::string));
	declareInterationClass(closeTest);
}

void UserAmbassador::createTestUpdateClass()
{
	InterationClass* testUpdate = new InterationClass("__Test_Update__", Subscribe_Only);
	testUpdate->addParameter("testid", DATATYPE(std::string));
	testUpdate->addParameter("testname", DATATYPE(std::string));
	testUpdate->addParameter("description", DATATYPE(std::string));
	testUpdate->addParameter("username", DATATYPE(std::vector<std::string>));
	testUpdate->addParameter("user_exparam", DATATYPE(std::vector<std::vector<std::string>>));
	testUpdate->addParameter("resid", DATATYPE(std::vector<std::string>));
	testUpdate->addParameter("exparam", DATATYPE(std::vector<std::string>));
	testUpdate->addParameter("exist", DATATYPE(bool));
	testUpdate->addParameter("count", DATATYPE(int));
	declareInterationClass(testUpdate);
}

void UserAmbassador::createUserMonitorClass()
{
	InterationClass* userMonitor = new InterationClass("__User_Monitor__", Publish_Subscribe);
	userMonitor->addParameter("username", DATATYPE(std::string));
	userMonitor->addParameter("timeStamp", DATATYPE(__int64));
	userMonitor->addParameter("launcher_timeStamp", DATATYPE(__int64));
	userMonitor->addParameter("receipted", DATATYPE(bool));
	userMonitor->addParameter("placeholder", DATATYPE(std::string));
	declareInterationClass(userMonitor);
}

void UserAmbassador::createExitTestReally()
{
	InterationClass *exitTestReally = new InterationClass("_EXIT_TEST_REALLY", Subscribe_Only);
	exitTestReally->addParameter("testid", DATATYPE(std::string));
	exitTestReally->addParameter("username", DATATYPE(std::string));
	declareInterationClass(exitTestReally);
}

void UserAmbassador::userAccessProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__User_Access__"))
	{
		
		Variant username = interationInstance->parameterValue("username");
		Variant access = interationInstance->parameterValue("access");
		Variant message = interationInstance->parameterValue("message");
	
		Variant res_var_id = interationInstance->parameterValue("res_vec_id");
		Variant res_var_name = interationInstance->parameterValue("res_vec_name");
		Variant res_var_exparam = interationInstance->parameterValue("res_vec_exparam");

		Variant test_var_id = interationInstance->parameterValue("test_vec_id");
		Variant test_var_name = interationInstance->parameterValue("test_vec_name");
		Variant test_var_description = interationInstance->parameterValue("test_vec_description");
		Variant test_var_user = interationInstance->parameterValue("test_vec_user");
		Variant test_var_resid = interationInstance->parameterValue("test_vec_resid");
		Variant test_var_exparam = interationInstance->parameterValue("test_vec_exparam");

		std::vector<std::string> res_vec_id = boost::get<std::vector<std::string>>(res_var_id);
		std::vector<std::string> res_vec_name = boost::get<std::vector<std::string>>(res_var_name);
		std::vector<std::vector<std::string>> res_vec_exparam = boost::get<std::vector<std::vector<std::string>>>(res_var_exparam);

		std::vector<std::string> test_vec_id = boost::get<std::vector<std::string>>(test_var_id);
		std::vector<std::string> test_vec_name = boost::get<std::vector<std::string>>(test_var_name);
		std::vector<std::string> test_vec_description = boost::get<std::vector<std::string>>(test_var_description);
		std::vector<std::vector<std::string>> test_vec_user = boost::get<std::vector<std::vector<std::string>>>(test_var_user);
		std::vector<std::vector<std::string>> test_vec_resid = boost::get<std::vector<std::vector<std::string>>>(test_var_resid);
		std::vector<std::vector<std::string>> test_vec_exparam = boost::get<std::vector<std::vector<std::string>>>(test_var_exparam);

		for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
		{
			if (instance()->m_vecUserApplication[i]->username() == boost::get<std::string>(username))
			{
				boost::mutex::scoped_lock lock(m_instance->m_userMutex);
				if (boost::get<bool>(access))
				{
					for (size_t r = 0; r < res_vec_id.size(); r++)
					{
						UserApplication::ResourceInfo resInfo;
						resInfo.guid = res_vec_id[r];
						resInfo.resname = res_vec_name[r];
						resInfo.exparam = res_vec_exparam[r];
						resInfo.enabled = true;
						instance()->m_vecUserApplication[i]->addResource(resInfo);
					}

					for (size_t t = 0; t < test_vec_id.size(); t++)
					{
						UserApplication::TestInfo testInfo;
						testInfo.guid = test_vec_id[t];
						testInfo.testname = test_vec_name[t];
						testInfo.description = test_vec_description[t];
						testInfo.exparam = test_vec_exparam[t];

						for (size_t u = 0; u < test_vec_user[t].size(); u++)
						{
							UserApplication::UserInfo userInfo;
							userInfo.name = test_vec_user[t][u];
							testInfo.users.push_back(userInfo);
						}

						for (size_t r = 0; r < test_vec_resid[t].size(); r++)
						{
							UserApplication::ResourceInfoList resList = instance()->m_vecUserApplication[i]->resourceList();
							for (size_t lr = 0; lr < resList.size(); lr++)
							{
								if (resList[lr].guid== test_vec_resid[t][r])
								{
									testInfo.resources.push_back(resList[lr]);
								}
							}
						}

						instance()->m_vecUserApplication[i]->addTest(testInfo);
					}

					instance()->m_vecUserApplication[i]->permit();
					instance()->m_vecUserApplication[i]->errorClear();
				}
				else
				{
					instance()->m_vecUserApplication[i]->setLastError(boost::get<std::string>(message));	
				}
				m_instance->m_userCondition.notify_one();
				break;
			}
		}

		if (instance()->m_vecUserApplication.size() >0)

		{
			instance()->m_vecUserApplication[0]->testLicenceProc(interationInstance);
		}

	}
}

void UserAmbassador::testLicenceProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Test_Licence__"))
	{
		
		Variant username = interationInstance->parameterValue("username");
		Variant testid = interationInstance->parameterValue("testid");//??
		Variant domain = interationInstance->parameterValue("domain");
		Variant message = interationInstance->parameterValue("message");
	
		for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
		{
			if (instance()->m_vecUserApplication[i]->username() == boost::get<std::string>(username))
			{
				boost::mutex::scoped_lock lock(m_instance->m_testLicenceMutex);
				if (boost::get<long>(domain) > 0)
				{
					if (instance()->m_vecUserApplication[i]->isOnline() &&
						!instance()->m_vecUserApplication[i]->inTest())
					{
						instance()->m_vecUserApplication[i]->joinTestDomain(boost::get<long>(domain));
						instance()->m_vecUserApplication[i]->errorClear();
						//if (instance()->m_vecUserApplication.size() >0)
						//{
						//	instance()->m_vecUserApplication[0]->testLicenceProc(interationInstance);
						//}
					}
					else
					{
						instance()->m_vecUserApplication[i]->setLastError("unknown exception!");
					}
				}
				else
				{
					instance()->m_vecUserApplication[i]->setLastError(boost::get<std::string>(message));
				}
				instance()->m_testLicenceCondition.notify_one();
				break;
			}
		}
		
		if (instance()->m_vecUserApplication.size() >0)
		{
			instance()->m_vecUserApplication[0]->testLicenceProc(interationInstance);
		}
	}
	
}


void UserAmbassador::resUpdateProc( InterationInstanceImpl* interationInstance )
{
	if (interationInstance->interationClass() == instance()->interationClass("__Resource_Update__"))
	{
		Variant id = interationInstance->parameterValue("id");
		Variant resname = interationInstance->parameterValue("resname");
		Variant exparam = interationInstance->parameterValue("exparam");
		Variant enable = interationInstance->parameterValue("enable");
		Variant exist = interationInstance->parameterValue("exist");

		UserApplication::ResourceInfo resInfo;
		resInfo.guid = boost::get<std::string>(id);
		resInfo.resname = boost::get<std::string>(resname);
		resInfo.exparam = boost::get<std::vector<std::string>>(exparam);
		resInfo.enabled = boost::get<bool>(enable);
		
		if (!boost::get<bool>(exist))
		{
			for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
			{
				if (instance()->m_vecUserApplication[i]->isOnline())
					instance()->m_vecUserApplication[i]->delResource(resInfo);
			}
		}
		else{
			for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
			{
				if (instance()->m_vecUserApplication[i]->isOnline())
					instance()->m_vecUserApplication[i]->updateResource(resInfo);
			}
		}
	}
}

void UserAmbassador::userMonitorProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__User_Monitor__"))
	{		
		Variant receipted = interationInstance->parameterValue("receipted");
		if (boost::get<bool>(receipted))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant timeStamp = interationInstance->parameterValue("timeStamp");
			Variant launcher = interationInstance->parameterValue("launcher_timeStamp");

			for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
			{
				if (instance()->m_vecUserApplication[i]->username() == boost::get<std::string>(username))
				{

					boost::system_time systime = boost::get_system_time();

					__int64 curtime = systime.time_of_day().total_milliseconds();

					int srctime = boost::get<__int64>(timeStamp);

					int delay = curtime - srctime;

					__int64 launcher_time = boost::get<__int64>(launcher);
			
					if (instance()->m_syncTime && delay < 2000
						&& ( launcher_time - srctime > delay || launcher_time < srctime) )
					{
						SYSTEMTIME st;
						GetSystemTime(&st);
						st.wHour = launcher_time % (1000 * 60 * 60 * 24) / (1000 * 60 * 60);
						st.wMinute = launcher_time % (1000 * 60 * 60) / (1000 * 60);
						st.wSecond = launcher_time % (1000 * 60) / 1000 ;
						st.wMilliseconds = launcher_time % 1000 + delay / 2;
						SetSystemTime(&st);

						std::stringstream logbuf;
						logbuf << "SetSystemTime " << launcher_time + delay / 2;
						instance()->log(logbuf.str());
					}

					num++;

					if (curtime - pretime>5000 && pretime != 0)
					{
						lost = ((num - 1)*lost + 1 ) / num;
					}
					else
					{
						lost = (num - 1)*lost / num;
					}

					pretime = curtime;

					double speed = 0;
					if (delay > 0)
					{
						if (instance()->m_monitorSpeed)
						{
							speed = 64000.0 / delay;
						}
						else
						{
							speed = 512.0 / delay;
						}
					}

					instance()->m_vecUserApplication[i]->monitor(delay, lost, speed);
			
					std::stringstream logbuf;
					logbuf << "No." << num << " - delay=" << delay << "ms, lost=" << lost * 100 << "%, speed=" << speed << "KB/s";
					instance()->log(logbuf.str());

				}

			}

		}
	}
}

void UserAmbassador::testAccessProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Test_Access__"))
	{
		Variant username = interationInstance->parameterValue("username");
		Variant testname = interationInstance->parameterValue("testname");
		Variant testid = interationInstance->parameterValue("testid");
		Variant access = interationInstance->parameterValue("access");
		Variant message = interationInstance->parameterValue("message");

		for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
		{
			if (instance()->m_vecUserApplication[i]->username() == boost::get<std::string>(username))
			{
				boost::mutex::scoped_lock lock(instance()->m_testAccessMutex);
				if (boost::get<bool>(access))
				{
					if (instance()->m_vecUserApplication[i]->isOnline() &&
						!instance()->m_vecUserApplication[i]->inTest())
					{
						instance()->m_vecUserApplication[i]->setTestID(boost::get<std::string>(testid));
						instance()->m_vecUserApplication[i]->errorClear();
					}
					else
					{
						instance()->m_vecUserApplication[i]->setLastError("unknown exception!");
					}
				}
				else
				{
					instance()->m_vecUserApplication[i]->setLastError(boost::get<std::string>(message));
				}
				instance()->m_testAccessCondition.notify_one();
				break;
			}
		}
	}
}

void UserAmbassador::testUpdateProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Test_Update__"))
	{
		Variant testid = interationInstance->parameterValue("testid");
		Variant testname = interationInstance->parameterValue("testname");
		Variant description = interationInstance->parameterValue("description");
		Variant username = interationInstance->parameterValue("username");
		Variant user_exparam = interationInstance->parameterValue("user_exparam");
		Variant resid = interationInstance->parameterValue("resid");
		Variant exparam = interationInstance->parameterValue("exparam");
		Variant exist = interationInstance->parameterValue("exist");
		Variant count = interationInstance->parameterValue("count");

		UserApplication::TestInfo testInfo;
		testInfo.guid = boost::get<std::string>(testid);
		testInfo.testname = boost::get<std::string>(testname);
		testInfo.description = boost::get<std::string>(description);
		testInfo.exparam = boost::get<std::vector<std::string>>(exparam);


		std::vector<std::string> test_vec_user = boost::get<std::vector<std::string>>(username);
		std::vector<std::vector<std::string>> test_vec_user_exparam = boost::get<std::vector<std::vector<std::string>>>(user_exparam);
		std::vector<std::string> test_vec_resid = boost::get<std::vector<std::string>>(resid);

		for (size_t u = 0; u < test_vec_user.size(); u++)
		{
			UserApplication::UserInfo userInfo;
			userInfo.name = test_vec_user[u];
			userInfo.exparam = test_vec_user_exparam[u];
			testInfo.users.push_back(userInfo);
		}

		if (!boost::get<bool>(exist))
		{
			for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
			{
				if (instance()->m_vecUserApplication[i]->isOnline())
				{
					testInfo.resources.clear();
					for (size_t r = 0; r < test_vec_resid.size(); r++)
					{
						UserApplication::ResourceInfoList resList = instance()->m_vecUserApplication[i]->resourceList();
						for (size_t lr = 0; lr < resList.size(); lr++)
						{
							if (resList[lr].guid == test_vec_resid[r])
							{
								testInfo.resources.push_back(resList[lr]);
							}
						}
					}
					instance()->m_vecUserApplication[i]->delTest(testInfo);
					if (testInfo.guid == instance()->m_vecUserApplication[i]->testID())
					{
						instance()->m_vecUserApplication[i]->leaveTestDomain();
					}
				}		
			}
		}
		else {
			for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
			{
				if (instance()->m_vecUserApplication[i]->isOnline())
				{
					testInfo.resources.clear();
					for (size_t r = 0; r < test_vec_resid.size(); r++)
					{
						UserApplication::ResourceInfoList resList = instance()->m_vecUserApplication[i]->resourceList();
						for (size_t lr = 0; lr < resList.size(); lr++)
						{
							if (resList[lr].guid == test_vec_resid[r])
							{
								testInfo.resources.push_back(resList[lr]);
							}
						}
					}
					instance()->m_vecUserApplication[i]->updateTest(testInfo);
				}
			}
		}
	}
}

void UserAmbassador::exitTestReallyProc(InterationInstanceImpl* interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("_EXIT_TEST_REALLY"))
	{
		for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
		{
		instance()->m_vecUserApplication[i]->receiveInterationInstance(interationInstance);
		}
	}
}

void UserAmbassador::monitorTimeout()
{

	__int64 curts = boost::get_system_time().time_of_day().total_milliseconds();

	char placeholder[64000] ={0};
	if (m_monitorSpeed)
	{
		memset(placeholder, 0xFF, 64000);
		placeholder[63999] = 0x00;
	}

	for (size_t i = 0; i < m_vecUserApplication.size(); i++)
	{
		if (m_vecUserApplication[i]->isOnline())
		{
			InterationInstanceImpl monitorInstance(interationClass("__User_Monitor__"));
			monitorInstance.setParameterValue("username", m_vecUserApplication[i]->username());
			monitorInstance.setParameterValue("timeStamp", curts);
			monitorInstance.setParameterValue("launcher_timeStamp", __int64(0));
			monitorInstance.setParameterValue("receipted", false);
			monitorInstance.setParameterValue("placeholder", std::string(placeholder));
			sendInterationInstance(&monitorInstance);
		}
	}

	m_timer.expires_at(m_timer.expires_at() + boost::posix_time::seconds(3));
	m_timer.async_wait(boost::bind(&UserAmbassador::monitorTimeout, this));
}

UserAmbassador::~UserAmbassador()
{
}

UserAmbassador * UserAmbassador::instance()
{
	if (!m_instance)
	{
		m_instance = new UserAmbassador;
	}
	return m_instance;
}

bool UserAmbassador::login(std::string username, std::string password)
{
	boost::mutex::scoped_lock lock(m_userMutex);

	log("login beg " + username + "," + password);

	InterationInstanceImpl userLoginInstance(instance()->interationClass("__User_Login__"));
	userLoginInstance.setParameterValue("username", username);
	userLoginInstance.setParameterValue("password", password);
	sendInterationInstance(&userLoginInstance);

	bool bRet = m_userCondition.timed_wait(lock,boost::get_system_time()+boost::posix_time::seconds(30));

	log("login end");

	return bRet;
}

void UserAmbassador::logout(std::string username)
{
	InterationInstanceImpl userLogoutInstance(instance()->interationClass("__User_Login__"));
	userLogoutInstance.setParameterValue("username", username);
	sendInterationInstance(&userLogoutInstance);
}

bool UserAmbassador::createTest(std::string username, std::string testname, std::string testpassword, std::string description, std::vector<std::string> resid, std::vector<std::string> exparam)
{

	boost::mutex::scoped_lock lock(m_testAccessMutex);

	log("createtest beg " + testname );

	InterationInstanceImpl newTestInstance(instance()->interationClass("__New_Test__"));
	newTestInstance.setParameterValue("username", username);
	newTestInstance.setParameterValue("testname", testname);
	newTestInstance.setParameterValue("password", testpassword);
	newTestInstance.setParameterValue("description", description);
	newTestInstance.setParameterValue("resid", resid);
	newTestInstance.setParameterValue("exparam", exparam);
	sendInterationInstance(&newTestInstance);

	bool bRet = m_testAccessCondition.timed_wait(lock, boost::get_system_time() + boost::posix_time::seconds(30));

	if (bRet)
	{
		log("createtest end true");
	}
	else
	{
		log("createtest end false");
	}

	return bRet;
}

bool UserAmbassador::enterTest(std::string username,std::string testid, std::string testpassword, std::vector<std::string> exparam)
{
	boost::mutex::scoped_lock lock(m_testLicenceMutex);

	log("entertest beg " + testid);

	InterationInstanceImpl enterTestInstance(instance()->interationClass("__Enter_Test__"));
	enterTestInstance.setParameterValue("username", username);
	enterTestInstance.setParameterValue("testid", testid);
	enterTestInstance.setParameterValue("password", testpassword);
	enterTestInstance.setParameterValue("exparam", exparam);
	sendInterationInstance(&enterTestInstance);

	bool bRet = m_testLicenceCondition.timed_wait(lock,boost::get_system_time()+boost::posix_time::seconds(5));

	log("entertest end");

	return bRet;
}

void UserAmbassador::exitTest(std::string username, std::string testid)
{
	InterationInstanceImpl exitTestInstance(interationClass("__Exit_Test__"));
	exitTestInstance.setParameterValue("username", username);
	exitTestInstance.setParameterValue("testid", testid);
	sendInterationInstance(&exitTestInstance);
}

void UserAmbassador::closeTest(std::string username, std::string testid)
{
	InterationInstanceImpl closeTestInstance(interationClass("__Close_Test__"));
	closeTestInstance.setParameterValue("username", username);
	closeTestInstance.setParameterValue("testid", testid);
	sendInterationInstance(&closeTestInstance);
}

void UserAmbassador::receiveInterationInstance(InterationInstanceImpl* interationInstance)
{

	//Ambassador::receiveInterationInstance(interationInstance);
	/*for (size_t i = 0; i < instance()->m_vecUserApplication.size(); i++)
	{
		m_vecUserApplication[i]->receiveInterationInstance(interationInstance);
	}*/
}

