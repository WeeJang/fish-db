set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")

set(src_dir ".")
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

message(STATUS " the current source dir " ${src_dir})
message(STATUS " the cpp files" ${src_cpp_files})
message(STATUS " the boost files" ${Boost_LIBRARIES})

add_executable(query_server ${src_cpp_files} ${src_c_files})

target_link_libraries(query_server ${Boost_LIBRARIES})

