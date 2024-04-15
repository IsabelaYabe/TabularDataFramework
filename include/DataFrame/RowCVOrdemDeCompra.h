#ifndef ROWCONTAVERDEORDEMDECOMPRA_H
#define ROWCONTAVERDEORDEMDECOMPRA_H

#include "RowContaVerde.h" 
#include <string>
#include <ctime> 

using namespace std;

class RowContaVerdeOrdemDeCompra : public RowContaVerde {
private:
    int ID_usuario;           
    int ID_produto;         
    int Quantidade;           
    tm Data_de_criacao;  
    tm Data_de_pagamento;  
    tm Data_de_entrega;   

public:
    // Construtor
    RowContaVerdeOrdemDeCompra( int idEvent, int idUsuario, int idProduto, int quantidade,
                               int yearCriacao, int monthCriacao, int dayCriacao, int hourCriacao, int minCriacao, int yearPagamento, int monthPagamento, int dayPagamento, int hourPagamento, int minPagamento,
                               int yearEntrega, int monthEntrega, int dayEntrega, int hourEntrega, int minEntrega)
    : RowContaVerde(6, idEvent), ID_usuario(idUsuario), ID_produto(idProduto), Quantidade(quantidade)
      {
            Data_de_criacao.tm_year = yearCriacao  - 1900; 
            Data_de_criacao.tm_mon = monthCriacao  - 1;   
            Data_de_criacao.tm_mday = dayCriacao ;
            Data_de_criacao.tm_hour = hourCriacao ;
            Data_de_criacao.tm_min = minCriacao ;
            Data_de_criacao.tm_sec = 0;
            Data_de_pagamento.tm_year = yearPagamento - 1900; 
            Data_de_pagamento.tm_mon = monthPagamento - 1;   
            Data_de_pagamento.tm_mday = dayPagamento;
            Data_de_pagamento.tm_hour = hourPagamento;
            Data_de_pagamento.tm_min = minPagamento;
            Data_de_pagamento.tm_sec = 0;
            Data_de_entrega.tm_year = yearEntrega - 1900; 
            Data_de_entrega.tm_mon = monthEntrega - 1;   
            Data_de_entrega.tm_mday = dayEntrega;
            Data_de_entrega.tm_hour = hourEntrega;
            Data_de_entrega.tm_min = minEntrega;
            Data_de_entrega.tm_sec = 0;}

    // Getters
    int getIdUsuario() const { return ID_usuario; }
    int getIdProduto() const { return ID_produto; }
    int getQuantidade() const { return Quantidade; }
    tm getDataCriacao() const { return Data_de_criacao; }
    tm getDataPagamento() const { return Data_de_pagamento; }
    tm getDataEntrega() const { return Data_de_entrega; }

    // Setters
    void setIdUsuario(int id) { ID_usuario = id; }
    void setIdProduto(int id) { ID_produto = id; }
    void setQuantidade(int quantidade) { Quantidade = quantidade; }
    void setDataCriacao(const tm& data) { Data_de_criacao = data; }
    void setDataPagamento(const tm& data) { Data_de_pagamento = data; }
    void setDataEntrega(const tm& data) { Data_de_entrega = data; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();   
        cout << ", ID_usuário: " << ID_usuario 
             << ", ID_produto: " << ID_produto 
             << ", Quantidade: " << Quantidade 
             << ", Data_de_criacao: " 
                  << (Data_de_criacao.tm_year + 1900) << '-' 
                  << (Data_de_criacao.tm_mon + 1) << '-' 
                  << Data_de_criacao.tm_mday << ' '
                  << Data_de_criacao.tm_hour << ':' 
                  << Data_de_criacao.tm_min 
             << ", Data_de_pagamento: " 
                  << (Data_de_pagamento.tm_year + 1900) << '-' 
                  << (Data_de_pagamento.tm_mon + 1) << '-' 
                  << Data_de_pagamento.tm_mday << ' '
                  << Data_de_pagamento.tm_hour << ':' 
                  << Data_de_pagamento.tm_min 
             << ",, Data_de_entrega: " 
                  << (Data_de_entrega.tm_year + 1900) << '-' 
                  << (Data_de_entrega.tm_mon + 1) << '-' 
                  << Data_de_entrega.tm_mday << ' '
                  << Data_de_entrega.tm_hour << ':' 
                  << Data_de_entrega.tm_min << endl;
    }
};

#endif // ROWCONTAVERDEORDEMDECOMPRA_H
