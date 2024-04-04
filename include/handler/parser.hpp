#ifndef PARSER_HPP
#define PARSER_HPP

#include "queue/queue.hpp"
#include <boost/beast/http.hpp>
#include <thread>
#include <atomic>
#include <boost/log/trivial.hpp>


class Parser {
public:
    explicit Parser(Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue);
    void start();
    void stop();
    static void parse_and_print(const boost::beast::http::request<boost::beast::http::string_body>& request);

private:
    Queue<boost::beast::http::request<boost::beast::http::string_body>>& m_queue;
    std::thread m_thread;
    std::atomic<bool> m_running;
    void consume();

};

#endif // PARSER_HPP
