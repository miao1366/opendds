#include "ResourceAmbassador.h"
#include <InterationClass.h>
#include <InterationInstanceImpl.h>

ResourceAmbassador* ResourceAmbassador::m_instance = nullptr;

__int64  ResourceAmbassador::prets = 0;
double ResourceAmbassador::lost = 0;
__int64 ResourceAmbassador::num = 0;

ResourceAmbassador::ResourceAmbassador()
	:Ambassador(0x000000FF), m_monitorSpeed(false), m_syncTime(false),
	m_timer(m_io, boost::posix_time::seconds(3))
{
	createResLoginClass();
	createResAccessClass();
	createResLogoutClass();
	//createTestLicenceClass();
	createResUpdateClass();
	
	createResRequestClass();
	createResReplyClass();

	createResMonitorClass();

	registerInterationReceiveProc(resAccessProc);
	//registerInterationReceiveProc(testLicenceProc);
	registerInterationReceiveProc(resMonitorProc);

	registerInterationReceiveProc(resRequestProc);

	waitConnection();

	m_timer.async_wait(boost::bind(&ResourceAmbassador::monitorTimeout, this));
	m_monitor = boost::thread(boost::bind(&boost::asio::io_service::run, &m_io));

}

void ResourceAmbassador::createResLoginClass()
{
	InterationClass* resLogin = new InterationClass("__Resource_Login__", Publish_Only);
	resLogin->addParameter("id", DATATYPE(std::string));
	resLogin->addParameter("resname", DATATYPE(std::string));
	resLogin->addParameter("exparam", DATATYPE(std::vector<std::string>));
	declareInterationClass(resLogin);
}

void ResourceAmbassador::createResAccessClass()
{
	InterationClass* resAccess = new InterationClass("__Resource_Access__", Subscribe_Only);
	resAccess->addParameter("id", DATATYPE(std::string));
	resAccess->addParameter("resname", DATATYPE(std::string));
	resAccess->addParameter("access", DATATYPE(bool));
	resAccess->addParameter("message", DATATYPE(std::string));
	declareInterationClass(resAccess);
}

void ResourceAmbassador::createResLogoutClass()
{
	InterationClass* resLogout = new InterationClass("__Resource_Logout__", Publish_Only);
	resLogout->addParameter("id", DATATYPE(std::string));
	resLogout->addParameter("resname", DATATYPE(std::string));
	declareInterationClass(resLogout);
}

//void ResourceAmbassador::createTestLicenceClass()
//{
//	InterationClass* testLicence = new InterationClass("__Test_Licence__", Subscribe_Only);
//	testLicence->addParameter("domain", DATATYPE(int));
//	testLicence->addParameter("message", DATATYPE(std::string));
//	declareInterationClass(testLicence);
//}

void ResourceAmbassador::createResUpdateClass()
{
	InterationClass* resUpdate = new InterationClass("__Resource_Update__", Publish_Only);
	resUpdate->addParameter("id", DATATYPE(std::string));
	resUpdate->addParameter("resname", DATATYPE(std::string));
	resUpdate->addParameter("exparam", DATATYPE(std::vector<std::string>));
	resUpdate->addParameter("enable", DATATYPE(bool));
	resUpdate->addParameter("exist", DATATYPE(bool));
	declareInterationClass(resUpdate);
}

void ResourceAmbassador::createResRequestClass()
{
	InterationClass* resRequest = new InterationClass("__Resource_Request__", Subscribe_Only);
	resRequest->addParameter("resid", DATATYPE(std::string));
	resRequest->addParameter("testid", DATATYPE(std::string));
	resRequest->addParameter("domain", DATATYPE(int));
	resRequest->addParameter("hold_or_free", DATATYPE(bool));
	declareInterationClass(resRequest);
}

void ResourceAmbassador::createResReplyClass()
{
	InterationClass* resReply = new InterationClass("__Resource_Reply__", Publish_Only);
	resReply->addParameter("testid", DATATYPE(std::string));
	resReply->addParameter("resid", DATATYPE(std::string));
	resReply->addParameter("holded", DATATYPE(bool));
	declareInterationClass(resReply);
}

void ResourceAmbassador::createResMonitorClass()
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

