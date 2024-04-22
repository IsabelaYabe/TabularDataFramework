#ifndef UTEIS_H
#define UTEIS_H

#include <regex>
#include "Row.h" 
#include "DataFrame.h"
#include "Time.h"

using namespace std;

/**
 * Formata uma string bruta adicionando quebras de linha explícitas após cada linha.
 *
 * @param raw A string bruta a ser formatada.
 * @return A string formatada.
 */
string formatString(const string& raw) {
    istringstream stream(raw);
    string line;
    string formatted;
    
    // Lê cada linha da entrada e adiciona uma quebra de linha explícita
    while (getline(stream, line)) {
        if (!line.empty()) {
            formatted += line + "\\n\"\n\"";  // Adiciona cada linha com \n no final para formatação
        }
    }
    
    // Remove o último caractere extra ("\n\"")
    if (!formatted.empty()) {
        formatted.pop_back();  // Remove o último caractere extra (")
        formatted.pop_back();  // Remove o último caractere extra (\n)
        formatted.pop_back();  // Remove o último caractere extra (")
    }
    
    return formatted;
}


/**
 * Verifica se uma string é um número.
 *
 * @param str A string a ser verificada.
 * @return Verdadeiro se a string for um número, falso caso contrário.
 */
bool isNumber(const string& str) {
    // Expressão regular que verifica se a string contém letras, números e hífens
    regex uuidRegex("^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}$");

    // Verifica se a string corresponde ao padrão de um UUID
    if (regex_match(str, uuidRegex)) {
        // Se corresponder ao padrão de um UUID, a string contém letras, números e hífens
        return false;
    }

    // Tenta converter a string para double usando um istringstream
    istringstream iss(str);
    double num;
    iss >> num;

    // Verifica se a conversão foi bem-sucedida e se alcançou o fim da string sem falhas
    return iss.good() || (iss.eof() && !iss.fail());
}

/**
 * Verifica se uma string está no formato de tempo (YYYY-MM-DD HH:MM:SS ou YYYY-MM-DD).
 *
 * @param str A string a ser verificada.
 * @return Verdadeiro se a string estiver no formato de tempo, falso caso contrário.
 */
