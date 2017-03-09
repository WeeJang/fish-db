#ifndef CORE_TRIPLE_H_
#define CORE_TRIPLE_H_

#include <string>

namespace core{

struct TripleSpec{

public:
	void sub(const std::string& sub) { sub_ = sub ; }
	void pre(const std::string& pre) { pre_ = pre ; }
	void obj(const std::string& obj) { obj_ = obj ; }

	std::string to_string();

private:
	std::string sub_;
	std::string pre_;
	std::string obj_;

};//TripleSpec


}//namespace core



#endif //CORE_TRIPLE_H
