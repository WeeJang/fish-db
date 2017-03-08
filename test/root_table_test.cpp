#include "../src/db/root_table.h"


namespace test{

class RootTableTest{

public:
	void test(){
		std::string rtable_file("/Users/jiangwei/Workspace/cayley-backend/data/rootable.dump");
		db::RootTable rtable(rtable_file);
		for(size_t i = 0 ; i < 8 ; i++){
			rtable.block_offset_list_.push_back(i);
		}
		rtable.dump();	
	}
};//class RootTableTest

} //namespace test


int main(int argc,char** argv){
	test::RootTableTest rtabletest;
	rtabletest.test();	
}
