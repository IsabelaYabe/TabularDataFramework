#ifndef ROW_H
#define ROW_H

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
#include <variant>
#include <typeindex>
#include <iomanip>

using namespace std;

bool operator!=(const tm& lhs, const tm& rhs) {
    return !(lhs.tm_sec == rhs.tm_sec &&
             lhs.tm_min == rhs.tm_min &&
             lhs.tm_hour == rhs.tm_hour &&
             lhs.tm_mday == rhs.tm_mday &&
             lhs.tm_mon == rhs.tm_mon &&
             lhs.tm_year == rhs.tm_year &&
             lhs.tm_wday == rhs.tm_wday &&
             lhs.tm_yday == rhs.tm_yday &&
             lhs.tm_isdst == rhs.tm_isdst);
}
using RowVariant = variant<int, double, tm, string, vector<int>>;

class Row {
    private:
        unordered_map<string, RowVariant> row;
        int ID;
    public:
        Row(int& id) : ID(id) {}

        
        void addColRow(const string& name, const RowVariant& value) {
                row[name] = value;
        }

        size_t size(){
            return row.size();
        }

        void removeCol(const string& name) {
            auto it = row.find(name);
            if (it == row.end()) {
                throw invalid_argument("Column not found");
            }
            row.erase(it);
        }

        RowVariant& getCol(const string& name) {
                auto it = row.find(name);
                if (it == row.end()) {
                    throw invalid_argument("Column not found");
                }
                return it->second;
            }

        const unordered_map<string, RowVariant>& getData() const {
            return row;
        }
        void printTypes() const {
            for (const auto& col : row) {
                cout << col.first << " type: " << visit([](auto&& arg) -> string {
                    return typeid(decltype(arg)).name();
                }, col.second) << endl;
            }
        }
        void printRow() const {
            for (const auto& [key, value] : row) {
                cout << key << ": ";
                visit([](auto&& arg) {
                    using T = decay_t<decltype(arg)>;
                    if constexpr (is_same_v<T, int> || is_same_v<T, double> || is_same_v<T, vector<int>>) {
                        if constexpr (is_same_v<T, vector<int>>) {
                            for (int v : arg) cout << v << " ";
                        } else {
                            cout << arg;
                        }
                    } else if constexpr (is_same_v<T, string>) {
                        cout << '"' << arg << '"';
                    } else if constexpr (is_same_v<T, tm>) {
                        cout << put_time(&arg, "%c");
                    }
                    cout << " ";
                }, value);
            }
            cout << endl;
        }

        string getTypeCol(const string& name) {
            auto it = row.find(name);
            if (it == row.end()) {
                throw invalid_argument("Column not found");
            }
            return visit([](auto&& arg) -> string {
                using T = decay_t<decltype(arg)>;
                return typeid(T).name();
            }, it->second);
        }

        void setID(int id) {
            ID = id;
        }

        int getID() const {
            return ID;
        }    
        ///////////////////////////////////////////////////
        //////////////////////////////////////////////////
        // Method to check if the Row contains a column.
    bool containsColumn(const string& name) const {
        return row.find(name) != row.end();
    }

    RowVariant getCol_(const string& name) const {
        auto it = row.find(name);
        if (it == row.end()) {
            throw invalid_argument("Column not found");
        }
        return it->second;
    }

    // Method to merge two Rows based on a column.
    bool mergeRows(const Row& otherRow, const string& mergeColumn) {
        if (!containsColumn(mergeColumn) || !otherRow.containsColumn(mergeColumn)) {
            throw invalid_argument("Merge column not found in one or both rows");
        }
    
        if (getCol_(mergeColumn) != otherRow.getCol_(mergeColumn)) {
            throw invalid_argument("Merge column values do not match");
        }
        // Add columns from otherRow that are not in this row.
        for (const auto& [key, value] : otherRow.getData()) {
            if (!containsColumn(key) && !containsColumn(key)) {
                addColRow(key, value);
            }
        }
        return true;
    }
  

    };

#endif // ROW_H