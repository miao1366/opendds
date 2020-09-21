#ifndef __COMMOM_H__
#define __COMMOM_H__

#include <string>
#include <vector>
#include <typeinfo>
#include <boost/variant.hpp> 
#include <istream>

#ifdef DATA_EXCHANGE_PLATFORM_DLL 
#define DATA_EXCHANGE_PLATFORM_EXPORT __declspec( dllexport )
#else
#define DATA_EXCHANGE_PLATFORM_EXPORT __declspec( dllimport )
#endif

namespace Data_Exchange_Platform{

	class InterationInstanceImpl;

	//交互类接收过程函数
	typedef void (*InterationRecvFunc) (InterationInstanceImpl* );

	//数据类型
	typedef const std::type_info DataType;
	
	//可变数据类型，支持bool,int,__int64,unsigned int,short,long,double,std::string
	typedef boost::variant<bool, int, __int64, unsigned int, short, long, double, std::string, \
		std::vector<bool>, std::vector<int>, std::vector<__int64>, std::vector<unsigned int>, \
		std::vector<short>, std::vector<long>, std::vector<double>, std::vector<std::string>, std::vector<std::vector<std::string>>> Variant;
	
	//可变数据类型为NULL值
	#define VariantNULL (Variant())

	//将数据类型转为参数，如DATATYPE(int)
	#define DATATYPE(typename) (typeid(typename))
	
	//可变数据类型指针
	typedef void* VariantPtr;
	
	//数据交互方式
	enum ExchangeType{
		None = 0x00,				//不交互
		Publish_Only = 0x01,		//仅发布
		Subscribe_Only = 0x02,		//仅订阅
		Publish_Subscribe = 0x03	//即发布又订阅
	};
	
}


#endif //__COMMOM_H__
