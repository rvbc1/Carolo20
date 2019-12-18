#ifndef ALPHABET_H
#define ALPHABET_H

#include <letter.h>
#include <string.h>


class Alphabet{
private:
    Letter *letters;
    int lenght;
    std::string name;
    char *subString(const char *str, int index, int lenght);
    std::string path;
public:
    Alphabet();
    Alphabet(std::string path);
    void load();
    void add(int id, const char *note, unsigned char width, unsigned char hight);
    void setPath(std::string path);
    Letter *getLast();
    Letter *getLetter(int id);
    void print();
};


#endif // ALPHABET_H
