#include<unordered_map>
#include<memory>

#include "iri_type.h"

namespace core{

template<typename IRIType>
class IRIIndex; //forward-declartion


template<typename T>
class MetaIRIIndex{
	using MAP_KEY_T   = IRITypeTrait<T>::type;
	using MAP_VALUE_T = std::shared_ptr<IRI_Index<T>>;
	using Map_Type 	  = std::unordered_map<MAP_KEY_T,MAP_VALUE_T>;
public:
	//get
	MAP_VALUE_T get_IRI_index(T iri_value){
		auto got = meta_iri_map_.find(iri_value.value());
		if(got == meta_iri_map_.end()){
			return nullptr;
		}else{
			return got->second;
		}
	}
	//insert	
	void insert_IRI_index(T iri_value,MAP_VALUE_T shp_iri_index){
		meta_iri_map_.insert(std::make_pair<MAP_KEY_T,MAP_VALUE_T>(iri_value.value(),shp_iri_index);
	}

private:
	Map_Type meta_iri_map_;
	
};

}//nanmespace core
