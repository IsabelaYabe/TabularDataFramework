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

using RowVariant = variant<int, double, string, tm, vector<int>>;

class Row {
    private:
        unordered_map<string, RowVariant> row;
        int ID;
    public:
        Row(int& id) : ID(id) {}

        template<typename T>
        void addColRow(const string& name, const T& value) {
            if constexpr (is_same_v<T, int> || is_same_v<T, double> || is_same_v<T, string> || is_same_v<T, tm> || is_same_v<T, vector<int>>) {
                row[name] = value;
            } else {
                throw invalid_argument("Type not supported");
            }
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

    };

#endif // ROW_H