#include "triple.h"

namespace core{

std::string TripleSpec::to_string(){
	std::string buffer;
	buffer.append(sub_.c_str());
	buffer.push_back('\t');
	buffer.append(pre_.c_str());
	buffer.push_back('\t');	
	buffer.append(obj_.c_str());
	return buffer;
}


}//namespace core
