#ifndef NETWORK_QUERY_HANDLER_H_
#define NETWORK_QUERY_HANDLER_H_
#include "message.hpp"

#include <boost/asio.hpp>

namespace network{

class QueryHandler::
	public std::enable_shared_from_this<QueryHandler>{

	using QueryHandlerFnType = std::function<std::string(std::string)>;
public:
	QueryHandler(boost::asio::io_service& service)
		:io_service_(service)
		,socket_(service)
		,write_strand_(service)
	{}

	void start(){
		read_packet();
	}

	boost::asio::ip::tcp::socket& socket(){
		return socket_;
	}

private:
	void close_socket(){
		boost::system::error_code ec;
		socket_.shutdown(boost::tcp::socket::shutdown_both,ec);
		m_socket.close(ec);
	}

	void handle_error(const boost::system::error_code& ec){
		close_socket();
		fprintf(stderr,"handle error : %s",ec.message().c_str());
	}

	void read_packet(){
		auto self = shared_from_this();	
		//parser header
		char header[Message::HEADER_LENGTH];
		boost::asio::async_read(socket_,boost::asio::buffer(&header,HEADER_LENGTH),\
			[=](const boost::system::error_code& ec,size_t size){
				if(error_code){
					fprintf(stderr,"read header of packet error!\n");
					handle_error(ec);
					return;
				}
				size_t body_size = *(reinterpret_cast<size_t*>(header));
				read_body(body_size);					
			});	
	}
	
	void read_body(size_t body_size){
		auto self = shared_from_this();
		char query_str[body_size];
		boost::asio::async_read(socket_,boost::asio::buffer(&query_str,body_size),\
			[=](const boost::system::error_code& ec,size_t size){
				if(error_code){
					fprintf(stderr,"read body of packet error!\n");
					handle_error(ec);
					return;
				}
				//handle query str
				std::cout << "get query str : " << std::endl;
				std::cout.write(&query_str,body_size);
				//CallBack
				//callback_(query_str);					
				//go on
				read_packet();
		});
	}


	void write_packet(const std::string& write_packet_str){
		auto self = shared_from_this();
		io_service_.post( write_strand_.wrap(\
			[self](){
				self->queue_message(write_packet_str);
			}));		
	} 


	void queue_message(const std::string& write_packet_str){
		bool write_in_progress = !write_packet_queue_.empty();
		write_packet_queue_.push(write_packet_str);
		
		if(!write_in_progress){
			do_write_packet();
		}
	}

	void do_write_packet(){
		auto self = shared_from_this();
		boost::asio::async_write(socket_
			,boost::asio::buffer(write_packet_queue_.front())
			,[self](const boost::system::error_code& ec,size_t size){
				self->write_packet_done();	
			});
	}
	
	void write_packet_done(){
		write_packet_queue_.pop();
		if(!write_packet_queue_.empty()){
			do_write_packet();	
		}
	}
		
private:
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::io_service::strand write_strand_;
	std::queue<std::string> write_packet_queue_;
	QueryHandlerFnType query_handler_fn_;
		
};//class query_handler


}//namespace network
#endif // NETWORK_QUERY_HANDLER_H_
