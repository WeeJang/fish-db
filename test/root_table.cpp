#include "root_table.h"

namespace db{

constexpr char RootTable::ROOT_MAGIC_NUM[8];

RootTable::RootTable(const std::string& root_table_file){
	table_name_ = root_table_file;				
}
			
uint64_t RootTable::get_block_index_by_global_offset(uint64_t global_offset){
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



void RootTable::dump(const std::string& root_table_file){
	int fd = open(root_table_file.c_str(),O_WRONLY | O_CREAT | O_TRUNC,0666);
	if(-1 == fd){
		printf("open root_table : %s  failed !\n",root_table_file.c_str());
		exit(-1);
	}
	
	size_t prtable_spec_flexible_size = sizeof(RootTableSpec) + sizeof(RootTableSpec::data_type) * (block_offset_list_.size() - 1);
	RootTableSpec* prtable_spec = (RootTableSpec*)malloc(prtable_spec_flexible_size);
	strcpy(prtable_spec->magic_num_,RootTable::ROOT_MAGIC_NUM);
	prtable_spec->list_size_ = block_offset_list_.size();
	for(size_t i = 0; i < block_offset_list_.size();i ++){
		prtable_spec->data_[i] = block_offset_list_[i];
	}
	
	if(::write(fd,prtable_spec,prtable_spec_flexible_size) == -1){
		printf("write root table: %s failed !\n",root_table_file.c_str());
		exit(-1);	
	}	

	free(prtable_spec);	
	close(fd);
}


void RootTable::fast_init_by_dump_file(const std::string& root_table_file){
	int fd = open(root_table_file.c_str(),O_RDONLY,666);
	if(-1 == fd){
		printf("open file %s failed !\n",root_table_file.c_str());
		exit(-1);
	}
	struct stat file_st;
	if(fstat(fd,&file_st) == -1){
		printf("get file state of %s failed !\n",root_table_file.c_str());
		exit(-1);
	}
	auto len = file_st.st_size;
	RootTableSpec* prtable_spec = (RootTableSpec*)mmap(nullptr,len,PROT_READ,MAP_SHARED,fd,0);
	if(prtable_spec == nullptr || prtable_spec == (void*)-1){
		printf("mmap file %s failed !\n",root_table_file.c_str());
		exit(-1);
	}
	
	block_offset_list_.clear();
	block_index_list_.clear();
	for(size_t i = 0 ; i < prtable_spec->list_size_; i ++){
		block_offset_list_.push_back(prtable_spec->data_[i]);	
		block_index_list_.push_back(nullptr);//NOTE: lazy load?
	}
	munmap(prtable_spec,len);
	close(fd);
}	

}//namespace db


