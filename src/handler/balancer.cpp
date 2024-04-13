#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "math.h"
#include "../include/handler/balancer.h"
#include "../include/DataFrame/DataFrame.h"
#include "../include/DataFrame/RowCA.h"
#include "../include/queue/queue.hpp"

using namespace std;

// Classe genérica para tratar o DataFrame
template<typename H>
class Handler {
public:
    // Fila de DataFrames de entrada
    Queue<DataFrame> dataframes_in;
    // Vetor de queues de saída
    vector<Queue<DataFrame>> dataframes_out;
    // Método virtual para processar a fila de DataFrames de entrada
    virtual void processDataframe(Queue<DataFrame>& dataframes) const = 0;
};

// Classe para balancear a carga dividindo o DataFrame em partes menores
class BalancerHandler : public Handler<BalancerHandler> {
public:
    // Fila de DataFrames de entrada
    Queue<DataFrame> dataframes_in;
    // Vetor de queues de saída
    vector<Queue<DataFrame>> dataframes_out;

    // Método para dividir o DataFrame em partes menores
    void processDataframe(Queue<DataFrame>& dataframes) {
        // Verifica se há algum DataFrame na fila
        if (!dataframes.empty()) {
            // Captura o DataFrame da fila
            DataFrame df = dataframes.front();
            dataframes.pop();

            // Divide o DataFrame em partes menores usando múltiplas threads
            size_t num_threads = std::thread::hardware_concurrency();
            size_t chunk_size = floor(df.getShape().first / num_threads);

            // Criar threads para processar o DataFrame
            std::vector<std::thread> threads;
            // Separa o DataFrame em partes menores e cria uma thread para cada parte
            for (size_t i = 0; i < num_threads; ++i) {
                size_t start_index = i * chunk_size;
                size_t end_index = (i == num_threads - 1) ? df.getShape().first : (i + 1) * chunk_size;

                // Criar uma nova thread para processar uma parte do DataFrame
                threads.emplace_back(&BalancerHandler::processChunk, this, df, start_index, end_index);
            }

            // Aguardar que todas as threads terminem
            for (auto& thread : threads) {
                thread.join();
            }
        }
        // Deleta o DataFrame antigo
        delete &dataframes;
    }

private:
    // Método para adicionar uma parte do DataFrame na fila de DataFrames de saída
    void processChunk(const DataFrame& df, size_t start_index, size_t end_index) const {
        // Criar um DataFrame para a parte do DataFrame
        DataFrame df_chunk;
        // Particione o DataFrame em partes menores
        for (size_t i = start_index; i < end_index; ++i) {
            df_chunk.insertRow(df.getRows()[i]);
        }
        // Adicionar a parte do DataFrame na fila de DataFrames de saída
        Queue<DataFrame> q = dataframes_out[0]; // TODO: Implementar lógica para escolher a fila de saída
        q.enqueue(df_chunk);
    }
};

int main() {
    DataFrame df;

    // Definindo colunas e tipos (usualmente, isso seria dinâmico ou parte de um método de DataFrame)
    vector<string> columns = {"IDEvent", "Data", "UserID", "Stimulus", "TargetComponent"};
    vector<string> types = {"int", "string", "int", "string", "string"};
    df.setColumns(columns);
    df.setTypes(types);
    
    // Criando e adicionando algumas linhas
    auto row1 = make_shared<RowCA>(1, "2022-10-01", 101, "Teste1", "ComponentA");
    auto row2 = make_shared<RowCA>(2, "2022-10-02", 102, "Teste2", "ComponentB");
    auto row3 = make_shared<RowCA>(3, "2022-10-03", 103, "Teste3", "ComponentC");
    auto row4 = make_shared<RowCA>(4, "2022-10-04", 104, "Teste4", "ComponentD");
    auto row5 = make_shared<RowCA>(5, "2022-10-05", 105, "Teste5", "ComponentE");
    auto row6 = make_shared<RowCA>(6, "2022-10-06", 106, "Teste6", "ComponentF");
    auto row7 = make_shared<RowCA>(7, "2022-10-07", 107, "Teste7", "ComponentG");
    auto row8 = make_shared<RowCA>(8, "2022-10-08", 108, "Teste8", "ComponentH");
    auto row9 = make_shared<RowCA>(9, "2022-10-09", 109, "Teste9", "ComponentI");
    auto row10 = make_shared<RowCA>(10, "2022-10-10", 110, "Teste10", "ComponentJ");
    auto row11 = make_shared<RowCA>(11, "2022-10-11", 111, "Teste11", "ComponentK");
    auto row12 = make_shared<RowCA>(12, "2022-10-12", 112, "Teste12", "ComponentL");
    auto row13 = make_shared<RowCA>(13, "2022-10-13", 113, "Teste13", "ComponentM");
    auto row14 = make_shared<RowCA>(14, "2022-10-14", 114, "Teste14", "ComponentN");
    auto row15 = make_shared<RowCA>(15, "2022-10-15", 115, "Teste15", "ComponentO");
    auto row16 = make_shared<RowCA>(16, "2022-10-16", 116, "Teste16", "ComponentP");
    auto row17 = make_shared<RowCA>(17, "2022-10-17", 117, "Teste17", "ComponentQ");
    auto row18 = make_shared<RowCA>(18, "2022-10-18", 118, "Teste18", "ComponentR");
    auto row19 = make_shared<RowCA>(19, "2022-10-19", 119, "Teste19", "ComponentS");
    auto row20 = make_shared<RowCA>(20, "2022-10-20", 120, "Teste20", "ComponentT");
    df.insertRow(row1);
    df.insertRow(row2);
    df.insertRow(row3);
    df.insertRow(row4);
    df.insertRow(row5);
    df.insertRow(row6);
    df.insertRow(row7);
    df.insertRow(row8);
    df.insertRow(row9);
    df.insertRow(row10);
    df.insertRow(row11);
    df.insertRow(row12);
    df.insertRow(row13);
    df.insertRow(row14);
    df.insertRow(row15);
    df.insertRow(row16);
    df.insertRow(row17);
    df.insertRow(row18);
    df.insertRow(row19);
    df.insertRow(row20);

    Queue<DataFrame> q_in;
    q_in.enqueue(df);
    vector<Queue<DataFrame>> q_out;
    BalancerHandler balancer;
    balancer.dataframes_in = q_in;
    balancer.dataframes_out = q_out;
    balancer.processDataframe(balancer.dataframes_in);


} 