#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

using namespace std;

using namespace std;

// Classe que representa uma linha do DataFrame
template<typename R>
class Row {
public:
    // Construtor com métodos virtuais
    virtual R* create() const = 0;
    // Destrutor
    virtual ~Row() = default;
};

// Instância da classe Row
class Row1: public Row<Row1> {
public:
    // Atributos
    int id;
    string name;
    // Construtor
    Row1(int id, string name): id(id), name(name) {}
    // Destrutor
    ~Row1() = default;
    // Método para criar uma nova instância de Row1
    Row1* create() const override {
        return new Row1(id, name);
    }
}; 

// Instância da classe Row
class Row2: public Row<Row2> {
public:
    // Atributos
    int id;
    string product_name;
    float price;
    // Construtor
    Row2(int id, string product_name, float price): id(id), product_name(product_name), price(price) {}
    // Destrutor
    ~Row2() = default;
    // Método para criar uma nova instância de Row2
    Row2* create() const override {
        return new Row2(id, product_name, price);
    }
}; 
#include <vector>

// Classe que representa o DataFrame
template<typename R>
class DataFrame {
private:
    std::vector<R> Rows; // Vetor de linhas
    size_t num_Rows = 0; // Número de linhas

public:
    // Adiciona uma linha ao DataFrame
    void addRow(const R& newRow) {
        Rows.push_back(newRow);
        num_Rows++;
    }

    // Retorna o número de linhas no DataFrame
    size_t size() const {
        return num_Rows;
    }

    // Retorna as linhas do DataFrame
    std::vector<R> getRows() const {
        return Rows;
    }
};


int main() {
    // Criar instância da classe DataFrame para linhas do tipo Row1
    DataFrame<Row1> df1;

    // Adicionar algumas linhas ao DataFrame
    df1.addRow(Row1(1, "John"));
    df1.addRow(Row1(2, "Alice"));

    // Exibir o número de linhas no DataFrame
    cout << "DataFrame 1 size: " << df1.size() << endl;
    cout<< df1.getRows()[0].name << endl;
    cout<< df1.getRows()[1].name << endl;


    // Criar instância da classe DataFrame para linhas do tipo Row2
    DataFrame<Row2> df2;

    // Adicionar algumas linhas ao DataFrame
    df2.addRow(Row2(1, "Product A", 10.99f));
    df2.addRow(Row2(2, "Product B", 20.49f));
    df2.addRow(Row2(3, "Product C", 15.79f));

    // Exibir o número de linhas no DataFrame
    cout << "DataFrame 2 size: " << df2.size() << endl;
    cout<< df2.getRows()[0].product_name << endl;
    cout<< df2.getRows()[1].product_name << endl;
    cout<< df2.getRows()[2].product_name << endl;

    return 0;
}
