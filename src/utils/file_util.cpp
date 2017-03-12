#include "file_util.h"

namespace utils{

int mkdir(const std::string& path){
	return ::mkdir(path.c_str(),S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);
}

 
std::vector<std::string> get_file_list(const std::string& dictionary_path){
	DIR* p_dir = ::opendir(dictionary_path.c_str());
	std::vector<std::string> file_list;
	if(p_dir == nullptr){
		fprintf(stderr,"open dir %s failed !",dictionary_path.c_str());
		return file_list;	
	}
	struct dirent* p_dirent;
	struct stat st;
	char dot_c = '.';
	while(( p_dirent = ::readdir(p_dir) ) != nullptr){
		if(strncmp(p_dirent->d_name,&dot_c,1) == 0){
			continue;	
		}
		::stat(p_dirent->d_name,&st);
		if(S_ISDIR(st.st_mode)){
			continue;
		}
		file_list.push_back(p_dirent->d_name);
	}		
	::closedir(p_dir);	
	return file_list;	
}

std::vector<std::string> get_file_list_with_prefix(const std::string& dictionary_path,\
							const std::string& prefix){
	std::vector<std::string> all_files = get_file_list(dictionary_path);
	std::vector<std::string> ret_files(all_files.size());
	
	auto iter_end = std::copy_if(all_files.begin(),all_files.end(),ret_files.begin(),\
		[&prefix](std::string elem) { return (0 ==::strncmp(elem.c_str(),prefix.c_str(),prefix.size())); });
	
	ret_files.resize(std::distance(ret_files.begin(),iter_end));
	return ret_files;
}

std::vector<std::string> get_dict_list(const std::string& dictionary_path){
	DIR* p_dir = ::opendir(dictionary_path.c_str());
	std::vector<std::string> file_list;
	if(p_dir == nullptr){
		fprintf(stderr,"open dir %s failed !",dictionary_path.c_str());
		return file_list;	
	}
	struct dirent* p_dirent;
	struct stat st;
	char dot_c = '.';
	while(( p_dirent = ::readdir(p_dir) ) != nullptr){
		if(strncmp(p_dirent->d_name,&dot_c,1) == 0){
			continue;	
		}
		::stat(p_dirent->d_name,&st);
		if(!S_ISDIR(st.st_mode)){
			continue;
		}
		file_list.push_back(p_dirent->d_name);
	}		
	::closedir(p_dir);	
	return file_list;	
}

std::vector<std::string> get_dict_list_with_prefix(const std::string& dictionary_path,\
							const std::string& prefix){
	std::vector<std::string> all_files = get_dict_list(dictionary_path);
	std::vector<std::string> ret_files(all_files.size());
	
	auto iter_end = std::copy_if(all_files.begin(),all_files.end(),ret_files.begin(),\
		[&prefix](std::string elem) { return (0 ==::strncmp(elem.c_str(),prefix.c_str(),prefix.size())); });
	
	ret_files.resize(std::distance(ret_files.begin(),iter_end));
	return ret_files;
}


bool write_to_file(const std::string& dump_file_path,char* serialized_bytes,size_t expected_size){
	int fd = -1;
	if((fd = ::open(dump_file_path.c_str(),O_CREAT | O_RDWR ,0666) == -1)){
		fprintf(stderr,"open file : %s failed !",dump_file_path.c_str());
		return false;
	}
	if( -1 == ::write(fd,serialized_bytes,expected_size)){
		fprintf(stderr,"write  to file : %s failed !",dump_file_path.c_str());
		return false;
	}
	return false;		
}


}//namespace utils
