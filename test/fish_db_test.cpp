#include "../src/core/fish_db_impl.h"

namespace test {

class FishDBTest {
 public:
  void test_create_db() {
    fishdb::FishDBImpl db;
    db.create_db("test_db");
    db.load_data("/Users/jiangwei/Workspace/fish-db/test/data/test.spo");
    db.close_db();
  }

  void test_open_db() {
    fishdb::FishDBImpl db;
    db.open_db("test_db");
    auto triple_spec = std::make_shared<core::TripleSpec>();
    db.get_triple_by_row_index(5, triple_spec);
    std::cout << triple_spec->to_string() << std::endl;
    db.get_triple_by_row_index(296, triple_spec);
    std::cout << triple_spec->to_string() << std::endl;
    db.close_db();
  }

};  // FishDBTest

}  // namespace test

int main(int argc, char** argv) {
  test::FishDBTest fishdb_test;
  fishdb_test.test_create_db();
  // fishdb_test.test_open_db();
  return 0;
}
