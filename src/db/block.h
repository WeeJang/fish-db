#ifndef DB_BLOCK_H_
#define DB_BLOCK_H_

#include <iostream>
#include "../utils/tiny_log.hpp"

#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace test {
class BlockTest; /*forward-declaration*/
}  // namespace test

namespace db {

class BlockIndex;  // forward-declaration

struct alignas(8) BlockHeader {
  friend class Block;
  friend class BlockIndex;

 private:
  uint64_t block_id_;         // block_id in group
  uint64_t row_start_index_;  // row start index of this block in global
  uint64_t row_count_;        // row count in this block
};                            // BlockHeader

struct alignas(8) BlockData {
  friend class Block;

 public:
  // constexpr static uint32_t BLOCK_DATA_SIZE_LIMIT = 1 << 27; //128MB
  constexpr static uint32_t BLOCK_DATA_SIZE_LIMIT = 1 << 12;  // 4k
  void init() { memset(data_, 0, sizeof(data_)); }
  void write(const char* p_data, size_t data_size);

 private:
  char data_[BLOCK_DATA_SIZE_LIMIT];
};  // BlockData

struct alignas(8) Block {
  friend class test::BlockTest;
  friend class BlockIndex;
  friend class BlockDataSeeker;
  constexpr static char BLOCK_MAGIC_NUM[8] = "JWBLOCK";

 public:
  void dump(const std::string& filename);
  const uint64_t block_id() const { return block_header_.block_id_; }
  const uint64_t row_start_index() const {
    return block_header_.row_start_index_;
  }
  const uint64_t row_count() const { return block_header_.row_count_; }
  const size_t get_flexible_size_of_block() const {
    return sizeof(Block) +
           (row_count() - 1) * sizeof(Block::row_data_offset_[0]);
  }

  void block_id(uint64_t block_id) { block_header_.block_id_ = block_id; }
  void row_start_index(uint64_t index) {
    block_header_.row_start_index_ = index;
  }
  void row_count(uint64_t row_count) { block_header_.row_count_ = row_count; }

  void write(const char* p_data, size_t size) {
    block_data_.write(p_data, size);
  }
  // TODO:ugly
  void insert_row_data_offset(uint64_t index, uint32_t offset_value) {
    row_data_offset_[index] = offset_value;
  }

 private:
  char magic_num_[8];
  char check_sum_[8];
  BlockHeader block_header_;
  BlockData block_data_;
  uint64_t row_data_offset_[1];

 public:
  using row_data_offset_type =
      std::remove_extent<decltype(row_data_offset_)>::type;
};

// helper function
std::unique_ptr<db::Block, std::function<void(db::Block*)>>
create_block_by_raw_data(const char* p_data, size_t data_size);
std::unique_ptr<db::Block, std::function<void(db::Block*)>>
load_from_disk_by_mmap(const std::string filename);

}  // namespace db

#endif  // DB_BLOCK_H_
