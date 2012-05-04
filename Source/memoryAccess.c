#include "memoryAccess.h"

unsigned int instMemory[256] = {0};
unsigned char dataMemory[1024] = {0};
unsigned int itemp[2], dtemp[2];

unsigned int initInstMemory(){
    // load i memory
    FILE *instImage;
    int i, j;
    unsigned char t[4] = {0};
    instImage = fopen("./iimage.bin", "rb");

    // read init PC
    fread(t, 1, 4, instImage);
    itemp[0] = toLittleEndian(t);
    //read instruction count
    fread(t, 1, 4, instImage);
    itemp[1] = toLittleEndian(t);

    // load instructions
    for(i=itemp[0]/4, j=i+itemp[1];i<j;i++) {
        fread(t, 1, 4, instImage);
        instMemory[i] = toLittleEndian(t);
    }

    fclose(instImage);
    // return init pc
    return itemp[0];
}

unsigned int initDataMemory(){
    FILE *dataImage;
    unsigned char t[4] = {0};
    dataImage = fopen("./dimage.bin", "rb");
    // read init SP
    fread(t, 1, 4, dataImage);
    dtemp[0] = toLittleEndian(t);
    //read instruction count
    fread(t, 1, 4, dataImage);
    dtemp[1] = toLittleEndian(t);
    // load data
    fread(dataMemory, sizeof(char), dtemp[1]*4, dataImage);
    fclose(dataImage);

    // return init sp
    return dtemp[0];
}

unsigned int toLittleEndian(unsigned char *t) {
    unsigned i, x = 0;
    for(i=0;i<4;i++) x += (unsigned int)t[i]<<(8*i);
    return x;
}

void showMemory() {
    int i;
    int end;
    // show instruction memory
    printf("iMemory\n");
    printf("initPc: %8X\ncount: %9X\n", itemp[0], itemp[1]);
    end = itemp[0] + itemp[1] * 4;
    for(i=itemp[0];i<end;i+=4) printf("%5X: %8X\n", i, instMemory[i/4]);
    printf("\n");
    // show data memory
    printf("dMemory\n");
    printf("initSp: %8X\ncount: %9X\n", dtemp[0], dtemp[1]);
    end = dtemp[1] * 4;
    for(i=0;i<end;i+=4) {
        // address
        printf("%5X: ", i);
        // data
        printf("%3X", dataMemory[i]);
        printf("%3X", dataMemory[i+1]);
        printf("%3X", dataMemory[i+2]);
        printf("%3X\n", dataMemory[i+3]);
    }
}

instruction instructionFetch(int n) {
    if(n%4 != 0) {    // misaligned
        instruction ins;
        ins.op = 0x3f;
        return ins;
    }
    else return instructionDecode(instMemory[n/4]);
}

instruction instructionDecode(unsigned int t) {
    instruction ins;
    ins.op = t >> 26;
    switch(instructionType(ins.op)) {
        case 'R':
            ins.rs = t << 6 >> 27;
            ins.rt = t << 11 >> 27;
            ins.rd = t << 16 >> 27;
            ins.c = t << 21 >> 27;
            ins.func = t << 26 >> 26;
            ins.type = 'R';
            break;
        case 'I':
            ins.rs = t << 6 >> 27;
            ins.rt = t << 11 >> 27;
            ins.c = t << 16 >> 16;
            ins.type = 'I';
            break;
        case 'J':
            ins.c = t << 6 >> 6;
            ins.type = 'J';
            break;
        case 'S':
            ins.type = 'S';
            break;
    }
    return ins;
}

char instructionType(int op) {
    if(op == 0x00) return 'R';
    else if(op == 0x3F) return 'S';
    else if(op == 0x02 || op ==0x03 ) return 'J';
    else return 'I';
}

void swap(char *c1, char *c2)
{
	char temp;
	temp = *c1;
	*c1 = *c2;
	*c2 = temp;
}

char* loadDataMemory(int a) { // a = Address
    int i;
    char* word;
    word = (char*)malloc(sizeof(char)*4);
    if(a<=1020 && a>=0){
    	for(i=0;i<4;i++){
    		word[i] = dataMemory[a+i];
    	}
    	swap(&word[0], &word[3]);
    	swap(&word[1], &word[2]);
    }
    return word;
}
void saveDataMemory(int a, char* word, int byteCount) {
    int i;
    swap(&word[0], &word[3]);
    swap(&word[1], &word[2]);
    if(a+byteCount <= 1023){
    	for(i=0; i<byteCount; i++) dataMemory[a+i] = word[i];
    }
}
