#ifndef ROWCA_H
#define ROWCA_H

#include "RowCadeAnalytics.h"
#include <string>
#include <iostream>

using namespace std;

class RowCA : public RowCadeAnalytics {
private:
    string Data_de_notificacao;
    int ID_usuario;   
    string Estimulo_realizado; 
    string Componente_alvo;

public:
    // Construtor
    RowCA(int idEvent, const string& data, int idUsuario, 
                     const string& estimuloRealizado, const string& componenteAlvo)
    : RowCadeAnalytics(4, idEvent), Data_de_notificacao(data), ID_usuario(idUsuario), 
      Estimulo_realizado(estimuloRealizado), Componente_alvo(componenteAlvo) {}

    // Getters
    string getData_notificacao() const { return Data_de_notificacao; }
    int getIdUsuario() const { return ID_usuario; }
    string getEstimuloRealizado() const { return Estimulo_realizado; }
    string getComponenteAlvo() const { return Componente_alvo; }

    // Setters
    void setData_notificacao(const string& data) { Data_de_notificacao = data; }
    void setIdUsuario(int id) { ID_usuario = id; }
    void setEstimuloRealizado(const string& estimulo) { Estimulo_realizado = estimulo; }
    void setComponenteAlvo(const string& componente) { Componente_alvo = componente; }

    // Método para imprimir os dados do evento, estendendo o método print da classe base
    void print() const override {
        RowCadeAnalytics::print();  // Chama o método print da classe base
        cout << ", Data_de_notificação: " << Data_de_notificacao 
             << ", ID_usuario: " << ID_usuario 
             << ", Estimulo_realizado: " << Estimulo_realizado 
             << ", Componente_alvo: " << Componente_alvo << endl;
    }
};

#endif // ROWCA_H
