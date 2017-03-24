#include "../utils/tiny_log.hpp"
#include "asio_generic_server.hpp"
#include "message.hpp"
#include "query_handler.hpp"


int main(int argc,char** argv){
	try{
		network::AsioGenericServer<network::QueryHandler> query_server;
		query_server.start_server(6666);
		LOG("fuck");	
	}catch(...){
		std::cout << "exception " << std::endl;
	}
	return 0;
}

