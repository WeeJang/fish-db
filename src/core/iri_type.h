#ifndef IRT_TYPE_H
#define IRT_TYPE_H

#include<string>
#include<cstdint>

namespace core{

template<typename T>
class IRITypeTrait{ 
public:
	using value_type = typename T::value_type;
};//class IRITypeTrait


namespace IRIType{

//当前用了128-md5中间的64bit,因此uint64可以支撑。以后变化了还需要改.
class HashValue{
public:
	using value_type = uint64_t;
	constexpr static char const* TYPE_STR = "HV";

public:	
	explicit HashValue(value_type value):value_(value){}
	value_type value() { return value_; }

private:
	value_type value_;
};

//字符串仅支持短字符串（人名，地名，公司名,股票代码,schema）
class ShortString{
public:
	using value_type = std::string;
	constexpr static char const* TYPE_STR = "SS";
public:
	explicit ShortString(value_type value):value_(value){}
	value_type value() { return value_; }	
private:
	value_type value_;
};

//helper function
bool is_hashvalue(const char* p_str,size_t len);

}//IRITYPE

}//namesapce core

#endif
