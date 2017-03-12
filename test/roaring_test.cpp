#include "../src/utils/roaring.hh"
#include "../src/utils/roaring.c"

#include <iostream>

int main(int argc,char** argv){

	Roaring64Map r1;
	for (uint64_t i = 18000000000000000100ull; i < 18000000000000001000ull; i++) {
    		r1.add(i);
  	}
	auto size_1 = r1.getSizeInBytes();
	r1.shrinkToFit();
	std::cout << r1.runOptimize() << std::endl;
	auto size_2 = r1.getSizeInBytes();
	std::cout << r1.runOptimize() << std::endl;
	auto size_3 = r1.getSizeInBytes();
	std::cout << "before run optimize : " << size_1 << std::endl;
	std::cout << "after  run optimize : " << size_2 << std::endl;
	std::cout << "after  run optimize : " << size_3 << std::endl;
	return 0;
}
