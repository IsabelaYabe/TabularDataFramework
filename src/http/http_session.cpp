#include "http/http_session.hpp"
#include <iostream>

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http; // from <boost/beast/http.hpp>
namespace net = boost::asio; // from <boost/asio.hpp>
using tcp = net::ip::tcp; // from <boost/asio/ip/tcp.hpp>

http_session::http_session(tcp::socket socket) : socket_(std::move(socket)) {}

void http_session::start() {
    do_read();
}

void http_session::do_read() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
}

void http_session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (ec == http::error::end_of_stream) {
        // The client closed the connection, do nothing
        return;
    }

    if (ec) {
        std::cerr << "Error: " << ec.message() << std::endl;
        return;
    }

    // Print the received HTTP request on the screen
    std::cout << "Received HTTP request:\n" << request_ << std::endl;

    // Extract and print the request body data
    std::string body = request_.body();
    std::cout << "Request body: " << body << std::endl;

    do_write();
}

void http_session::do_write() {
    auto res = std::make_shared<http::response<http::string_body>>(
        http::status::ok, request_.version());
    res->set(http::field::server, "Boost Beast Simplified Server");
    res->set(http::field::content_type, "text/plain");
    res->keep_alive(request_.keep_alive());
    res->body() = "Request received successfully!";
    res->prepare_payload();

    http::async_write(socket_, *res,
        [self = shared_from_this(), res](beast::error_code ec, std::size_t) {
            if (ec == http::error::end_of_stream) {
                // The client closed the connection, do nothing
                return;
            }

            if (ec) {
                std::cerr << "Error: " << ec.message() << std::endl;
                return;
            }

            if (self->request_.keep_alive()) {
                self->do_read();
            }
        });
}