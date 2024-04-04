#include <iostream>
#include <boost/asio.hpp>
#include "http/http_server.hpp"
#include "handler/parser.hpp"
#include "queue/queue.hpp"

Queue<boost::beast::http::request<boost::beast::http::string_body>> requestQueue;
Parser parser(requestQueue);

int main() {
    try {
        parser.start();
        boost::asio::io_context ioc{1};
        short port = 8080; // Substitua por argv[1] se necessário, com validação adequada
        http_server server(ioc, port, requestQueue);
        ioc.run();
        parser.stop();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}