#ifndef DB_BLOCK_INDEX_H_
#define DB_BLOCK_INDEX_H_

#include "../utils/tiny_log.hpp"
#include "block.h"

#include <string>
#include <vector>

#include <cstddef>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace db{

class BlockIndex{
	friend class test::BlockIndexTest;
	using IndexType = Block::row_data_offset_type;
public:
	explicit BlockIndex(const Block&);
	explicit BlockIndex(const std::string& blockfile_name);

	const uint64_t block_id() const { return block_id_; }
	const uint64_t row_start_index() const { return row_start_index_; }
	const uint64_t row_count() const { return row_count_; }	
	IndexType at(size_t row_num) { return index_[row_num]; }
private:
	uint64_t block_id_;
	uint64_t row_start_index_;
	uint64_t row_count_;	
	std::vector<IndexType> index_;			
};//class BlockIndex

}//namespace db	

#endif //DB_BLOCK_INDEX_H_
