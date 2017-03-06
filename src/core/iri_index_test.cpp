#include<typeinfo>
#include<iostream>

#include "iri_index.hpp"

int main(int argc,char** argv){
	core::IRI_Index<int> a(110);
	auto sub_index = a.sub_index();	
	std::cout << typeid(sub_index).name() << std::endl;		
	return 0;
}
