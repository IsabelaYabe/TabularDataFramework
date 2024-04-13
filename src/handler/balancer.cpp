#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include "math.h"

using namespace std;

// Classe que representa uma linha do DataFrame
class Row {
};

// Classe que representa o DataFrame
class DataFrame {
public:
    std::vector<Row> Rows; // Vetor de linhas
    int numRows = 0; // Número de linhas
    

    // Adiciona uma linha ao DataFrame
    void addRow(const Row& newRow) {
        Rows.push_back(newRow);
        numRows++;
    }

};

// ------------------------------------------------------------------------------------

// Classe genérica para tratar o DataFrame
template<typename H>
class Handler {
public:
    // Fila de DataFrames de entrada
    queue<DataFrame> dataframes_in;
    // Vetor de queues de saída
    vector<queue<DataFrame>> dataframes_out;
    // Método virtual para processar a fila de DataFrames de entrada
    virtual void processDataframe(queue<DataFrame>& dataframes) const = 0;
};

// Classe para balancear a carga dividindo o DataFrame em partes menores
class BalancerHandler : public Handler<BalancerHandler> {
public:
    // Fila de DataFrames de entrada
    std::queue<DataFrame> dataframes_in;
    // Vetor de queues de saída
    vector<queue<DataFrame>> dataframes_out;

    // Método para dividir o DataFrame em partes menores
    void processDataframe(std::queue<DataFrame>& dataframes) {
        // Verifica se há algum DataFrame na fila
        if (!dataframes.empty()) {
            // Captura o DataFrame da fila
            DataFrame df = dataframes.front();
            dataframes.pop();

            // Divide o DataFrame em partes menores usando múltiplas threads
            size_t num_threads = std::thread::hardware_concurrency();
            size_t chunk_size = floor(df.numRows / num_threads);

            // Criar threads para processar o DataFrame
            std::vector<std::thread> threads;
            // Separa o DataFrame em partes menores e cria uma thread para cada parte
            for (size_t i = 0; i < num_threads; ++i) {
                size_t start_index = i * chunk_size;
                size_t end_index = (i == num_threads - 1) ? df.numRows : (i + 1) * chunk_size;

                // Criar uma nova thread para processar uma parte do DataFrame
                threads.emplace_back(&BalancerHandler::processChunk, this, df, start_index, end_index);
            }

            // Aguardar que todas as threads terminem
            for (auto& thread : threads) {
                thread.join();
            }
        }
    }

private:
    // Método para adicionar uma parte do DataFrame na fila de DataFrames de saída
    void processChunk(const DataFrame& df, size_t start_index, size_t end_index) const {
        // Criar um DataFrame para a parte do DataFrame
        DataFrame df_chunk;
        // Particione o DataFrame em partes menores
        for (size_t i = start_index; i < end_index; ++i) {
            df_chunk.addRow(df.Rows[i]);
        }
        // Adicionar a parte do DataFrame na fila de DataFrames de saída
        queue<DataFrame> q = dataframes_out[0]; // TODO: Implementar lógica para escolher a fila de saída
        q.push(df_chunk);
    }
};