// Launcher.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <common.h>
#include <ManagerAmbassador.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread_time.hpp>

using namespace Data_Exchange_Platform;

#ifdef QT_DLL

#include <DBHelper.h>
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QStringList>
#include <QPluginLoader>

#endif

int main(int argc, char *argv[])
{

#ifdef QT_DLL

	QCoreApplication app(argc, argv);
	QCoreApplication::addLibraryPath("./plugins");

	bool bopen = DBHelper::instance()->initDB();

	

#endif

	std::string cmd;
	if (argc>1)
	{
		cmd = std::string("DCPSInfoRepo -ORBListenEndpoints iiop://:") + argv[1];
		boost::thread infoRepoThread(&std::system, cmd.c_str());
	}
	
	//boost::thread::sleep(boost::get_system_time()+boost::posix_time::seconds(3));

	ManagerAmbassador::instance();

#ifdef QT_DLL
	app.exec();
#else
	while (true);
#endif

    return 0;
}

