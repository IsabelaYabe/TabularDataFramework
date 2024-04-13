#ifndef ROWCONTAVERDEPRODUTOS_H
#define ROWCONTAVERDEPRODUTOS_H

#include "RowContaVerde.h"  
#include <vector>
#include <string>

using namespace std;

class RowContaVerdeProdutos : public RowContaVerde {
private:
    int ID_produto;        
    string Nome;              
    vector<int> Imagem;      
    string Descricao;       
    float Preco;                

public:
    // Construtor 
    RowContaVerdeProdutos(int idEvent, int id, const string& nome, const vector<int>& imagem, const string& descricao, float preco)
        : RowContaVerde(5, idEvent), ID_produto(id), Nome(nome), Imagem(imagem), Descricao(descricao), Preco(preco) {}

    // Getters
    int getIDProduto() const { return ID_produto; }
    string getNome() const { return Nome; }
    const vector<int>& getImagem() const { return Imagem; }
    string getDescricao() const { return Descricao; }
    float getPreco() const { return Preco; }

    // Setters
    void setIDProduto(int id) { ID_produto = id; }
    void setNome(const string& nome) { Nome = nome; }
    void setImagem(const vector<int>& imagem) { Imagem = imagem; }
    void setDescricao(const string& descricao) { Descricao = descricao; }
    void setPreco(float preco) { Preco = preco; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();
        cout << ", ID_produto: " << ID_produto
             << ", Nome: " << Nome
             << ", Descricao: " << Descricao
             << ", Preco: " << Preco
             << ", Imagem data size: " << Imagem.size() << endl;  // Assume que 'Imagem' armazena tamanhos de dados de imagens
    }
};

#endif // ROWCONTAVERDEPRODUTOS_H
