#ifndef BALANCER_H
#define BALANCER_H

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "math.h"
#include "../DataFrame/DataFrame.h"
#include "..DataFrame/RowCA.h"
#include "../queue/queue.hpp"

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
    void processDataframe(Queue<DataFrame>& dataframes) override;

private:
    // Método para adicionar uma parte do DataFrame na fila de DataFrames de saída
    void processChunk(const DataFrame& df, size_t start_index, size_t end_index) const;
};

#endif // BALANCER_H
