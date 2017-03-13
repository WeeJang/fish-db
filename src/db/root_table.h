#ifndef DB_ROOT_TABLE_H_
#define DB_ROOT_TABLE_H_

#include "block_index.h"
#include "block_data_seeker.h"

#include <vector>
#include <memory>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace test{ class RootTableTest; /* forward-declaration */ } //

namespace db{
//-----------------
// | MAGIC_NUM | -- char[8] 
// | LIST_SIZE | -- uint64
// | DATA .....| -- uint64[LIST_SIZE]
//-----------------
struct RootTableSpec{
	friend class RootTable;
public:
	size_t get_flexible_size(){ return sizeof(RootTableSpec) + sizeof(data_type) * (list_size_ - 1);}
private:
	char magic_num_[8];
	uint64_t list_size_;
	uint64_t data_[1];	
	using data_type = std::remove_extent<decltype(data_)>::type;
};//struct RootTableSpec


class RootTable{
	friend class test::RootTableTest;
	constexpr static char ROOT_MAGIC_NUM[8] = "JWROOTT";
public:
	RootTable(){}
	RootTable(const std::string& root_table_file);
			
	uint64_t get_block_index_by_global_offset(uint64_t global_offset);	
	void modify_block_index_at(uint64_t block_id,uint64_t block_offset,std::shared_ptr<BlockIndex> p_block_index);	
	bool append_block_index(uint64_t block_offset,std::shared_ptr<BlockIndex> p_block_index);	
	void dump(const std::string& root_table_file);
	void dump() { dump(table_name_); }
	void fast_init_by_dump_file(const std::string& root_table_file);

	void resize_offset_list(size_t new_size) {
		block_offset_list_.resize(new_size);
		block_index_list_.resize(new_size);
	}
	
	/**
	* 0 correct
	*/	
	int get_seek_pos_by_row_index(uint64_t row_index,uint32_t& ret_block_id,uint64_t& ret_block_offset);

	
private:
	std::shared_ptr<BlockDataSeeker> get_block_data_seeker(size_t block_id);

private:
	std::vector<uint64_t> block_offset_list_;
	std::vector<std::shared_ptr<BlockIndex>> block_index_list_;	
	std::string table_name_; //block name : table_name.part_id
				
};//class RootTable


}//namespace db


#endif //DB_ROOT_TABLE_H_
