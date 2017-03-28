#include "session.hpp"

#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include <boost/date_time/posix_time/posix_time.hpp>



int main(int argc, char** argv) {
  //try {
    int test_count = atoi(argv[1]);
    
    boost::asio::io_service io_service;
    boost::asio::io_service::work work(io_service);
    boost::asio::deadline_timer timer(io_service,boost::posix_time::seconds(20));

    std::string target_ip = "127.0.0.1";
    uint16_t target_port = 6666;
    size_t thread_pool_size = 8;
    std::vector<std::shared_ptr<test::Session>> test_vec;
    
    for(size_t i = 0 ; i < test_count ; i++){
	test_vec.push_back(std::make_shared<test::Session>(io_service,target_ip,target_port));
	test_vec.back()->start();
    }
    
    timer.async_wait([&](const boost::system::error_code& ec){
	 if(ec){
	    std::cout << "print correct count error :" << ec << std::endl;
	    return;
	  }
	  size_t count(0) ;
	  for(auto& sess : test_vec){
	    if(sess->is_corrent()){ count ++; }
	  }  
	  std::cout << "COUNT : " << count << std::endl;
  });


    std::vector<std::thread> thread_pool;
    for(size_t i = 0 ; i < thread_pool_size ; i++){
        thread_pool.emplace_back([&]() { io_service.run(); });
    }


    for(auto& thread: thread_pool){
      thread.join();
    } 	 

  //} catch (std::exception& e) {
  //  std::cerr << e.what() << std::endl;
  //}

  return 0;
}
