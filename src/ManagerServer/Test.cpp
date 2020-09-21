#include "Test.h"
#include "User.h"
#include "Resource.h"

Test::Test(std::string name, std::string password, long domain_ID, User * own)
	:m_name(name), m_password(password), m_domain_ID(domain_ID), m_begin(time(0)), m_ownUser(own) {
	boost::uuids::random_generator rgen;
	boost::uuids::uuid uuid = rgen();
	m_testID = boost::lexical_cast<std::string>(uuid);
	m_ownUser->setOwnTest(this);

	m_ambassador = new TimeServerAmbassador(domain_ID);
}

Test::~Test()
{

	for (std::list<User*>::iterator iu = m_lstUser.begin();
		iu != m_lstUser.end(); iu++)
	{
		(*iu)->exitTest();
	}

	for (std::list<Resource*>::iterator ir = m_lstResource.begin();
		ir != m_lstResource.end(); ir++)
	{
		(*ir)->exitTest();
	}
	
	delete m_ambassador;
}

void Test::enter(User * user)
{
	m_lstUser.push_back(user);
	user->enterTest(this);
}

void Test::enter(Resource * resource)
{
	m_lstResource.push_back(resource);
	resource->enterTest(this);
}

void Test::exit(User * user)
{
	m_user_exparam.erase(user->name());
	m_lstUser.remove(user);
	user->exitTest();
}

void Test::exit(Resource * resource)
{
	m_lstResource.remove(resource);
	resource->exitTest();
}
