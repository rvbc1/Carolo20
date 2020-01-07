#include "alphabet.h"


Alphabet::Alphabet(){
    letters = NULL;
    lenght = 0;
}

Alphabet::Alphabet(std::string path){
    letters = NULL;
    lenght = 0;
    this->path = path;
    load();
}

char *Alphabet::subString(const char *str, int index, int lenght){
    char *sub_string = new char[lenght + 1];
    memcpy(sub_string, &str[index], lenght);
    sub_string[lenght] = '\0';
    return sub_string;
}


void Alphabet::load(){
    FILE * pFile;
    char buffer [100];

    pFile = fopen (path.c_str() , "r");
    if (pFile == NULL) perror ("Error opening file");
    else {
        const char NAME_STR[] = "NAME";
        const char DEFAULT_WIDTH_STR[] = "DEFAULT_WIDTH";
        const char DEFAULT_HIGHT_STR[] = "DEFAULT_HIGHT";
        const char CHAR_STR[] = "CHAR";
        const char NOTE_STR[] = "NOTE";
        const char WIDTH_STR[] = "WIDTH";
        const char HIGHT_STR[] = "HIGHT";


        int char_id;
        uint8_t Default_Width = 5;
        uint8_t Default_Hight = 8;
        uint8_t width;
        uint8_t hight;
        uint8_t row = 0;
        std::string note = "";
        char not_added = true;
        while ( ! feof (pFile) ){
            if ( fgets (buffer , 100 , pFile) == NULL ) break;
            if(buffer[0] == '.'){
                if(!strcmp(NAME_STR, subString(buffer, 1, strlen(NAME_STR)))){
                    name =  subString(buffer, strlen(NAME_STR) + 2, strlen(buffer) - strlen(NAME_STR) - 1);
                }
                else if (!strcmp(DEFAULT_WIDTH_STR, subString(buffer, 1, strlen(DEFAULT_WIDTH_STR)))){
                    Default_Width = atoi(subString(buffer, strlen(DEFAULT_WIDTH_STR) + 2, strlen(buffer) - strlen(DEFAULT_WIDTH_STR) - 3));
                }
                else if (!strcmp(DEFAULT_HIGHT_STR, subString(buffer, 1, strlen(DEFAULT_HIGHT_STR)))){
                    Default_Hight = atoi(subString(buffer, strlen(DEFAULT_HIGHT_STR) + 2, strlen(buffer) - strlen(DEFAULT_HIGHT_STR) - 3));
                }
                else if (!strcmp(CHAR_STR, subString(buffer, 1, strlen(CHAR_STR)))){
                    char_id = atoi(subString(buffer, strlen(CHAR_STR) + 2, strlen(buffer) - strlen(CHAR_STR) - 3));
                    not_added = true;
                    width = Default_Width;
                    hight = Default_Hight;
                    row = 0;
                }
                else if (!strcmp(NOTE_STR, subString(buffer, 1, strlen(NOTE_STR)))){
                    note = subString(buffer, strlen(NOTE_STR) + 4, strlen(buffer) - strlen(NOTE_STR) - 5);
                }
                else if (!strcmp(WIDTH_STR, subString(buffer, 1, strlen(WIDTH_STR)))){
                    width = atoi(subString(buffer, strlen(WIDTH_STR) + 2, strlen(buffer) - strlen(WIDTH_STR) - 3));
                }
                else if (!strcmp(HIGHT_STR, subString(buffer, 1, strlen(HIGHT_STR)))){
                    hight = atoi(subString(buffer, strlen(HIGHT_STR) + 2, strlen(buffer) - strlen(HIGHT_STR) - 3));
                }
            }
            else{
                if (not_added){
                    add(char_id, note.c_str(), width, hight);
                    not_added = false;
                    note = "";
                }
                for(unsigned int i = 0; i < strlen(buffer) - 1; i++){
                    if(buffer[i] == '*') getLetter(char_id)->setBit(i, row, 1);
                }
                row++;
            }
        }
        fclose (pFile);
    }
}

void Alphabet::add(int id, const char *note, unsigned char width, unsigned char hight){
    if(lenght == 0) letters = new Letter(id, note, width, hight);
    else getLast()->setNext(new Letter(id, note, width, hight));

    lenght++;
}

Letter *Alphabet::getLast(){
    Letter *address = letters;
    while(address->getNext() != NULL) address = address->getNext();
    return address;
}

Letter *Alphabet::getLetter(int id){
    if (id > lenght) return NULL;
    Letter *return_address = letters;
    for(int i = 0; i < id; i++) return_address = return_address->getNext();
    return return_address;
}

void Alphabet::print(){
    for(int i = 0; i < lenght; i++){
        printf("%d, %s, ", getLetter(i)->getId(), getLetter(i)->getName().c_str());
        for(int j = 0; j < getLetter(i)->getWidth(); j++){
            printf("%d, ", getLetter(i)->getByte(j));
        }
        printf("\n");
    }
}
