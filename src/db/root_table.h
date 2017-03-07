#ifndef DB_ROOT_TABLE_H_
#define DB_ROOT_TABLE_H_

#include <vector>

namespace db{

class RootTable{

public:
	RootTable(const std::string& root_table_file);
			
	uint64_t get_block_index_by_global_offset(uint64_t global_offset);	
private:
	std::vector<uint64_t> block_offset_list_;
	std::vector<std::shared_ptr<BlockIndex>> block_index_list_;	
				
};//class RootTable


}//namespace db


#endif //DB_ROOT_TABLE_H_
