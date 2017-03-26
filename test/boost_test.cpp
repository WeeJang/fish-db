#include <boost/variant.hpp>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
  boost::variant<int, std::string> variant_var = "hello";
  std::string& str = boost::get<std::string>(variant_var);
  str += " world";
  std::cout << variant_var << std::endl;

  return 0;
}
