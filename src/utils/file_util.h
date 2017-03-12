#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include<cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>

namespace utils{


int mkdir(const std::string& path);
 
std::vector<std::string> get_file_list(const std::string& dictionry_path);
std::vector<std::string> get_file_list_with_prefix(const std::string& dictionry_path,const std::string& prefix_str);

std::vector<std::string> get_dict_list(const std::string& dictionry_path);
std::vector<std::string> get_dict_list_with_prefix(const std::string& dictionry_path,const std::string& prefix_str);

bool write_to_file(const std::string& dump_file_path,char* serialized_bytes,size_t expected_size);

}//namespace utils

#endif
