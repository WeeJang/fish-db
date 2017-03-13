#ifndef FISHDB_H_
#define FISHDB_H_

#include "iri_index.hpp"
#include "meta_iri_index.hpp"
#include "triple.h"
#include "../db/block_data_seeker.h"
#include "../db/root_table.h"
#include "../utils/file_util.h"
#include "../utils/tiny_log.hpp"

#include <string>
#include <vector>
#include <unordered_map>
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

	int get_triple_by_row_index(uint64_t row_index,std::shared_ptr<core::TripleSpec> p_triple_spec);

private:
	void init();
	std::shared_ptr<db::BlockDataSeeker> get_block_data_seeker(size_t block_id);

private:
	std::string db_dir_path_;	
	std::string db_roottable_path_;
	std::string db_block_dir_path_;
	std::string db_iri_dir_path_;
	std::string db_name_;
	db::RootTable root_table_;
	core::MetaIRIIndex<core::IRIType::HashValue> hv_meta_iri_index_;
	core::MetaIRIIndex<core::IRIType::ShortString> ss_meta_iri_index_;	
	std::unordered_map<size_t,std::shared_ptr<db::BlockDataSeeker>> block_data_seeker_map_;

};//class DB

}//namespace fishdb

#endif // FISH_DB_H
