#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "math.h"
#include "../queue/Queue.h"
#include "../dataframe/dataframe.h"

using namespace std;


// Classe genérica para tratar o DataFrame
class Handler {
public:
    // Fila de DataFrames de entrada
    Queue<DataFrame*> *dataframes_in;
    // Vetor de queues de saída
    vector<Queue<DataFrame*>> *dataframes_out;

    // Construtor da classe
    Handler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out)
        : dataframes_in(dataframes_in), dataframes_out(dataframes_out) {}
};

class BalanceHandler: public Handler {
    public:
        // Construtor da classe
        BalanceHandler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para balancear o DataFrame
        void BalancerFunction();
    private:
        // Método para processar uma parte do DataFrame
        void ProcessChunk(DataFrame df,int start_index, int end_index);
};

// Classe para filtrar o DataFrame por horário
class CleanCache: public Handler {
    public:
        // Construtor da classe
        CleanCache(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para filtrar o DataFrame por horário
        void CleanCacheFunction();
};

// Classe para filtrar as ações do usuário e conectá-las com os produtos
class FilterHandler: public Handler {
    public:
        // Construtor da classe
        FilterHandler(Queue<DataFrame*> *dataframes_in, vector<Queue<DataFrame*>> *dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para filtrar as ações do usuário e conectá-las com os produtos
        void FilterFunction(DataFrame df_products);
};


#endif // HANDLER_H
