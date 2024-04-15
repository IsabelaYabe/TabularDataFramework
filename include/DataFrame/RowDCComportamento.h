#ifndef ROWDCCOMPORTAMENTO_H
#define ROWDCCOMPORTAMENTO_H

#include "RowDataCat.h"  // Inclui o header da classe base

using namespace std;

// Classe para gerenciar informações de logs de comportamento do usuário
class RowDCComportamento : public RowDataCat {
private:
    int ID_usuario;       
    string Estimulo_realizado; 
    string Componente_alvo;

public:
    // Construtor
    RowDCComportamento(int idEvent, int year, int month, int day, int hour, int min, const string& tipoDeLog,
                       const string& conteudoTextual, int idUsuario, const string& estimuloRealizado,
                       const string& componenteAlvo)
        : RowDataCat(6, idEvent, year, month, day, hour, min, tipoDeLog, conteudoTextual),  // Chamada ao construtor da classe base
          ID_usuario(idUsuario), Estimulo_realizado(estimuloRealizado), Componente_alvo(componenteAlvo) {}

    // Getters 
    int getIDUsuario() const { return ID_usuario; }
    string getEstimuloRealizado() const { return Estimulo_realizado; }
    string getComponenteAlvo() const { return Componente_alvo; }

    // Setters 
    void setIDUsuario(int id) { ID_usuario = id; }
    void setEstimuloRealizado(const string& estimulo) { Estimulo_realizado = estimulo; }
    void setComponenteAlvo(const string& componente) { Componente_alvo = componente; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowDataCat::print();  
        cout << ", ID_usuario: " << ID_usuario
             << ", Estimulo_realizado: " << Estimulo_realizado
             << ", Componente_alvo: " << Componente_alvo << endl;
    }
};

#endif // ROWDCCOMPORTAMENTO_H
