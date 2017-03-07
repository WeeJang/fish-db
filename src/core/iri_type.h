#include<cstdint>

namespace core{

template<typename T>


class IRITypeTrait{ 
	using value_type = T::value_type	
};//class IRITypeTrait


namespace IRIType{

//当前用了128-md5中间的64bit,因此uint64可以支撑。以后变化了还需要改.
class HashValue{
	using value_type = uint64_t;
pulbic:	
	explicit HashValue(value_type value):value_(value){}
	value_type value() { return value_; }

private:
	value_type value_
};

//字符串仅支持短字符串（人名，地名，公司名,股票代码）
class ShortString{
	using value_type = std::string;
public:
	explicit ShortString(value_type str):value_(value){}
	value_type value() { return value_; }	
private:
	value_type value_;
};

}//IRITYPE


}//namesapce core

