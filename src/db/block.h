#ifndef DB_BLOCK_H_
#define DB_BLOCK_H_

#include "../utils/tiny_log.hpp"
#include <iostream>

#include <type_traits>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include <cstdint>
#include <cstring>
#include <cassert>
#include <cstdio>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>


namespace db{

struct alignas(8) BlockHeader{
public:
	//BlockHeader():block_id_(0),\
	//		row_start_index_(0),\
	//			row_count_(0){}

	uint64_t block_id_; //block_id in group
	uint64_t row_start_index_; //row start index of this block in global
	uint64_t row_count_; // row count in this block
};//BlockHeader

struct alignas(8) BlockData{
	constexpr static uint32_t BLOCK_DATA_SIZE_LIMIT = 1 << 27; //128MB
	//BlockData(){ memset(data_,0,sizeof(data_));}

	void write(const char* p_data,size_t data_size){
		memset(data_,0,sizeof(data_));
		assert(data_size <= BLOCK_DATA_SIZE_LIMIT);
		memcpy(data_,p_data,data_size);
	}
private:	
	char data_[BLOCK_DATA_SIZE_LIMIT];
};//BlockData


class BlockIndex; //forward-declaration

struct alignas(8) Block{

	friend class BlockIndex;
	friend class BlockDataSeeker;
public:
	//Block():check_sum_(0) { strcpy(magic_num_,BLOCK_MAGIC_NUM);  };
	
	void dump(const std::string& filename);
	
	const uint64_t block_id() const { return block_header_.block_id_; }
	const uint64_t row_start_index() const { return block_header_.row_start_index_; }	
	const uint64_t row_count() const { return block_header_.row_count_ ;}

	const size_t get_flexible_size_of_block() const {
		return sizeof(Block) + (row_count() - 1) * sizeof(Block::row_data_offset_[0]);
	}

	void write(const char* p_data,size_t size){
		block_data_.write(p_data,size);
	}
	
	//TODO:ugly
	void insert_row_data_offset(uint64_t index,uint32_t offset_value){ row_data_offset_[index] = offset_value ;}	

	//just for test,not use friend class
	void output_offset(void) {
		std::cout << "offset : [ " ;
		for(size_t i = 0 ; i < block_header_.row_count_ ; i++){
			std::cout << row_data_offset_[i] << " |";
		}
		std::cout << " ] " << std::endl;
	}		

//private:
	void block_id(uint64_t block_id){ block_header_.block_id_ = block_id; }
	void row_start_index(uint64_t row_start_index) { block_header_.row_start_index_ = row_start_index; }	
	void row_count(uint64_t row_count) { block_header_.row_count_ = row_count; }		

public:
	constexpr static char BLOCK_MAGIC_NUM[8] = "JWBLOCK";
private:
	char        magic_num_[8];
	char        check_sum_[8];
	BlockHeader block_header_;
	BlockData   block_data_;
	uint64_t    row_data_offset_[1]; // 如果BLOCK长度大于1<<32则需要改变这里.
public:
	using row_data_offset_type = std::remove_extent<decltype(row_data_offset_)>::type;
};


//helper function
std::unique_ptr<db::Block,std::function<void(db::Block*)>> create_block_by_raw_data(const char* p_data,size_t data_size);
std::unique_ptr<db::Block,std::function<void(db::Block*)>> load_from_disk_by_mmap(const std::string filename);


}//namespace db



#endif //DB_BLOCK_H_
