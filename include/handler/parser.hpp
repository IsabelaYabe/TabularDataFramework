#ifndef PARSER_HPP
#define PARSER_HPP

#include "queue/queue.hpp"
#include <boost/beast/http.hpp>
#include <thread>
#include <atomic>
#include <boost/log/trivial.hpp>

/**
 * @class Parser
 * @brief Classe responsável por processar requisições HTTP recebidas.
 * 
 * Parser é uma classe que gerencia o processamento de requisições HTTP, consumindo-as de uma fila e processando-as em um thread separado.
 */
class Parser {
public:
    /**
    * @brief Construtor que inicializa o Parser com uma fila de requisições HTTP.
    * 
    * @param queue Referência para uma fila de requisições HTTP que o Parser irá consumir.
    */
    explicit Parser(Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue);
     /**
    * @brief Inicia o processo de consumo das requisições HTTP da fila em um novo thread.
    */
    void start();
    /**
    * @brief Para o processo de consumo de requisições, sinalizando ao thread de consumo para terminar.
    */
    void stop();
    /**
    * @brief Método estático para parsear e imprimir o conteúdo de uma requisição HTTP.
    * 
    * @param request A requisição HTTP que será parseada e impressa.
    */
    static void parse_and_print(const boost::beast::http::request<boost::beast::http::string_body>& request);

private:
    Queue<boost::beast::http::request<boost::beast::http::string_body>>& m_queue; ///< Fila de requisições HTTP a ser consumida.
    std::thread m_thread; ///< Thread usado para consumir requisições da fila.
    std::atomic<bool> m_running; ///< Flag atômica que controla a execução do thread de consumo.
    /**
    * @brief Método privado que executa em um thread, consumindo requisições da fila e processando-as.
    */
    void consume();

};

#endif // PARSER_HPP
