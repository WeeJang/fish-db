#include "../src/db/block_data_seeker.h"
#include "../src/core/triple.h"

namespace test{

class BlockDataSeekerTest{

public:
	void test(){
		std::string dump_file("/Users/jiangwei/Workspace/cayley-backend/data/block_test.dump");
		db::BlockDataSeeker seeker(dump_file);
		auto triple = std::make_shared<core::TripleSpec>();
		seeker.get_triple_by_index(9,triple);
		std::cout << triple->to_string() << std::endl;
	}

};



}//namespace test



int main(int argc,char** argv){
	test::BlockDataSeekerTest seeker_test;
	seeker_test.test();
}
