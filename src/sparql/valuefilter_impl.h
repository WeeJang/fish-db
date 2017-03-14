#ifndef _VALUE_FILTER_IMPL_H_
#define _VALUE_FILTER_IMPL_H_

#include <set>
#include <string>
#include <sstream>

#include "log.h"

//1.山哥定义的数值过滤接口
//class ValueFilter
//{
//public:
//    virtual ~ValueFilter() {}
//    //return true on valid
//    virtual bool operator()(const ReturnValue& val) = 0;
//    bool is_lastest_data_filter = false;
//};
//

//个人愚见：像filter这种东西，直接做成做成函数接口应该更简洁明了
//std::function + std::bind

//2.当前已有的接口实现
//2.1 数值过滤
//

//---- just test ---



namespace Index{

//TODO:等集成进入之后，直接换成forward-declaration
class Bigpita{
public:

	struct value{
		double real;
		std::string str;
	};

	class ReturnValue{
	public:
		bool is_real_nan() const {return false;}
		value val;
	};

	class ValueFilter
	{
	public:
		virtual bool operator()(const ReturnValue&) = 0;
		virtual std::string to_string() const = 0;
	};

};//Bigpita

}//Index



//---- just test ---

//forward-declara
namespace Index{
class Bigpita;
//struct Bigpita::ReturnValue;
//class Bigpita::ValueFilter;
}//Index


namespace sparql{

class ValueFilterImpl : public Index::Bigpita::ValueFilter
{	
public:
	bool operator()(const Index::Bigpita::ReturnValue&) override {return true;}
	std::string to_string() const override;
};

//AmountType : unsigned char,signed char,int,long,float,double
//Type : = != < <=  > >=
class AmountFilter : public Index::Bigpita::ValueFilter
{
public:
	enum Type { Equal, NotEqual, Less, LessOrEq , Greater, GreaterOrEq };
	AmountFilter(AmountFilter::Type type, double datum):type_(type), datum_(datum){ LOG("AmountFilter Init"); }
	bool operator()(const Index::Bigpita::ReturnValue&) override ;
	std::string to_string() const override;
private:
	Type type_;
	double datum_;
};

class StringFilter : public Index::Bigpita::ValueFilter
{
public:
	StringFilter(const std::set<std::string> datum_set):datum_set_(datum_set){ LOG("StringFilter Init"); }
	bool operator()(const Index::Bigpita::ReturnValue&) override ;
	std::string to_string() const override;
protected:	
	std::set<std::string> datum_set_;
};

// TODO:这个地方需要保证的是，构造的datum_set中的表示日期的字符串格式需要是合法的数据格式
class DateFilter : public StringFilter
{
public:
	DateFilter(std::set<std::string> datum_set):StringFilter(datum_set){ LOG("DateFilter  Init"); }
	std::string to_string() const override;
};

}//sparql

#endif
