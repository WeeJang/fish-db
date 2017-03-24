#ifndef NETWORK_MESSAGE_H_
#define NETWORK_MESSAGE_H_

#include "../utils/tiny_log.hpp"

#include <string>

#include <cstring>

namespace test { class MessageTest; }

namespace network{

/**
*    protocal:
	|-------------------------all-data-----------------------|
*	|------header--------------|---......----data---....-----|
*	|---->sizeof(size_t)<------|--->header.decode()-bytes<---|
*/

class Message{
	
	friend test::MessageTest;
public:
	constexpr static size_t HEADER_LENGTH = sizeof(size_t); //sizeof(size_t) bytes;

public:
	Message(const std::string& data_info){
		data_length_ = data_info.size();
		LOG("encode ...");
		p_data_ = new char[data_length_ + HEADER_LENGTH];
		memcpy(p_data_ + HEADER_LENGTH,data_info.c_str(),data_length_);		
		encode_header();
	}

	Message(const char* p_message,size_t message_size){
		p_data_ = new char[message_size];
		memcpy(p_data_,p_message,message_size);		
		decode_header();
	}

	virtual ~Message(){
		delete p_data_;	
	}
	
	const size_t data_length() const {
		return data_length_;
	}

	const char* data_body() const {
		return p_data_ + HEADER_LENGTH;
	}

	const char* message_c_str() const {
		return p_data_;
	}
	
	const size_t message_length() const {
		return data_length_ + HEADER_LENGTH;
	}

private:
	void decode_header(){
		size_t* p_header = reinterpret_cast<size_t*>(p_data_);
		data_length_ = *p_header;	 
		LOG("data_length : %d",data_length_);				
	}
	
	void encode_header(){
		char* p_data_length = reinterpret_cast<char*>(&data_length_);
		memcpy(p_data_,p_data_length,HEADER_LENGTH);
	}
private:
	char* p_data_;
	size_t data_length_;

};//class Message


}//namesapce network

#endif //network_message
