#pragma once

#include "common.h"
#include <string>

#ifdef QT_DLL

#include <QString>

class DATA_EXCHANGE_PLATFORM_EXPORT DBHelper
{
public:
	static DBHelper* instance();

	bool initDB();

	std::string verifyUser(std::string username,std::string password);
	std::string verifyResource(std::string resname);

	void recordTest(std::string testid, std::string resid, time_t begin, time_t end);
	void updateResourceTotalNum(std::string resid, int total );
	void updateResourceAvailableNum(std::string resid, int available);
	void recordUser(std::string testid, std::string userid);

private:

	static DBHelper* m_instanse;

	QString user_UserTable;
	QString user_UserID;
	QString user_UserName;
	QString user_Password;

	QString res_ResTable;
	QString res_ResID;
	QString res_ResName;
	QString res_ResTotalNum;
	QString res_ResAvailableNum;

	QString test_usage_UsageTable;
	QString test_usage_UsagePK;
	QString test_usage_ResID;
	QString test_usage_BeginTime;
	QString test_usage_EndTime;

	QString test_respond_RespondTable;
	QString test_respond_UsagePK;
	QString test_respond_PK;
	QString test_respond_UserID;
	
};

#endif

