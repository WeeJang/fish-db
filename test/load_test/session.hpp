#include "../../src/network/message.hpp"
#include "../../src/utils/tiny_log.hpp"
#include "test_query_str.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace test {

class Session : public std::enable_shared_from_this<Session> {
 public:
  Session(boost::asio::io_service& io_service,boost::asio::io_service::strand& strand,
	  std::string& target_ip,
          uint16_t target_port)
      : io_service_(io_service),
	strand_(strand),
        is_socket_closed_(false),
        is_correct_(false),
        socket_(io_service),
        endpoint_(boost::asio::ip::address::from_string(target_ip),
                  target_port) {
    LOG("session ctor");
    // start();
  }
  ~Session() {
    if (!is_socket_closed_) {
      boost::system::error_code close_ec;
      socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, close_ec);
      if (close_ec) {
        std::cerr << "close ec is : " << close_ec << std::endl;
      }
      socket_.close();
    }
    LOG("session dtor");
  }

  void start() {
    auto self = shared_from_this();
    socket_.async_connect(endpoint_, [=](const boost::system::error_code& ec) {
      if (ec) {
        self->handle_error(ec);
        return;
      }
      self->query(test_query::sparql_str);
    });
  }
 
 const bool is_corrent() const { return is_correct_; }

 private:
  void query(std::string query_str) {
    auto self = shared_from_this();
    network::Message query_msg(query_str);
    boost::asio::async_write(
        socket_,
        boost::asio::buffer(query_msg.message_c_str(),
                            query_msg.message_length()),
        [=](const boost::system::error_code& ec, std::size_t bytes_transfered) {
          if (ec) {
            self->handle_error(ec);
            return;
          }
          self->read_header();
        });
  }

  void read_header() {
    auto self = shared_from_this();
    auto header = std::make_shared<size_t>();
    boost::asio::async_read(
        socket_, boost::asio::buffer(header.get(), sizeof(size_t)),
        [=](const boost::system::error_code& ec, std::size_t bytes_transfered) {
          if (ec) {
            self->handle_error(ec);
            return;
          }
          self->read_body(*header);
        });
  }

  void read_body(size_t body_size) {
    auto self = shared_from_this();
    auto body = std::make_shared<std::vector<char>>(body_size, 0);
    boost::asio::async_read(
        socket_, boost::asio::buffer(body->data(), body_size),
        [=](const boost::system::error_code& ec, std::size_t bytes_transfered) {
          if (ec) {
            self->handle_error(ec);
            return;
          }
	  self->is_correct_=true;
          io_service_.post(strand_.wrap([=](){ std::cout.write(body->data(), body_size) << "\n"; }));
        });
  }

  void handle_error(const boost::system::error_code& ec) {
    io_service_.post(strand_.wrap([=](){ std::cerr << "handle ec is : " <<  ec.message()  << std::endl; }));
    boost::system::error_code close_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, close_ec);
    if (close_ec) {
      io_service_.post(strand_.wrap([=](){ std::cerr << "close ec is : " << close_ec << std::endl; }));
    }
    socket_.close();
    is_socket_closed_ = true;
  }

 private:
  boost::asio::io_service& io_service_;
  boost::asio::io_service::strand& strand_;
  boost::asio::ip::tcp::socket socket_;
  boost::asio::ip::tcp::endpoint endpoint_;
  bool is_socket_closed_;
  bool is_correct_;

};  // class Session

}  // namespace test
