#ifndef ROWDCFALHAS_H
#define ROWDCFALHAS_H

#include "RowDataCat.h"  

class RowDCFalhas : public RowDataCat {
private:
    string Componente_alvo;
    string Arquivo_codigo;
    string Gravidade;

public:
    // Construtor 
    RowDCFalhas(int idEvent, const string& dataNotificacao, const string& tipoDeLog,
                const string& conteudoTextual, const string& componenteAlvo, 
                const string& arquivoCodigo, const string& gravidade)
        : RowDataCat(6, idEvent, dataNotificacao, tipoDeLog, conteudoTextual),
          Componente_alvo(componenteAlvo), Arquivo_codigo(arquivoCodigo), Gravidade(gravidade) {}
    
    // Getters
    string getComponenteAlvo() const { return Componente_alvo; }
    string getArquivoCodigo() const { return Arquivo_codigo; }
    string getGravidade() const { return Gravidade; }

    // Setters
    void setComponenteAlvo(const string& componenteAlvo) { Componente_alvo = componenteAlvo; }
    void setArquivoCodigo(const string& arquivoCodigo) { Arquivo_codigo = arquivoCodigo; }
    void setGravidade(const string& gravidade) { Gravidade = gravidade; }
    
    // Sobrescreve o método print para incluir informações adicionais
    void print() const override {
        RowDataCat::print();  
        cout << ", Componente_alvo: " << Componente_alvo
             << ", Arquivo_codigo: " << Arquivo_codigo
             << ", Gravidade: " << Gravidade << endl;
    }
};

#endif // ROWDCFALHAS_H