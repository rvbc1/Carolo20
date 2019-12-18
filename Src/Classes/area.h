#ifndef AREA_H
#define AREA_H

#include <stdint.h>
#include <string>

class Area{
private:
    uint8_t rows;
    uint8_t columns;
    uint8_t row_on_disp, column_on_disp; //Left upper corner
    std::string content;
    Area *next;
    bool clear_area;
public:
    Area(uint8_t rows_on_disp, uint8_t column_on_disp);
    void setContent(std::string text);
    std::string getContnet();
    uint8_t getDisplayRow();
    uint8_t getDisplayColumn();
    Area *getNext();
    void setNext(Area* pointer);
    void setSize(uint8_t rows, uint8_t columns);
    bool getClearStartus();
    uint8_t getRows();
    uint8_t getColumns();
};

#endif // AREA_H
