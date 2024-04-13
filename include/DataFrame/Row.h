#ifndef ROW_H
#define ROW_H

#include <iostream>

using namespace std;

class Row {
private:
    int numColumns;
    int IDEvent;

public:
    // Construtor
    Row(int num, int id): numColumns(num), IDEvent(id) {}

    // Getters
    int getNumColumns() const { return numColumns; }
    virtual int getIDEvent() const { return IDEvent; }

    // Setters
    void setNumColumns(int num) { numColumns = num; }
    void setIDEvent(int id) { IDEvent = id; }

    virtual void print() const {
        cout << "IDEvent: " << IDEvent;
    }
};

#endif // ROW_H
