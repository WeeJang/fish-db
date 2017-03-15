#include "../src/utils/file_util.h"

#include <iostream>
#include <vector>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void test_file(const std::string& dictionary_path){
	DIR* p_dir = ::opendir(dictionary_path.c_str());
	if(p_dir == nullptr){
		fprintf(stderr,"open dir %s failed !",dictionary_path.c_str());
		exit(-1);	
	}
	struct dirent* p_dirent;
	struct stat st;
	char dot_c = '.';
	while(( p_dirent = ::readdir(p_dir) ) != nullptr){
		std::string cur_path(dictionary_path);
		cur_path.push_back('/');
		cur_path.append(p_dirent->d_name);
		::stat(cur_path.c_str(),&st);
		std::cout << cur_path.c_str() << "is _dir: " << S_ISDIR(st.st_mode) << std::endl;
	}		
	::closedir(p_dir);	
}

int main(int argc,char** argv){	
	std::string dir_path("/Users/jiangwei/Workspace/cayley-backend/data/test_db/");
	std::string prefix("HV");
	//std::vector<std::string> files = utils::get_file_list_with_prefix(dir_path,prefix);		
	//std::vector<std::string> files = utils::get_dict_list_with_prefix(dir_path,prefix);		
	std::vector<std::string> files = utils::get_dict_list(dir_path);		
	for(auto elem : files){
		std::cout << elem << " " << std::endl;
	}

	std::cout << "files ============:" << std::endl;

	files = utils::get_file_list(dir_path);		
	for(auto elem : files){
		std::cout << elem << " " << std::endl;
	}

	//std::string new_dir_path("/Users/jiangwei/Workspace/cayley-backend/test//hello");
	//std::cout << utils::mkdir(new_dir_path) << std::endl;
	
	//test_file(dir_path);	
	
	return 0;
}

 

