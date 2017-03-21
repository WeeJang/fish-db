#include "triple.h"

namespace core{

const std::string& TripleSpec::at(TripleElemPos pos) const {
	if(pos == TripleElemPos::sub){
		return sub_;
	}else if(pos == TripleElemPos::pre){
		return pre_;
	}else if(pos == TripleElemPos::obj){
		return obj_;	
	}else{
		fprintf(stderr,"triplespec at error\n");
		exit(-1);
	}
}

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
	size_t cur_pos  = 0;
	for(;  cur_pos < line_str.size() ; cur_pos ++){
		if('\n' == line_str[cur_pos]){
			if(triple_elem_pos != 3){
				fprintf(stderr,"make_triple_by_line failed !\n");
				return -1;
			}
			break;
		}
		if('\t' != line_str[cur_pos]){
			continue;		
		}
		switch(triple_elem_pos){
			case 0:
				p_triple->sub(line_str.substr(last_pos,cur_pos));
				break;	
			case 1:
				p_triple->pre(line_str.substr(last_pos,cur_pos));
				break;
			default:
				fprintf(stderr,"make_triple_by_line failed in case !\n");
				return -1;
		}
		last_pos += ( cur_pos + 1 );	
		triple_elem_pos ++;
	}
	p_triple->obj(line_str.substr(last_pos,cur_pos));
	return 0;
}


}//namespace core
