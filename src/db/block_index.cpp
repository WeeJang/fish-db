#include "block_index.h"

#include <iostream>

namespace db{

BlockIndex::BlockIndex(const Block& block){
	block_id_ = block.block_id();
	row_start_index_ = block.row_start_index();
	row_count_ = block.row_count();
	for(size_t i = 0 ; i < row_count_ ; i ++){
		index_.push_back(block.row_data_offset_[i]);
	}
}

BlockIndex::BlockIndex(const std::string& filename){
	int fd = open(filename.c_str(),O_RDONLY); 
	if(-1 == fd){
		printf("block_index open file %s failed !\n",filename.c_str());
		exit(-1);
	}
	size_t magic_num_size = 8;
	char magic_num[magic_num_size];
	if(read(fd,magic_num,magic_num_size) == -1){
		printf("read %s failed!\n",filename.c_str());
		exit(-1);
	}
	if(strcmp(magic_num,Block::BLOCK_MAGIC_NUM) != 0){
		printf("file: %s is illegal, magic num check failed !\n",filename.c_str());
		exit(-1);
	}

	//TODO check check_sum
	if(lseek(fd,8 * sizeof(char),SEEK_CUR) == -1){
		printf("file:lseek failed \n");
		exit(-1);
	}
	std::cout << "!!!!!!!!" << std::endl;
	std::cout << lseek(fd,0,SEEK_CUR) << "seek !!!" << sizeof(char) << std::endl;	
	std::cout << "!!!!!!!!" << std::endl;
	
	size_t bheader_size = sizeof(BlockHeader);	
	BlockHeader* p_blockheader = (BlockHeader*)malloc(bheader_size);
	if(read(fd,p_blockheader,bheader_size) == -1){
		printf("read blockheader from %s failed !",filename.c_str());
		exit(-1);
	}
	
	
	std::cout << bheader_size << "header_size" << std::endl;
	block_id_ = p_blockheader->block_id_;
	row_start_index_ = p_blockheader->row_start_index_;
	row_count_ = p_blockheader->row_count_;
	free(p_blockheader);
	
	std::cout << "===========" << std::endl;
	std::cout << "bock_id " << block_id_ << std::endl;
	std::cout << "row_start_index_ " << row_start_index_ << std::endl;
	std::cout << "row_count_" << row_count_ << std::endl;
	std::cout << "===========" << std::endl;

	
	if(lseek(fd,sizeof(BlockData),SEEK_CUR) == -1){
		printf("file: %s lseek failed \n",filename.c_str());
		exit(-1);
	}
	size_t bindex_size = sizeof(IndexType) * row_count_ ;
	
	std::cout << bindex_size << "bind_size" << std::endl;
	IndexType* p_index = (IndexType*)malloc(bindex_size); 
	if(read(fd,p_index,bindex_size) == -1){
		printf("file : %s read index failed!\n",filename.c_str());	
		exit(-1);	
	}
	char check_eof;
	if(read(fd,&check_eof,1) != 0){
		printf("file :%s check_eof failed!\n",filename.c_str());
		exit(-1);
	}
	for(size_t i = 0; i < row_count_ ; i++){
		index_.push_back(*(p_index + i));
	}
	free(p_index);
	close(fd);
}


}//namesapce db
