#include "block.h"

namespace db {

void BlockData::write(const char* p_data, size_t data_size) {
  assert(data_size <= BLOCK_DATA_SIZE_LIMIT);
  init();
  memcpy(data_, p_data, data_size);
}

// NOTE: this
constexpr char Block::BLOCK_MAGIC_NUM[8];

void Block::dump(const std::string& filename) {
  strcpy(magic_num_, BLOCK_MAGIC_NUM);
  int fd = open(filename.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
  if (fd == -1) {
    printf("open file %s failed !\n", filename.c_str());
    exit(-1);
  }
  if (::write(fd, this, get_flexible_size_of_block()) == -1) {
    printf("write file %s failed !\n", filename.c_str());
    exit(-1);
  }
  close(fd);
}

//----------------------helper function---------------------//
std::unique_ptr<Block, std::function<void(Block*)>> create_block_by_raw_data(
    const char* p_data, size_t data_size) {
  assert(data_size <= BlockData::BLOCK_DATA_SIZE_LIMIT);
  std::vector<uint32_t> offset_list;

  offset_list.push_back(0);
  for (size_t cur_data_pos = 0; cur_data_pos < data_size;) {
    if (*(p_data + cur_data_pos) == '\n') {
      offset_list.push_back(++cur_data_pos);
    } else {
      ++cur_data_pos;
    }
  }
  if (*(p_data + data_size - 1) == '\n') {
    offset_list.pop_back();
  }

  size_t flexible_block_size =
      sizeof(Block) +
      (offset_list.size() - 1) * sizeof(Block::row_data_offset_type);
  char* buf = (char*)malloc(flexible_block_size);
  Block* p_block = new (buf) Block;
  auto deleter = [buf](Block* p_block) {
    p_block->~Block();
    free(buf);
  };
  p_block->row_count(offset_list.size());
  for (int i = 0; i < offset_list.size(); i++) {
    p_block->insert_row_data_offset(i, offset_list[i]);
  }
  p_block->write(p_data, data_size);
  // TODO : check_sum_
  return std::unique_ptr<Block, std::function<void(Block*)>>(p_block, deleter);
}

std::unique_ptr<Block, std::function<void(Block*)>> load_from_disk_by_mmap(
    const std::string filename) {
  int fd = ::open(filename.c_str(), O_RDONLY, 0666);
  if (fd == -1) {
    printf("open file %s failed !\n", filename.c_str());
    exit(-1);
  }
  struct stat file_st;
  if (fstat(fd, &file_st) == -1) {
    printf("get file state of %s failed!\n", filename.c_str());
    exit(-1);
  }
  auto len = file_st.st_size;
  Block* p_block = (Block*)mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
  if (p_block == nullptr || p_block == (void*)-1) {
    printf("mmap file  %s failed !\n", filename.c_str());
    exit(-1);
  }

  auto deleter = [fd, len](Block* p_block) {
    munmap(p_block, len);
    close(fd);
  };
  return std::unique_ptr<Block, std::function<void(Block*)>>(p_block, deleter);
}

}  // namespace db
