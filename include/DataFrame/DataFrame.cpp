#include "DataFrame.h"
#include <stdexcept>

DataFrame::DataFrame() : shape(0, 0), nRows(0) {}


void DataFrame::updateShape() {
    int cols = data.size();
    int rows = 0;
    if (!data.empty()) {
        auto it = data.begin();
        rows = it->second.size();
    }
    setShape(rows, cols);
}

void DataFrame::setShape(int rows, int cols) {
    shape = std::make_pair(rows, cols);
    nRows = rows;
}

std::pair<int, int> DataFrame::getShape() {
    return shape;
}

std::vector<std::string> DataFrame::getColumns(){
    return column_order;
}

template<typename T>
void DataFrame::addColumn(std::string name, std::vector<T>& col_data) {
    if (data.empty()) {
        nRows = col_data.size();
    } else if (col_data.size() != nRows) {
        throw std::length_error("All columns must have the same number of elements");
    }
    std::vector<std::any> new_col(col_data.begin(), col_data.end());
    data[name] = new_col;
    columnsTypes[name] = &typeid(T);
    column_order.push_back(name);
    updateShape();
}

void DataFrame::removeColumn( std::string name) {
    if (data.find(name) == data.end()) {
        throw std::out_of_range("Column not found.");
    }
    data.erase(name);
    column_order.erase(std::remove(column_order.begin(), column_order.end(), name), column_order.end());
    columnsTypes.erase(name);
    updateShape();
}

template <typename T>
void DataFrame::modifyColumn( std::string name,  std::vector<T>& new_values) {
    if (new_values.size() != shape.fisrt()) {
        std::cerr << "Error: The number of elements in new_values does not match the DataFrame's row count." << std::endl;
    } else {
        try {
            DataFrame.modifyColumn<int>("columnName", new_values);
        } catch ( std::exception& e) {
            std::cerr << "Exception occurred: " << e.what() << std::endl;
        }
    }
}

// Adiciona uma linha ao DataFrame
void DataFrame::addRow(std::unordered_map<std::string, std::any>& new_row) {
    for ( auto& col : column_order) {
        if (new_row.find(col) == new_row.end()) {
            throw std::invalid_argument("Missing value for column " + col);
        }
        data[col].push_back(new_row.at(col));
    }
    updateShape();
}

// Remove uma linha pelo índice
void DataFrame::removeRow(int index) {
    if (index < 0 || index >= nRows) {
        throw std::out_of_range("Row index out of range.");
    }
    for (auto& [key, vec] : data) {
        vec.erase(vec.begin() + index);
    }
    updateShape();
}

// Modifica uma linha pelo índice
void DataFrame::modifyRow(int index, std::unordered_map<std::string, std::any>& new_values) {
    if (index < 0 || index >= nRows) {
        throw std::out_of_range("Row index out of range.");
    }
    for ( auto& [key, value] : new_values) {
        if (data.find(key) == data.end()) {
            throw std::invalid_argument("Column " + key + " does not exist.");
        }
        data[key][index] = value;
    }
}

// Recupera uma linha do DataFrame pelo índice
std::unordered_map<std::string, std::any> DataFrame::getRow(int index) {
    if (index < 0 || index >= nRows) {
        throw std::out_of_range("Row index out of range.");
    }
    std::unordered_map<std::string, std::any> row;
    for ( std::string& col_name : column_order) {
        row[col_name] = data[col_name][index];
    }
    return row;
}

    
// Acessar dados de uma coluna específica
template<typename T>
std::vector<T> DataFrame::getColumn(std::string name) {
    return data[name];
}

// Retorna o valor na posição especificada
std::any DataFrame::at( std::string column, int index) {
    if (data.find(column) == data.end() || index >= data[column].size()) {
        throw std::out_of_range("Column name or index out of range.");
    }
    return data[column][index];
}

// Exemplo de função para contar os elementos não nulos em uma coluna
int DataFrame::countNonNull( std::string column) {
    int count = 0;
    for ( auto& item : data[column]) {
        if (item.has_value()) {
            count++;
        }
    }
    return count;
}

// Implementação do iloc para intervalo de linhas
DataFrame DataFrame::iloc(int start, int end) {
    if (start < 0 || end > nRows || start > end) {
        throw std::out_of_range("Index out of range.");
    }
    DataFrame result;
    for ( auto& col_name : column_order) {
        std::vector<std::any> col_slice(data[col_name].begin() + start, data[col_name].begin() + end);
        result.data[col_name] = col_slice;
        result.column_order.push_back(col_name);
        result.columnsTypes[col_name] = columnsTypes[col_name];
    }
    result.updateShape();
    return result;
}

// Imprimir os dados para depuração
void DataFrame::printData() {
    for ( auto& col_name : column_order) {
         auto& col_data = data[col_name];
        std::cout << col_name << ": ";
        for ( auto& val : col_data) {
            if (val.type() == typeid(int)) {
                std::cout << std::any_cast<int>(val) << " ";
            } else if (val.type() == typeid(float)) {
                std::cout << std::any_cast<float>(val) << " ";
            } else if (val.type() == typeid(std::string)) {
                std::cout << std::any_cast<std::string>(val) << " ";
            }
        }
        std::cout << std::endl;
    }
}

