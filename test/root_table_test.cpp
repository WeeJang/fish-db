#include "../src/db/root_table.h"
#include <iostream>

namespace test {

class RootTableTest {
 public:
  void test() {
    std::string rtable_file(
        "/Users/jiangwei/Workspace/fish-db/data/rootable.dump");
    db::RootTable rtable(rtable_file);
    for (size_t i = 0; i < 8; i++) {
      rtable.block_offset_list_.push_back(i);
    }
    rtable.dump();

    db::RootTable rtable2;
    rtable2.fast_init_by_dump_file(rtable_file);
    for (auto elem : rtable2.block_offset_list_) {
      std::cout << elem << std::endl;
    }
  }
};  // class RootTableTest

}  // namespace test

int main(int argc, char** argv) {
  test::RootTableTest rtabletest;
  rtabletest.test();
}
