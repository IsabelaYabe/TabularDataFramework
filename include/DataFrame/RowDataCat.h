#ifndef ROWDATACAT_H
#define ROWDATACAT_H

#include <iostream>
#include <string>

#include "Row.h"

class RowDataCat: public Row{
private:
    string Data_de_notificacao;
    string Tipo_de_log;
    string Conteudo_textual;

public:
    RowDataCat(int num, int idEvent, const string& dataNotificacao, const string& tipoDeLog, const string& conteudoTextual)
        : Row(num, idEvent), Data_de_notificacao(dataNotificacao), Tipo_de_log(tipoDeLog), Conteudo_textual(conteudoTextual) {}
    
    // Getters
    string getDataDeNotificacao() const { return Data_de_notificacao; }
    string getTipoDeLog() const { return Tipo_de_log; }
    string getConteudoTextual() const { return Conteudo_textual; }

    // Setters
    void setDataDeNotificacao(const string& data) { Data_de_notificacao = data; }
    void setTipoDeLog(const string& tipo) { Tipo_de_log = tipo; }
    void setConteudoTextual(const string& conteudo) { Conteudo_textual = conteudo; }

    virtual void print() const override {
        Row::print(); 
        cout << ", Data_de_notificacao: " << Data_de_notificacao
             << ", Tipo_de_log: " << Tipo_de_log
             << ", Conteudo_textual: " << Conteudo_textual;
    }
};

#endif // ROWDATACAT_H