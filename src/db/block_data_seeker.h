#ifndef DB_BLOCK_DATA_SEERKER_
#define DB_BLOCK_DATA_SEERKER_

#include "block.h"
#include "../core/triple.h"
#include "../utils/tiny_log.hpp"


#include <string>
#include <memory>

#include <cstddef>
#include <cassert>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

namespace test{
	class BlockDataSeekerTest;	
}//namespace test


namespace core{
	class TripleSpec; //forward-declartion
}//namesapce core

namespace db{

class BlockDataSeeker{
public:
	BlockDataSeeker(const std::string& block_file_name);
	virtual ~BlockDataSeeker();

	int get_triple_by_index(uint64_t index_offset,std::shared_ptr<core::TripleSpec> p_triple_spec);

private:
	const static size_t start_offset_of_block_;
	const static size_t end_offset_of_block_; 
	int fd_;
	bool is_file_open_;	


};//class BlockDataSeeker

}//namesapce db


#endif // DB_BLOCK_DATA_SEERKER_
