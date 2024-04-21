#include "handler/parser.hpp"

Parser::Parser(Queue<std::string>& queue, int numThreads)
    : m_queue(queue), m_numThreads(numThreads), m_running(false) {
        start();
    }

Parser::~Parser() {
    stop();
}

void Parser::start() {
    m_running = true;
    for (int i = 0; i < m_numThreads; ++i) {
        m_threads.emplace_back(&Parser::consume, this);
    }
}

void Parser::stop() {
    m_running = false; // Signal all threads to stop
    for (auto &thread : m_threads) {
        if (thread.joinable()) {
            thread.join(); // Wait for all threads to finish
        }
    }
}

void Parser::consume() {
    while (m_running || !m_queue.empty()) {
        auto data = m_queue.dequeue();
        if (data) {
            parse(data.value()); // Handle the data
        } else {
            std::this_thread::yield(); // Queue is empty, yield to reduce CPU usage
        }
    }
}

void Parser::parse(const std::string& data) {
    // Add parsing logic here
    std::cout << "Processing data: " << data << std::endl;
}
