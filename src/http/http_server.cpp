#include "http/http_server.hpp"
#include "http/http_session.hpp"

http_server::http_server(boost::asio::io_context& ioc, short port, Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue)
    : acceptor_(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)), requestQueue_(queue) {
    do_accept();
}

void http_server::do_accept() {
    acceptor_.async_accept(
        [this](boost::beast::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec) {
                std::make_shared<http_session>(std::move(socket), requestQueue_)->start();
            }
            do_accept();
        });
}