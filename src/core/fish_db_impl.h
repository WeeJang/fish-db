#ifndef FISHDB_H_
#define FISHDB_H_

#include "iri_index.hpp"
#include "meta_iri_index.hpp"
#include "triple.h"
#include "../db/root_table.h"
#include "../utils/file_util.h"

#include <string>
#include <vector>
#include <memory>

namespace test{ class FishDBTest; }

namespace fishdb{

class FishDBImpl {
	friend class test::FishDBTest;
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
	void init();

private:
	std::string db_dir_path_;	
	std::string db_roottable_path_;
	std::string db_block_dir_path_;
	std::string db_iri_dir_path_;
	std::string db_name_;
	db::RootTable root_table_;
	core::MetaIRIIndex<core::IRIType::HashValue> hv_meta_iri_index_;
	core::MetaIRIIndex<core::IRIType::ShortString> ss_meta_iri_index_;	
};//class DB

}//namespace fishdb

#endif // FISH_DB_H
