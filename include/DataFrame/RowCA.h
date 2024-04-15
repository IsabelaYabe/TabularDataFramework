#ifndef ROWCA_H
#define ROWCA_H

#include "RowCadeAnalytics.h"
#include <string>
#include <iostream>
#include <ctime> 

using namespace std;

class RowCA : public RowCadeAnalytics {
private:
    tm Data_de_notificacao; 
    int ID_usuario;   
    string Estimulo_realizado; 
    string Componente_alvo;

public:
    // Construtor
    RowCA(int idEvent, int year, int month, int day, int hour, int min, int idUsuario, 
                     const string& estimuloRealizado, const string& componenteAlvo)
    : RowCadeAnalytics(4, idEvent), ID_usuario(idUsuario), 
      Estimulo_realizado(estimuloRealizado), Componente_alvo(componenteAlvo) {
            Data_de_notificacao.tm_year = year - 1900; 
            Data_de_notificacao.tm_mon = month - 1;   
            Data_de_notificacao.tm_mday = day;
            Data_de_notificacao.tm_hour = hour;
            Data_de_notificacao.tm_min = min;
            Data_de_notificacao.tm_sec = 0;
            }

    // Getters
    tm getData_notificacao() const { return Data_de_notificacao; }
    int getIdUsuario() const { return ID_usuario; }
    string getEstimuloRealizado() const { return Estimulo_realizado; }
    string getComponenteAlvo() const { return Componente_alvo; }

    // Setters
    void setData_notificacao(const tm& data) { Data_de_notificacao = data; }
    void setIdUsuario(int id) { ID_usuario = id; }
    void setEstimuloRealizado(const string& estimulo) { Estimulo_realizado = estimulo; }
    void setComponenteAlvo(const string& componente) { Componente_alvo = componente; }

    // Método para imprimir os dados do evento, estendendo o método print da classe base
    void print() const override {
        RowCadeAnalytics::print();  // Chama o método print da classe base
        cout << ", Data_de_notificacao: " 
                  << (Data_de_notificacao.tm_year + 1900) << '-' 
                  << (Data_de_notificacao.tm_mon + 1) << '-' 
                  << Data_de_notificacao.tm_mday << ' '
                  << Data_de_notificacao.tm_hour << ':' 
                  << Data_de_notificacao.tm_min 
             << ", ID_usuario: " << ID_usuario 
             << ", Estimulo_realizado: " << Estimulo_realizado 
             << ", Componente_alvo: " << Componente_alvo << endl;
    }
};

#endif // ROWCA_H