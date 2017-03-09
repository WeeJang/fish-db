#include<iostream>
#include<sdsl/bit_vectors.hpp>
#include<memory>
#include<vector>
#include<functional>

class Demo{

constexpr static int c = 4;
constexpr static int c_a[5] = {1,1,1,1,1};

public:
	//Demo(){ int a = 4;};	

private:
//public:
	int demo[3];	


public:
using data_type = std::remove_extent<decltype(demo)>::type;
};

uint64_t get_block_index_by_global_offset(const std::vector<uint64_t> block_offset_list_,uint64_t global_offset){
	size_t list_len = block_offset_list_.size();
	size_t lower_pos = 0;
	size_t upper_pos = list_len -1;
	do{
		if(upper_pos - lower_pos <= 3){
			break;						
		}
		size_t mid_pos = (lower_pos + upper_pos) / 2;
		if(global_offset <= block_offset_list_[mid_pos]){
			upper_pos = mid_pos;
		}else{
			lower_pos = mid_pos;	
		}

	}while(true);
	for(; lower_pos <= upper_pos ; lower_pos ++){
		if(block_offset_list_[lower_pos] > global_offset){
			break;
		}	
	}
	return --lower_pos;
}



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
	
	{
		int* p = new int(0);
		int m = 4;
		std::unique_ptr<int,std::function<void(int*)>> a(p,[m](int* D){ delete D ; std::cout << m << " hello world " << std::endl; } );	
		std::cout << *a << std::endl;
	}

	{
		std::cout << "===========" << std::endl;
		std::cout << sizeof(Demo)  << std::endl;
		std::cout << sizeof(Demo::data_type) << std::endl;
		std::cout << "is trival" << std::is_trivial<Demo>::value << std::endl;
	}
	std::cout << "fuck" << std::endl;

	**/
	std::vector<uint64_t> vec = {0,5,8,15,39};
	std::cout << 0 << get_block_index_by_global_offset(vec,0) << std::endl; 
	std::cout << 0 << get_block_index_by_global_offset(vec,1) << std::endl; 
	std::cout << 1 << get_block_index_by_global_offset(vec,5) << std::endl; 
	std::cout << 1 << get_block_index_by_global_offset(vec,6) << std::endl; 
	std::cout << 2 << get_block_index_by_global_offset(vec,8) << std::endl; 
	std::cout << 2 << get_block_index_by_global_offset(vec,10) << std::endl; 
	std::cout << 3 << get_block_index_by_global_offset(vec,15) << std::endl; 
	std::cout << 3 << get_block_index_by_global_offset(vec,24) << std::endl; 
	std::cout << 4 << get_block_index_by_global_offset(vec,39) << std::endl; 
	std::cout << 4 <<get_block_index_by_global_offset(vec,46) << std::endl; 
	
	return 0;
}

