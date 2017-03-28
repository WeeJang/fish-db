set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")

set(src_dir "../src/")
set(boost_include_dir "/usr/local/include/")
set(boost_libraries_dir "/usr/local/lib/")

find_package(Boost COMPONENTS system REQUIRED)
if(NOT Boost_FOUND)
	message(FATAL_ERROR "not found boost lib")
endif()

include_directories(
	${src_dir}
	${boost_include_dir}
)

link_directories(
	${boost_libraries_dir}
)



file(GLOB_RECURSE src_cpp_files "${src_dir}/*/*.cpp")
file(GLOB_RECURSE src_c_files "${src_dir}/*/*.c")

file(GLOB_RECURSE src_core_cpp_files "${src_dir}/core/*.cpp")
file(GLOB_RECURSE src_db_cpp_files "${src_dir}/db/*.cpp")
file(GLOB_RECURSE src_utils_cpp_files "${src_dir}/utils/*.cpp")

message(STATUS " the current source dir " ${src_dir})
message(STATUS " the cpp files" ${src_cpp_files})
message(STATUS " the boost files" ${Boost_LIBRARIES})

#add_executable(block_data_seeker_test block_data_seeker_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(block_index_test block_index_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(block_test block_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(boost_asio_test1 boost_asio_test1.cpp ${src_cpp_files} ${src_c_files})
#add_executable(file_util_test file_util_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(fish_db_test fish_db_test.cpp ${src_core_cpp_files} ${src_db_cpp_files} ${src_utils_cpp_files} ${src_c_files})
#add_executable(iri_index_test iri_index_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(message_test message_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(query_test query_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(roaring_test roaring_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(root_table_test root_table_test.cpp ${src_cpp_files} ${src_c_files})
#add_executable(sparql_test sparql_test.cpp ${src_cpp_files} ${src_c_files})

#add_executable(query_client_test query_client.cpp)
#target_link_libraries(query_client_test ${Boost_LIBRARIES})


add_subdirectory(load_test load_test_bin)
