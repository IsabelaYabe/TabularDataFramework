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
#include "Row.h" // Make sure the path to Row.h is correctly set up
#include "DataFrame.h"
#include "Time.h"
using namespace std;
void sep(int n=100) {
    cout << string(n, '-') << endl;
};
int main() {
    cout << "Testando Time" << endl;
    cout << "Testando compareTimer com " << endl;

        // Example date-time string
    std::string dateTimeStr = "09-23-2019 03:55:45";

    // Convert string to Time object
    auto maybeTime = Time::fromString(dateTimeStr);

    // Check if the conversion was successful
    if (maybeTime.has_value()) {
        std::cout << "Successfully parsed the date-time string." << std::endl;
        std::cout << "The date-time is: ";
        maybeTime->print();  // Use the print method of Time class to display the date-time
    } else {
        std::cout << "Failed to parse the date-time string." << std::endl;
    }

    Time tm1(23, 9, 2019, 3, 55, 45); 

    Time tm2(20, 9, 2019, 2, 55, 12);
    
    bool result = Time::compareTime(tm1, tm2, Time::TimeMeasure::Year, Time::CompareType::Equal);
    std::cout << "Comparison result: " << result << std::endl;
    cout << "tm1 é igual tm2? " << (result ? "Sim" : "Não") << endl;
    sep();
    cout<< "Testando Row" << endl;
    sep();
    cout<< "Testando RowMerge" << endl;
    Row row0;
    Row row00;

    // Add columns to row1
    row0.addColRow("ID", 1);
    row0.addColRow("Name", string("John Doe"));
    row0.addColRow("Age", 30);
    row0.addColRow("City", string("New York"));

    // Add columns to row2
    row00.addColRow("ID", 1);
    row00.addColRow("Name", string("Mary"));
    row00.addColRow("Address", string("1234 Elm Street"));
    row00.addColRow("Phone", string("555-1234"));
    row00.addColRow("City", string("New York"));

    // Print the rows before merging
    cout << "Row 0 before merging:" << endl;
    row0.printRow();
    cout << "Row 00 before merging:" << endl;
    row00.printRow();

    // Attempt to merge the rows
    try {
        row0.mergeRows(row00, "City");
        cout << "Row 0 after merging:" << endl;
        row0.printRow();
    } catch (const std::exception& e) {
        cout << "Exception occurred: " << e.what() << endl;
    }
    sep();
    // Create a Row instance with an ID of 1
    Row row;

    // Add various types of data to the row
    row.addColRow("Age", 23);                       // Add an int
    row.addColRow("Temperature", 36.6);             // Add a double
    row.addColRow("Name", string("John Doe"));      // Add a string
    row.addColRow("LastName", string("Doe"));       // Add a string	

    // Adding a time structure
    time_t now = time(0);
    tm now_tm = *localtime(&now);
    row.addColRow("CurrentTime", now_tm);           // Add a tm structure

    // Adding a vector of integers
    vector<int> numbers = {1, 2, 3, 4, 5};
    row.addColRow("Numbers", numbers);              // Add a vector<int>

    // Printing the entire row
    cout<< "print Row" << endl;
    row.printRow();
    cout << "===================" << endl;

    cout << "Size: " << row.size() << endl;
    // Accessing a specific column and printing its type
    auto age = row.getCol("Age");
    cout << "Type of 'Age' column: " << row.getTypeCol("Age") << endl;
    cout << "Type of 'Temperature' column: " << row.getTypeCol("Temperature") << endl;
    cout << "Type of 'CurrentTime' column: " << row.getTypeCol("CurrentTime") << endl;
    cout << "Type of 'Numbers' column: " << row.getTypeCol("Numbers") << endl;
    cout << "Type of 'Name' column: " << row.getTypeCol("Name") << endl;
    cout << "Type of 'LastName' column: " << row.getTypeCol("LastName") << endl;

    cout << "Value of 'Age' column: " << get<int>(age) << endl;

    // Remove a column and try to access it
    row.removeCol("Temperature");
    try {
        auto temp = row.getCol("Temperature");
    } catch (const std::exception& e) {
        cout << "Caught an exception trying to access removed column: " << e.what() << endl;
    }

    // Accessing and modifying vector<int> column
    auto& numbersCol = get<vector<int>>(row.getCol("Numbers"));
    numbersCol.push_back(6);
    cout << "Updated Numbers column: ";
    for (int num : numbersCol) {
        cout << num << " ";
    }
    cout << endl;
/////////////////////////////////////////////////////////////////////////
    sep();
    cout << "Testando DataFrame" << endl;
    sep();
    //
///////////////////////////////////////////////////////////////////////////////

    // Create column names and types
    vector<string> colNames = {"Name", "Age", "Scores"};
    vector<string> colTypes = {"string", "int", "vector<int>"};

    // Initialize DataFrame with column names and types
    DataFrame df(colNames, colTypes);

    
    auto row1 = make_shared<Row>();
    row1->addColRow("Name", string("Alice"));
    row1->addColRow("Age", 30);
    row1->addColRow("Scores", vector<int>{85, 92, 88});

    auto row2 = make_shared<Row>();
    row2->addColRow("Name", string("Bob"));
    row2->addColRow("Age", 25);
    row2->addColRow("Scores", vector<int>{75, 84, 79});

    auto row3 = make_shared<Row>();
    row3->addColRow("Name", string("Charlie"));
    row3->addColRow("Age", 35);
    row3->addColRow("Scores", vector<int>{90, 95, 92});

    df.insertRow(row1);
    df.insertRow(row2);
    df.insertRow(row3);
    cout << "linhas inseridas" << endl;
    cout << "Colunas: "; 
    for (const auto& col : df.getColumns()) cout << col << endl;
    cout << "Tipos: ";
    df.printTypes();
    cout << "Mostrando 3 primeiras linhas:" << endl;
    df.head(3);
    cout << "Mostrando 2 últimas linhas:" << endl;
    df.tail(2);
    sep();

    cout << "Testando containsColumns" << endl;
    if(df.containsColumn("Name")) {
        cout << "Coluna 'Name' encontrada" << endl;
    } else {
        cout << "Coluna 'Name' não encontrada" << endl;
    }
    cout << "Testando removeRow e removebyIDEvent:" << endl;
    // Removendo a primeira linha do DataFrame
    try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com primeira linha removida" << endl;
    df.head(2);
        try {
        df.removeRow(0);
    } catch (const out_of_range& e) {
        cerr << "Erro: " << e.what() << endl;
    }
    cout << "df com a segunda linha removida" << endl;
    df.head(1);

    df.head(1);
    df.insertRow(row1);
    df.insertRow(row2);
    sep();
    cout << "Testando getRows:" << endl;
    cout<<  "Acessando a primeira linha do DataFrame " << endl;
    try {
        auto firstRow = df.getRows()[0];
        firstRow->printRow();  // Supondo que o método print() está definido em Row
    } catch (const std::out_of_range& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    sep();

    cout << "Criando df2" << endl;
    // Initialize DataFrame with column names and types
    DataFrame df2(colNames, colTypes);

    // Create rows and add data
    auto row4 = make_shared<Row>();
    row4->addColRow("Name", string("Dudu"));
    row4->addColRow("Age", 26);
    row4->addColRow("Scores", vector<int>{55, 22, 18});

    auto row5 = make_shared<Row>();
    row5->addColRow("Name", string("Herus"));
    row5->addColRow("Age", 31);
    row5->addColRow("Scores", vector<int>{95, 83, 39});

    auto row6 = make_shared<Row>();
    row6->addColRow("Name", string("Elis"));
    row6->addColRow("Age", 5);
    row6->addColRow("Scores", vector<int>{100, 102, 13});
    
    df2.insertRow(row4);
    df2.insertRow(row5);
    df2.insertRow(row6);
    try {
        df.merge(df2);
        std::cout << "Dados em df1 após o merge:" << std::endl;
        df.head(df.getRowCount());
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erro ao mesclar: " << e.what() << std::endl;
    }

    sep();
    cout << "Testando innerJoin" << endl;
    // Create column names and types
    vector<string> colNamesIJ = {"Name", "Age", "Scores" , "IDi"};
    vector<string> colTypesIJ = {"string", "int", "vector<int>", "int"};

    // Initialize DataFrame with column names and types
    DataFrame df11(colNamesIJ, colTypesIJ);

    // Create rows and add data
    auto row11 = make_shared<Row>();
    row11->addColRow("Name", string("Alice"));
    row11->addColRow("Age", 30);
    row11->addColRow("Scores", vector<int>{85, 92, 88});
    row11->addColRow("IDi", 1);

    auto row21 = make_shared<Row>();
    row21->addColRow("Name", string("Bob"));
    row21->addColRow("Age", 25);
    row21->addColRow("Scores", vector<int>{75, 84, 79});
    row21->addColRow("IDi", 2);

    auto row31 = make_shared<Row>();
    row31->addColRow("Name", string("Charlie"));
    row31->addColRow("Age", 35);
    row31->addColRow("Scores", vector<int>{90, 95, 92});
    row31->addColRow("IDi", 3);

    df11.insertRow(row11);
    df11.insertRow(row21);
    df11.insertRow(row31);

   // Initialize DataFrame with column names and types
    vector<string> colNamesIJ2 = {"City", "Cats", "IDi"};
    vector<string> colTypesIJ2 = {"string", "int", "int"};
    DataFrame df12(colNamesIJ2, colTypesIJ2);

    // Create rows and add data
    auto row12 = make_shared<Row>();
    row12->addColRow("City", string("Rio de Janeiro"));
    row12->addColRow("Cats", 3);
    row12->addColRow("IDi", 1);

    auto row22 = make_shared<Row>();
    row22->addColRow("City", string("Moscow"));
    row22->addColRow("Cats", 2);
    row22->addColRow("IDi", 2);

    auto row32 = make_shared<Row>();
    row32->addColRow("City", string("Sidney"));
    row32->addColRow("Cats", 5);
    row32->addColRow("IDi", 4);

    df12.insertRow(row12);
    df12.insertRow(row22);
    df12.insertRow(row32);

    // Realizando o inner join nos DataFrames
    df11.innerJoin(df12, "IDi");
    df11.printDataFrame();    

    sep();
    cout << "Testando groupBySum" << endl;
    // Create column names and types
    vector<string> colNamesGBS = {"Name", "Age", "Scores" , "ID"};
    vector<string> colTypesGBS = {"string", "int", "vector<int>", "int"};
    DataFrame dfGBS(colNamesGBS, colTypesGBS);

    // Create rows and add data
    auto rowGBS1 = make_shared<Row>();
    rowGBS1->addColRow("Name", string("Alice"));
    rowGBS1->addColRow("Age", 30);
    rowGBS1->addColRow("Scores", vector<int>{85, 92, 88});
    rowGBS1->addColRow("ID", 1);

    auto rowGBS2 = make_shared<Row>();
    rowGBS2->addColRow("Name", string("Bob"));
    rowGBS2->addColRow("Age", 25);
    rowGBS2->addColRow("Scores", vector<int>{75, 84, 79});
    rowGBS2->addColRow("ID", 2);

    auto rowGBS3 = make_shared<Row>();
    rowGBS3->addColRow("Name", string("Bob"));
    rowGBS3->addColRow("Age", 35);
    rowGBS3->addColRow("Scores", vector<int>{90, 95, 92});
    rowGBS3->addColRow("ID", 3);

    dfGBS.insertRow(rowGBS1);
    dfGBS.insertRow(rowGBS2);
    dfGBS.insertRow(rowGBS3);   

    // Realizando o groupBySum
    //auto result = dfGBS.groupBySum("Name");
    //result.printDataFrame();
    sep();   
    return 0;
}
