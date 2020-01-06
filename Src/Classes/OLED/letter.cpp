#include "letter.h"

Letter::Letter(int id, const char *name, unsigned char width, unsigned char hight){
    this->id = id;
    this->name = name;
    this->width = width;
    this->hight = hight;
    values = new unsigned char[width * (hight - 1)/8];
    next = NULL;
    resetValues();
}


Letter *Letter::getNext(){
    return next;
}

void Letter::setNext(Letter *next){
    this->next = next;
}

void Letter::resetValues(){
    for(int i = 0; i < width; i++) values[i] = 0;
}

uint8_t Letter::getByte(unsigned char column){
    // if(column >= width) return 0;
    return values[column];
}

uint8_t Letter::getValue(unsigned char row, unsigned char column){
    if(column >= width || row >= hight) return 0;
    return getBit(column + (row/8)*width, row);
}

uint8_t Letter::getWidth(){
    return width;
}

uint8_t Letter::getHight(){
    return hight;
}

void Letter::setBit(int column, int bit, int value){
    changeBit(values[column], bit, value);
}

uint8_t Letter::getBit(int column, unsigned char bit){
    char mask = 0x01;
    mask <<= bit;
    char return_value = (values[column] & mask) ? 0x01 : 0x00;
    return return_value;
}

int Letter::getId(){
    return id;
}

std::string Letter::getName(){
    return name;
}

