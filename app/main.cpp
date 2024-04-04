#include <iostream>
#include <boost/asio.hpp>
#include "http/http_server.hpp"

int main() {
    try {
        boost::asio::io_context ioc{1};
        short port = 8080; // Substitua por argv[1] se necessário, com validação adequada
        http_server server(ioc, port);
        ioc.run();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}