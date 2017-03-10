#ifndef FISHDB_H_
#define FISHDB_H_

#include "triple.h"
#include "../utils/file_util.h"

#include <string>
#include <vector>
#include <memory>

namespace fishdb{

class FishDbImpl {

public:
	FishDBImpl() { }
	virtual ~FishDBImpl() {}

	int open_db(const std::string& db_name) ;
	int create_db(const std::string& db_name) ;
	int close_db() ;
		
	int load_data(const std::string& triple_file_path);

	/**
	*	get_triple("df:96666","df:type.object.name","?",query_result);
	*	get_triple("?","df:type.object.name","?",query_result);
	*/	
	int get_triple(const std::string& sub_str,\
				const std::string& pre_str,\
				const std::string& obj_str,\
				std::shared_ptr<std::vector<core::TripleSpec>> query_result);

private:
	void init(){
		db_dir_path_ =std::string("/Users/jiangwei/Workspace/cayley-backend/data/");
		db_dir_path_ += db_name_;
		
		db_block_dir_path_ = db_dir_path_;	
		db_roottable_dir_path_ = db_dir_path_;
		
		db_block_dir_path_ += std::string("/block/");
		db_roottable_path_ += std::string("/root");
		db_iri_dir_path_   += std::string("/iri_index/");	
	}
	
private:
	std::string db_dir_path_;	
	std::string db_roottable_path_;
	std::string db_block_dir_path_;
	std::string db_iri_dir_path_;
	std::string db_name_;
	db::RootTable root_table_;
	core::MetaIRIIndex<HashValue> hv_meta_iri_index_;
	core::MetaIRIIndex<ShortString> ss_meta_iri_index_;	
};//class DB

}//namespace fishdb

#endif // FISH_DB_H
