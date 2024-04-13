#ifndef ROWCADEANALYTICS_H
#define ROWCADEANALYTICS_H

#include "Row.h"  // Inclui o arquivo da classe base Row

class RowCadeAnalytics : public Row {
public:
    // Construtor
    RowCadeAnalytics(int numColumns, int idEvent) : Row(numColumns, idEvent) {}

    virtual void print() const override {
        Row::print();  
    }
};

#endif // ROWCADEANALYTICS_H
