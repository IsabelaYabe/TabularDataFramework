#ifndef ROWCONTAVERDEORDEMDECOMPRA_H
#define ROWCONTAVERDEORDEMDECOMPRA_H

#include "RowContaVerde.h" 
#include <string>

using namespace std;

class RowContaVerdeOrdemDeCompra : public RowContaVerde {
private:
    int ID_usuario;           
    int ID_produto;         
    int Quantidade;           
    string Data_de_criacao;  
    string Data_de_pagamento;  
    string Data_de_entrega;   

public:
    // Construtor
    RowContaVerdeOrdemDeCompra( int idEvent, int idUsuario, int idProduto, int quantidade,
                               const string& dataCriacao, const string& dataPagamento,
                               const string& dataEntrega)
    : RowContaVerde(6, idEvent), ID_usuario(idUsuario), ID_produto(idProduto), Quantidade(quantidade),
      Data_de_criacao(dataCriacao), Data_de_pagamento(dataPagamento), Data_de_entrega(dataEntrega) {}

    // Getters
    int getIdUsuario() const { return ID_usuario; }
    int getIdProduto() const { return ID_produto; }
    int getQuantidade() const { return Quantidade; }
    string getDataCriacao() const { return Data_de_criacao; }
    string getDataPagamento() const { return Data_de_pagamento; }
    string getDataEntrega() const { return Data_de_entrega; }

    // Setters
    void setIdUsuario(int id) { ID_usuario = id; }
    void setIdProduto(int id) { ID_produto = id; }
    void setQuantidade(int quantidade) { Quantidade = quantidade; }
    void setDataCriacao(const string& data) { Data_de_criacao = data; }
    void setDataPagamento(const string& data) { Data_de_pagamento = data; }
    void setDataEntrega(const string& data) { Data_de_entrega = data; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();   
        cout << ", ID_usuário: " << ID_usuario 
             << ", ID_produto: " << ID_produto 
             << ", Quantidade: " << Quantidade 
             << ", Data_de_criação: " << Data_de_criacao 
             << ", Data_de_pagamento: " << Data_de_pagamento 
             << ", Data_de_entrega: " << Data_de_entrega << endl;
    }
};

#endif // ROWCONTAVERDEORDEMDECOMPRA_H
