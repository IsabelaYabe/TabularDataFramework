#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "math.h"
#include "../queue/queue.h"

using namespace std;


// Classe genérica para tratar o DataFrame
class Handler {
    private:
        // Fila de DataFrames de entrada
        Queue<DataFrame> dataframes_in;
        // Vetor de queues de saída
        vector<Queue<DataFrame>> dataframes_out;
    public: 
        // Construtor da classe
        Handler(Queue<DataFrame> dataframes_in, vector<Queue<DataFrame>> dataframes_out): dataframes_in(dataframes_in), dataframes_out(dataframes_out) {};
};
// Classe para filtrar o DataFrame por horário
class FilterHandler: public Handler {
    public:
        // Construtor da classe
        FilterHandler(Queue<DataFrame> dataframes_in, vector<Queue<DataFrame>> dataframes_out): Handler(dataframes_in, dataframes_out){};
        // Método para filtrar o DataFrame por horário
        void filterByTime(){};
        

};

#endif // HANDLER_H
