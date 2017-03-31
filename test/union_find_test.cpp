#include "../src/utils/union_find.hpp"

#include <iostream>
#include <vector>

int main(int argc,char** argv){
   std::vector<int> a = {1,2,3,4,5};
   utils::UnionFind<int> uf(a);
   uf.union_set(1,2);
   uf.union_set(1,4);
   std::cout << uf.connected(2,4) << std::endl;
   std::cout << uf.connected(1,5) << std::endl;
   return 0 ;
}
