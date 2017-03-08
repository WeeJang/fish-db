#include "../src/db/block.h"

#include<iostream>

int main(int argc,char** argv){
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
	p_block_2->output_offset();	
	
	return 0;
}


