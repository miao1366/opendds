#include "ManagerAmbassador.h"
#include <InterationClass.h>
#include <InterationInstanceImpl.h>
#include <common.h>

#include "User.h"
#include "Resource.h"
#include "Test.h"

#include "DBHelper.h"

namespace Data_Exchange_Platform
{
	ManagerAmbassador* ManagerAmbassador::m_instance = nullptr;

	ManagerAmbassador::ManagerAmbassador()
		:Ambassador(0x000000FF), m_domain_index(0x0000FF00),
		m_userThread(boost::bind(&ManagerAmbassador::userMonitorThread,this)),
		m_resThread(boost::bind(&ManagerAmbassador::resMonitorThread, this))
	{
		createUserLoginClass();
		createResLoginClass();

		createUserAccessClass();
		createResAccessClass();
		createResUpdateClass();

		createUserLogoutClass();
		createResLogoutClass();

		createEnterTestClass();
		createExitTestClass();
		createTestLicenceClass();

		createNewTestClass();
		createTestAccessClass();
		createCloseTestClass();
		createTestUpdateClass();
		createResRequestClass();
		createResReplyClass();

		createUserMonitorClass();
		createResMonitorClass();

		createExitTestReally();

		registerInterationReceiveProc(userLoginProc);
		registerInterationReceiveProc(resLoginProc);
		registerInterationReceiveProc(resUpdateProc);

		registerInterationReceiveProc(userLogoutProc);
		registerInterationReceiveProc(resLogoutProc);

		registerInterationReceiveProc(enterTestProc);
		registerInterationReceiveProc(exitTestProc);

		registerInterationReceiveProc(newTestProc);
		registerInterationReceiveProc(closeTestProc);
		registerInterationReceiveProc(resReplyProc);

		registerInterationReceiveProc(userMonitorProc);
		registerInterationReceiveProc(resMonitorProc);

	}

	ManagerAmbassador::~ManagerAmbassador()
	{

	}

	ManagerAmbassador * ManagerAmbassador::instance()
	{
		if (!m_instance)
		{
			m_instance = new ManagerAmbassador;
		}
		return m_instance;
	}
	
	void ManagerAmbassador::createUserLoginClass()
	{
		InterationClass* userLogin = new InterationClass("__User_Login__", Subscribe_Only);
		userLogin->addParameter("username", DATATYPE(std::string));
		userLogin->addParameter("password", DATATYPE(std::string));
		declareInterationClass(userLogin);
	}

