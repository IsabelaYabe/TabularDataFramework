#include "RepositorySaver/RepositorySaver.hpp"
#include <sstream>


RepositorySaver::RepositorySaver(Queue<DataFrame>& queue, const std::string& dbPath, const std::string& table)
    : inputQueue(queue), tableName(table) {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        throw std::runtime_error("Failed to open the database.");
    }
}

RepositorySaver::~RepositorySaver() {
    sqlite3_close(db);
}

std::string variantToSQLString(const RowVariant& variant) {
    return std::visit([](auto&& arg) -> std::string {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float> || std::is_same_v<T, double>) {
            return std::to_string(arg);
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "'" + arg + "'";
        } else {
            return "'" + std::to_string(arg) + "'";
        }
    }, variant);
}

std::string RepositorySaver::buildInsertQuery(const DataFrame& df) const {
    if (df.getRows().empty()) {
        throw std::runtime_error("DataFrame is empty, no data to insert.");
    }

    std::stringstream sql;
    sql << "INSERT INTO " << tableName << " (";

    const auto& columns = df.getColumns();
    for (const auto& col : columns) {
        sql << col << ", ";
    }
    sql.seekp(-2, std::ios_base::end);
    sql << ") VALUES ";

    for (const auto& rowPtr : df.getRows()) {
        sql << "(";
        for (const auto& col : columns) {
            auto valueVariant = rowPtr->getCol(col);
            sql << variantToSQLString(valueVariant);
            sql << ", ";
        }
        sql.seekp(-2, std::ios_base::end);
        sql << "), ";
    }
    sql.seekp(-2, std::ios_base::end);
    sql << ";";
    
    return sql.str();
}
