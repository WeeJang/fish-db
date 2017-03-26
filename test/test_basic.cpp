#include "../src/utils/tiny_log.hpp"

#include <iostream>
#include <memory>
#include <vector>

void test_inter(int num, char ch) {
  if (num) {
    std::cout << ch << " : " << num << std::endl;
  }

  if (!num) {
    std::cout << "!" << ch << " : " << num << std::endl;
  }
}

class Demo {
 public:
  Demo() { std::cout << "Demo init" << std::endl; }
  ~Demo() { std::cout << "Demo des" << std::endl; }

  void print() { std::cout << b_ << std::endl; }
  void set_value(int a) { a_ = a; }
  void set_b() { b_ = true; }

 public:
  int a_;
  bool b_;
};

class Test {
 public:
  void test(int a) {
    LOG("==========================");
    Demo demo;
    demo.print();
    // LOG("this : %p , demo is %p",this,&demo);
    // demo.set_b();
    // demo.print();
  }
};

int main() {
  /*
  int a = 0;
  int b = 1;
  int c = -1;

  test_inter(a,'a');
  test_inter(b,'b');
  test_inter(c,'c');
  std::vector<std::shared_ptr<Demo>> vec;
  {
          auto p_a = std::make_shared<Demo>();
          p_a->a = 100;
          vec.push_back(p_a);
  }

  std::cout << vec[0]->a <<std::endl;
  */
  for (size_t i = 1; i < 1000; i++) {
    auto p_a = std::make_shared<Test>();
    p_a->test(i);
  }

  return 0;
}
