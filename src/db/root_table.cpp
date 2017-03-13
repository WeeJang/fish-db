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
			break;
		}	
	}

	return --lower_pos;
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


void RootTable::modify_block_index_at(uint64_t block_id,uint64_t block_offset,std::shared_ptr<BlockIndex> p_block_index){
	block_offset_list_[block_id] = block_offset;
	block_index_list_[block_id] = p_block_index;	

}

bool RootTable::append_block_index(uint64_t block_offset,std::shared_ptr<BlockIndex> p_block_index){
	block_offset_list_.push_back(block_offset);	
	block_index_list_.push_back(p_block_index);
	return true;
}

	

int RootTable::get_seek_pos_by_row_index(uint64_t row_index,uint32_t& ret_block_id,uint64_t& ret_block_offset){
	ret_block_id = get_block_index_by_global_offset(row_index);
	LOG("get ret_block_id : %d, block_index_list_ size : %d",ret_block_id,block_index_list_.size());
	auto p_block_index = block_index_list_[ret_block_id];
	LOG("p_block_index : %p",p_block_index.get());
	LOG("get row_start_index : %llu",p_block_index->row_start_index());
	auto row_index_in_this_block = row_index - p_block_index->row_start_index();
	LOG("row_index in this block : %d",row_index_in_this_block);
	ret_block_offset = p_block_index->at(row_index_in_this_block);
	LOG("get ret_block_offset : %d",ret_block_offset);
	return 0;					
}

}//namespace db


