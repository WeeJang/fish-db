#include "block_index.h"

#include <iostream>

namespace db {

BlockIndex::BlockIndex(const Block& block) {
  block_id_ = block.block_id();
  row_start_index_ = block.row_start_index();
  row_count_ = block.row_count();
  for (size_t i = 0; i < row_count_; i++) {
    index_.push_back(block.row_data_offset_[i]);
  }
}

BlockIndex::BlockIndex(Block* p_block) {
  block_id_ = p_block->block_id();
  row_start_index_ = p_block->row_start_index();
  row_count_ = p_block->row_count();
  for (size_t i = 0; i < row_count_; i++) {
    index_.push_back(p_block->row_data_offset_[i]);
  }
}

BlockIndex::BlockIndex(const std::string& filename) {
  LOG("block index open : %s", filename.c_str());
  int fd = open(filename.c_str(), O_RDONLY);
  if (-1 == fd) {
    printf("block_index open file %s failed !\n", filename.c_str());
    exit(-1);
  }
  size_t magic_num_size = offsetof(Block, check_sum_);
  char magic_num[magic_num_size];
  if (read(fd, magic_num, magic_num_size) == -1) {
    printf("read %s failed!\n", filename.c_str());
    exit(-1);
  }
  if (strcmp(magic_num, Block::BLOCK_MAGIC_NUM) != 0) {
    printf("file: %s is illegal, magic num check failed !\n", filename.c_str());
    exit(-1);
  }

  // TODO check check_sum
  if (lseek(fd, offsetof(Block, block_header_), SEEK_SET) == -1) {
    printf("file:lseek failed \n");
    exit(-1);
  }

  size_t bheader_size = sizeof(BlockHeader);
  BlockHeader* p_blockheader = (BlockHeader*)malloc(bheader_size);
  if (read(fd, p_blockheader, bheader_size) == -1) {
    printf("read blockheader from %s failed !", filename.c_str());
    exit(-1);
  }

  block_id_ = p_blockheader->block_id_;
  row_start_index_ = p_blockheader->row_start_index_;
  row_count_ = p_blockheader->row_count_;
  free(p_blockheader);

  if (lseek(fd, offsetof(Block, row_data_offset_), SEEK_SET) == -1) {
    printf("file:lseek failed \n");
    exit(-1);
  }

  size_t bindex_size = sizeof(IndexType) * row_count_;
  IndexType* p_index = (IndexType*)malloc(bindex_size);
  if (read(fd, p_index, bindex_size) == -1) {
    printf("file : %s read index failed!\n", filename.c_str());
    exit(-1);
  }
  char check_eof;
  if (read(fd, &check_eof, 1) != 0) {
    printf("file :%s check_eof failed!\n", filename.c_str());
    exit(-1);
  }
  for (size_t i = 0; i < row_count_; i++) {
    index_.push_back(*(p_index + i));
  }
  free(p_index);
  close(fd);
  LOG("block index open successfully");
  LOG("block_id : %d, row_start_index_ : %d,row_count_ : %d", block_id_,
      row_start_index_, row_count_);
}

}  // namespace db
