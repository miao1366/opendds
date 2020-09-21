#pragma once

#include <string>
#include <boost\thread\thread_time.hpp>

class Test;

class User
{
public:
	User(std::string name)
		:m_name(name), m_test(nullptr), m_ownTest(nullptr), m_pubHandle(0){}

	std::string name(){
		return m_name;
	}

	Test* test() {
		return m_test;
	}

	void setTimeStamp(boost::system_time ts) {
		m_timeStamp = ts;
	}

	boost::system_time timeStamp() {
		return m_timeStamp;
	}

	void setUserID(std::string id) {
		m_userid = id;
	}

	std::string userID() {
		return m_userid;
	}

	Test* ownTest() {
		return m_ownTest;
	}

	void setPubHandle(long handle) {
		m_pubHandle = handle; 
	}
	long pubHandle() { 
		return m_pubHandle; 
	}

private:
	friend class Test;

	void enterTest(Test* test) {
		m_test = test;
	}

	void exitTest() {
		m_test = nullptr;
	}

	void setOwnTest(Test* test) {
		m_ownTest = test;
	}

	std::string m_name;
	Test* m_test;
	boost::system_time m_timeStamp;

	std::string m_userid;

	Test* m_ownTest;

	long m_pubHandle;

};