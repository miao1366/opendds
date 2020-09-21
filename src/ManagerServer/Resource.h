#pragma once

#include <string>
#include <vector>
#include <boost\thread\thread_time.hpp>

class Test;

class Resource
{
public:
	Resource(std::string name, std::string id, std::vector<std::string> exparam)
		:m_name(name), m_id(id), m_enabled(true), m_test(nullptr), m_exparam(exparam), m_pubHandle(0){}

	std::vector<std::string> exparam() {
		return m_exparam;
	}

	std::string name() {
		return m_name;
	}

	std::string id() {
		return m_id;
	}

	void setEnabled(bool enable) {
		m_enabled = enable;
	}

	bool enabled() {
		return m_enabled;
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

	void setResID(std::string id) {
		m_resid = id;
	}

	std::string resID() {
		return m_resid;
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

	std::string m_id;
	std::string m_name;
	std::vector<std::string> m_exparam;
	bool m_enabled;
	Test* m_test;

	boost::system_time m_timeStamp;

	std::string m_resid;

	long m_pubHandle;
};

