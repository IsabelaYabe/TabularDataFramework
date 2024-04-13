#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <algorithm>

#include "Row.h"

using namespace std;

class DataFrame {
private:
    vector<int> index;  // Vetor de inteiros para armazenar os índices
    vector<string> columns;  // Vetor de strings para armazenar os nomes das colunas
    vector<string> types;  // Vetor de strings para armazenar os tipos das colunas
    pair<int, int> shape{0, 0};  // Par de inteiros para armazenar a forma do dataframe
    vector<shared_ptr<Row>> rows;  // Vetor de ponteiros inteligentes para as linhas

public:
    // Construtor
    DataFrame() {}

    // Getters
    const vector<int>& getIndex() const { return index; }
    const vector<string>& getColumns() const { return columns; }
    const vector<string>& getTypes() const { return types; }
    pair<int, int> getShape() const { return shape; }
    bool isEmpty() const { return rows.empty(); }
    const vector<shared_ptr<Row>>& getRows() const { return rows; }

    // Setters
    void setIndex(const vector<int>& newIndex) { index = newIndex; }
    void setColumns(const vector<string>& newColumns) {
        columns = newColumns;
        shape.second = newColumns.size();
    }
    void setTypes(const vector<string>& newTypes) { types = newTypes; }
    void setRows(const vector<shared_ptr<Row>>& newRows) {
        rows = newRows;
        shape.first = newRows.size();
    }

    // Métodos
    void insertRow(const shared_ptr<Row>& row) {
        rows.push_back(row);
        shape.first = rows.size();
    }

    void removeRow(int index) {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        rows.erase(rows.begin() + index);
        shape.first = rows.size();
    }

    void removeRowByIDEvent(int idEvent) {
    auto it = std::find_if(rows.begin(), rows.end(), [idEvent](const std::shared_ptr<Row>& row) {
        return row->getIDEvent() == idEvent;
    });

    if (it != rows.end()) {
        rows.erase(it);
        shape.first = rows.size(); // Atualiza o número de linhas
    } else {
        // Opcional: Lançar exceção ou imprimir uma mensagem de erro/notificação
        std::cerr << "IDEvent " << idEvent << " não encontrado no DataFrame." << std::endl;
    }
    }

    const shared_ptr<Row>& getRow(int index) const {
        if (index < 0 || index >= static_cast<int>(rows.size())) throw out_of_range("Index out of range");
        return rows[index];
    }

    void getRowIDEvent(int idEvent) const {
        for (const auto& row : rows) {
            if (row->getIDEvent() == idEvent) row->print();
        }
    }
    
    int getIndexByIDEvent(int idEvent) const {
        auto it = std::find_if(rows.begin(), rows.end(), [idEvent](const std::shared_ptr<Row>& row) {
            return row->getIDEvent() == idEvent;
        });

        if (it != rows.end()) {
            return std::distance(rows.begin(), it);
        } else {
            // Opcional: Lançar exceção ou imprimir uma mensagem de erro/notificação
            std::cerr << "IDEvent " << idEvent << " não encontrado no DataFrame." << std::endl;
            return -1;
        }
    }
    
    void merge(const DataFrame& other) {
    if (columns != other.columns || types != other.types) {
        throw invalid_argument("DataFrames have different columns or types and cannot be merged.");
    }
    rows.insert(rows.end(), other.rows.begin(), other.rows.end());
    shape.first = rows.size();
    }


    void head(int n) const {
    if (n > static_cast<int>(rows.size())) {
        n = rows.size(); // Limita n ao tamanho do vetor se for maior que o número de linhas
    }
    for (int i = 0; i < n; i++) {
        rows[i]->print();
    }
    }

    void tail(int n) const {
        int start = std::max(0, static_cast<int>(rows.size()) - n);
        for (int i = start; i < static_cast<int>(rows.size()); i++) {
            rows[i]->print();
        }
    }
};

#endif // DATAFRAME_H
