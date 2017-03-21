#ifndef CORE_TRIPLE_H_
#define CORE_TRIPLE_H_

#include <string>
#include <cstdlib>

namespace core{


enum class TripleElemPos{
	sub = 0,
	pre,
	obj
}; //class TripleElemPos


struct TripleSpec{

public:
	void sub(const std::string& sub) { sub_ = sub ; }
	void pre(const std::string& pre) { pre_ = pre ; }
	void obj(const std::string& obj) { obj_ = obj ; }
	const std::string& at(TripleElemPos pos) const ;	
	std::string to_string();

private:
	std::string sub_;
	std::string pre_;
	std::string obj_;

};//TripleSpec

//helper function
TripleSpec make_triple_by_line(const std::string triple_line);

}//namespace core



#endif //CORE_TRIPLE_H
