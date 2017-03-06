#include<unordered_map>
#include<memory>

#include "iri_type.h"

namespace core{


template<typename IRIType>
class IRIIndex; //forward-declartion


template<typename T>
class MetaIRIIndex{
	std::shared_ptr<IRIIndex<T>> get_IRI_index(T iri_value);	
};


template<>
class MetaIRIIndex<IRIType::HashValue>{
	using MAP_KEY_T   = IRITypeTrait<IRIType::HashValue>::type;
	using MAP_VALUE_T = std::shared_ptr<IRI_Index<IRIType::HashValue>>;
	using Map_Type 	  = std::unordered_map<MAP_KEY_T,MAP_VALUE_T>;
public:
	MAP_VALUE_T get_IRI_index(IRIType::HashValue iri_value){
		auto got = meta_iri_map_.find(iri_value.value());
		if(got == meta_iri_map_.end()){
			return nullptr;
		}else{
			return got->second;
		}
	}
	
	void insert_IRI_index(IRIType::HashValue iri_value,MAP_VALUE_T shp_iri_index){
		meta_iri_map_[iri_value.value()] = shp_iri_index;	
	}

private:
	Map_Type meta_iri_map_;
	
};

template<>
class MetaIRIIndex<IRITYPE::ShortString>{

};



}//nanmespace core
