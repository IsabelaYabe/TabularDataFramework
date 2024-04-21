#include <iostream>
#include <vector>
#include <memory>
#include "DataFrame.h"
#include "Row.h"
#include "RepositorySaver.h"

using namespace std;

int main() {
    cout << "Iniciando teste do Repository Saver sem fila" << endl;

    // Define column names and types for the DataFrame
    vector<string> colNames = {"idproduto", "quantidade"};
    vector<string> colTypes = {"int", "int"};

    // Create a DataFrame with specified columns and types
    DataFrame df(colNames, colTypes);

    // Create and add rows with data to the DataFrame
    auto row1 = make_shared<Row>(1);
    row1->addColRow("idproduto", 101);
    row1->addColRow("quantidade", 5);

    auto row2 = make_shared<Row>(2);
    row2->addColRow("idproduto", 102);
    row2->addColRow("quantidade", 10);

    df.insertRow(row1);
    df.insertRow(row2);

    // Setting up the RepositorySaver
    string dbPath = "path_to_your_database.db";
    string tableName = "your_table_name";
    RepositorySaver saver(dbPath, tableName);

    // Building the insert query using the Repository Saver
    try {
        string sqlQuery = saver.buildInsertQuery(df);
        cout << "Generated SQL Query: " << endl;
        cout << sqlQuery << endl;
        
        // Here you would normally execute the query against the SQLite database
        // This example just prints the query for demonstration purposes
    } catch (const std::exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
