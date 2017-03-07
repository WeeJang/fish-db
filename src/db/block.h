#ifndef DB_BLOCK_H_
#define DB_BLOCK_H_
#include "block_metadata.h"

#include <vector>
#include <memory>

#include <cstdint>
#include <cstring>
#include <cassert>
#include <cstdio>

namespace db{

struct BlockHeader{
public:
	BlockHeader():block_id_(0),\
			row_start_index_(0),\
			row_count_(0){}

	uint64_t block_id_; //block_id in group
	uint64_t row_start_index_; //row start index of this block in global
	uint64_t row_count_; // row count in this block
};//BlockHeader

struct BlockData{
	constexpr static uint32_t BLOCK_DATA_SIZE_LIMIT = 1 << 27; //128MB
	BlockData(){
		memset(data_,0,sizeof(data_));
	}

	void write(const char* p_data,size_t data_size){
		memset(data_,0,sizeof(data_));
		assert(data_size <= BLOCK_DATA_SIZE_LIMIT);
		memcpy(data_,p_data,data_size);
	}
private:	
	char data_[BLOCK_DATA_SIZE_LIMIT];
};//BlockData

struct DataIndex{	
	DataIndex()
	
};//DataIndex

class Block{
	constexpr static BLOCK_MAGIC_NUM[8] = "CAYBACK";

public:
	Block():check_sum_(0) { strcpy(magic_num_,BLOCK_MAGIC_NUM);  };
	
	void dump(const std::string filename);
	
	const uint64_t block_id() const { return block_header_.block_id_; }
	const uint64_t row_start_index() const { return block_header_.row_start_index_; }	
	const uint64_t row_count() const { return block_header_.row_count ;}

	const size_t get_flexible_size_of_block() const {
		return sizeof(Block) + (row_count() - 1) * sizeof(Block::row_data_offset[0]);
	}

private:
	void block_id(uint64_t block_id){ block_header_.block_id_ = block_id; }
	void row_start_index(uint64_t row_start_index) { block_header_.row_start_index_ = row_start_index; }	
	void row_count(uint64_t row_count) { block_header_.row_count_ = row_count; }	

private:
	char        magic_num_[8];
	char        check_sum_;
	BlockHeader block_header_;
	BlockData   block_data_;
	uint32_t    row_data_offset[1]; // 如果BLOCK长度大于1<<32则需要改变这里.
};


//helper function
std::unique_ptr<Block,std::function<void(Block*)>> create_block_by_raw_data(const char* p_data,size_t data_size);
std::unique_ptr<Blcok,std::function<void(Block*)>> load_from_disk_by_mmap(const std::string filename);


}//namespace db



#endif //DB_BLOCK_H_
