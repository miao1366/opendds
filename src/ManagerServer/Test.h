#pragma once

#include <string>
#include <list>
#include <map>
#include <time.h>
#include <boost\uuid\uuid.hpp>
#include <boost\uuid\uuid_io.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\uuid\uuid_generators.hpp>

#include <TimeServerAmbassador.h>

using namespace Data_Exchange_Platform;

class User;
class Resource;

class Test
{
public:
	Test(std::string name, std::string password, long domain_ID, User* own);

	~Test();

	std::string name() {
		return m_name;
	}

	std::string password() {
		return m_password;
	}

	long domain_ID() {
		return m_domain_ID;
	}

	void enter(User* user);
	void enter(Resource* resource);

	void exit(User* user);
	void exit(Resource* resource);

	std::list<User*>& vecUser() {
		return m_lstUser;
	}
	std::list<Resource*>& vecResource() {
		return m_lstResource	;
	}

	time_t beginTime() {
		return m_begin;
	}

	//tmp
	void setResID(std::string resID) {
		m_resID = resID;
	}

	std::string resID() {
		return m_resID;
	}

	std::string testID() {
		return m_testID;
	}

	void setDescription(std::string des) {
		m_description = des;
	}

	std::string description() {
		return m_description;
	}

	void setExparam(std::vector<std::string> param) {
		m_exparam = param;
	}

	std::vector<std::string> exparam() {
		return m_exparam;
	}

	void setUserExparam(std::string username, std::vector<std::string> param) {
		m_user_exparam[username] = param;
	}

	std::vector<std::string> userExparam(std::string username) {
		if (m_user_exparam.find(username) != m_user_exparam.end())
		{
			return m_user_exparam.at(username);
		}
		return std::vector<std::string>();
	}

	User* owner() {
		return m_ownUser;
	}

private:

	std::string m_name;
	std::string m_password;
	long m_domain_ID;

	std::list<User*> m_lstUser;
	std::list<Resource*> m_lstResource;
	time_t m_begin;

	std::string m_resID;

	std::string m_testID;
	std::string m_description;
	std::vector<std::string> m_exparam;

	std::map<std::string, std::vector<std::string>> m_user_exparam;

	User* m_ownUser;

	TimeServerAmbassador* m_ambassador;
};

