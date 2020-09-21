#ifndef __MANAGERAMBASSADOR_H__
#define __MANAGERAMBASSADOR_H__

#include <common.h>
#include <Ambassador.h>
#include <vector>
#include <boost\thread.hpp>

class Test;
class User;
class Resource;

namespace Data_Exchange_Platform
{

	class DATA_EXCHANGE_PLATFORM_EXPORT ManagerAmbassador : public Ambassador
	{
	public:
		~ManagerAmbassador();

		static ManagerAmbassador* instance();

	private:
		ManagerAmbassador();

		void createUserLoginClass();
		void createResLoginClass();

		void createUserAccessClass();
		void createResAccessClass();
		void createResUpdateClass();

		void createUserLogoutClass();
		void createResLogoutClass();

		void createEnterTestClass();
		void createExitTestClass();
		void createTestLicenceClass();

		void createNewTestClass();
		void createTestAccessClass();
		void createCloseTestClass();
		void createTestUpdateClass();
		void createResRequestClass();
		void createResReplyClass();

		//void createUserQueryClass();
		//void createResQueryClass();

		void createUserMonitorClass();
		void createResMonitorClass();

		void createExitTestReally();

		static void userLoginProc(InterationInstanceImpl* interationInstance);
		static void resLoginProc(InterationInstanceImpl* interationInstance);
		static void resUpdateProc(InterationInstanceImpl* interationInstance);

		static void userLogoutProc(InterationInstanceImpl * interationInstance);
		static void resLogoutProc(InterationInstanceImpl * interationInstance);

		static void enterTestProc(InterationInstanceImpl* interationInstance);
		static void exitTestProc(InterationInstanceImpl* interationInstance);

		static void newTestProc(InterationInstanceImpl* interationInstance);
		static void closeTestProc(InterationInstanceImpl* interationInstance);
		static void resReplyProc(InterationInstanceImpl* interationInstance);

		static void userMonitorProc(InterationInstanceImpl* interationInstance);
		static void resMonitorProc(InterationInstanceImpl* interationInstance);

		void userMonitorThread();
		void resMonitorThread();

		void eraseUser(std::vector<User*>::iterator iu);
		void eraseResource(std::vector<Resource*>::iterator ir);
		void eraseTest(std::vector<Test*>::iterator it);

		void testUpate(Test* test, bool exist=true);

		void subscriptionLost(std::string classname, long handle);

	private:
		static ManagerAmbassador* m_instance;

		std::vector<Test*> m_vecTest;
		std::vector<User*> m_vecUser;
		std::vector<Resource*> m_vecResource;

		long m_domain_index;

		boost::thread m_userThread;
		boost::mutex m_userMutex;
		boost::condition_variable m_userCondition;

		boost::thread m_resThread;
		boost::mutex m_resMutex;
		boost::condition_variable m_resCondition;
	};

}

#endif // !__MANAGERAMBASSADOR_H__
