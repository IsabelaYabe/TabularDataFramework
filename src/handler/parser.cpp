#include "handler/parser.hpp"
#include "queue/queue.hpp"
#include <iostream>
/**
 * @brief Construtor do Parser.
 * 
 * Inicializa o parser associando-o a uma fila específica de requisições HTTP.
 * 
 * @param queue Referência a uma fila de requisições HTTP para ser processada pelo parser.
 */
Parser::Parser(Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue) : m_queue(queue), m_running(false) {}
/**
 * @brief Inicia o thread de processamento das requisições.
 *
 * Este método inicia um thread separado que continuamente consome e processa requisições
 * da fila enquanto `m_running` for verdadeiro ou até que a fila esteja vazia.
 */
void Parser::start() {
    m_running = true;
    m_thread = std::thread(&Parser::consume, this);
    std::cout << "Parser thread started.\n";
}

/**
 * @brief Para o thread de processamento das requisições.
 *
 * Este método sinaliza para o thread parar de processar ao definir `m_running` como falso
 * e junta-se ao thread se ele for juntável, garantindo que todos os recursos sejam limpos adequadamente.
 */
void Parser::stop() {
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    }
    std::cout << "Parser thread stopped.\n";
}

/**
 * @brief Consome e processa requisições da fila.
 *
 * Este método é executado em um thread separado e é responsável por consumir requisições da fila,
 * processá-las chamando `parse_and_print` e gerenciar o estado de execução com base em `m_running`.
 */
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
/**
 * @brief Analisa e imprime o corpo de uma requisição HTTP.
 *
 * @param request A requisição HTTP a ser analisada e impressa.
 */
void Parser::parse_and_print(const boost::beast::http::request<boost::beast::http::string_body>& request) {
    std::cout << "[parser] Parsed Request Body: " << request.body() << std::endl;
}
