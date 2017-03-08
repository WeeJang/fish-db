#include "../src/db/block.h"

#include<iostream>

int main(int argc,char** argv){
	//db::Block block;		
	const char* test_data = "s1\tp1\to1\ns2\tp2\to2\n";
	auto p_block =  db::create_block_by_raw_data(test_data,18 * sizeof(char));
	//std::unique_ptr<Blcok,std::function<void(Block*)>> load_from_disk_by_mmap(const std::string filename);
	std::string dump_str("/Users/jiangwei/Workspace/cayley-backend/data/block_test.dump");
	p_block->dump(dump_str);
	return 0;
}


