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


//helper function
int make_triple_by_line(const std::string& line_str,std::shared_ptr<TripleSpec> p_triple){
	char triple_elem_pos = 0;	
	size_t last_pos = 0;
	for(size_t i = 0 ;  i < line_str.size() ; i++){
		if('\n' == line_str[i]){
			if(triple_elem_pos != 3){
				fprintf(stderr,"make_triple_by_line failed !"\n);
				return -1;
			}
			break;
		}
		if('\t' != line_str[i]){
			continue;		
		}
		switch(triple_elem_pos){
			case 0:
				p_triple->sub(line_str.substr(last_pos,i));
				break;	
			case 1:
				p_triple->pre(line_str.substr(last_pos,i));
				break;
			default:
				fprintf(stderr,"make_triple_by_line failed in case !"\n);
				return -1;
		}
		last_pos += ( i + 1 );	
		triple_elem_pos ++;
	}
	p_triple->obj(line_str.substr(last_pos,i));
	return 0;
}


}//namespace core
