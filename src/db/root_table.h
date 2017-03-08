#ifndef DB_ROOT_TABLE_H_
#define DB_ROOT_TABLE_H_

#include <vector>

namespace db{

//-----------------
// | MAGIC_NUM | -- char[8] 
// | LIST_SIZE | -- uint64
// | DATA .....| -- uint64[LIST_SIZE]
//-----------------
struct RootTableSpec{
	char magic_num_[8];
	uint64_t list_size_;
	uint64_t data_[1];	
};//struct RootTableSpec


class RootTable{
	constexpr static char ROOT_MAGIC_NUM[8] = "JWROOTT";
public:
	RootTable(const std::string& root_table_file);
			
	uint64_t get_block_index_by_global_offset(uint64_t global_offset);	

	void dump(const std::string& root_table_file = nullptr);
	void fast_init_by_dump_file(const std::string& root_table_file);
private:
	std::vector<uint64_t> block_offset_list_;
	std::vector<std::shared_ptr<BlockIndex>> block_index_list_;	
	std::string table_name_; //block name : table_name.part_id
				
};//class RootTable


}//namespace db


#endif //DB_ROOT_TABLE_H_
