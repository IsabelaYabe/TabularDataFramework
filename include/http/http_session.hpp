#ifndef HTTP_SESSION_HPP
#define HTTP_SESSION_HPP

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include "queue/queue.hpp"
#include <iostream>
class http_session : public std::enable_shared_from_this<http_session> {
public:
    http_session(boost::asio::ip::tcp::socket socket, Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue);

    void start();

private:
    boost::asio::ip::tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;

    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void do_write();
    Queue<boost::beast::http::request<boost::beast::http::string_body>>& requestQueue_;
};

#endif // HTTP_SESSION_HPP