#include "iri_type.h"


#include <memory>
#include <unordered_map>
#include <sstream>

namespace core{

template<typename IRIType>
class IRIIndex; //forward-declartion

template<typename T>
class MetaIRIIndex{
	using MAP_KEY_T   = IRITypeTrait<T>::value_type;
	using MAP_VALUE_T = std::shared_ptr<IRIIndex<T>>;
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

	//save
	void save_to_files(const std::string& meta_iri_path){
		for(auto it = meta_iri_map_.begin() ; it != meta_iri_map_.end(); ++it){
			std::stringstream path_ss;
			path_ss << meta_iri_path << "/" << T::TYPE_STR << "_" << it->first.value();	
			auto cur_dir = path_ss.str();
			if(-1 == utils::mkdir(cur_dir)){
				fprintf(stderr,"MetaIRIIndex save to file mkdir %s failed !\n",cur_dir.c_str());
				exit(-1);
			}
			if(!(it->second->save_to_files(cur_dir))){
				fprintf(stderr,"MetaIRIIndex save to %s failed !\n",cur_dir.c_str());
				exit(-1);
			}	
		}			
	}	

	//load <HashValue>
	void load_from_files(const std::string& meta_iri_path,\
			typename std::enable_if<std::is_same<T,HashValue>::value,int>::type = 0 ){
		//TODO: ugly !
		std::vector<std::string> dict_list = utils::get_dict_list_with_prefix(meta_iri_path,HashValue::TYPE_STR);
		for(auto& iri_name : dict_list){
			auto hex_value_str = iri_index.substr(3);
			MAP_KEY_T iri_int = ::strtol(hex_value_str.c_str(),nullptr,16);
			auto iri_index = std::make_shared<IRIIndex<HashValue>>(iri_int);
			iri_index.load_from_files(iri_index,dict_list.append('\').append(iri_name));
			meta_iri_map_[iri_int] = iri_index;
		}
	}

	//load <ShortString>
	void load_from_files(const std::string& meta_iri_path,\
			typename std::enable_if<std::is_same<T,ShortString>::value,int>::type = 0 ){
		//TODO: ugly !
		std::vector<std::string> dict_list = utils::get_dict_list_with_prefix(meta_iri_path,ShortString::TYPE_STR);
		for(auto& iri_name : dict_list){
			auto ss_value_str = iri_index.substr(3);
			auto iri_index = std::make_shared<IRIIndex<ShortString>>(ss_value_str);
			iri_index.load_from_files(iri_index,dict_list.append('\').append(iri_name));
			meta_iri_map_[ss_value_str] = iri_index;
		}
	}
private:
	Map_Type meta_iri_map_;	
};

}//nanmespace core
