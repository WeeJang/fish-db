//#include "../core/fish_db_impl.h"
#include "../utils/tiny_log.hpp"

#include "asio_generic_server.hpp"
#include "message.hpp"
#include "query_handler.hpp"

#include <memory>
#include <iostream>

int main(int argc,char** argv){
	try{
		auto p_fishdb = std::make_shared<fishdb::FishDBImpl>();
		p_fishdb->open_db("test_db");
		network::AsioGenericServer<network::QueryHandler> query_server(p_fishdb,1);
		query_server.start_server(6666);
		LOG("fuck");
		p_fishdb->close_db();
	}catch(...){
		std::cout << "exception " << std::endl;
	}
	return 0;
}

