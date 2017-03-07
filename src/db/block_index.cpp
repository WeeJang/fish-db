#include "block_index.h"

namespace db{

BlockIndex::BlockIndex(const Block& block){
	block_id = block.block_id();
	row_start_index = block.row_start_index();
	row_count = block.row_count();
	for(size_t i = 0 ; i < row_count ; i ++){
		index_.push_back(row_data_offset[i]);
	}
}

BlockIndex::BlockIndex(const string& filename)
	int fd = open(filename.c_str(),O_RDONLY | O_DIRECT );
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
	if(lseek(fd,sizeof(char),SEEK_CUR) == -1){
		printf("file: %s lseek failed \n");
		exit(-1);
	}
	size_t bheader_size = sizeof(Block::BlockHeader);	
	BlockHeader* p_blockheader = (BlockHeader*)malloc(bheader_size);
	if(read(fd,p_blockheader,bheader_size) == -1){
		printf("read blockheader from %s failed !",filename.c_str());
		exit(-1);
	}
	block_id = p_blockheader->block_id();
	row_start_index = p_blockheader->row_start_index();
	row_count = p_blockheader->row_count();
	free(p_blockheader);
	
	if(lseek(fd,sizeof(Block::BlockData),SEEK_CUR) == -1){
		printf("file: %s lseek failed \n",filename.c_str());
		exit(-1);
	}
	size_t bindex_size = sizeof(IndexType) * row_count ;
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
	for(size_t i = 0; i < row_count ; i++){
		index_.push_back(*(p_index + i));
	}
	free(p_index);
	close(fd);
}//namesapce db
