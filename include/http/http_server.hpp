#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "http_session.hpp"
#include "queue/queue.hpp"

class http_server {
public:
    http_server(boost::asio::io_context& ioc, short port, Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue);

private:
    boost::asio::ip::tcp::acceptor acceptor_;

    void do_accept();
    Queue<boost::beast::http::request<boost::beast::http::string_body>>& requestQueue_;
};

#endif // HTTP_SERVER_HPP