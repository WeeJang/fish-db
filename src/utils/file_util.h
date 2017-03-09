#ifndef FILE_UTIL_H_
#define FILE_UTIL_H_

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include<cstring>

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>


namespace utils{


int mkdir(const char* path);
 
std::vector<std::string> get_file_list(const std::string& dictionry_path);
std::vector<std::string> get_file_list_with_prefix(const std::string& dictionry_path,const std::string& prefix_str);


}//namespace utils

#endif
