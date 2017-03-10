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
	int close_db()  ;
	
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
	std::string db_dir_path_;	
	std::string db_name_;
	db::RootTable root_table_;

};//class DB

}//namespace fishdb

#endif // FISH_DB_H
