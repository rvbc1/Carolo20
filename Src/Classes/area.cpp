#include "area.h"

Area::Area(uint8_t row_on_disp, uint8_t column_on_disp){
    this->row_on_disp = row_on_disp;
    this->column_on_disp = column_on_disp;
    this->next = NULL;
    this->clear_area = false;
}

void Area::setContent(std::string text){
    this->content = text;
}

std::string Area::getContnet(){
    return content;
}

uint8_t Area::getDisplayRow(){
    return row_on_disp;
}

uint8_t Area::getDisplayColumn(){
    return column_on_disp;
}

Area *Area::getNext(){
    return next;
}

void Area::setNext(Area *pointer){
    this->next = pointer;
}

void Area::setSize(uint8_t rows, uint8_t columns){
    this->columns = columns;
    this->rows = rows;
    this->clear_area = true;
}

bool Area::getClearStartus(){
    return clear_area;
}

uint8_t Area::getRows(){
    return rows;
}

uint8_t Area::getColumns(){
    return columns;
}
