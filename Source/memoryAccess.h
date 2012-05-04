typedef struct {
    int op, rs, rt, rd, c, func;
    char type;
} instruction;

unsigned int initInstMemory();
unsigned int initDataMemory();
unsigned int toLittleEndian(unsigned char*);
char* loadDataMemory(int);
void saveDataMemory(int, char*, int);
void showMemory();

instruction instructionFetch(int);
instruction instructionDecode(unsigned int);
char instructionType(int op);
