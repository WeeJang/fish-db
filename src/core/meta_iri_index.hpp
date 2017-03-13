#include "iri_type.h"
#include "../utils/file_util.h"
#include "../utils/tiny_log.hpp"

#include <memory>
#include <unordered_map>
#include <sstream>
#include <iostream>  //TODO: remove

namespace core{

template<typename IRIType>
class IRIIndex; //forward-declartion

template<typename T>
class MetaIRIIndex{
public:
	using MAP_KEY_T   = typename IRITypeTrait<T>::value_type;
	using MAP_VALUE_T = typename std::shared_ptr<IRIIndex<T>>;
	using Map_Type 	  = typename std::unordered_map<MAP_KEY_T,MAP_VALUE_T>;
public:
	MetaIRIIndex(){ LOG("MetaIRIIndex init"); }
	//get
	MAP_VALUE_T get_IRI_index(T iri_value){
		auto got = meta_iri_map_.find(iri_value.value());
		if(got == meta_iri_map_.end()){
			return {};
		}else{
			return got->second;
		}
	}

	MAP_VALUE_T operator[] (const MAP_KEY_T& key){
		std::cout << "find " << key << std::endl;	
		auto got = meta_iri_map_.find(key);
		if(got == meta_iri_map_.end()){
			std::cout << "new !!! " << key << std::endl;	
			auto new_elem = std::make_shared<IRIIndex<T>>(T(key));
			meta_iri_map_[key] = new_elem;
			return new_elem;
		}else{
			std::cout << "exit !!! " << key << std::endl;	
			return got->second;	
		}
	}
	
	MAP_VALUE_T operator[] (const T& key){
		return  operator[](key.value());	
	}
	//insert	
	void insert_IRI_index(T iri_value,MAP_VALUE_T shp_iri_index){
		meta_iri_map_.insert(std::make_pair<MAP_KEY_T,MAP_VALUE_T>(iri_value.value(),shp_iri_index));
	}

	//save
	void save_to_files(const std::string& meta_iri_path){
		for(auto it = meta_iri_map_.begin() ; it != meta_iri_map_.end(); ++it){
			std::stringstream path_ss;
			path_ss << meta_iri_path << "/" << T::TYPE_STR << "_" << it->first;	
			auto cur_dir = path_ss.str();
			if(-1 == utils::mkdir(cur_dir)){
				fprintf(stderr,"MetaIRIIndex save to file mkdir %s failed !\n",cur_dir.c_str());
				exit(-1);
			}
			if(!(it->second->dump_to_files(cur_dir))){
				fprintf(stderr,"MetaIRIIndex save to %s failed !\n",cur_dir.c_str());
				exit(-1);
			}	
		}			
	}
	
	void load_from_files(const std::string& meta_iri_path){
		load_from_files_<T>(meta_iri_path);
	}

private:
	//load <HashValue>
	template<typename U>
	typename std::enable_if<std::is_same<U,IRIType::HashValue>::value,void>::type
	load_from_files_(const std::string& meta_iri_path){
		//TODO: ugly !
		std::vector<std::string> dict_list = utils::get_dict_list_with_prefix(meta_iri_path,IRIType::HashValue::TYPE_STR);
		LOG("hash value load from file : %s, prefix : %s",meta_iri_path.c_str(),IRIType::HashValue::TYPE_STR);	
		LOG("hash value load from file : %zu",dict_list.size());	
		for(auto& iri_name : dict_list){
			auto hex_value_str = iri_name.substr(3);
			MAP_KEY_T iri_int = ::strtol(hex_value_str.c_str(),nullptr,16);
			auto iri_index = std::make_shared<IRIIndex<IRIType::HashValue>>(IRIType::HashValue(iri_int));
			std::string iri_index_file(meta_iri_path);
			iri_index_file.push_back('/');
			iri_index_file.append(iri_name);
			iri_index->load_from_files(iri_index_file);
			meta_iri_map_[iri_int] = iri_index;
		}
	}

	//load <ShortString>
	template<typename U>
	typename std::enable_if<std::is_same<U,IRIType::ShortString>::value,void>::type
	load_from_files_(const std::string& meta_iri_path){
		//TODO: ugly !
		std::vector<std::string> dict_list = utils::get_dict_list_with_prefix(meta_iri_path,IRIType::ShortString::TYPE_STR);
		LOG("hash value load from file : %s, prefix : %s",meta_iri_path.c_str(),IRIType::ShortString::TYPE_STR);	
		LOG("shortstring load from file : %zu",dict_list.size());	
		for(auto& iri_name : dict_list){
			auto ss_value_str = iri_name.substr(3);
			auto iri_index = std::make_shared<IRIIndex<IRIType::ShortString>>(IRIType::ShortString(ss_value_str));
			std::string iri_index_file(meta_iri_path);
			iri_index_file.push_back('/');
			iri_index_file.append(iri_name);
			iri_index->load_from_files(iri_index_file);
			meta_iri_map_[ss_value_str] = iri_index;
		}
	}
private:
	Map_Type meta_iri_map_;	
};

}//nanmespace core
