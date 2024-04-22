#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include "math.h"

#include "../../include/queue/Queue.hpp"
#include "../../include/handler/handler.h"
#include "../../include/dataframe/dataframe.h"
#include "../../include/DataFrame/Time.h"

using namespace std;

// Método para adicionar uma parte do DataFrame na fila de DataFrames de saída
void BalanceHandler::ProcessChunk(DataFrame df,int start_index, int end_index) {
        // Criar um DataFrame para o pedaço do DataFrame original
        DataFrame df_chunk;
        // Particione o DataFrame em partes menores
        for (size_t i = start_index; i < end_index; ++i) {
            df_chunk.insertRow(df.getRows()[i]);
        }
        // Adiciona a parte do DataFrame em uma das filas de saída de DataFrames
        BalanceHandler::dataframes_out->at(0).enqueue(&df_chunk);

    };


void BalanceHandler::BalancerFunction() {
    Queue<DataFrame*>& q = *dataframes_in;
    // Verifica se há algum DataFrame na fila
    if (!q.empty()) {
        // Captura o DataFrame da fila
        DataFrame* df_ptr = q.dnqueue(); // Captura o ponteiro para o DataFrame da fila
        DataFrame df = *df_ptr;

        // Divide o DataFrame em partes menores usando múltiplas threads
        size_t num_threads = std::thread::hardware_concurrency();
        size_t chunk_size = floor(df.getRowCount() / num_threads);

        // Criar threads para processar o DataFrame
        vector<thread> threads;
        // Separa o DataFrame em partes menores e cria uma thread para cada parte
        for (size_t i = 0; i < num_threads; ++i) {
            size_t start_index = i * chunk_size;
            size_t end_index = (i == num_threads - 1) ? df.getRowCount() : (i + 1) * chunk_size;

            // Criar uma nova thread para processar uma parte do DataFrame
            threads.emplace_back(&BalanceHandler::ProcessChunk, this, df, start_index, end_index);
        }

        // Aguardar que todas as threads terminem
        for (auto& thread : threads) {
            thread.join();
        }
    }
};


void CleanCache::CleanCacheFunction() {
    Queue<DataFrame*>& q = *dataframes_in;
    // Verifica se há algum DataFrame na fila
    if (!q.empty()) {
        // Captura o DataFrame da fila
        DataFrame* df_ptr = q.dnqueue(); // Captura o ponteiro para o DataFrame da fila
        DataFrame df = *df_ptr;

        // Captura o horário atual
        auto currentTimePoint = std::chrono::system_clock::now();
        time_t now = std::chrono::system_clock::to_time_t(currentTimePoint);
        std::string timeString = std::ctime(&now);
        cout<<timeString<<endl;
        Time currentTime = Time::fromString(timeString).value();
        
        // Captura a coluna de horários
        for (int i = 0; i < df.getRowCount(); ++i) {
            // Captura o horário da linha
            RowVariant row_time = df.getRows()[i]->getCol_("time");
            Time rowTime = get<Time>(row_time);
            // Verifica se o horário é anterior ao horário atual
            bool isBefore = Time::compareTime(rowTime, currentTime, Time::TimeMeasure::Year, Time::CompareType::Less);
            // Verifica se o horário é posterior ou igual ao horário atual decrescido de 1 hora
            Time currentTimeMinusHour = currentTime;
            currentTimeMinusHour.setHour(currentTimeMinusHour.getHour() - 1);
            bool isAfter = Time::compareTime(rowTime, currentTimeMinusHour, Time::TimeMeasure::Hour, Time::CompareType::GreaterOrEqual);
            if (isBefore && isAfter) {
                // Remove a linha do DataFrame
                df.removeRow(i);
            }
        };

        // Adiciona o DataFrame filtrado em uma das filas de saída de DataFrames
        CleanCache::dataframes_out->at(0).enqueue(&df);

    }
};

void FilterHandler::FilterFunction(DataFrame df_products) {
    Queue<DataFrame*>& q = *dataframes_in;
    // Verifica se há algum DataFrame na fila
    if (!q.empty()) {
        // Captura o DataFrame da fila
        DataFrame* df_ptr = q.dnqueue(); // Captura o ponteiro para o DataFrame da fila
        DataFrame df = *df_ptr;

        // Filtra os cliques do usuário 
        for (int i = 0; i < df.getRowCount(); ++i) {
            // Captura o tipo de ação do usuário
            RowVariant row_action = df.getRows()[i]->getCol_("action");
            string action = get<string>(row_action);
            // Remove as linhas que não são cliques em produtos
            if (action != "clickProduct") {
                // Remove a linha do DataFrame
                df.removeRow(i);
            }
            // Merge com o DataFrame de produtos
            df.innerJoin(df_products, "product_id");
            // Agrupa por produto somando a quantidade de cliques
            df.groupByFreq("product_id");

        }
        // Adiciona o DataFrame filtrado em uma das filas de saída de DataFrames
        FilterHandler::dataframes_out->at(0).enqueue(&df);
    }

};