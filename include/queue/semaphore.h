#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <iostream>
#include <mutex>
#include <condition_variable>

using namespace std;

class Semaphore {
    private:
        // Contador do semáforo
        int count;
        // Contador máximo do semáforo
        int max_count;
        // Mutex para controlar o acesso ao semáforo    
        mutex mtx;
        // Variável de condição para aceitar notificações
        condition_variable cv;
    public:
        // Construtor do semáforo com contagem inicial e máxima
        explicit Semaphore(int count = 0, int max_count = 1): count(count), max_count(max_count) {};
        // Método para notificar o semáforo que um recurso foi liberado
        void notify(){
            unique_lock<mutex> lck(mtx);
            while (count == max_count){
                cv.wait(lck);
            }
            count++;
            cv.notify_one();
        };
        // Método para aguardar até que um recurso esteja disponível
        void wait(){
            unique_lock<mutex> lck(mtx);
            while (count == 0){
                cv.wait(lck);
            }
            count--;
        };
};

#endif //SEMAPHORE_H