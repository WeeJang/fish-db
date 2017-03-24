#ifndef ASIO_GENERIC_SERVER_H_
#define ASIO_GENERIC_SERVER_H_

#include <vector>
#include <thread>
#include <memory>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <cstdio>

namespace network{

template<typeneme ConnectionHandler>
class AsioGenericServer{
public:
	AsioGenericServer(size_t thread_count = 1):thread_count_(thead_count),tcp_acceptor_(io_service_){
	}
	
	void start_server(uint16_t port){
		//set up the acceptor to listen on the tcp port
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),port);
		acceptor_.open(endpoint.protocal());
		acceptor_.set_option(tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();
		
		auto handler = std::make_shared<ConnectionHandler>(io_service_);

		acceptor_.async_accept(handler->socket(),\
					[=](auto error){
						handler_new_connection(handler,error);
					}
		);

		for(size_t i = 0; i < thread_count_ ; i++){
			thread_pool_.emplace_back( [=]{io_service_.run();} );	
		}
	}

private:
	handler_new_connection(std::shared_ptr<ConnectionHandler> handler,const boost::system::error_code& error){
		if(error){
			return;
		}
		handler->start();
		
		auto new_handler = std::make_shared<ConnectionHandler>(io_service_);
		acceptor_.async_accept(new_handler->socket(),\
					[=](const boost::system::error_code& error){
						handler_new_connection(new_handler,error);					
					}
		);

	}	

private:
	boost::asio::io_service io_service_;	
	boost::asio::ip::tcp::acceptor acceptor_;
	size_t thread_count_;
	std::vector<std::thread> thread_pool_;

};//class FishKGServer

}//namesapce network
#endif //ASIO_GENERIC_SERVER_H_


