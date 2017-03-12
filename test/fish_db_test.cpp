#include "../src/core/fish_db_impl.h"


namespace test{

class FishDBTest{

public:
	void test(){
		fishdb::FishDBImpl db;
		db.create_db("test_db");
		db.load_data("/Users/jiangwei/Workspace/cayley-backend/test/data/test.spo");
		db.close_db();
	}

};//FishDBTest


}//namespace


int main(int argc,char** argv){
	test::FishDBTest fishdb_test;
	fishdb_test.test();	
	return 0;	
}
