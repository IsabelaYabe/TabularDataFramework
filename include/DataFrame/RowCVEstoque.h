#ifndef ROWCONTAVERDEESTOQUE_H
#define ROWCONTAVERDEESTOQUE_H

#include "RowContaVerde.h"  

class RowContaVerdeEstoque : public RowContaVerde {
private:
    int ID_produto;    
    int Quantidade;   

public:
    // Construtor 
    RowContaVerdeEstoque(int idEvent, int idProduto, int quantidade)
        : RowContaVerde(2, idEvent), ID_produto(idProduto), Quantidade(quantidade) {}

    // Getters
    int getIdProduto() const { return ID_produto; }
    int getQuantidade() const { return Quantidade; }

    // Setters
    void setIdProduto(int id) { ID_produto = id; }
    void setQuantidade(int quantidade) { Quantidade = quantidade; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();  
        cout << ", ID_produto: " << ID_produto
             << ", Quantidade Disponível: " << Quantidade << endl;
    }
};

#endif // ROWCONTAVERDEESTOQUE_H