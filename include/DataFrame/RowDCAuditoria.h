#ifndef ROWDCAUDITORIA_H
#define ROWDCAUDITORIA_H

#include "RowDataCat.h"  

class RowDCAuditoria : public RowDataCat {
private:
    int ID_usuario;          
    string Acao_realizada;   

public:
    // Construtor 
    RowDCAuditoria(int idEvent, int year, int month, int day, int hour, int min, const string& tipoDeLog,
                   const string& conteudoTextual, int idUsuario, const string& acaoRealizada)
        : RowDataCat(5, idEvent, year, month, day, hour, min, tipoDeLog, conteudoTextual),  
          ID_usuario(idUsuario), Acao_realizada(acaoRealizada) {}
    
    // Getters
    int getIDUsuario() const { return ID_usuario; }
    string getAcaoRealizada() const { return Acao_realizada; }

    // Setters
    void setIDUsuario(int idUsuario) { ID_usuario = idUsuario; }
    void setAcaoRealizada(const string& acaoRealizada) { Acao_realizada = acaoRealizada; }

    // Sobrescreve o método print da classe base para incluir a impressão dos atributos adicionais desta classe
    void print() const override {
        RowDataCat::print();  // Chama o método print da classe base para imprimir os atributos comuns
        cout << ", ID_usuario: " << ID_usuario  
             << ", Acao_realizada: " << Acao_realizada << endl; 
    }
};

#endif // ROWDCAUDITORIA_H
