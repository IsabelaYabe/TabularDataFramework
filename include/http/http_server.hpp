#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include "http_session.hpp"

class http_server {
public:
    http_server(boost::asio::io_context& ioc, short port);

private:
    boost::asio::ip::tcp::acceptor acceptor_;

    void do_accept();
};

#endif // HTTP_SERVER_HPP