void ResourceAmbassador::resAccessProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Resource_Access__"))
	{
		
		Variant id = interationInstance->parameterValue("id");
		Variant resname = interationInstance->parameterValue("resname");
		Variant access = interationInstance->parameterValue("access");
		Variant message = interationInstance->parameterValue("message");
		
		for (size_t i = 0; i < instance()->m_vecServiceResource.size(); i++)
		{
			if (instance()->m_vecServiceResource[i]->id() == boost::get<std::string>(id) &&
				instance()->m_vecServiceResource[i]->resname() == boost::get<std::string>(resname) )
			{
				boost::mutex::scoped_lock lock(m_instance->m_resMutex);
				if (boost::get<bool>(access))
				{
					instance()->m_vecServiceResource[i]->permit();
					instance()->m_vecServiceResource[i]->errorClear();
					m_instance->m_resCondition.notify_one();
				}
				else
				{
					instance()->m_vecServiceResource[i]->setLastError(boost::get<std::string>(message));
					m_instance->m_resCondition.notify_one();
				}
				break;
			}
		}
		
	}
}

void ResourceAmbassador::resRequestProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Resource_Request__"))
	{
		Variant resid = interationInstance->parameterValue("resid");
		Variant testid = interationInstance->parameterValue("testid");
		Variant domain = interationInstance->parameterValue("domain");
		Variant hold_or_free = interationInstance->parameterValue("hold_or_free");

		for (size_t i = 0; i < instance()->m_vecServiceResource.size(); i++)
		{
			if (instance()->m_vecServiceResource[i]->id() == boost::get<std::string>(resid))
			{
				if (boost::get<bool>(hold_or_free))
				{
					if (instance()->m_vecServiceResource[i]->isOnline() &&
						!instance()->m_vecServiceResource[i]->inTest())
					{
						instance()->m_vecServiceResource[i]->joinTestDomain(boost::get<long>(domain));

						bool holded = false;
						holded = instance()->m_vecServiceResource[i]->holdRequest(boost::get<std::string>(testid));

						InterationInstanceImpl resReplyInstance(instance()->interationClass("__Resource_Reply__"));

						resReplyInstance.setParameterValue("testid", testid);
						resReplyInstance.setParameterValue("resid", instance()->m_vecServiceResource[i]->id());
						resReplyInstance.setParameterValue("holded", holded);

						instance()->sendInterationInstance(&resReplyInstance);
					}
				}
				else
				{
					if (instance()->m_vecServiceResource[i]->isOnline() &&
						instance()->m_vecServiceResource[i]->inTest())
					{

						instance()->m_vecServiceResource[i]->leaveTestDomain();
						instance()->m_vecServiceResource[i]->freeRequest(boost::get<std::string>(testid));

						bool holded = false;

						InterationInstanceImpl resReplyInstance(instance()->interationClass("__Resource_Reply__"));

						resReplyInstance.setParameterValue("testid", testid);
						resReplyInstance.setParameterValue("resid", instance()->m_vecServiceResource[i]->id());
						resReplyInstance.setParameterValue("holded", holded);

						instance()->sendInterationInstance(&resReplyInstance);
					}
				}
			}
			
		}
		
	}
}

//void ResourceAmbassador::testLicenceProc(InterationInstanceImpl * interationInstance)
//{
//	if (interationInstance->interationClass() == instance()->interationClass("__Test_Licence__"))
//	{
//		Variant domain = interationInstance->parameterValue("domain");
//		Variant message = interationInstance->parameterValue("message");
//
//		for (size_t i = 0; i < instance()->m_vecServiceResource.size(); i++)
//		{
//			if (instance()->m_vecServiceResource[i]->isOnline()&&
//				!instance()->m_vecServiceResource[i]->inTest())
//			{
//				instance()->m_vecServiceResource[i]->joinTestDomain(boost::get<long>(domain));
//			}
//		}
//		
//	}
//}

