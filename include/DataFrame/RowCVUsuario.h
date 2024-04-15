#ifndef ROWCONTAVERDEUSUARIO_H
#define ROWCONTAVERDEUSUARIO_H

#include "RowContaVerde.h"  
#include <string>
#include <ctime> 

using namespace std;

class RowContaVerdeUsuario : public RowContaVerde {
private:
    int ID_usuario;           
    string Nome;        
    string Sobrenome;         
    string Endereco;           
    tm Data_de_cadastro;  
    tm Data_de_aniversario;

public:
    // Construtor
    RowContaVerdeUsuario(int idEvent, int id, const string& nome, const string& sobrenome,
                         const string& endereco, int yearCadastro, int monthCadastro, int dayCadastro, int hourCadastro, int minCadastro, int yearAniversario, int monthAniversario, int dayAniversario, int hourAniversario, int minAniversario)
    : RowContaVerde(6, idEvent), ID_usuario(id), Nome(nome), Sobrenome(sobrenome), 
      Endereco(endereco) {
            Data_de_cadastro.tm_year = yearCadastro  - 1900; 
            Data_de_cadastro.tm_mon = monthCadastro  - 1;   
            Data_de_cadastro.tm_mday = dayCadastro ;
            Data_de_cadastro.tm_hour = hourCadastro ;
            Data_de_cadastro.tm_min = minCadastro ;
            Data_de_cadastro.tm_sec = 0;
            Data_de_aniversario.tm_year = yearAniversario - 1900; 
            Data_de_aniversario.tm_mon = monthAniversario - 1;   
            Data_de_aniversario.tm_mday = dayAniversario;
            Data_de_aniversario.tm_hour = hourAniversario;
            Data_de_aniversario.tm_min = minAniversario;
            Data_de_aniversario.tm_sec = 0;
      }

    // Getters
    int getIDUsuario() const { return ID_usuario; }
    string getNome() const { return Nome; }
    string getSobrenome() const { return Sobrenome; }
    string getEndereco() const { return Endereco; }
    tm getDataCadastro() const { return Data_de_cadastro; }
    tm getDataAniversario() const { return Data_de_aniversario; }

    // Setters
    void setIDUsuario(int id) { ID_usuario = id; }
    void setNome(const string& nome) { Nome = nome; }
    void setSobrenome(const string& sobrenome) { Sobrenome = sobrenome; }
    void setEndereco(const string& endereco) { Endereco = endereco; }
    void setDataCadastro(const tm& data) { Data_de_cadastro = data; }
    void setDataAniversario(const tm& data) { Data_de_aniversario = data; }

    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowContaVerde::print();  
        cout << ", ID_usuario: " << ID_usuario 
             << ", Nome: " << Nome 
             << ", Sobrenome: " << Sobrenome 
             << ", Endereco: " << Endereco 
             << " Data_de_cadastro: " 
                  << (Data_de_cadastro.tm_year + 1900) << '-' 
                  << (Data_de_cadastro.tm_mon + 1) << '-' 
                  << Data_de_cadastro.tm_mday << ' '
                  << Data_de_cadastro.tm_hour << ':' 
                  << Data_de_cadastro.tm_min 
             << ", Data_de_aniversario: " 
                  << (Data_de_aniversario.tm_year + 1900) << '-' 
                  << (Data_de_aniversario.tm_mon + 1) << '-' 
                  << Data_de_aniversario.tm_mday << ' '
                  << Data_de_aniversario.tm_hour << ':' 
                  << Data_de_aniversario.tm_min<< endl;
    }
};

#endif // ROWCONTAVERDEUSUARIO_H
