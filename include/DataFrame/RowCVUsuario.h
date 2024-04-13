#ifndef ROWCONTAVERDEUSUARIO_H
#define ROWCONTAVERDEUSUARIO_H

#include "RowContaVerde.h"  
#include <string>

using namespace std;

class RowContaVerdeUsuario : public RowContaVerde {
private:
    int ID_usuario;           
    string Nome;        
    string Sobrenome;         
    string Endereco;           
    string Data_de_cadastro;  
    string Data_de_aniversario;

public:
    // Construtor
    RowContaVerdeUsuario(int idEvent, int id, const string& nome, const string& sobrenome,
                         const string& endereco, const string& dataCadastro,
                         const string& dataAniversario)
    : RowContaVerde(6, idEvent), ID_usuario(id), Nome(nome), Sobrenome(sobrenome), 
      Endereco(endereco), Data_de_cadastro(dataCadastro), Data_de_aniversario(dataAniversario) {}

    // Getters
    int getIDUsuario() const { return ID_usuario; }
    string getNome() const { return Nome; }
    string getSobrenome() const { return Sobrenome; }
    string getEndereco() const { return Endereco; }
    string getDataCadastro() const { return Data_de_cadastro; }
    string getDataAniversario() const { return Data_de_aniversario; }

    // Setters
    void setIDUsuario(int id) { ID_usuario = id; }
    void setNome(const string& nome) { Nome = nome; }
    void setSobrenome(const string& sobrenome) { Sobrenome = sobrenome; }
    void setEndereco(const string& endereco) { Endereco = endereco; }
    void setDataCadastro(const string& data) { Data_de_cadastro = data; }
    void setDataAniversario(const string& data) { Data_de_aniversario = data; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();  
        cout << ", ID_usuario: " << ID_usuario 
             << ", Nome: " << Nome 
             << ", Sobrenome: " << Sobrenome 
             << ", Endereco: " << Endereco 
             << ", Data_de_cadastro: " << Data_de_cadastro 
             << ", Data_de_aniversario: " << Data_de_aniversario << endl;
    }
};

#endif // ROWCONTAVERDEUSUARIO_H
