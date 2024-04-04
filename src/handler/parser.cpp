#include "handler/parser.hpp"
#include "queue/queue.hpp"
#include <iostream>

Parser::Parser(Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue) : m_queue(queue), m_running(false) {}

void Parser::start() {
    m_running = true;
    m_thread = std::thread(&Parser::consume, this);
    std::cout << "Parser thread started.\n";
}

void Parser::stop() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
    std::cout << "Parser thread stopped.\n";
}

void Parser::consume() {
    while (m_running || !m_queue.empty()) {
        auto request = m_queue.dequeue();
        if (request) {
            std::cout << "Processing request.\n";
            parse_and_print(*request);
            std::cout << "Request processed.\n";
        }
    }
}

void Parser::parse_and_print(const boost::beast::http::request<boost::beast::http::string_body>& request) {
    std::cout << "[parser] Parsed Request Body: " << request.body() << std::endl;
}
