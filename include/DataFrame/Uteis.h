#ifndef UTEIS_H
#define UTEIS_H

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
#include "Row.h" 
#include "DataFrame.h"
#include "Time.h"
using namespace std;

bool isNumber(const std::string& str) {
    std::istringstream iss(str);
    double num;
    iss >> num;
    // Verifica se a leitura do número foi bem-sucedida e se alcançou o fim da string sem falhas.
    return iss.good() || (iss.eof() && !iss.fail());
}

bool isTimeFormat(const std::string& str) {
    // Verifica o tamanho da string para os dois formatos permitidos
    if (str.size() != 19 && str.size() != 10) return false;

    // Verifica o formato YYYY-MM-DD para os primeiros 10 caracteres
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) {
            // Espera-se que índices 4 e 7 sejam '-'
            if (str[i] != '-') return false;
        } else {
            // Espera-se que todos os outros sejam dígitos
            if (!isdigit(str[i])) return false;
        }
    }

    // Se a string tiver 10 caracteres, ela já está no formato correto
    if (str.size() == 10) return true;

    // Verifica o formato HH:MM:SS para os caracteres restantes
    for (int i = 11; i < 19; ++i) {
        if (i == 13 || i == 16) {
            // Espera-se que índices 13 e 16 sejam ':'
            if (str[i] != ':') return false;
        } else {
            // Espera-se que todos os outros sejam dígitos
            if (!isdigit(str[i])) return false;
        }
    }

    // A string corresponde ao formato de data e hora esperado
    return true;
}



vector<vector<string>> splitLogEntries(const string &log) {
    vector<vector<string>> entries;
    istringstream logStream(log);
    string line;

    // Dividir a string em linhas
    while (getline(logStream, line)) {
        vector<string> lineEntries;
        istringstream lineStream(line);
        string entry;

        // Dividir cada linha em partes usando '|'
        while (getline(lineStream, entry, '|')) {
            // Remover espaços em branco extras no início e no final
            entry.erase(entry.find_last_not_of(" \t\n\r\f\v") + 1);
            entry.erase(0, entry.find_first_not_of(" \t\n\r\f\v"));
            lineEntries.push_back(entry);
        }

        entries.push_back(lineEntries);
    }

    return entries;
}

vector<DataFrame> processLogs(const string& logData) {

    auto logEntries = splitLogEntries(logData);
    map<vector<string>, DataFrame> dataFramesMap;

    for (const auto& entry : logEntries) {
        vector<string> columns = {"Data de Notificação", "Tipo", "Conteúdo textual"};
        vector<string> types = {"Time", "string", "string"};
        vector<RowVariant> values;

        if (!entry.empty() && entry.size() >= 2) {
            optional<Time> optTime = Time::fromString(entry[0]);
            if (optTime) {
                values.push_back(*optTime);
            } else {
                cout << "Invalid time format" << endl;
                continue; // Skip this entry
            }

            values.push_back(entry[1]); // Assuming these are valid as per types
            values.push_back(entry[2]);

            for (int i = 3; i < entry.size(); i++) {
                auto pos = entry[i].find(':');
                if (pos != string::npos && pos + 1 < entry[i].size()) {
                    string colName = entry[i].substr(0, pos);
                    string colValue = entry[i].substr(pos + 2);
                    columns.push_back(colName);

                    if (isTimeFormat(colValue)) {
                        optional<Time> time = Time::fromString(colValue);
                        if (time) {
                            values.push_back(*time);
                            types.push_back("Time");
                        } else {
                            cout << "Invalid time format for additional column" << endl;
                            continue; // Skip this value
                        }
                    } else if (isNumber(colValue)) {
                        values.push_back(stod(colValue));
                        types.push_back("double");
                    } else {
                        values.push_back(colValue);
                        types.push_back("string");
                    }
                }
            }

            if (values.size() != columns.size()) {
                cout << "Mismatch in number of columns and values" << endl;
                continue; // Skip this row
            }
            auto row = make_shared<Row>(columns, values);
            // Aqui inserir lógica que separa os valores em diferentes DataFrames
            // Se a chave não existir, cria um novo DataFrame
            // Se a chave existir, insere a linha no DataFrame existente
            // Check if the DataFrame with these columns already exists
            if (dataFramesMap.find(columns) == dataFramesMap.end()) {
                // Create a new DataFrame if not found
                DataFrame newDf;
                newDf.setColumns(columns);
                newDf.setTypes(types);
                dataFramesMap[columns] = newDf;
            }
            // Insert the row into the correct DataFrame
            dataFramesMap[columns].insertRow(row);
        }
    }
    vector<DataFrame> dataFrames;
    for (auto& [key, df] : dataFramesMap) {
        dataFrames.push_back(df);
    }
    return dataFrames;
}

