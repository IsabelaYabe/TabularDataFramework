#include <iostream>
#include "DataFrame.h"


void sep(int n=100) {
    std::cout << std::string(n, '-') << std::endl;
};


int main() {
    DataFrame df;

    std::cout << "DataFrame inicial:" << std::endl;
    df.printData();
    std::cout << "Shape: " << std::endl;
    std::cout << "Num linhas: " << df.getShape().first << std::endl;
    std::cout << "Num colunas: " << df.getShape().second << std::endl;
    
    
    sep();
    // Adiciona colunas iniciais ao DataFrame
    df.addColumn("Age", std::vector<int>{25, 30, 35});
    df.addColumn("Name", std::vector<std::string>{"Alice", "Bob", "Cathy"});
    df.addColumn("Salary", std::vector<float>{50000.0, 60000.0, 70000.0});

    std::cout << "DataFrame inicial com linhas e colunas:" << std::endl;
    df.printData();
    std::cout << "Columns " << std::endl;
    for (auto col : df.getColumns()) {
        std::cout << col << std::endl;
    }
    std::cout << "Shape: " << std::endl;
    std::cout << "Num linhas: " << df.getShape().first << std::endl;
    std::cout << "Num colunas: " << df.getShape().second << std::endl;

    sep();
    std::cout << "Modificando coluna coluna do DataFrame:" << std::endl;
    // Altera os valores da coluna "Salary"
    df.modifyColumn("Salary", std::vector<float>{52000.0, 62000.0, 72000.0, 82000.0});
    df.printData();

    sep();
    std::cout << "Imprimindo coluna do DataFrame:" << std::endl;
    // Recupera e imprime os valores da coluna "Name"
    std::vector<float> names = df.getColumn("Name");
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    sep();
    std::cout << "Modificando linha do DataFrame:" << std::endl;
    // Modifica os valores de uma linha específica (index 1)
    std::unordered_map<std::string, std::any> modified_row = {{"Name", "Robert"}, {"Salary", 61000.0}};
    df.modifyRow(1, modified_row);
    df.printData();

    sep();
    std::cout << "Adicionando linha ao DataFrame:" << std::endl;
    // Adiciona uma nova linha ao DataFrame
    std::unordered_map<std::string, std::any> new_row = {{"Age", 40}, {"Name", "David"}, {"Salary", 80000.0}};
    df.addRow(new_row);
    df.printData();

    sep();
    std::cout << "Removendo coluna age" << std::endl;
    // Removendo a coluna "Age"
    df.removeColumn("Age");

    // Imprime o DataFrame após remover a coluna "Age"
    std::cout << "\nDataFrame Após Remover a Coluna 'Age':" << std::endl;
    df.printData();

    df.addColumn("Age", std::vector<int>{25, 30, 35});

    sep();
    std::cout << "Removendo linha do DataFrame pelo index 1:" << std::endl;
    // Remove uma linha específica (index 1)
    df.removeRow(1);
    df.printData();

    sep();
    std::cout << "Acessando linha index 1: " << std::endl;
    // Acessando a segunda linha do DataFrame
    std::unordered_map<std::string, std::any> row = df.getRow(1);

    // Imprimindo os dados da linha acessada
    std::cout << "\nDados da Segunda Linha:" << std::endl;
    for (const auto& [key, value] : row) {
        std::cout << key << ": ";
        if (value.type() == typeid(int)) {
            std::cout << std::any_cast<int>(value) << std::endl;
        } else if (value.type() == typeid(float)) {
            std::cout << std::any_cast<float>(value) << std::endl;
        } else if (value.type() == typeid(std::string)) {
            std::cout << std::any_cast<std::string>(value) << std::endl;
        }
    }

    sep();
    std::cout << "Acessando coluna Salary:" << std::endl;
        // Acessando a coluna "Salary"
    std::vector<float> salaries = df.getColumn("Salary");

    // Imprimindo os valores da coluna "Salary"
    std::cout << "\nValores da Coluna 'Salary':" << std::endl;
    for (const float& salary : salaries) {
        std::cout << salary << " ";
    }
    std::cout << std::endl;

    std::cout << "Acessando o valor da coluna Salary e linha de indice 2:" << std:: endl;
        // Acessando o valor na coluna "Salary" e linha de índice 2
    std::any salaryValue = df.at("Salary", 2);

    // Imprimindo o valor acessado
    std::cout << "\nSalário da Terceira Pessoa: ";
    if (salaryValue.type() == typeid(float)) {
        std::cout << std::any_cast<float>(salaryValue);
    } else {
        std::cout << "Tipo inesperado de dados!";
    }
    std::cout << std::endl;

    std::cout << "Contando valores nulos em coluna:" << std::endl;
    DataFrame df3;

    // Adicionando colunas com dados ao DataFrame
    df3.addColumn("Idade", std::vector<std::any>{25, std::any{}, 35, 40});
    df3.addColumn("Nome", std::vector<std::string>{"Alice", "Bob", "Cathy", "David"});
    df3.addColumn("Salario", std::vector<std::any>{50000.0, 60000.0, std::any{}, 80000.0});

    // Imprime o DataFrame original
    std::cout << "DataFrame Original:" << std::endl;
    df.printData();

    // Contando elementos não-nulos na coluna "Age"
    int nonNullAge = df.countNonNull("Idade");
    std::cout << "\nNúmero de Elementos Não-Nulos na Coluna 'Idade': " << nonNullAge << std::endl;

    // Contando elementos não-nulos na coluna "Salary"
    int nonNullSalary = df.countNonNull("Salario");
    std::cout << "Número de Elementos Não-Nulos na Coluna 'Salario': " << nonNullSalary << std::endl;
    
    sep();
    std::cout << "Obtém DataFrame da linha de índice 1 ao 3:" << std::endl;
    // Obtém um DataFrame que contém apenas as linhas de índice 1 a 3
    DataFrame subset = df.iloc(1, 3);
    subset.printData();

    sep();  
    std::cout << "Obtém DataFrame merge em nomes:" << std::endl;
    // Cria um segundo DataFrame e faz a junção com o primeiro usando a chave "Name"
    DataFrame df2;
    df2.addColumn("Name", std::vector<std::string>{"Alice", "Bob", "Eva"});
    df2.addColumn("City", std::vector<std::string>{"New York", "San Francisco", "Los Angeles"});
    DataFrame merged_df = df.merge(df2, "Name");
    merged_df.printData();

    std::cout << "GroupBySum e GroupByAVG:" << std::endl;
    DataFrame df4;

    // Adicionando dados ao DataFrame
    df4.addColumn("Department", std::vector<std::string>{"Sales", "Sales", "HR", "HR", "IT"});
    df4.addColumn("Salary", std::vector<float>{50000, 60000, 30000, 35000, 70000});

    // Colunas para agregar
    std::vector<std::string> aggCols{"Salary"};

    // Aplicando groupByAvg
    //DataFrame avgDf4 = df4.groupByAvg("Department", aggCols);

    // Imprimindo o resultado
    std::cout << "Resultado do GroupBy Avg:" << std::endl;
    //avgDf4.printData();

    // Aplicando groupBySUm
    //DataFrame sumDf4 = df4.groupBySum("Department", aggCols);

    return 0;  
}