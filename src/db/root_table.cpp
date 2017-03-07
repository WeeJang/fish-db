
#include "root_table.h"

	//std::vector<uint64_t> block_offset_list_;
	//std::vector<std::shared_ptr<BlockIndex>> block_index_list_;	
namespace db{

RootTable(const std::string& root_table_file){
	
}
			
size_t RootTable::get_block_index_by_global_offset(uint64_t global_offset){
	size_t list_len = block_offset_list_.size();
	size_t lower_pos = 0;
	size_t upper_pos = list_len -1;
	
	do{
		if(upper_pos - lower_pos <= 3){
			break;						
		}
		size_t mid_pos = (lower_pos + upper_pos) / 2;
		if(global_offset <= block_offset_list_[mid_pos]){
			upper_pos = mid_pos;
		}else{
			lower_pos = mid_pos;	
		}

	}while(true);

	for(; lower_pos <= upper_pos ; lower_pos ++){
		if(block_offset_list_[lower_pos] > global_offset){
			lower_pos--;
			break;
		}	
	}
	return lower_pos;
}



void RootTable::dump(const std::string& root_table_file = nullptr ){
	if(root_table_file == nullptr){
		root_table_file = table_name_;	
	}
	
	int fd = open(root_table_file.c_str(),O_WRONLY | O_TRUNC,666);
	if(-1 == fd){
		printf("open root_table : %s  failed !\n",root_table_file.c_str());
		exit(-1);
	}
	
	if(write(fd,ROOT_MAGIC_NUM,sizeof(char) * 8) == -1){
		printf("write root table: %s magic number failed !\n",root_table_file.c_str());
		exit(-1);	
	}	
	
				
}

void RootTable::fast_init_by_dump_file(const std::string& root_table_file);


}//namespace db


#endif //DB_ROOT_TABLE_H_