void ResourceAmbassador::resMonitorProc(InterationInstanceImpl * interationInstance)
{
	if (interationInstance->interationClass() == instance()->interationClass("__Resource_Monitor__"))
	{
		Variant receipted = interationInstance->parameterValue("receipted");
		if (boost::get<bool>(receipted))
		{
			Variant id = interationInstance->parameterValue("id");
			Variant resname = interationInstance->parameterValue("resname");
			Variant timeStamp = interationInstance->parameterValue("timeStamp");
			Variant launcher = interationInstance->parameterValue("launcher_timeStamp");

			for (size_t i = 0; i < instance()->m_vecServiceResource.size(); i++)
			{
				if (instance()->m_vecServiceResource[i]->id() == boost::get<std::string>(id) &&
					instance()->m_vecServiceResource[i]->resname() == boost::get<std::string>(resname))
				{
					boost::system_time systime = boost::get_system_time();

					__int64 curtime = systime.time_of_day().total_milliseconds();

					int srctime = boost::get<__int64>(timeStamp);

					int delay = curtime - srctime;

					__int64 launcher_time = boost::get<__int64>(launcher);

					if (instance()->m_syncTime && delay < 2000
						&& (launcher_time - srctime > delay || launcher_time < srctime))
					{
						SYSTEMTIME st;
						GetSystemTime(&st);
						st.wHour = launcher_time % (1000 * 60 * 60 * 24) / (1000 * 60 * 60);
						st.wMinute = launcher_time % (1000 * 60 * 60) / (1000 * 60);
						st.wSecond = launcher_time % (1000 * 60) / 1000;
						st.wMilliseconds = launcher_time % 1000 + delay / 2;
						SetSystemTime(&st);

						std::stringstream logbuf;
						logbuf << "SetSystemTime " << launcher_time + delay / 2;
						instance()->log(logbuf.str());
					}

					num++;

					if (curtime - prets>5000 && prets != 0)
					{
						lost = ((num - 1)*lost + 1 ) / num;
					}
					else
					{
						lost = (num - 1)*lost / num;
					}

					prets = curtime;

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

					instance()->m_vecServiceResource[i]->monitor(delay, lost, speed);

					std::stringstream logbuf;
					logbuf << "No." << num << " - delay=" << delay << "ms, lost=" << lost * 100 << "%, speed=" << speed << "KB/s";
					instance()->log(logbuf.str());
				}
			}

		}
	}
}

void ResourceAmbassador::monitorTimeout()
{
	__int64 curts = boost::get_system_time().time_of_day().total_milliseconds();

	char placeholder[64000] = { 0 };
	if (m_monitorSpeed)
	{
		memset(placeholder, 0xFF, 64000);
		placeholder[63999] = 0x00;
	}

	for (size_t i = 0; i < m_vecServiceResource.size(); i++)
	{
		if (m_vecServiceResource[i]->isOnline())
		{
			InterationInstanceImpl monitorInstance(interationClass("__Resource_Monitor__"));
			monitorInstance.setParameterValue("id", m_vecServiceResource[i]->id());
			monitorInstance.setParameterValue("resname", m_vecServiceResource[i]->resname());
			monitorInstance.setParameterValue("timeStamp", curts);
			monitorInstance.setParameterValue("launcher_timeStamp", __int64(0));
			monitorInstance.setParameterValue("receipted", false);
			monitorInstance.setParameterValue("placeholder", std::string(placeholder));
			sendInterationInstance(&monitorInstance);
		}
	}

	m_timer.expires_at(m_timer.expires_at() + boost::posix_time::seconds(3));
	m_timer.async_wait(boost::bind(&ResourceAmbassador::monitorTimeout, this));
}

ResourceAmbassador::~ResourceAmbassador()
{
	for (size_t i = 0; i < instance()->m_vecServiceResource.size(); i++)
	{
		if (m_vecServiceResource[i]->isOnline())
		{
			m_vecServiceResource[i]->logout();
		}
	}
}

ResourceAmbassador * ResourceAmbassador::instance()
{
	if (!m_instance)
	{
		m_instance = new ResourceAmbassador;
	}
	return m_instance;
}

bool ResourceAmbassador::login(std::string resource_name, std::string id, std::vector<std::string> exparam )
{
	boost::mutex::scoped_lock lock(m_resMutex);
	InterationInstanceImpl resLoginInstance(instance()->interationClass("__Resource_Login__"));
	resLoginInstance.setParameterValue("id", id);
	resLoginInstance.setParameterValue("resname", resource_name);
	resLoginInstance.setParameterValue("exparam", exparam);
	sendInterationInstance(&resLoginInstance);

	bool bRet = m_resCondition.timed_wait(lock,boost::get_system_time()+boost::posix_time::seconds(30));
	log("resid: " + id);
	return bRet;
}

void ResourceAmbassador::updateState(std::string resource_name, std::string id, std::vector<std::string> exparam, bool enabled, bool exist )
{
	InterationInstanceImpl resUpdateInstance(instance()->interationClass("__Resource_Update__"));
	resUpdateInstance.setParameterValue("id", id);
	resUpdateInstance.setParameterValue("resname", resource_name);
	resUpdateInstance.setParameterValue("exparam", exparam);
	resUpdateInstance.setParameterValue("enable", enabled);
	resUpdateInstance.setParameterValue("exist", exist);
	sendInterationInstance(&resUpdateInstance);
}

bool ResourceAmbassador::logout(std::string resource_name, std::string id)
{
	InterationInstanceImpl resLogoutInstance(instance()->interationClass("__Resource_Logout__"));
	resLogoutInstance.setParameterValue("id", id);
	resLogoutInstance.setParameterValue("resname", resource_name);
	sendInterationInstance(&resLogoutInstance);
	return true;
}