bool isTimeFormat(const string& str) {
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


/**
 * Divide entradas de log em um vetor de vetores de strings.
 *
 * @param log A string de log a ser dividida.
 * @return Um vetor de vetores de strings, onde cada vetor interno representa uma entrada de log.
 */
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

/**
 * Divide entradas de log em um vetor de vetores de strings.
 *
 * @param log A string de log a ser dividida.
 * @return Um vetor de vetores de strings, onde cada vetor interno representa uma entrada de log.
 */
vector<vector<string>> splitLogEntries2(const string &log) {
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

/**
 * Processa dados de log em DataFrames.
 *
 * @param logData A string contendo os dados de log.
 * @return Um vetor de DataFrames, cada um representando um conjunto distinto de dados de log.
 */
vector<DataFrame> processLogs(const string& logData) {
    // Divide os registros em entradas individuais.
    auto logEntries = splitLogEntries2(logData);
    map<vector<string>, DataFrame> dataFramesMap;

    // Itera sobre cada entrada de registro.
    for (const auto& entry : logEntries) {
        // Define as colunas padrão para os registros de log.
        vector<string> columns = {"Data de Notificação", "Tipo", "Conteúdo textual"};
        vector<string> types = {"Time", "string", "string"};
        // Vetor para armazenar valores das colunas para uma linha específica.
        vector<RowVariant> values;

        if (!entry.empty()) {
            // Tenta converter o primeiro campo para um objeto Time
            optional<Time> optTime = Time::fromString(entry[0]);
            if (optTime) {
                values.push_back(*optTime);
            } else {
                cout << "Invalid time format" << endl;
                continue; 
            }

            // Adiciona os próximos dois campos
            values.push_back(entry[1]); 
            values.push_back(entry[2]);

            // Processar campos adicionais que podem ter pares chave-valor
            for (int i = 3; i < entry.size(); i++) {
                auto pos = entry[i].find(':');
                if (pos != string::npos && pos + 1 < entry[i].size()) {
                    // Extrai o nome da coluna e o valor a partir do par chave-valor
                    string colName = entry[i].substr(0, pos);
                    string colValue = entry[i].substr(pos + 2);
                    // Adiciona o nome da coluna ao vetor de colunas
                    columns.push_back(colName);
                    // Verifica o tipo de valor e adiciona ao vetor de valores com o tipo correspondente
                    if (isTimeFormat(colValue)) {
                        optional<Time> time = Time::fromString(colValue);
                        values.push_back(*time);
                        types.push_back("Time");
                    } else if (isNumber(colValue)) {
                        values.push_back(stod(colValue));
                        types.push_back("double");
                    } else {
                        values.push_back(colValue);
                        types.push_back("string");
                    }
                }
            }
            // Se o número de valores não corresponder ao número de colunas, imprime um erro e pula esta linha
            if (values.size() != columns.size()) {
                cout << "Mismatch in number of columns and values" << endl;
                continue; //Pular esta linha
            }
            // Cria uma nova linha com as colunas e valores obtidos
            auto row = make_shared<Row>(columns, values);
            // Verifica se o DataFrame com essas colunas já existe
            if (dataFramesMap.find(columns) == dataFramesMap.end()) {
                //Cria um novo DataFrame se não for encontrado
                DataFrame newDf;
                newDf.setColumns(columns);
                newDf.setTypes(types);
                dataFramesMap[columns] = newDf;
            }
            //Insere a linha no DataFrame correto
            dataFramesMap[columns].insertRow(row);
            // Limpa os vetores para a próxima entrada
            values.clear();
        }
        // Limpa os vetores para a próxima entrada
        columns.clear();
        types.clear();
        values.clear(); 
    }
    // Converte o mapa de DataFrames para um vetor
    vector<DataFrame> dataFrames;
    for (auto& [key, df] : dataFramesMap) {
        dataFrames.push_back(df);
    }
    return dataFrames;
}


/**
 * @brief Divide uma string CSV em seus campos individuais, tratando cada vírgula como um delimitador.
 *
 * @param line A linha CSV a ser dividida.
 * @return Um vetor de strings, cada uma representando um campo separado por vírgula na linha original.
 */
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

/**
 * @brief Cria uma lista de vetores contendo strings que representam cabeçalhos válidos para CSVs.
 *
 * @return Um vetor de vetores de strings, cada um representando um cabeçalho CSV válido.
 */
vector<vector<string>> createValidHeaders() {
    vector<vector<string>> validHeaders = {
        splitCsvLine("ID,Nome,Sobrenome,Endereço,Data de Cadastro,Data de Aniversário"),
        splitCsvLine("ID,Nome,Imagem,Descrição,Preço"),
        splitCsvLine("ID do Usuário,ID do Produto,Quantidade,Data de Criação,Data do Pagamento,Data da Entrega"),
        splitCsvLine("ID do Produto,Quantidade Disponível")
    };
    return validHeaders;
}

/**
 * @brief Verifica se uma linha representa um cabeçalho de CSV, comparando-a com uma lista de cabeçalhos válidos.
 *
 * @param line A linha a ser verificada.
 * @param validHeaders Uma lista de vetores de strings que representam cabeçalhos válidos.
 * @return Verdadeiro se a linha for um cabeçalho válido, falso caso contrário.
 */
bool isHeader(const vector<string>& line, const vector<vector<string>>& validHeaders) {
    for (const auto& header : validHeaders) {
        if (line == header) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Divide os dados CSV em linhas e campos, levando em consideração a possibilidade de campos entre aspas que podem conter vírgulas.
 *
 * @param csvData Os dados CSV como uma string única.
 * @return Um vetor de vetores de strings, com cada vetor interno representando uma linha do CSV e seus campos.
 */
vector<vector<string>> splitCsvEntries(const string& csvData) {
    vector<vector<string>> entries;
    // Stream para ler os dados CSV linha por linha
    istringstream csvStream(csvData);
    string line;

    // Lê cada linha do dado CSV
    while (getline(csvStream, line)) {
        // Vetor para armazenar as entradas (campos) da linha atual
        vector<string> lineEntries;
        // Stream para processar cada linha individualmente
        istringstream lineStream(line);
        // Variáveis para auxiliar no processamento de cada campo
        string entry;
        // Indica se o processamento está dentro de aspas, usado para campos que contêm vírgulas
        bool inQuotes = false;
        // Variável temporária para construir cada campo (entry)
        string tempEntry;

        // Processa cada caractere na linha
        for (char c : line) {
            if (c == '"' && (tempEntry.empty() || tempEntry.back() != '\\')) {
                inQuotes = !inQuotes;  
            } else if (c == ',' && !inQuotes) {
                lineEntries.push_back(tempEntry);
                tempEntry = "";
            } else {
                tempEntry += c;
            }
        }
        // Após processar todos os caracteres, verifica se ainda há um campo sendo construído
        if (!tempEntry.empty()) {
            lineEntries.push_back(tempEntry);  //Adiciona o último campo
        }
        // Adiciona a linha processada (com seus campos) ao vetor de entradas
        entries.push_back(lineEntries);
    }

    return entries;
}

/**
 * @brief Divide os dados CSV em linhas e campos, levando em consideração a possibilidade de campos entre aspas que podem conter vírgulas.
 *
 * @param csvData Os dados CSV como uma string única.
 * @return Um vetor de vetores de strings, com cada vetor interno representando uma linha do CSV e seus campos.
 */
vector<vector<string>> splitCsvEntries2(const string& csv) {
    vector<vector<string>> data;
    istringstream csvStream(csv);
    string line;

    // Processar cada linha do CSV
    while (getline(csvStream, line)) {
        vector<string> row;
        istringstream lineStream(line);
        string cell;

        // Dividir cada linha em células usando ','
        while (getline(lineStream, cell, ',')) {
            // Remover espaços em branco extras no início e no final (opcional)
            cell.erase(cell.find_last_not_of(" \t\n\r\f\v") + 1);
            cell.erase(0, cell.find_first_not_of(" \t\n\r\f\v"));
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}

/**
 * @brief Processa dados CSV em um DataFrame, identificando os tipos de dados de cada coluna e criando linhas no DataFrame.
 *
 * @param csvData Os dados CSV como uma string única.
 * @return Um DataFrame contendo os dados processados do CSV.
 */
DataFrame processCsvData(const string& csvData) {
    auto csvEntries = splitCsvEntries(csvData);
    // Se não houver entradas, retorna um DataFrame vazio
    if (csvEntries.empty()) return {};

    // Assume que a primeira entrada contém os nomes das colunas.
    vector<string> columns = csvEntries[0];
    
    // Remove os nomes das colunas da lista de entradas.
    csvEntries.erase(csvEntries.begin());  
    // Cria um novo DataFrame para armazenar os dados processados.
    DataFrame newDf;
    vector<string> types;
    
    // Variável de controle para adicionar tipos apenas uma vez.
    int j = 0;
    for (const auto& entry : csvEntries) {

        // Vetor para armazenar os valores convertidos para o tipo correto
        vector<RowVariant> values;
        if (!entry.empty()){
            // Itera sobre cada valor dentro de uma entrada.
            for (int i = 0; i < entry.size(); i++){
                // Armazena o valor atual para processamento.
                string colValue = entry[i];
                // Verifica de qual tipo é o valor.
                if (isTimeFormat(colValue)) {
                    optional<Time> time = Time::fromString(colValue);
                    values.push_back(*time);
                    if(j==0){
                        types.push_back("Time"); 
                    }
                } else if (isNumber(colValue)) {
                    values.push_back(stod(colValue));
                    if(j==0){
                        types.push_back("double"); 
                    }                    
                } else {
                    values.push_back(colValue);
                    if(j==0){
                        types.push_back("string");
                    }   
                }
            }
        }
        // Cria uma nova linha com os valores e a adiciona ao DataFrame
        auto row = make_shared<Row>(columns, values);
        // Na primeira passagem, define as colunas e os tipos no DataFrame
        if(j==0){
            newDf.setColumns(columns);
            newDf.setTypes(types);
            // Aumenta o controle para não adicionar os tipos novamente
            j++;
        }
        // Insere a linha no DataFrame.
        newDf.insertRow(row);
        // Limpa o vetor de valores para a próxima entrada.
        values.clear();
    }

    return newDf;
}

/**
 * @brief Verifica se uma string representa uma data e hora concatenada sem espaço entre eles.
 *
 * @param input A string a ser verificada.
 * @return Verdadeiro se a string for uma data e hora concatenada, falso caso contrário.
 */
bool isConcatenatedDateTime(const string& input) {
    // Regex para identificar strings no formato "YYYY-MM-DDHH:MM:SS"
    regex pattern(R"(\d{4}-\d{2}-\d{2}\d{2}:\d{2}:\d{2})");

    // Verifica se a string corresponde ao padrão
    return regex_match(input, pattern);
}

/**
 * @brief Corrige o formato de uma string que representa uma data e hora concatenada, inserindo um espaço entre a data e a hora.
 *
 * @param dateTime A string de data e hora a ser corrigida.
 */
void fixDateTimeFormat(string& dateTime) {
    // Regex para identificar datas no formato "YYYY-MM-DDHH:MM:SS"
    regex pattern("(\\d{4}-\\d{2}-\\d{2})(\\d{2}:\\d{2}:\\d{2})");

    // Substitui a string encontrada pelo padrão com a mesma string seguida de um espaço entre a data e a hora
    string corrected = regex_replace(dateTime, pattern, "$1 $2");
    dateTime = corrected;

}

/**
 * @brief Divide entradas JSON em uma coleção de mapas, com cada mapa representando um objeto JSON com pares chave-valor.
 *
 * @param jsonString A string contendo dados JSON.
 * @return Um vetor de mapas, com cada mapa contendo os pares chave-valor de um objeto JSON.
 */
vector<map<string, string>> splitJsonEntries(const string &jsonString) {
    vector<map<string, string>> entries;
    istringstream jsonStream(jsonString);
    string segment;

    // Encontrar cada objeto JSON na lista
    while (getline(jsonStream, segment, '{')) {
        size_t endPos = segment.find('}');
        if (endPos == string::npos) continue;

        string object = segment.substr(0, endPos);
        map<string, string> entryMap;
        istringstream objStream(object);
        string pair;

        // Extrair cada par chave-valor
        while (getline(objStream, pair, ',')) {
            size_t colonPos = pair.find(':');
            if (colonPos == string::npos) continue;

            string key = pair.substr(0, colonPos);
            string value = pair.substr(colonPos + 2, pair.size() - colonPos - 3); // Corrigido para remover as aspas

            // Remover aspas e espaços desnecessários
            key.erase(remove(key.begin(), key.end(), '"'), key.end());
            value.erase(remove(value.begin(), value.end(), '"'), value.end());
            key.erase(remove(key.begin(), key.end(), ' '), key.end());
            value.erase(remove(value.begin(), value.end(), ' '), value.end());

            entryMap[key] = value;
        }

        if (!entryMap.empty()) {
            entries.push_back(entryMap);
        }
    }

    return entries;
}

/**
 * @brief Processa uma string de dados JSON e cria um DataFrame com colunas correspondentes aos pares chave-valor dos objetos JSON.
 *
 * @param jsonData A string contendo dados JSON.
 * @return Um DataFrame contendo os dados processados do JSON.
 */
DataFrame processJson(string& jsonData) {
    auto jsonEntries = splitJsonEntries(jsonData);
    if (jsonEntries.empty()) return {};

    DataFrame newDf;
    vector<string> columns;
    vector<string> types;

    // Presumimos que todas as entradas JSON têm o mesmo conjunto de chaves
    // Inicializar colunas e tipos baseado na primeira entrada
    if (!jsonEntries.empty()) {
        for (auto& pair : jsonEntries[0]) {
            columns.push_back(pair.first);
            if(isConcatenatedDateTime(pair.second)) {
                fixDateTimeFormat(pair.second);
            }  // Simplificação, poderia ser ajustado conforme o tipo real dos dados
            if (isTimeFormat(pair.second)) {
                types.push_back("Time");
            } else if (isNumber(pair.second)) {
                types.push_back("double");
            } else {
                types.push_back("string");
            } // Simplificação, poderia ser ajustado conforme o tipo real dos dados
        }
        newDf.setColumns(columns);
        newDf.setTypes(types);
    }

    // Processar cada entrada de JSON
    for (auto& entry : jsonEntries) {
        vector<RowVariant> values;
        for (auto& col : columns) {
            if(isConcatenatedDateTime(entry.at(col))) {
                fixDateTimeFormat(entry.at(col));
            }
            if (isTimeFormat(entry.at(col))) {
                optional<Time> time = Time::fromString(entry.at(col));
                values.push_back(*time);
            } else if (isNumber(entry.at(col))) {
                values.push_back(entry.at(col));
            } else {
                values.push_back(entry.at(col));
            } // Adicionar cada valor ao vetor de RowVariant
        }
        auto row = make_shared<Row>(columns, values);
        newDf.insertRow(row);
        values.clear(); 
    }

    return newDf;
}

#endif // UTEIS_H