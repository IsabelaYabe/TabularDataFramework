#ifndef ROWDATACAT_H
#define ROWDATACAT_H

#include <iostream>
#include <string>
#include <ctime> 
#include "Row.h"

using namespace std;

class RowDataCat : public Row {
private:
    tm Data_de_notificacao; 
    string Tipo_de_log;
    string Conteudo_textual;

public:
    // Modificado o construtor para aceitar componentes de data separadamente
    RowDataCat(int num, int idEvent, int year, int month, int day, int hour, int min, const string& tipoDeLog, const string& conteudoTextual)
        : Row(num, idEvent), Tipo_de_log(tipoDeLog), Conteudo_textual(conteudoTextual) {
            Data_de_notificacao.tm_year = year - 1900; // tm_year é o ano desde 1900
            Data_de_notificacao.tm_mon = month - 1;   // tm_mon é o mês desde janeiro (0-11)
            Data_de_notificacao.tm_mday = day;
            Data_de_notificacao.tm_hour = hour;
            Data_de_notificacao.tm_min = min;
            Data_de_notificacao.tm_sec = 0;           // Segundos, geralmente definido para 0 se não usado
        }
    
    // Getters
    tm getDataDeNotificacao() const { return Data_de_notificacao; }
    string getTipoDeLog() const { return Tipo_de_log; }
    string getConteudoTextual() const { return Conteudo_textual; }

    // Setters
    void setDataDeNotificacao(const tm& data) { Data_de_notificacao = data; }
    void setTipoDeLog(const string& tipo) { Tipo_de_log = tipo; }
    void setConteudoTextual(const string& conteudo) { Conteudo_textual = conteudo; }

    // Modificado o método print para exibir a data formatada
    virtual void print() const override {
        Row::print();
        cout << ", Data_de_notificacao: " 
                  << (Data_de_notificacao.tm_year + 1900) << '-' 
                  << (Data_de_notificacao.tm_mon + 1) << '-' 
                  << Data_de_notificacao.tm_mday << ' '
                  << Data_de_notificacao.tm_hour << ':' 
                  << Data_de_notificacao.tm_min
                  << ", Tipo_de_log: " << Tipo_de_log
                  << ", Conteudo_textual: " << Conteudo_textual;
    }
};

#endif // ROWDATACAT_H
