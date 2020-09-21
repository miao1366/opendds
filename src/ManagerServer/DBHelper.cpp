#include "DBHelper.h"
#include <iostream>

#ifdef QT_DLL

#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QUuid>

DBHelper* DBHelper::m_instanse = nullptr;

DBHelper * DBHelper::instance()
{
	if (!m_instanse)
	{
		m_instanse = new DBHelper;
	}
	return m_instanse;
}

bool DBHelper::initDB()
{
	QSettings set("./db.ini", QSettings::IniFormat);

	QString driver = set.value("common/Driver").toString();
	QString host = set.value("common/Host").toString();
	QString database = set.value("common/Database").toString();
	QString user = set.value("common/User").toString();
	QString password = set.value("common/Password").toString();

	user_UserTable = set.value("user/UserTable").toString();
	user_UserID = set.value("user/UserID").toString();
	user_UserName = set.value("user/UserName").toString();
	user_Password = set.value("user/Password").toString();

	res_ResTable = set.value("res/ResTable").toString();
	res_ResID = set.value("res/ResID").toString();
	res_ResName = set.value("res/ResName").toString();
	res_ResTotalNum = set.value("res/ResTotalNum").toString();
	res_ResAvailableNum = set.value("res/ResAvailableNum").toString();

	test_usage_UsageTable = set.value("test_usage/UsageTable").toString();
	test_usage_ResID = set.value("test_usage/ResID").toString();
	test_usage_BeginTime = set.value("test_usage/BeginTime").toString();
	test_usage_EndTime = set.value("test_usage/EndTime").toString();
	test_usage_UsagePK = set.value("test_usage/UsagePK").toString();

	test_respond_RespondTable = set.value("test_respond/RespondTable").toString();
	test_respond_UsagePK = set.value("test_respond/UsagePK").toString();
	test_respond_PK = set.value("test_respond/PK").toString();
	test_respond_UserID = set.value("test_respond/UserID").toString();

	qDebug() << driver << host << database << user << password;
	QSqlDatabase db = QSqlDatabase::addDatabase(driver);
	db.setHostName(host);
	db.setDatabaseName(database);
	db.setUserName(user);
	db.setPassword(password);

	return db.open();
}

std::string DBHelper::verifyUser(std::string username, std::string password)
{
	QSqlQuery query;
	QString sql = "select " + user_UserID +
		" from " + user_UserTable +
		" where " + user_UserName + " = ? and lower(" + user_Password +
		") = lower(?) ";
	query.prepare(sql);
	query.bindValue(0, QString::fromStdString(username));
	QCryptographicHash crypt(QCryptographicHash::Md5);
	crypt.addData(password.c_str(), password.length());
	query.bindValue(1, crypt.result().toHex().constData());

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}

	std::string userid;
	while (query.next()) {
		userid = query.value(0).toString().toStdString();
	}

	return userid;
}

std::string DBHelper::verifyResource(std::string resname)
{
	QSqlQuery query;
	QString sql = "select " + res_ResID +
		" from " + res_ResTable +
		" where " + res_ResName + " = ? ";
	query.prepare(sql);
	query.bindValue(0, QString::fromStdString(resname));

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}

	std::string resid;
	while (query.next()) {
		resid = query.value(0).toString().toStdString();
	}

	return resid;
}

void DBHelper::recordTest(std::string testid, std::string resid, time_t begin, time_t end)
{
	QSqlQuery query;
	QString sql = "insert into " + test_usage_UsageTable +
		" ( "+ test_usage_UsagePK + "," + test_usage_ResID + ","+ test_usage_BeginTime + "," + test_usage_EndTime  + " ) " +
		" values ( ? ,? ,? ,? ) ";
	query.prepare(sql);
	query.bindValue(0, QString::fromStdString(testid));
	query.bindValue(1, QString::fromStdString(resid));
	query.bindValue(2, QDateTime::fromTime_t(begin).toString("yyyy-MM-dd hh:mm:ss"));
	query.bindValue(3, QDateTime::fromTime_t(end).toString("yyyy-MM-dd hh:mm:ss"));

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}
}

void DBHelper::updateResourceTotalNum(std::string resid, int total)
{
	QSqlQuery query;
	QString sql = "update " + res_ResTable +
		" set " + res_ResTotalNum + " = ?  " 
		" where " + res_ResID + " = ? ";
	query.prepare(sql);
	query.bindValue(0, total);
	query.bindValue(1, QString::fromStdString(resid));

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}
}

void DBHelper::updateResourceAvailableNum(std::string resid, int available)
{
	QSqlQuery query;
	QString sql = "update " + res_ResTable +
		" set " + res_ResAvailableNum + " = ? "
		" where " + res_ResID + " = ? ";
	query.prepare(sql);
	query.bindValue(0, available);
	query.bindValue(1, QString::fromStdString(resid));

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}
}

void DBHelper::recordUser(std::string testid, std::string userid)
{
	QSqlQuery query;
	QString sql = "insert into " + test_respond_RespondTable +
		" ( " + test_respond_UsagePK + "," + test_respond_PK + "," + test_respond_UserID + ")" +
		" values ( ?, ? ,? )";
	query.prepare(sql);
	query.bindValue(0, QString::fromStdString(testid));
	QString pk = QUuid::createUuid().toString();
	query.bindValue(1, pk);
	query.bindValue(2, QString::fromStdString(userid));

	qDebug() << query.executedQuery();
	if (query.exec()) {
		qDebug() << " success .";
	}
	else {
		qDebug() << " fail !" << query.lastError();
	}
}

#endif