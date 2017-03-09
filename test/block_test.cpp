#include "../src/db/block.h"

#include<iostream>

namespace test{
class BlockTest{

public:
	void test(){
		//db::Block block;		
		const char* test_data = "s1\tp1\to1\ns2\tp2\to2\n";
		auto p_block =  db::create_block_by_raw_data(test_data,18 * sizeof(char));
		std::string dump_str("/Users/jiangwei/Workspace/cayley-backend/data/block_test.dump");
		p_block->dump(dump_str);
		
		auto p_block_2 = db::load_from_disk_by_mmap(dump_str);
		std::cout << p_block_2->block_id() << std::endl;
		std::cout << p_block_2->row_start_index() << std::endl;
		std::cout << p_block_2->row_count() << std::endl;
		std::cout << p_block_2->get_flexible_size_of_block() << std::endl;

		std::cout << "offset : [ " ;
		for(size_t i = 0 ; i < p_block_2->row_count() ; i++){
			std::cout << p_block_2->row_data_offset_[i] << " |";
		}
		std::cout << " ] " << std::endl;
	}

};//class BlockTest
}//namespace test




int main(int argc,char** argv){
	test::BlockTest blocktest;
	blocktest.test();	
	return 0;
}


