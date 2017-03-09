#include "block.h"

namespace db{

constexpr char Block::BLOCK_MAGIC_NUM[8];


void Block::dump(const std::string& filename){
	strcpy(magic_num_,BLOCK_MAGIC_NUM);
	int fd = open(filename.c_str(),O_WRONLY | O_TRUNC | O_CREAT ,0666);
	if(fd == -1){
		printf("open file %s failed !\n", filename.c_str());
		exit(-1);				
	}	
	std::cout << "alignof  : " << alignof(Block)  << std::endl;
	std::cout << "BlockHeader size :" << sizeof(BlockHeader)  << std::endl;
	std::cout << "BlockData size :" << sizeof(BlockData)  << std::endl;
	std::cout << "magic_num_ size :" << sizeof(magic_num_)  << std::endl;
	std::cout << "check_sum_ size :" << sizeof(check_sum_)  << std::endl;
	std::cout << "row_data_off size :" << sizeof(row_data_offset_)  << std::endl;
	std::cout << "row_data_offset_type size :" << sizeof(row_data_offset_type)  << std::endl;
	std::cout << "Block size :" << sizeof(Block)  << std::endl;
	std::cout << "dump size :" << get_flexible_size_of_block() << std::endl;
	std::cout << "is pod : " << std::is_trivial<BlockHeader>::value << std::endl;
	std::cout << "is pod : " << std::is_trivial<BlockData>::value << std::endl;
	std::cout << "is pod : " << std::is_trivial<Block>::value << std::endl;
	int ret_code = ::write(fd,this,get_flexible_size_of_block());
	if(ret_code == -1){
		printf("write file %s failed !\n",filename.c_str());
		exit(-1);	
	}
	close(fd);
}



//----------------------helper function---------------------//
std::unique_ptr<Block,std::function<void(Block*)>> create_block_by_raw_data(const char* p_data,size_t data_size){
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
	
	size_t flexible_block_size = sizeof(Block) + (offset_list.size() - 1) * sizeof(Block::row_data_offset_type);
	char* buf = (char*)malloc(flexible_block_size); 
	Block* p_block = new (buf) Block;
	auto deleter = [buf](Block* p_block){ p_block->~Block(); free(buf); };	
	p_block->row_count(offset_list.size());
	
	for(int i = 0 ; i < offset_list.size() ; i ++){
		p_block->insert_row_data_offset(i,offset_list[i]);
	}
	
	p_block->write(p_data,data_size);	
	
	//TODO : check_sum_
	return std::unique_ptr<Block,std::function<void(Block*)>>(p_block,deleter);
}

std::unique_ptr<Block,std::function<void(Block*)>> load_from_disk_by_mmap(const std::string filename){	
	int fd = open(filename.c_str(),O_RDONLY,666);
	if(fd == -1){
		printf("open file %s failed !\n",filename.c_str());
		exit(-1);
	}
	struct stat file_st;
	if(fstat(fd,&file_st) == -1){
		printf("get file state of %s failed!\n",filename.c_str());
		exit(-1);
	}
	auto len = file_st.st_size;
	
	LOG("file len %d",len);	
	
	Block* p_block = (Block*)mmap(nullptr,len,PROT_READ,MAP_SHARED,fd,0);
	if(p_block == nullptr || p_block == (void*) -1){
		printf("mmap file  %s failed !\n",filename.c_str());
		exit(-1);	
	}

	auto deleter = [fd,len](Block* p_block){
		munmap(p_block,len);
		close(fd);
	};
	return std::unique_ptr<Block,std::function<void(Block*)>>(p_block,deleter);	
}



}//namespace db

