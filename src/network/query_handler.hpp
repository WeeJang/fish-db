#ifndef NETWORK_QUERY_HANDLER_H_
#define NETWORK_QUERY_HANDLER_H_
#include "message.hpp"

#include <queue>
#include <boost/asio.hpp>
#include <iostream>

#include <queue>
#include <boost/asio.hpp>
#include <iostream>

namespace network{

class QueryHandler:
	public std::enable_shared_from_this<QueryHandler>{

	using QueryHandlerFnType = std::function<std::string(std::string)>;
public:
	QueryHandler(boost::asio::io_service& service)
		:io_service_(service)
		,socket_(service)
		,write_strand_(service)
	{}
	
	~QueryHandler(){
		LOG("handler dconstor !!!! FUCK");
	}
	
	QueryHandler(const QueryHandler&) = delete;
	QueryHandler(QueryHandler&&) = delete;
	QueryHandler& operator=(const QueryHandler&) = delete;

	void start(){
		read_packet();
	}

	boost::asio::ip::tcp::socket& socket(){
		return socket_;
	}

private:
	void close_socket(){
		boost::system::error_code ec;
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both,ec);
		socket_.close(ec);
	}

	void handle_error(const boost::system::error_code& ec){
		close_socket();
		fprintf(stderr,"handle error : %s",ec.message().c_str());
	}

	void read_packet(){
		auto self = shared_from_this();	
		//parser header
		auto header = std::make_shared<size_t>();
		LOG("this : %p header adderess %p",this,header.get());
		boost::asio::async_read(socket_,boost::asio::buffer(header.get(),Message::HEADER_LENGTH),\
			[=](const boost::system::error_code& ec,size_t size){
				if(ec){
					fprintf(stderr,"read header of packet error!\n");
					self->handle_error(ec);
					return;
				}
				LOG("read header %zu",*header);
				size_t body_size = *header;
				self->read_body(body_size);
			});	
	}
	
	void read_body(size_t body_size){
		auto self = shared_from_this();
		LOG("this %p",this);
		char* query_str = new char[body_size];
		LOG("this %p | malloc query_str %p",this,query_str);
		boost::asio::async_read(socket_,boost::asio::buffer(query_str,body_size),\
			[=](const boost::system::error_code& ec,size_t size){
				if(ec){
					fprintf(stderr,"read body of packet error!\n");
					self->handle_error(ec);
					return;
				}
				//handle query str
				std::cout << "get query str : " << body_size << std::endl;
				std::cout.write(query_str,body_size) << "\n";
				delete [] query_str;
				//CallBack
				//callback_(query_str);			
				std::string ret_str("reture str");
				auto query_ret = std::make_shared<network::Message>(ret_str);
				self->write_packet(query_ret);	
				//go on
				self->read_packet();
		});
	}


	void write_packet(std::shared_ptr<network::Message> write_packet_str){
		auto self = shared_from_this();
		io_service_.post( write_strand_.wrap(\
			[=](){
				self->queue_message(write_packet_str);
			}));		
	} 


	void queue_message(std::shared_ptr<network::Message> write_packet_str){
		bool write_in_progress = !write_packet_queue_.empty();
		write_packet_queue_.push(write_packet_str);
		
		if(!write_in_progress){
			do_write_packet();
		}
	}

	void do_write_packet(){
		auto self = shared_from_this();
		auto p_msg = write_packet_queue_.front();
		
		boost::asio::async_write(socket_
			,boost::asio::buffer(p_msg->message_c_str(),p_msg->message_length())
			,[=](const boost::system::error_code& ec,size_t size){
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
	std::queue<std::shared_ptr<network::Message>> write_packet_queue_;
	QueryHandlerFnType query_handler_fn_;
		
};//class query_handler


}//namespace network
#endif // NETWORK_QUERY_HANDLER_H_

