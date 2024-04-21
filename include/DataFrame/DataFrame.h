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
#include <map>

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
    const vector<shared_ptr<Row>>& getRows() const{ return rows; }
    void setColumns(const vector<string>& newColumns) {
        columns = newColumns;
    }
    bool containsColumn(const string& name) const {
        return find(columns.begin(), columns.end(), name) != columns.end();
    }
    void setTypes(const vector<string>& newTypes) { 
        if (columns.size() != newTypes.size()) {
            throw invalid_argument("Number of column names must match number of column types.");
        }
        for (size_t i = 0; i < columns.size(); ++i) {
            types[columns[i]] = newTypes[i];
        }
    }
    void setRows(const vector<shared_ptr<Row>>& newRows) {
        rows = newRows;
    }
    size_t getRowSize() const {
        return rows.size();
    }
    size_t getColumnSize() const {
        return columns.size();
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
/*        try {
            if (!validateRow(*row)) {
                throw invalid_argument("Row is not compatible with DataFrame.");
            }
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
            return;
        }*/
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
        int start = max(0, static_cast<int>(rows.size()) - n);
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
    
    void innerJoin(DataFrame& other, const string& joinColumn) {
        if (!containsColumn(joinColumn) || !other.containsColumn(joinColumn)) {
            throw invalid_argument("Join column must be present in both DataFrames.");
        }

        vector<int> rowsToRemove;

        for (int i = 0; i < static_cast<int>(rows.size()); ++i) {
            bool matchFound = false;
            for (const auto& otherRow : other.rows) {
                try {
                    // Assuming mergeRows checks for join column match and merges the rows if they match
                    if (rows[i]->mergeRows(*otherRow, joinColumn)) {
                        matchFound = true;
                        break;
                    }
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
            if (!matchFound) {
                rowsToRemove.push_back(i);
            }
        }

        // Remove rows that did not find a match
        // Must be done in reverse to avoid invalidating indices
        for (auto it = rowsToRemove.rbegin(); it != rowsToRemove.rend(); ++it) {
            removeRow(*it);
        }
    }
    
    DataFrame groupByFreq(const string& groupColumn) {
        if (!containsColumn(groupColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }

        // Maps to store sums, keyed by the group column's value
        map<RowVariant, double> sumMap;

        // Iterate over rows to accumulate sums
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);  // Corrected function name to match your usage
            double sumValue = 1;  // This assumes we are just counting occurrences for now

            if (sumMap.find(groupValue) != sumMap.end()) {
                sumMap[groupValue] += sumValue;
            } else {
                sumMap[groupValue] = sumValue;
            }
        }
        // Creating a new DataFrame to hold the result
        
        vector<string> columnNames = {groupColumn, "Sum"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);
        // Fill the new DataFrame with the results from the sumMap
        for (const auto& [key, value] : sumMap) {
            vector<RowVariant> rowData = {key, value};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow);  // Ensure this method correctly handles new rows
        }

        return result;
    }
    
    DataFrame groupBySum(const string& groupColumn, const string& sumColumn) {
        if (!containsColumn(groupColumn) || !containsColumn(sumColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }

        // Map para armazenar as somas, chaveado pelo valor da coluna de agrupamento
        map<RowVariant, double> sumMap;

        // Iterar sobre as linhas para acumular as somas
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);
            double valueToAdd = get<double>(row->getCol_(sumColumn)); // Assumindo que a coluna é do tipo int

            if (sumMap.find(groupValue) != sumMap.end()) {
                sumMap[groupValue] += valueToAdd;
            } else {
                sumMap[groupValue] = valueToAdd;
            }
        }

        // Criando um novo DataFrame para conter o resultado
        vector<string> columnNames = {groupColumn, "Sum"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);

        // Preencher o novo DataFrame com os resultados de sumMap
        for (const auto& [key, value] : sumMap) {
            vector<RowVariant> rowData = {key, value};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow);
        }

        return result;
    }

    
    DataFrame groupByAvg(const string& groupColumn, const string& avgColumn) {
        if (!containsColumn(groupColumn) || !containsColumn(avgColumn)) {
            throw invalid_argument("Both columns must exist in the DataFrame.");
        }

        // Maps to store sums and counts, keyed by the group column's value
        
        map<RowVariant, pair<double, int>> avgMap; // First element is sum, second is count
        
        // Iterate over rows to accumulate sums and counts
        for (const auto& row : rows) {
            RowVariant groupValue = row->getCol_(groupColumn);
            double valueToAdd = get<double>(row->getCol_(avgColumn)); // Assuming the column is int type


            if (avgMap.find(groupValue) != avgMap.end()) {
                avgMap[groupValue].first += valueToAdd;
                avgMap[groupValue].second += 1;
            } else {
                avgMap[groupValue] = make_pair(valueToAdd, 1);
            }
        }

        // Creating a new DataFrame to hold the result
        vector<string> columnNames = {groupColumn, "Average"};
        vector<string> columnTypes = {getTypes().at(groupColumn), "double"};
        DataFrame result(columnNames, columnTypes);

        // Fill the new DataFrame with the results from avgMap
        for (const auto& [key, value] : avgMap) {
            double average = static_cast<double>(value.first) / value.second;
            vector<RowVariant> rowData = {key, average};
            shared_ptr<Row> newRow = make_shared<Row>(columnNames, rowData);
            result.insertRow(newRow);
        }

        return result;
    }


    

};

#endif // DATAFRAME_H