#ifndef DB_BLOCK_DATA_SEERKER_
#define DB_BLOCK_DATA_SEERKER_

#include "../core/triple.h"
#include "../utils/tiny_log.hpp"
#include "block.h"

#include <memory>
#include <string>

#include <cassert>
#include <cstddef>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace db {

class BlockDataSeeker {
 public:
  BlockDataSeeker(const std::string& block_file_name);
  virtual ~BlockDataSeeker();

  int get_triple_by_index(uint64_t index_offset,
                          std::shared_ptr<core::TripleSpec> p_triple_spec);

  // non-copyable
  BlockDataSeeker(const BlockDataSeeker&) = delete;
  BlockDataSeeker& operator=(const BlockDataSeeker&) = delete;

 private:
  const static size_t start_offset_of_block_;
  const static size_t end_offset_of_block_;
  int fd_;
  bool is_file_open_;
};  // class BlockDataSeeker

}  // namespace db

#endif  // DB_BLOCK_DATA_SEERKER_
