#ifndef ROWCONTAVERDE_H
#define ROWCONTAVERDE_H

#include "Row.h"

class RowContaVerde: public Row {      
public:
    // Construtor 
    RowContaVerde(int num, int idEvent): Row(num, idEvent) {}
    
    virtual void print() const override {
        Row::print(); 
    }
};

#endif // ROWCONTAVERDE_H
