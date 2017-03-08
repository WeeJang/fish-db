#include "../src/db/block_index.h"
#include "../src/utils/tiny_log.hpp"

#include <iostream>
#include <string>

namespace db{
	class BlockIndex;
}  


namespace test{

class BlockIndexTest{

public:
	BlockIndexTest(const std::string& blockfile_name):
		blockfile_name_(blockfile_name){};
	
	void test(){
		db::BlockIndex bindex(blockfile_name_);	
		std::cout << bindex.block_id() << std::endl;
		std::cout << bindex.row_start_index() << std::endl;
		std::cout << bindex.row_count() << std::endl;	
		std::cout << "[" ;
		for(auto elem : bindex.index_ ){
			std::cout << elem << " " ;
		}
		std::cout << " ]" << std::endl;
	}	
	
private:
	std::string blockfile_name_;	
};

}//test


int main(int argc,char** argv){
	std::string dump_str("/Users/jiangwei/Workspace/cayley-backend/data/block_test.dump");
	test::BlockIndexTest index_test(dump_str);
	index_test.test();
	return 0;
}


