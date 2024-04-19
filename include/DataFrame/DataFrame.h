#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <any>
#include <stdexcept>
#include <typeinfo>
#include <ctime> 
#include <numeric> 
#include <functional>
#include <utility>  
#include <type_traits>
#include <typeinfo>
#include <memory>
#include <algorithm>

using namespace std;

#include "Row.h"

using namespace std;

class DataFrame {
private:
    vector<string> columns;  // Vector of strings to store column names
    unordered_map<string, string> types;  // Map to store column names and their data types
    vector<shared_ptr<Row>> rows;  // Vector of smart pointers to Row objects

public:
    // Default constructor
    DataFrame() {}

    // Constructor to initialize with column names and types
    DataFrame(const vector<string>& colNames, const vector<string>& colTypes) {
        if (colNames.size() != colTypes.size()) {
            throw invalid_argument("Number of column names must match number of column types.");
        }

        columns = colNames;
        for (size_t i = 0; i < columns.size(); ++i) {
            types[columns[i]] = colTypes[i];
        }
    }

    const vector<string>& getColumns() const { return columns; }
    const unordered_map<string, string>& getTypes() const { return types; }
    const vector<shared_ptr<Row>>& getRows() const { return rows; }
    void setColumns(const vector<string>& newColumns) {
        columns = newColumns;
    }
    void setTypes(const unordered_map<string, string>& newTypes) { types = newTypes;}
        void setRows(const vector<shared_ptr<Row>>& newRows) {
        rows = newRows;
    }
    bool validateRow(const Row& row) const {
        // Verifica se todos os campos da Row estão nas colunas do DataFrame e se os tipos são compatíveis
        const auto& row_data = row.getData(); // Supõe que há um método getData() que retorna a map da Row

        for (const auto& [key, value] : row_data) {
            auto col_it = types.find(key);
            if (col_it == types.end()) {
                cout << "Column '" << key << "' not found in DataFrame." << endl;
                return false;
            }

            // Verifica o tipo usando type_index
            const type_index typeIndexDataframe(typeid(value));
            const type_index typeIndexRow(typeid(decltype(value))); // Precisa de modificação conforme a implementação real

            if (typeIndexDataframe != typeIndexRow) {
                cout << "Type mismatch for column '" << key << "'. Expected " << col_it->second << " but found " << typeid(value).name() << endl;
                return false;
            }
        }

        return true;
    }
    // Method to add a row
    void insertRow(const shared_ptr<Row>& row) {
        try {
            if (!validateRow(*row)) {
                throw invalid_argument("Row is not compatible with DataFrame.");
            }
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            return;
        }
        rows.push_back(row);
    }
    
    void removeRow(int index) {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        rows.erase(rows.begin() + index);
    }

    const shared_ptr<Row>& getRow(int index) const {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        return rows[index];
    }

    // Method to get the number of rows
    size_t getRowCount() const {
        return rows.size();
    }

    // Method to get the number of columns
    size_t getColumnCount() const {
        return columns.size();
    }
    void merge(const DataFrame& other) {
        if (columns != other.columns || types != other.types) {
            throw invalid_argument("DataFrames have different columns or types and cannot be merged.");
        }
        rows.insert(rows.end(), other.rows.begin(), other.rows.end());
    }
    void head(int n) const {
        if (n > static_cast<int>(rows.size())) {
            n = rows.size(); // Limita n ao tamanho do vetor se for maior que o número de linhas
        }
        for (int i = 0; i < n; i++) {
            rows[i]->printRow();
        }
    }
    void tail(int n) const {
        int start = std::max(0, static_cast<int>(rows.size()) - n);
        for (int i = start; i < static_cast<int>(rows.size()); i++) {
            rows[i]->printRow();
        }
    }
    void printTypes() const {
        if (!rows.empty()) {
            rows[0]->printTypes(); // Correctly calling a method that should exist in `Row`
        }
    }
    // Method to print DataFrame details
    void printDataFrame() const {
        for (const auto& col : columns) {
            cout << col << " ";
        }
        cout << endl;

        for (const auto& row : rows) {
            row->printRow();
        }
    }
};

#endif // DATAFRAME_H