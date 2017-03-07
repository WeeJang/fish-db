#include<iostream>
#include<sdsl/bit_vectors.hpp>
#include<memory>
#include<functional>

class Demo{
	Demo(){ std::cout << "cons" << std::endl; }	
	~Demo(){ std::cout << "dest" << std::endl; }
};



int main(int argc,char** argv){
	//auto b = sdsl::bit_vector(100000001,0);
	//for(size_t i = 0; i < b.size() ; i+=400321){
	//	b[i] = 0;
	//}
	/**
	sdsl::bit_vector b = {0,1,1,0,1,0,0,1,0,0,0,1,1,1};
	std::cout << b.size() << std::endl;
	std::cout << sdsl::size_in_bytes(b) << std::endl;
	sdsl::rrr_vector<63> rrr_b(b);
	std::cout << sdsl::size_in_bytes(rrr_b) << std::endl;
	sdsl::sd_vector<> sd_b(b);
	std::cout << sdsl::size_in_bytes(sd_b) << std::endl; 
	size_t ones = sdsl::sd_vector<>::rank_1_type(&sd_b)(sd_b.size());
	sdsl::sd_vector<>::select_1_type sdb_sel(&sd_b);
	std::cout << sdb_sel.size() << std::endl;
	std::cout << ones << std::endl;
	for(size_t i = 1; i <= ones ; i ++){
		std::cout << sdb_sel(i) << " ";
	}
	**/
	
	{
		int* p = new int(0);
		int m = 4;
		std::unique_ptr<int,std::function<void(int*)>> a(p,[m](int* D){ delete D ; std::cout << m << " hello world " << std::endl; } );	
		std::cout << *a << std::endl;
	}
	std::cout << "fuck" << std::endl;


	return 0;
}

