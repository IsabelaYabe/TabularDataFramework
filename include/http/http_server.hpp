#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include "http_session.hpp"
#include "queue/queue.hpp"

/**
 * @class http_server
 * @brief Classe responsável por iniciar e gerenciar um servidor HTTP.
 * 
 * Esta classe configura um servidor HTTP que aceita conexões em um determinado porto e 
 * encaminha as requisições recebidas para uma fila de processamento.
 */
class http_server {
public:
    /**
     * @brief Constrói um servidor HTTP.
     * 
     * Inicializa o servidor com o contexto de I/O fornecido, o porto para escutar e uma fila
     * para as requisições HTTP que serão processadas.
     *
     * @param ioc Referência ao contexto de I/O do Boost.Asio para gerenciar a I/O assíncrona.
     * @param port O porto TCP no qual o servidor deve escutar por novas conexões.
     * @param queue Referência a uma fila para onde as requisições HTTP recebidas serão enfileiradas.
     */
    http_server(boost::asio::io_context& ioc, short port, Queue<boost::beast::http::request<boost::beast::http::string_body>>& queue);

private:
    boost::asio::ip::tcp::acceptor acceptor_; ///< Aceitador TCP usado para aceitar conexões de entrada.
    /**
     * @brief Inicia o processo assíncrono de aceitação de novas conexões HTTP.
     * 
     * Este método configura o aceitador para iniciar a aceitação de conexões. Quando uma nova
     * conexão é aceita, uma nova sessão HTTP é criada para lidar com a requisição.
     */
    void do_accept();
    Queue<boost::beast::http::request<boost::beast::http::string_body>>& requestQueue_; ///< Fila de requisições onde as requisições HTTP são armazenadas após serem recebidas.
};

#endif // HTTP_SERVER_HPP