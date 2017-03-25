#ifndef ASIO_GENERIC_SERVER_H_
#define ASIO_GENERIC_SERVER_H_

#include "../utils/tiny_log.hpp"

#include <vector>
#include <thread> 
#include <memory>

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <cstdio>

namespace network{

template<typename ConnectionHandler>
class AsioGenericServer{
public:
	AsioGenericServer(size_t thread_count = 1)
		:thread_count_(thread_count),acceptor_(io_service_),work_(io_service_){
	}
	
	~AsioGenericServer(){
		io_service_.stop();
		LOG("dtor");
	}

	AsioGenericServer(const AsioGenericServer&) = delete;
	AsioGenericServer(AsioGenericServer&&) = delete;
	AsioGenericServer& operator=(const AsioGenericServer&) = delete;

	void start_server(uint16_t port){
		//set up the acceptor to listen on the tcp port
		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(),port);
		acceptor_.open(endpoint.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.bind(endpoint);
		acceptor_.listen();
		
		auto handler = std::make_shared<ConnectionHandler>(io_service_);
		LOG("create handler : %p",handler.get());	

		acceptor_.async_accept(handler->socket(),\
					[=](const boost::system::error_code& error){
						handler_new_connection(handler,error);
					}
		);

		for(size_t i = 0; i < thread_count_ ; i++){
			thread_pool_.emplace_back( [=](){io_service_.run();} );	
		}
		
		for(auto& thread : thread_pool_){
			thread.join();
		}
		LOG("thread all finish");	
	}

	
private:
	void handler_new_connection(std::shared_ptr<ConnectionHandler> handler,const boost::system::error_code& error){
		LOG("hello");
		if(error){
			return;
		}
		handler->start();
	
		auto new_handler = std::make_shared<ConnectionHandler>(io_service_);
		LOG("hello");
		LOG("create handler : %p",new_handler.get());	
		acceptor_.async_accept(new_handler->socket(),\
					[=](const boost::system::error_code& error){
						handler_new_connection(new_handler,error);					
					}
		);
		LOG("hello");

	}	

private:
	boost::asio::io_service io_service_;	
	boost::asio::io_service::work work_;	
	boost::asio::ip::tcp::acceptor acceptor_;
	size_t thread_count_;
	std::vector<std::thread> thread_pool_;

};//class FishKGServer

}//namesapce network
#endif //ASIO_GENERIC_SERVER_H_


