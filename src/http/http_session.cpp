#include "http/http_session.hpp"
#include "handler/parser.hpp"
#include "queue/queue.hpp"


namespace beast = boost::beast; // from <boost/beast.hpp>
namespace net = boost::asio; // from <boost/asio.hpp>
using tcp = net::ip::tcp; // from <boost/asio/ip/tcp.hpp>

http_session::http_session(boost::asio::ip::tcp::socket socket, Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue)
    : socket_(std::move(socket)), requestQueue_(queue) {}

void http_session::start() {
    do_read();
}

void http_session::do_read() {
    auto self = shared_from_this();
    boost::beast::http::async_read(socket_, buffer_, request_,
        [self](beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
}

void http_session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    if (!ec) {
        requestQueue_.enqueue(std::move(request_));
        do_write();
    } else if (ec != boost::beast::http::error::end_of_stream) {
        std::cerr << "Error: " << ec.message() << std::endl;
    }
}



void http_session::do_write() {
    auto res = std::make_shared<boost::beast::http::response<boost::beast::http::string_body>>(
        boost::beast::http::status::ok, request_.version());
    res->set(boost::beast::http::field::server, "Boost Beast Simplified Server");
    res->set(boost::beast::http::field::content_type, "text/plain");
    res->keep_alive(request_.keep_alive());
    res->body() = "Request received successfully!";
    res->prepare_payload();

    boost::beast::http::async_write(socket_, *res,
        [self = shared_from_this(), res](beast::error_code ec, std::size_t) {
            if (ec == boost::beast::http::error::end_of_stream) {
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