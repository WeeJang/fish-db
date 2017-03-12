#include<typeinfo>
#include<iostream>

#include "../src/core/iri_index.hpp"

int main(int argc,char** argv){
	core::IRIIndex<int> a(110);
	auto sub_index = a.sub_index();	
	a.add_index(0,0);
	std::cout << typeid(sub_index).name() << std::endl;		
	return 0;
}
