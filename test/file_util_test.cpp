#include "../src/utils/file_util.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc,char** argv){	
	std::string dir_path("/Users/jiangwei/Workspace/cayley-backend/test//");
	std::string prefix("root");
	std::vector<std::string> files = utils::get_file_list_with_prefix(dir_path,prefix);		
	for(auto elem : files){
		std::cout << elem << " " << std::endl;
	}
	return 0;
}
