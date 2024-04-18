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

struct AnyHash {
    std::size_t operator()( std::any& key)  {
        if (key.type() == typeid(int)) {
            return std::hash<int>()(std::any_cast<int>(key));
        } else if (key.type() == typeid(float)) {
            return std::hash<float>()(std::any_cast<float>(key));
        } else if (key.type() == typeid(std::string)) {
            return std::hash<std::string>()(std::any_cast<std::string>(key));
        } else if (key.type() == typeid(std::vector<int>)) {
             auto& vec = std::any_cast< std::vector<int>&>(key);
            return hash_vector_int(vec);
        } else if (key.type() == typeid(struct tm)) {
             auto& tm_val = std::any_cast< struct tm&>(key);
            return hash_tm(tm_val);
        } else {
            throw std::invalid_argument("Unsupported type for AnyHash");
        }
    }

private:
    std::size_t hash_vector_int( std::vector<int>& vec)  {
        std::size_t seed = 0;
        for (int i : vec) {
            seed ^= std::hash<int>()(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }

    std::size_t hash_tm( struct tm& tm_val)  {
        std::size_t h1 = std::hash<int>()(tm_val.tm_min);
        std::size_t h2 = std::hash<int>()(tm_val.tm_hour);
        std::size_t h3 = std::hash<int>()(tm_val.tm_mday);
        std::size_t h4 = std::hash<int>()(tm_val.tm_mon);
        std::size_t h5 = std::hash<int>()(tm_val.tm_year);
        return h1 ^ h2 ^ h3 ^ h4 ^ h5;
    }
};

struct AnyEqual {
    bool operator()( std::any& lhs,  std::any& rhs)  {
        if (lhs.type() != rhs.type()) {
            return false; // Elementos de tipos diferentes são sempre diferentes
        }

        if (lhs.type() == typeid(int)) {
            return std::any_cast<int>(lhs) == std::any_cast<int>(rhs);
        } else if (lhs.type() == typeid(float)) {
            return std::any_cast<float>(lhs) == std::any_cast<float>(rhs);
        } else if (lhs.type() == typeid(std::string)) {
            return std::any_cast<std::string>(lhs) == std::any_cast<std::string>(rhs);
        } else if (lhs.type() == typeid(std::vector<int>)) {
             auto& vec1 = std::any_cast< std::vector<int>&>(lhs);
             auto& vec2 = std::any_cast< std::vector<int>&>(rhs);
            return vec1 == vec2;
        } else if (lhs.type() == typeid(struct tm)) {
             auto& tm1 = std::any_cast< struct tm&>(lhs);
             auto& tm2 = std::any_cast< struct tm&>(rhs);
            return tm1.tm_min == tm2.tm_min &&
                   tm1.tm_hour == tm2.tm_hour &&
                   tm1.tm_mday == tm2.tm_mday &&
                   tm1.tm_mon == tm2.tm_mon &&
                   tm1.tm_year == tm2.tm_year;
        } else {
            throw std::invalid_argument("Unsupported type for AnyEqual");
        }
    }
};

class DataFrame {
private:
    std::unordered_map<std::string, std::vector<std::any>> data; // Armazena os dados em colunas
    std::unordered_map<std::string, std::type_info*> columnsTypes; // Tipos das colunas
    std::vector<std::string> column_order; // Ordem das colunas
    std::pair<int, int> shape; // Número de linhas e colunas
    int nRows; // Número de linhas

    void updateShape();

public:
    DataFrame();

    void setShape(int rows, int cols);

    std::pair<int, int> getShape();

    std::vector<std::string> getColumns();

    template <typename T>
    void addColumn(std::string name, std::vector<T>& col_data);

    void removeColumn(std::string name);

    template <typename T>
    void modifyColumn(std::string name, std::vector<T>& new_values);

    void addRow(std::unordered_map<std::string, std::any>& new_row);

    void removeRow(int index);

    void modifyRow(int index, std::unordered_map<std::string, std::any>& new_values);

    std::unordered_map<std::string, std::any> getRow(int index);

    template <typename T>
    std::vector<T> getColumn(std::string name);

    std::any at( std::string column, int index);

    int countNonNull( std::string column);

    DataFrame iloc(int start, int end);

    void printData();

    DataFrame merge( DataFrame& other,  std::string key);

    // DataFrame groupBySum( std::string& key,  std::vector<std::string>& aggCols = {});

    // DataFrame groupByAvg( std::string& key, std::vector<std::string>& aggCols = {});
};

#endif // DATAFRAME_H