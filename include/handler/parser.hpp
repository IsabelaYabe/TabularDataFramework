#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "queue/queue.hpp"

class Parser {
public:
    Parser(Queue<std::string>& queue, int numThreads);
    ~Parser();

    void start();
    void stop();

private:
    void consume();
    void parse(const std::string& data);

    Queue<std::string>& m_queue;
    std::vector<std::thread> m_threads;
    std::thread m_thread;
    int m_numThreads;
    std::mutex m_mutex;
    std::condition_variable m_cond_var;
    std::atomic<bool> m_running;
};

#endif // PARSER_HPP