// Função para dividir uma string por vírgula, tratando cada parte como um campo separado
vector<string> splitCsvLine(const string& line) {
    vector<string> result;
    stringstream lineStream(line);
    string cell;

    while (getline(lineStream, cell, ',')) {
        // Remove espaços em branco no início e no fim de cada campo
        cell.erase(0, cell.find_first_not_of(" \t\n\r"));
        cell.erase(cell.find_last_not_of(" \t\n\r") + 1);
        result.push_back(cell);
    }

    return result;
}

// Cria um vetor de vetores de strings para cabeçalhos válidos
vector<vector<string>> createValidHeaders() {
    vector<vector<string>> validHeaders = {
        splitCsvLine("ID,Nome,Sobrenome,Endereço,Data de Cadastro,Data de Aniversário"),
        splitCsvLine("ID,Nome,Imagem,Descrição,Preço"),
        splitCsvLine("ID do Usuário,ID do Produto,Quantidade,Data de Criação,Data do Pagamento,Data da Entrega"),
        splitCsvLine("ID do Produto,Quantidade Disponível")
    };
    return validHeaders;
}

// Verifica se uma linha é um cabeçalho
bool isHeader(const vector<string>& line, const vector<vector<string>>& validHeaders) {
    for (const auto& header : validHeaders) {
        if (line == header) {
            return true;
        }
    }
    return false;
}


vector<vector<string>> splitCsvEntries(const string& csvData) {
    vector<vector<string>> entries;
    istringstream csvStream(csvData);
    string line;

    while (getline(csvStream, line)) {
        vector<string> lineEntries;
        istringstream lineStream(line);
        string entry;
        bool inQuotes = false;
        string tempEntry;

        for (char c : line) {
            if (c == '"' && (tempEntry.empty() || tempEntry.back() != '\\')) {
                inQuotes = !inQuotes;  // Toggle the state of within quotes
            } else if (c == ',' && !inQuotes) {
                lineEntries.push_back(tempEntry);
                tempEntry = "";
            } else {
                tempEntry += c;
            }
        }
        if (!tempEntry.empty()) {
            lineEntries.push_back(tempEntry);  // Add the last field
        }
        entries.push_back(lineEntries);
    }

    return entries;
}

vector<DataFrame> processCsvData(const string& csvData) {
    vector<vector<string>> headersValid = createValidHeaders();
    auto csvEntries = splitCsvEntries(csvData);
    if (csvEntries.empty()) return {};

    vector<vector<string>> headers;
    csvEntries.erase(csvEntries.begin());  

    map<vector<string>, DataFrame> dataFramesMap;
    vector<string> header = csvEntries[0]; 
    for (const auto& entry : csvEntries) {
        if (isHeader(entry,headersValid)){
            headers.push_back(entry);
            header = entry;
        }
        vector<string> columns = header;
        vector<string> types;  // Assume all are strings initially
        vector<RowVariant> values;
        if (!entry.empty() && entry.size() >=2 ){
            for (int i = 0; i < entry.size(); i++){
                string colValue = entry[i];
                if (isTimeFormat(colValue)) {
                    optional<Time> time = Time::fromString(colValue);
                    if (time) {
                        values.push_back(*time);
                        types.push_back("Time");
                    } else {
                        cout << "Invalid time format for additional column" << endl;
                        continue; // Skip this value
                    }
                } else if (isNumber(colValue)) {
                    values.push_back(stod(colValue));
                    types.push_back("double");
                } else {
                    values.push_back(colValue);
                    types.push_back("string");
                }
            }
        }
        auto row = make_shared<Row>(columns, values);

        if (dataFramesMap.find(columns) == dataFramesMap.end()) {
            DataFrame newDf;
            newDf.setColumns(columns);
            newDf.setTypes(types);
            dataFramesMap[columns] = newDf;
        }
        dataFramesMap[columns].insertRow(row);
    }

    vector<DataFrame> dataFrames;
    for (auto& [key, df] : dataFramesMap) {
        dataFrames.push_back(df);
    }
    return dataFrames;
}


#endif // UTEIS_H