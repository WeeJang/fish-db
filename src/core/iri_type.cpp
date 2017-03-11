#include "iri_type.h"

namespace core{

namespace IRIType{

constexpr char HashValue::TYPE_STR[3];
constexpr char ShortString::TYPE_STR[3];

bool is_hashvalue(const char* p_str,size_t len){
	if(len != 16){
		return false;
	}	
	for(size_t i = 0 ; i < len ; i++){
		if((*(p_str + i) >= '0') && (*(p_str + i) <= '9')){
			continue;
		} 	
		if((*(p_str + i) >= 'a') && (*(p_str + i) <= 'z')){
			continue;
		} 	
		if((*(p_str + i) >= 'A') && (*(p_str + i) <= 'Z')){
			continue;
		} 
		return false;	
	}
	return true;		
}


}//IRIType

}//core
