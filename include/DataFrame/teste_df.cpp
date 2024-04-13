#include <iostream>
#include "DataFrame.h"
#include "RowCA.h"  // Supondo que essa classe está definida apropriadamente

void sep(int n=100) {
    cout << string(n, '-') << endl;
};

int main() {
    DataFrame df;

    // Definindo colunas e tipos (usualmente, isso seria dinâmico ou parte de um método de DataFrame)
    vector<string> columns = {"IDEvent", "Data", "UserID", "Stimulus", "TargetComponent"};
    vector<string> types = {"int", "string", "int", "string", "string"};
    df.setColumns(columns);
    df.setTypes(types);

    // Testando os métodos head e tail
    cout << "Testando inserir, getColumns, getTypes, getIndex, getShape, isEmpty, head e tail:" << endl;
    cout << "isEmpty: " << df.isEmpty() << endl;
        // Criando e adicionando algumas linhas
    auto row1 = make_shared<RowCA>(1, "2022-10-01", 101, "Teste1", "ComponentA");
    auto row2 = make_shared<RowCA>(2, "2022-10-02", 102, "Teste2", "ComponentB");
    auto row3 = make_shared<RowCA>(3, "2022-10-03", 103, "Teste3", "ComponentC");
    df.insertRow(row1);
    df.insertRow(row2);
    df.insertRow(row3);
    cout << "linhas inseridas" << endl;
    cout << "isEmpty: " << df.isEmpty() << endl;
    cout << "Indexs: " << endl;
    for (const auto& idx : df.getIndex()) cout << idx << endl;
    cout << "Shape: " << df.getShape().first << " x " << df.getShape().second << endl;  // Deve imprimir "3 x 5
    cout << "Colunas: "; 
    for (const auto& col : df.getColumns()) cout << col << endl;
    cout << "Tipos: ";
    for (const auto& type : df.getTypes()) cout << type << endl;
    cout << "Mostrando 3 primeiras linhas:" << endl;
    df.head(3);
    cout << "Mostrando 2 últimas linhas:" << endl;
    df.tail(2);
    sep();

    cout << "Testando removeRow e removebyIDEvent:" << endl;
    // Removendo a primeira linha do DataFrame
    try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com primeira linha removida" << endl;
    df.head(2);
        try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com a segunda linha removida" << endl;
    df.head(1);

    cout << "Removendo a linha com IDEvent = 3" << endl;
    df.removeRowByIDEvent(3);
    df.head(1);
    df.insertRow(row1);
    df.insertRow(row2);
    df.insertRow(row3);
    sep();
    cout << "Testando getRow, getRowIDEvent e getIndexByIDEvent:" << endl;
    cout<<  "Acessando a primeira linha do DataFrame " << endl;
    try {
        auto firstRow = df.getRow(0);
        firstRow->print();  // Supondo que o método print() está definido em Row
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    cout << "Acessando a linha com IDEvent = 2" << endl;
    df.getRowIDEvent(2);
    cout << "Index da linha com IDEvent = 2: " << df.getIndexByIDEvent(2) << endl;
    df.getIndexByIDEvent(2);
    sep();

    cout << "Criando df2" << endl;
    DataFrame df2;
    df2.setColumns(columns);
    df2.setTypes(types);
    // Adicionando dados ao df2
    df2.insertRow(make_shared<RowCA>(4, "2022-10-03", 103, "Teste3", "ComponentC"));
    df2.insertRow(make_shared<RowCA>(5, "2022-10-04", 104, "Teste4", "ComponentD"));

    try {
        df.merge(df2);
        std::cout << "Dados em df1 após o merge:" << std::endl;
        df.head(df.getShape().first);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro ao mesclar: " << e.what() << std::endl;
    }


    return 0;
}
