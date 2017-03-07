#include "block.cpp"

namespace db{

std::unique_ptr<Block,std::function<void(void)>> create_block_by_raw_data(const char* p_data,size_t data_size){
	assert(data_size <= BlockData::BLOCK_DATA_SIZE_LIMIT);
	
	size_t   cur_data_pos = 0;
	std::vector<uint32_t> offset_list;	
	
	offset_list.push_back(0);
	for(; cur_data_pos < data_size ;){
		if(*(p_data + cur_data_pos) == '\n'){
			offset_list.push_back(++cur_data_pos);	
		}else{
			++cur_data_pos;
		}	
	}
	
	row_count(offset_list.size());

	size_t flexible_block_size = sizeof(Block) + (offset_list.size() - 1) * sizeof(Block::row_data_offset[0]);
	char* buf = (char*)malloc(flexible_block_size); 
	Block* p_block = new (buf) Block;
	auto deleter = [p_block, buf](){ p_block->~Block(); free(buf); };	
	
	for(int i = 0 ; i < offset_list.size() ; i ++){
		p_block->row_data_offset[i] = offset_list[i];	
	}
	
	p_block->write(p_data,data_size);	
	
	//TODO : check_sum_
	return std::unique_ptr<Block,std::function<void(void)>(p_block,deleter);
}


void Block::dump(const std::string filename){
	int fd = open(filename.c_str(),O_WRONLY | O_TRUNC | O_CREAT | O_DIRECT ,666);
	if(fd == -1){
		printf("open file %s failed !", filename.c_str());
		exit(-1);				
	}			
	int ret_code = write(fd,this,get_flexible_size_of_block());
	if(ret_code = -1){
		printf("write file %s failed !",filename.c_str());
		exit(-1);	
	}
	close(fd);
}

void Block::load(const std::string filename){	

}



}//namespace db

