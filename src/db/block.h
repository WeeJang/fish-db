#ifndef DB_BLOCK_H_
#define DB_BLOCK_H_
#include "block_metadata.h"

#include <cstdint>
#include <cstring>


namespace db{


struct BlockHeader{
	uint64_t block_id_; //block_id in group
	uint64_t row_start_index_; //row start index of this block in global
	uint64_t row_count_; // row count in this block
};//BlockHeader

struct BlockData{
	constexpr static uint32_t BLOCK_DATA_SIZE_LIMIT = 2 << 27; //128MB
	BlockData(){
		memset(data_,0,sizeof(data_));
	}
private:	
	char data_[BLOCK_DATA_SIZE_LIMIT];
};//BlockData

class Block{
	constexpr static BLOCK_MAGIC_NUM[8] = "CAYBACK1";
public:
	Block() { strcpy(magic_num_,BLOCK_MAGIC_NUM); };

	void block_id(uint64_t block_id){ block_header_.block_id_ = block_id; }
	const uint64_t block_id() const { return block_header_.block_id_; }
	void row_start_index(unit64_t row_start_index) { block_header_.row_start_index_ = row_start_index ;}	
	const uint64_t row_start_index const { return block_header_.row_start_index_; }	
	void row_count(uint64_t row_count) { block_header_.row_count_ = row_count ;}	
	const uint64_t row_count() const { return block_header_.row_count ;}

private:
	char        magic_num_[8];
	char        check_sum_;
	BlockHeader block_header_;
	BlockData   block_data_;	
};


}//namespace db



#endif //DB_BLOCK_H_
