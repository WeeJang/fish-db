#include "message.hpp"

#include<array>
#include<iostream>

#include<boost/asio.hpp>
#include<boost/system/error_code.hpp>

std::string sparql_str_2 = "select ?id ?name ?parent ?child where { \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.isin> cne100001zg7 .  \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.short_name> ?name .  \
 		   ?id <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.parent> ?parent .  \
 		   ?parent <http://rdf.dingfu.com/ns/business.stock_ticker_symbol.child> ?child .  \
        }";


int main(int argc,char** argv){
	try{
		boost::asio::io_service io_service;
		boost::asio::ip::tcp::resolver r(io_service);
		boost::asio::ip::tcp::resolver::query q("localhost","6666");
		boost::system::error_code ec;
	
		boost::asio::ip::tcp::socket socket(io_service);
		boost::asio::connect(socket,r.resolve(q));
	
		network::Message query_msg(sparql_str_2);
		std::cout << "data length " << sparql_str_2.length() << std::endl;
		std::cout << "data length " << query_msg.data_length() << std::endl;
		boost::asio::write(socket,boost::asio::buffer(query_msg.message_c_str(),query_msg.message_length()),ec);
		if(ec){
			throw boost::system::system_error(ec);
		}	
		
		size_t header_buffer;
		boost::asio::read(socket,boost::asio::buffer(&header_buffer,sizeof(size_t)),ec);
		if(ec){
			throw boost::system::system_error(ec);
		}
	
		std::cout << "header buffer " << header_buffer << std::endl;	
	
		char* body_buffer = new char[header_buffer];
		boost::asio::read(socket,boost::asio::buffer(body_buffer,header_buffer),ec);
		if(ec){
			throw boost::system::system_error(ec);
		}
		std::cout << "query result" << std::endl;
		std::cout.write(body_buffer,header_buffer)  << "\n";
		delete [] body_buffer;
		std::cin >> header_buffer;
	}catch (std::exception& e){
		std::cerr << e.what() << std::endl;
	}

	

	return 0;
}