// Retorna um novo DataFrame que é o resultado da junção inner entre este DataFrame e outro
DataFrame DataFrame::merge( DataFrame& other,  std::string key) {
    if (data.find(key) == data.end() || other.data.find(key) == other.data.end()) {
        throw std::invalid_argument("Key column not found in one or both DataFrames.");
    }
    DataFrame result;
    std::unordered_map<std::any, std::vector<size_t>, AnyHash, AnyEqual> key_map_this;
    std::unordered_map<std::any, std::vector<size_t>, AnyHash, AnyEqual> key_map_other;
    // Indexando as posições das chaves no primeiro DataFrame
    for (size_t i = 0; i < data[key].size(); i++) {
        key_map_this[data[key][i]].push_back(i);
    }
    // Indexando as posições das chaves no segundo DataFrame
    for (size_t i = 0; i < other.data.at(key).size(); i++) {
        key_map_other[other.data.at(key)[i]].push_back(i);
    }
    // Construindo o novo DataFrame a partir das junções das chaves
    for ( auto& [key_val, positions_this] : key_map_this) {
        if (key_map_other.find(key_val) != key_map_other.end()) {
             auto& positions_other = key_map_other[key_val];
            for (auto pos_this : positions_this) {
                for (auto pos_other : positions_other) {
                    // Adicionando linhas do primeiro DataFrame
                    for ( auto& col : column_order) {
                        result.data[col].push_back(data[col][pos_this]);
                    }
                    // Adicionando linhas do segundo DataFrame
                    for ( auto& col : other.column_order) {
                        if (col != key) {
                            result.data[col].push_back(other.data.at(col)[pos_other]);
                        }
                    }
                }
            }
        }
    }
    // Atualizando ordem das colunas e tipos no novo DataFrame
    result.column_order = column_order;
    for ( auto& col : other.column_order) {
        if (col != key && std::find(column_order.begin(), column_order.end(), col) == column_order.end()) {
            result.column_order.push_back(col);
        }
    }
    result.columnsTypes = columnsTypes; // Assume that both DataFrames have compatible types
    for ( auto& [col, type] : other.columnsTypes) {
        if (result.columnsTypes.find(col) == result.columnsTypes.end()) {
            result.columnsTypes[col] = type;
        }
    }
    result.updateShape();
    return result;
}

/***    
DataFrame DataFrame::groupBySum( std::string& key,  std::vector<std::string>& aggCols = {}) {
    if (data.find(key) == data.end()) {
        throw std::invalid_argument("Key column not found in DataFrame.");
    }
    // Mapeia cada valor único na coluna-chave para as linhas correspondentes
    std::unordered_map<std::any, std::vector<size_t>, AnyHash, AnyEqual> group_map;
    for (size_t i = 0; i < data[key].size(); ++i) {
        group_map[data[key][i]].push_back(i);
    }
    // Criar um novo DataFrame para armazenar os resultados
    DataFrame result;
    result.column_order.push_back(key); // Adiciona a coluna chave ao novo DataFrame
    result.columnsTypes[key] = &typeid(std::any_cast< std::vector<std::any>&>(data[key]).front());
    // Selecionando colunas para agregar, com base em aggCols ou todas as colunas numéricas
    for ( auto& col : (aggCols.empty() ? column_order : aggCols)) {
        if (col != key && (columnsTypes[col] == &typeid(int) || columnsTypes[col] == &typeid(float))) {
            result.data[col] = std::vector<std::any>();
            result.column_order.push_back(col);
            result.columnsTypes[col] = columnsTypes[col];
        }
    }
    // Aplica a função de agregação para cada grupo em cada coluna agregável
    for ( auto& [group_key, indices] : group_map) {
        result.data[key].push_back(group_key); // Adiciona o valor do grupo
        for ( auto& col : result.column_order) {
            if (col != key) {
                std::vector<double> values;
                for (auto idx : indices) {
                    values.push_back(std::any_cast<double>(data[col][idx]));
                }
                double sum = std::accumulate(values.begin(), values.end(), 0.0);
                result.data[col].push_back(sum); // Aplica a função de soma
            }
        }
    }
    result.updateShape(); // Atualiza o shape do novo DataFrame
    return result;
}

DataFrame DataFrame::groupByAvg( std::string& key,  std::vector<std::string>& aggCols = {}) {
    if (data.find(key) == data.end()) {
        throw std::invalid_argument("Key column not found in DataFrame.");
    }
    // Mapeia cada valor único na coluna-chave para as linhas correspondentes
    std::unordered_map<std::any, std::vector<size_t>, AnyHash, AnyEqual> group_map;
    for (size_t i = 0; i < data[key].size(); ++i) {
        group_map[data[key][i]].push_back(i);
    }
    // Criar um novo DataFrame para armazenar os resultados
    DataFrame result;
    result.column_order.push_back(key); // Adiciona a coluna chave ao novo DataFrame
    result.columnsTypes[key] = &typeid(std::any_cast< std::vector<std::any>&>(data[key]).front());
    for ( auto& col : (aggCols.empty() ? column_order : aggCols)) {
        if (col != key && (columnsTypes[col] == &typeid(int) || columnsTypes[col] == &typeid(float))) {
            result.data[col] = std::vector<std::any>();
            result.column_order.push_back(col);
            result.columnsTypes[col] = columnsTypes[col];
        }
    }
    // Aplica a função de agregação para cada grupo em cada coluna agregável
    for ( auto& [group_key, indices] : group_map) {
        result.data[key].push_back(group_key); // Adiciona o valor do grupo
        for ( auto& col : result.column_order) {
            if (col != key) {
                std::vector<double> values;
                for (auto idx : indices) {
                    values.push_back(std::any_cast<double>(data[col][idx]));
                }
                double sum = std::accumulate(values.begin(), values.end(), 0.0);
                double mean = sum / values.size();
                result.data[col].push_back(mean);
            }
        }
    }
    result.updateShape(); // Atualiza o shape do novo DataFrame
    return result;
}
***/