	void ManagerAmbassador::createResLoginClass()
	{
		InterationClass* resLogin = new InterationClass("__Resource_Login__", Subscribe_Only);
		resLogin->addParameter("id", DATATYPE(std::string));
		resLogin->addParameter("resname", DATATYPE(std::string));
		resLogin->addParameter("exparam", DATATYPE(std::vector<std::string>));
		declareInterationClass(resLogin);
	}
	void ManagerAmbassador::createUserAccessClass()
	{
		InterationClass* userAccess = new InterationClass("__User_Access__", Publish_Only);
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
	void ManagerAmbassador::createResAccessClass()
	{
		InterationClass* resAccess = new InterationClass("__Resource_Access__", Publish_Only);
		resAccess->addParameter("id", DATATYPE(std::string));
		resAccess->addParameter("resname", DATATYPE(std::string));
		resAccess->addParameter("access", DATATYPE(bool));
		resAccess->addParameter("message", DATATYPE(std::string));
		declareInterationClass(resAccess);
	}

	void ManagerAmbassador::createResUpdateClass()
	{
		InterationClass* resUpdate = new InterationClass("__Resource_Update__", Publish_Subscribe);
		resUpdate->addParameter("id", DATATYPE(std::string));
		resUpdate->addParameter("resname", DATATYPE(std::string));
		resUpdate->addParameter("exparam", DATATYPE(std::vector<std::string>));
		resUpdate->addParameter("enable", DATATYPE(bool));
		resUpdate->addParameter("exist", DATATYPE(bool));
		declareInterationClass(resUpdate);
	}

	void ManagerAmbassador::createUserLogoutClass()
	{
		InterationClass* userLogout = new InterationClass("__User_Logout__", Subscribe_Only);
		userLogout->addParameter("username", DATATYPE(std::string));
		declareInterationClass(userLogout);
	}
	void ManagerAmbassador::createResLogoutClass()
	{
		InterationClass* resLogout = new InterationClass("__Resource_Logout__", Subscribe_Only);
		resLogout->addParameter("id", DATATYPE(std::string));
		resLogout->addParameter("resname", DATATYPE(std::string));
		declareInterationClass(resLogout);
	}
	void ManagerAmbassador::createEnterTestClass()
	{
		InterationClass* enterTest = new InterationClass("__Enter_Test__", Subscribe_Only);
		enterTest->addParameter("username", DATATYPE(std::string));
		enterTest->addParameter("testid", DATATYPE(std::string));
		enterTest->addParameter("password", DATATYPE(std::string));
		enterTest->addParameter("exparam", DATATYPE(std::vector<std::string>));
		declareInterationClass(enterTest);
	}
	void ManagerAmbassador::createExitTestClass()
	{
		InterationClass* exitTest = new InterationClass("__Exit_Test__", Subscribe_Only);
		exitTest->addParameter("username", DATATYPE(std::string));
		exitTest->addParameter("testid", DATATYPE(std::string));
		declareInterationClass(exitTest);
	}
	void ManagerAmbassador::createTestLicenceClass()
	{
		InterationClass* testLicence = new InterationClass("__Test_Licence__", Publish_Only);
		testLicence->addParameter("username", DATATYPE(std::string));
		testLicence->addParameter("testid", DATATYPE(std::string));
		testLicence->addParameter("domain", DATATYPE(long));
		testLicence->addParameter("message", DATATYPE(std::string));
		declareInterationClass(testLicence);
	}
	void ManagerAmbassador::createNewTestClass()
	{
		InterationClass* newTest = new InterationClass("__New_Test__", Subscribe_Only);
		newTest->addParameter("username", DATATYPE(std::string));
		newTest->addParameter("testname", DATATYPE(std::string));
		newTest->addParameter("password", DATATYPE(std::string));
		newTest->addParameter("description", DATATYPE(std::string));
		newTest->addParameter("resid", DATATYPE(std::vector<std::string>));
		newTest->addParameter("exparam", DATATYPE(std::vector<std::string>));
		declareInterationClass(newTest); 
	}
	void ManagerAmbassador::createTestAccessClass()
	{
		InterationClass* testAccess = new InterationClass("__Test_Access__", Publish_Only);
		testAccess->addParameter("username", DATATYPE(std::string));
		testAccess->addParameter("testname", DATATYPE(std::string));
		testAccess->addParameter("testid", DATATYPE(std::string));
		testAccess->addParameter("access", DATATYPE(bool));
		testAccess->addParameter("message", DATATYPE(std::string));
		declareInterationClass(testAccess);
	}
	void ManagerAmbassador::createCloseTestClass()
	{
		InterationClass* closeTest = new InterationClass("__Close_Test__", Subscribe_Only);
		closeTest->addParameter("username", DATATYPE(std::string));
		closeTest->addParameter("testid", DATATYPE(std::string));
		declareInterationClass(closeTest);
	}
	void ManagerAmbassador::createTestUpdateClass()
	{
		InterationClass* testUpdate = new InterationClass("__Test_Update__", Publish_Only);
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
	void ManagerAmbassador::createResRequestClass()
	{
		InterationClass* resRequest = new InterationClass("__Resource_Request__", Publish_Only);
		resRequest->addParameter("resid", DATATYPE(std::string));
		resRequest->addParameter("testid", DATATYPE(std::string));
		resRequest->addParameter("domain", DATATYPE(int));
		resRequest->addParameter("hold_or_free", DATATYPE(bool));
		declareInterationClass(resRequest);
	}

	void ManagerAmbassador::createResReplyClass()
	{
		InterationClass* resReply = new InterationClass("__Resource_Reply__", Subscribe_Only);
		resReply->addParameter("testid", DATATYPE(std::string));
		resReply->addParameter("resid", DATATYPE(std::string));
		resReply->addParameter("holded", DATATYPE(bool));
		declareInterationClass(resReply);
	}

	void ManagerAmbassador::createUserMonitorClass()
	{
		InterationClass* userMonitor = new InterationClass("__User_Monitor__", Publish_Subscribe);
		userMonitor->addParameter("username", DATATYPE(std::string));
		userMonitor->addParameter("timeStamp", DATATYPE(__int64));
		userMonitor->addParameter("launcher_timeStamp", DATATYPE(__int64));
		userMonitor->addParameter("receipted", DATATYPE(bool));
		userMonitor->addParameter("placeholder", DATATYPE(std::string));
		declareInterationClass(userMonitor);
	}

	void ManagerAmbassador::createResMonitorClass()
	{
		InterationClass* resMonitor = new InterationClass("__Resource_Monitor__", Publish_Subscribe);
		resMonitor->addParameter("id", DATATYPE(std::string));
		resMonitor->addParameter("resname", DATATYPE(std::string));
		resMonitor->addParameter("timeStamp", DATATYPE(__int64));
		resMonitor->addParameter("launcher_timeStamp", DATATYPE(__int64));
		resMonitor->addParameter("receipted", DATATYPE(bool));
		resMonitor->addParameter("placeholder", DATATYPE(std::string));
		declareInterationClass(resMonitor);
	}

	void ManagerAmbassador::createExitTestReally()
	{
		InterationClass *exitTestReally = new InterationClass("_EXIT_TEST_REALLY", Publish_Only);
		exitTestReally->addParameter("testid", DATATYPE(std::string));
		exitTestReally->addParameter("username", DATATYPE(std::string));
		declareInterationClass(exitTestReally);
	}

	void ManagerAmbassador::userLoginProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass()==instance()->interationClass("__User_Login__"))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant password = interationInstance->parameterValue("password");
			//TODO
			//数据库检查用户名密码

			boost::mutex::scoped_lock lock(m_instance->m_userMutex);
			
			instance()->log("userlogin beg " + boost::get<std::string>(username) + "," + boost::get<std::string>(password));

			bool access = true ;//MODIF

			std::string msg = "welcome!";

#ifdef QT_DLL
			std::string userid = DBHelper::instance()->verifyUser(boost::get<std::string>(username)
				, boost::get<std::string>(password));

			if (!userid.empty()){
#endif
				for (int i = 0; i < instance()->m_vecUser.size(); i++)
				{
					if (instance()->m_vecUser[i]->name() == boost::get<std::string>(username))
					{
						access = false;
						msg = "the current user is logged in!";
					}
				}

#ifdef QT_DLL
			}
			else {
				access = false;
				msg = "the current user is vaild!";
			}
#endif

			if (access)
			{
				User* user = new User(boost::get<std::string>(username));
				user->setPubHandle(interationInstance->pubHandle());
				user->setTimeStamp(boost::get_system_time());
#ifdef QT_DLL
				user->setUserID(userid);
#endif
				instance()->m_vecUser.push_back(user);
			}

			InterationInstanceImpl userAccessInstance(instance()->interationClass("__User_Access__"));
			userAccessInstance.setParameterValue("username", username);
			userAccessInstance.setParameterValue("access", access);
			userAccessInstance.setParameterValue("message", msg);

			std::vector<std::string> res_vec_id;
			std::vector<std::string> res_vec_name;
			std::vector<std::vector<std::string>> res_vec_exparam;

			for (int i = 0;i<instance()->m_vecResource.size();i++)
			{
				Resource* res = instance()->m_vecResource[i];
				res_vec_id.push_back(res->id());
				res_vec_name.push_back(res->name());
				res_vec_exparam.push_back(res->exparam());
			}

			userAccessInstance.setParameterValue("res_vec_id", res_vec_id);
			userAccessInstance.setParameterValue("res_vec_name", res_vec_name);
			userAccessInstance.setParameterValue("res_vec_exparam", res_vec_exparam);

			std::vector<std::string> test_vec_id;
			std::vector<std::string> test_vec_name;
			std::vector<std::string> test_vec_description;
			std::vector<std::vector<std::string>> test_vec_user;
			std::vector<std::vector<std::string>> test_vec_resid;
			std::vector<std::vector<std::string>> test_vec_exparam;

			for (int t = 0; t<instance()->m_vecTest.size(); t++)
			{
				Test* test = instance()->m_vecTest[t];
				test_vec_id.push_back(test->testID());
				test_vec_name.push_back(test->name());
				test_vec_description.push_back(test->description());
				test_vec_exparam.push_back(test->exparam());

				std::vector<std::string> vec_user;
				std::list<User*> userlist = test->vecUser();
				for (std::list<User*>::iterator iu = userlist.begin();
					iu != userlist.end(); iu++)
				{
					vec_user.push_back((*iu)->name());
				}
				test_vec_user.push_back(vec_user);

				std::vector<std::string> vec_resid;
				std::list<Resource*> reslist = test->vecResource();
				for (std::list<Resource*>::iterator ir = reslist.begin();
					ir != reslist.end(); ir++)
				{
					vec_resid.push_back((*ir)->id());
				}
				test_vec_resid.push_back(vec_resid);
			}

			userAccessInstance.setParameterValue("test_vec_id", test_vec_id);
			userAccessInstance.setParameterValue("test_vec_name", test_vec_name);
			userAccessInstance.setParameterValue("test_vec_description", test_vec_description);
			userAccessInstance.setParameterValue("test_vec_user", test_vec_user);
			userAccessInstance.setParameterValue("test_vec_resid", test_vec_resid);
			userAccessInstance.setParameterValue("test_vec_exparam", test_vec_exparam);

			instance()->sendInterationInstance(&userAccessInstance);

			if (access)
			{
				instance()->log("userlogin end true");
			}
			else
			{
				instance()->log("userlogin end false");
			}

			//试验更新
			for (int t = 0; t < instance()->m_vecTest.size(); t++)
			{
				instance()->testUpate(instance()->m_vecTest[t]);
			}

			instance()->log("testupate end");
		}
	}

	void ManagerAmbassador::resLoginProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Resource_Login__"))
		{
			Variant id = interationInstance->parameterValue("id");
			Variant resname = interationInstance->parameterValue("resname");
			Variant exparam = interationInstance->parameterValue("exparam");
			//TODO
			//数据库检查资源

			boost::mutex::scoped_lock lock(m_instance->m_resMutex);

			bool access = true;//MODIF
			std::string msg = "welcome";

#ifdef QT_DLL
			std::string resid = DBHelper::instance()->verifyResource(boost::get<std::string>(resname));
			if (resid.empty())
			{
				access = false;
				msg = "the current resource is not exist!";
			}
#endif

			if (access)
			{
				Resource* res = new Resource(boost::get<std::string>(resname), boost::get<std::string>(id), boost::get<std::vector<std::string>>(exparam));
				res->setPubHandle(interationInstance->pubHandle());
				res->setTimeStamp(boost::get_system_time());
#ifdef QT_DLL
				res->setResID(resid);
#endif
				instance()->m_vecResource.push_back(res);

#ifdef QT_DLL
				int total_num = 0;
				for (int i = 0; i < instance()->m_vecResource.size();i++)
				{
					if (instance()->m_vecResource[i]->name() == boost::get<std::string>(resname))
					{
						total_num++;
					}
				}
				DBHelper::instance()->updateResourceTotalNum(resid, total_num);
#endif
			}

			InterationInstanceImpl resAccessInstance(instance()->interationClass("__Resource_Access__"));
			resAccessInstance.setParameterValue("id", id);
			resAccessInstance.setParameterValue("resname", resname);
			resAccessInstance.setParameterValue("access", access);
			resAccessInstance.setParameterValue("message", msg);
			instance()->sendInterationInstance(&resAccessInstance);
		}
	}

	void ManagerAmbassador::resUpdateProc( InterationInstanceImpl* interationInstance )
	{	
		if (interationInstance->interationClass() == instance()->interationClass("__Resource_Update__"))
		{
			Variant id = interationInstance->parameterValue("id");
			Variant resname = interationInstance->parameterValue("resname");
			Variant exparam = interationInstance->parameterValue("exparam");
			Variant enable = interationInstance->parameterValue("enable");
			Variant exist = interationInstance->parameterValue("exist");

			boost::mutex::scoped_lock lock(instance()->m_resMutex);
			for(int i = 0;i<instance()->m_vecResource.size(); i++ )
			{
				Resource* res = instance()->m_vecResource[i];
				if(res->id()==boost::get<std::string>(id)
					&&res->name()==boost::get<std::string>(resname))
				{
					//res->setExparam(boost::get<std::vector<std::string>>(exparam));
					res->setEnabled(boost::get<bool>(enable));
				}
			}

		}
	}

	void ManagerAmbassador::enterTestProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Enter_Test__"))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant testid = interationInstance->parameterValue("testid");
			Variant password = interationInstance->parameterValue("password");
			Variant exparam = interationInstance->parameterValue("exparam");

			instance()->log("entertest beg " + boost::get<std::string>(testid) );

			//TODO
			long domain = -1;
			std::string message;
			
			for (int t = 0; t < instance()->m_vecTest.size(); t++)
			{
				if (instance()->m_vecTest[t]->testID() == boost::get<std::string>(testid))
				{
					if (instance()->m_vecTest[t]->password() == boost::get<std::string>(password))
					{
						int u = 0;
						for (; u < instance()->m_vecUser.size(); u++)
						{
							if (instance()->m_vecUser[u]->name() == boost::get<std::string>(username))
							{
								instance()->m_vecTest[t]->enter(instance()->m_vecUser[u]);
								instance()->m_vecTest[t]->setUserExparam(boost::get<std::string>(username),
									boost::get<std::vector<std::string>>(exparam));
								domain = instance()->m_vecTest[t]->domain_ID();

#ifdef QT_DLL
								DBHelper::instance()->recordUser(instance()->m_vecTest[t]->testID(), 
									instance()->m_vecUser[u]->userID());
#endif
								break;
							}
						}
						if (u >= instance()->m_vecUser.size())
						{
							message = "the current user is not exist";
						}
						else
						{
							//试验更新
							instance()->testUpate(instance()->m_vecTest[t]);
						}

					}
					else
					{
						message = "password error";
					}
				}
				else 
				{
					message = "the test id is not exist";
				}
			}

			InterationInstanceImpl testLicInstance(instance()->interationClass("__Test_Licence__"));
			testLicInstance.setParameterValue("username", username);
			testLicInstance.setParameterValue("testid", testid);
			testLicInstance.setParameterValue("domain", domain);
			testLicInstance.setParameterValue("message", message);
			instance()->sendInterationInstance(&testLicInstance);

			instance()->log("entertest end");
		}
	}
	void ManagerAmbassador::exitTestProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Exit_Test__"))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant testid = interationInstance->parameterValue("testid");
			//TODO
			
			std::vector<Test*>& vecTest = instance()->m_vecTest;
			for (std::vector<Test*>::iterator t = vecTest.begin(); t != vecTest.end() ; t++)
			{
				if ((*t)->testID() == boost::get<std::string>(testid))
				{
					if ((*t)->owner()->name() == boost::get<std::string>(username))
					{
						instance()->eraseTest(t);
						break;
					}

					std::list<User*> userlist = (*t)->vecUser();
					for (std::list<User*>::iterator u = userlist.begin();u!=userlist.end();u++)
					{
						if ((*u)->name() == boost::get<std::string>(username))
						{
							(*t)->exit(*u);
						}
					}
					//试验更新
					instance()->testUpate((*t));
				}
				
			}
		}
	}
	void ManagerAmbassador::newTestProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__New_Test__"))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant testname = interationInstance->parameterValue("testname");
			Variant password = interationInstance->parameterValue("password");
			Variant description = interationInstance->parameterValue("description");
			Variant resid = interationInstance->parameterValue("resid");
			Variant exparam = interationInstance->parameterValue("exparam");

			//TODO

			instance()->log("newtest beg " + boost::get<std::string>(testname));

			std::string testid;
			bool access = false;
			std::string message;

			User* ownUser = nullptr;

			for (int u = 0; u < instance()->m_vecUser.size(); u++)
			{
				if (instance()->m_vecUser[u]->name() == boost::get<std::string>(username))
				{
					ownUser = instance()->m_vecUser[u];
					break;
				}
			}

			Test* test = nullptr;
			if (ownUser||ownUser->ownTest())
			{
				int t = 0;
				for (; t < instance()->m_vecTest.size(); t++)
				{
					if (instance()->m_vecTest[t]->name() == boost::get<std::string>(testname))
					{
						message = "the current test is exist";
						break;
					}
				}
				if (t >= instance()->m_vecTest.size()|| instance()->m_vecTest.empty())
				{
					std::vector<std::string> vec_resid = boost::get<std::vector<std::string>>(resid);

					std::vector<Resource*> vecResource;

					for (int i = 0; i < vec_resid.size(); i++)
					{
						for (int r = 0 ; r < instance()->m_vecResource.size(); r++)
						{
							if (instance()->m_vecResource[r]->id() == vec_resid[i]
								&& !instance()->m_vecResource[r]->test())
							{
								vecResource.push_back(instance()->m_vecResource[r]);
								//break;
							}
						}
					}
					if (vec_resid.size() == vecResource.size())
					{
						test = new Test(boost::get<std::string>(testname),
							boost::get<std::string>(password),
							instance()->m_domain_index++, ownUser);
						test->setDescription(boost::get<std::string>(description));
						test->setExparam(boost::get<std::vector<std::string>>(exparam));
						for (int r = 0; r < vecResource.size(); r++)
						{
							//请求资源未验证
							InterationInstanceImpl testReqInstance(instance()->interationClass("__Resource_Request__"));
							testReqInstance.setParameterValue("resid", vecResource[r]->id());
							testReqInstance.setParameterValue("testid", test->testID());
							testReqInstance.setParameterValue("domain", test->domain_ID());
							testReqInstance.setParameterValue("hold_or_free", true);
							instance()->sendInterationInstance(&testReqInstance);

							test->enter(vecResource[r]);
							test->setResID(vecResource[r]->resID());			

#ifdef QT_DLL
							int available = 0;
							for (int ar = 0; ar < instance()->m_vecResource.size(); ar++)
							{
								if (instance()->m_vecResource[ar]->name() == vecResource[r]->name()
									&& !instance()->m_vecResource[ar]->test())
								{
									available++;
								}
							}

							DBHelper::instance()->updateResourceAvailableNum(vecResource[r]->resID(), available);
#endif

						}
						instance()->m_vecTest.push_back(test);
					}
					else 
					{
						message = "insufficient resources";
					}
					
				}
			}
			else 
			{
				message = "the current user can create a test!";
			}

			if (test)
			{
				testid = test->testID();
				access = true;
				//试验更新
				instance()->testUpate(test);
			}

			InterationInstanceImpl testAcsInstance(instance()->interationClass("__Test_Access__"));
			testAcsInstance.setParameterValue("username", username);
			testAcsInstance.setParameterValue("testname", testname);
			testAcsInstance.setParameterValue("testid", testid);
			testAcsInstance.setParameterValue("access", access);
			testAcsInstance.setParameterValue("message", message);
			instance()->sendInterationInstance(&testAcsInstance);

			instance()->log("newtest end");
		}
	}
	void ManagerAmbassador::closeTestProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Close_Test__"))
		{
			Variant username = interationInstance->parameterValue("username");
			Variant testid = interationInstance->parameterValue("testid");		

			//TODO
			std::vector<Test*>& vecTest = instance()->m_vecTest;
			for (std::vector<Test*>::iterator t = vecTest.begin(); t != vecTest.end(); t++)
			{
				if ((*t)->testID() == boost::get<std::string>(testid)&&
					(*t)->owner()->name() == boost::get<std::string>(username))
				{
					instance()->eraseTest(t);
					break;
				}
			}
		}
	}
	void ManagerAmbassador::resReplyProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Resource_Reply__"))
		{
			Variant testid = interationInstance->parameterValue("testid");
			Variant resid = interationInstance->parameterValue("resid");
			Variant holded = interationInstance->parameterValue("holded");
			//TODO

		}
	}
	void ManagerAmbassador::userMonitorProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass()->interationName() == "__User_Monitor__")
		{
			Variant username = interationInstance->parameterValue("username");
			Variant receipted = interationInstance->parameterValue("receipted");
			if (!boost::get<bool>(receipted))
			{
				boost::mutex::scoped_lock lock(m_instance->m_userMutex);
				for (int i = 0; i < instance()->m_vecUser.size(); i++)
				{
					if (instance()->m_vecUser[i]->name() == boost::get<std::string>(username))
					{
						instance()->m_vecUser[i]->setTimeStamp(boost::get_system_time());
					}
				}
				instance()->m_userCondition.notify_one();
				__int64 curts = boost::get_system_time().time_of_day().total_milliseconds();
				interationInstance->setParameterValue("launcher_timeStamp", curts);
				interationInstance->setParameterValue("receipted", true);
				instance()->sendInterationInstance(interationInstance);
			}
		}
	}

	void ManagerAmbassador::resMonitorProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass()->interationName() == "__Resource_Monitor__")
		{
			Variant id = interationInstance->parameterValue("id");
			Variant resname = interationInstance->parameterValue("resname");
			Variant receipted = interationInstance->parameterValue("receipted");
			if (!boost::get<bool>(receipted))
			{
				boost::mutex::scoped_lock lock(m_instance->m_resMutex);
				for (int i = 0; i < instance()->m_vecResource.size(); i++)
				{
					if (instance()->m_vecResource[i]->id() == boost::get<std::string>(id)
						&&instance()->m_vecResource[i]->name() == boost::get<std::string>(resname))
					{
						instance()->m_vecResource[i]->setTimeStamp(boost::get_system_time());
					}
				}
				instance()->m_resCondition.notify_one();
				__int64 curts = boost::get_system_time().time_of_day().total_milliseconds();
				interationInstance->setParameterValue("launcher_timeStamp", curts);
				interationInstance->setParameterValue("receipted", true);
				instance()->sendInterationInstance(interationInstance);
			}
		}
	}

	void ManagerAmbassador::userMonitorThread()
	{
		while (false)//不再使用
		{
			boost::mutex::scoped_lock lock(m_userMutex);
			boost::system_time cur_time = boost::get_system_time();
			boost::system_time start_time = cur_time;
			bool bWait = !m_vecUser.empty();

			for (std::vector<User*>::iterator it = m_vecUser.begin(); it != m_vecUser.end(); it++)
			{
				if ((*it)->timeStamp() + boost::posix_time::seconds(5) < cur_time)
				{
					eraseUser(it);
					bWait = false;
					break;
				}
				else 
				{
					if (start_time > (*it)->timeStamp())
					{
						start_time = (*it)->timeStamp();
					}
				}
			}

			if (bWait)
			{
				m_userCondition.timed_wait(lock, start_time + boost::posix_time::seconds(5));
			}
			else {
				m_userCondition.wait(lock);
			}
		}
	}

	void ManagerAmbassador::resMonitorThread()
	{
		while (false)//不再使用
		{
			boost::mutex::scoped_lock lock(m_resMutex);
			boost::system_time cur_time = boost::get_system_time();
			boost::system_time start_time = cur_time;
			bool bWait = !m_vecResource.empty();

			for (std::vector<Resource*>::iterator it = m_vecResource.begin(); it != m_vecResource.end(); it++)
			{
				if ((*it)->timeStamp() + boost::posix_time::seconds(5) < cur_time)
				{
					
					InterationInstanceImpl resUpdateInstance(interationClass("__Resource_Update__"));
					resUpdateInstance.setParameterValue("id", (*it)->id());
					resUpdateInstance.setParameterValue("resname", (*it)->name());
					resUpdateInstance.setParameterValue("exparam", (*it)->exparam());
					resUpdateInstance.setParameterValue("enable", (*it)->enabled());
					resUpdateInstance.setParameterValue("exist", false);
					
					instance()->eraseResource(it);

					sendInterationInstance(&resUpdateInstance);
					bWait = false;
					break;
				}
				else
				{
					if (start_time > (*it)->timeStamp())
					{
						start_time = (*it)->timeStamp();
					}
				}
			}

			if (bWait)
			{
				m_resCondition.timed_wait(lock, start_time + boost::posix_time::seconds(5));
			}
			else {
				m_resCondition.wait(lock);
			}
		}
	}

	void ManagerAmbassador::eraseUser(std::vector<User*>::iterator iu)
	{
		User* puser = (*iu);
		m_vecUser.erase(iu);

		//if (puser->ownTest())
		//{
		//	Test* ptest = puser->ownTest();
		//	for (std::vector<Test*>::iterator it = m_vecTest.begin();
		//		it != m_vecTest.end(); it++)
		//	{
		//		if ((*it) == ptest)
		//		{
		//			eraseTest(it);
		//			break;
		//		}
		//	}
		//}
		if (puser->test())
		{
			Test* ptest = puser->test();
			ptest->exit(puser);

			InterationInstanceImpl impl(instance()->interationClass("_EXIT_TEST_REALLY"));
			impl.setParameterValue("testid",ptest->testID());
			impl.setParameterValue("username", puser->name());
			sendInterationInstance(&impl);


			//TODO
			if (ptest->vecUser().empty())
			{
				for (std::vector<Test*>::iterator it = m_vecTest.begin();
				it!= m_vecTest.end();it++)
				{
					if ((*it)== ptest)
					{
						eraseTest(it);
						break;
					}
				}
				
			}
		}
		log("erase user " + puser->name());
		delete puser;
	}

	void ManagerAmbassador::eraseResource(std::vector<Resource*>::iterator ir)
	{
		Resource* pres = (*ir);
		m_vecResource.erase(ir);
#ifdef QT_DLL
		int total_num = 0;
		for (int i = 0; i < m_vecResource.size(); i++)
		{
			if (m_vecResource[i]->name() == pres->name())
			{
				total_num++;
			}
		}
		DBHelper::instance()->updateResourceTotalNum(pres->resID(), total_num);
#endif
		if (pres->test())
		{
			Test* ptest = pres->test();
			ptest->exit(pres);
			//TODO
		}
		log("erase res " + pres->id() );
		delete pres;
	}

	void ManagerAmbassador::eraseTest(std::vector<Test*>::iterator it)
	{
		Test* ptest = (*it);
		m_vecTest.erase(it);

		int user_num = 0;

		for (std::list<User*>::iterator iu = ptest->vecUser().begin();
		iu != ptest->vecUser().end(); iu++)
		{
			//ptest->exit((*iu));
			user_num++;
			//TODO
		}
		
		//boost::mutex::scoped_lock lock(m_resMutex);
		for (std::list<Resource*>::iterator ir = ptest->vecResource().begin();
		ir != ptest->vecResource().end(); ir++)
		{
			//资源释放未验证
			InterationInstanceImpl testReqInstance(interationClass("__Resource_Request__"));
			testReqInstance.setParameterValue("resid", (*ir)->id());
			testReqInstance.setParameterValue("testid", ptest->testID());
			testReqInstance.setParameterValue("domain", ptest->domain_ID());
			testReqInstance.setParameterValue("hold_or_free", false);
			sendInterationInstance(&testReqInstance);

			//ptest->exit((*ir));
#ifdef QT_DLL
			int available = 0;
			for (size_t i = 0; i < instance()->m_vecResource.size(); i++)
			{
				if (instance()->m_vecResource[i]->name() == (*ir)->name()
					&& !instance()->m_vecResource[i]->test())
				{
					available++;
				}

			}
			DBHelper::instance()->updateResourceAvailableNum((*ir)->resID(), available);
#endif
			
			//TODO
		}

#ifdef QT_DLL
		DBHelper::instance()->recordTest(ptest->testID(),ptest->resID(), ptest->beginTime(), time(0));
#endif

		//试验更新
		testUpate(ptest,false);
		log("erase test " + ptest->name());
		delete ptest;


	}

	void ManagerAmbassador::testUpate(Test * test,bool exist)
	{
		InterationInstanceImpl testAcsInstance(interationClass("__Test_Update__"));
		testAcsInstance.setParameterValue("testid", test->testID());
		testAcsInstance.setParameterValue("testname", test->name());
		testAcsInstance.setParameterValue("description", test->description());
		testAcsInstance.setParameterValue("exparam", test->exparam());
		testAcsInstance.setParameterValue("exist", exist);
		int count = m_vecTest.size();
		testAcsInstance.setParameterValue("count", count);

		std::vector<std::string> vec_user;
		std::vector<std::vector<std::string>> vec_user_exparam;
		std::list<User*> userlist = test->vecUser();
		for (std::list<User*>::iterator u = userlist.begin(); u != userlist.end(); u++)
		{
			std::string user_name = (*u)->name();
			vec_user.push_back(user_name);
			vec_user_exparam.push_back(test->userExparam(user_name));
		}
		testAcsInstance.setParameterValue("username", vec_user);
		testAcsInstance.setParameterValue("user_exparam", vec_user_exparam);

		std::vector<std::string> vec_resid;
		std::list<Resource*> reslist = test->vecResource();
		for (std::list<Resource*>::iterator r = reslist.begin(); r != reslist.end(); r++)
		{
			vec_resid.push_back((*r)->id());
		}
		testAcsInstance.setParameterValue("resid", vec_resid);

		sendInterationInstance(&testAcsInstance);
	}

	void ManagerAmbassador::subscriptionLost(std::string classname, long handle)
	{
		boost::mutex::scoped_lock ulock(instance()->m_userMutex);
		for (std::vector<User*>::iterator it = instance()->m_vecUser.begin();
			it != instance()->m_vecUser.end(); it++)
		{
			if ((*it)->pubHandle() == handle)
			{
				instance()->eraseUser(it);
				break;
			}
		}

		boost::mutex::scoped_lock rlock(instance()->m_resMutex);
		for (std::vector<Resource*>::iterator it = instance()->m_vecResource.begin();
			it != instance()->m_vecResource.end(); it++)
		{
			if ((*it)->pubHandle() == handle)
			{
				instance()->eraseResource(it);
				break;
			}
		}
	}

	void ManagerAmbassador::userLogoutProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__User_Logout__"))
		{
			Variant username = interationInstance->parameterValue("username");

			boost::mutex::scoped_lock lock(instance()->m_userMutex);
			for ( std::vector<User*>::iterator it = instance()->m_vecUser.begin();
				it != instance()->m_vecUser.end(); it++ )
			{
				if ((*it)->name()==boost::get<std::string>(username))
				{
					instance()->eraseUser(it);
					break;
				}
			}
			//TODO
		}
	}
	void ManagerAmbassador::resLogoutProc(InterationInstanceImpl * interationInstance)
	{
		if (interationInstance->interationClass() == instance()->interationClass("__Resource_Logout__"))
		{
			Variant id = interationInstance->parameterValue("id");
			Variant resname = interationInstance->parameterValue("resname");

			boost::mutex::scoped_lock lock(instance()->m_resMutex);
			for ( std::vector<Resource*>::iterator it = instance()->m_vecResource.begin();
				it != instance()->m_vecResource.end(); it++ )
			{
				if ((*it)->id()==boost::get<std::string>(id)
					&&(*it)->name()==boost::get<std::string>(resname))
				{
					instance()->eraseResource(it);
					break;
				}
			}
			//TODO
		}
	}
}
