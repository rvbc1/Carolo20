#ifndef LETTER_H
#define LETTER_H

#include <string.h>
#include <bitoperations.h>
#include <string>


class Letter{
private:
    uint8_t width;
    uint8_t hight;
    uint8_t *values;
    std::string name;
    int id;
    Letter *next;
public:
    void resetValues();
    Letter(int id, const char *name, unsigned char width, unsigned char hight);
    Letter *getNext();
    void setNext(Letter *next);
    uint8_t getValue(unsigned char row, unsigned char column);
    uint8_t getByte(unsigned char column);
    uint8_t getWidth();
    uint8_t getHight();
    void setBit(int column, int bit, int value);
    uint8_t getBit(int column, unsigned char bit);
    int getId();
    std::string getName();
};



#endif // LETTER_H
