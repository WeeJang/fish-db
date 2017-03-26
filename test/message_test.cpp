#include "../src/network/message.hpp"

#include <iostream>

namespace test {

class MessageTest {
 public:
  void test() {
    std::string info("hello word!");
    network::Message msg(info);
    network::Message new_msg(msg.message_c_str(), msg.message_length());
    std::cout << std::string(new_msg.data_body(), new_msg.data_length())
              << std::endl;
  }
};

}  // namespace test

int main(int argc, char** argv) {
  test::MessageTest msg_test;
  msg_test.test();
  return 0;
}
