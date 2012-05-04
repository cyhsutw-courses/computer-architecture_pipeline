#include <stdio.h>
#include <stdlib.h>

void ht(int, char, char, char);

int main()
{
   /* short int i = 0xFFFF;
    int j = (unsigned short int)i;
    printf("%8X", j);*/
    ht(5, 'A', 'B', 'C');
    return 0;
}
void ht(int n,char a,char b,char c){
    if(n!=1) {
        ht(n-1, a, c, b);
        ht(n-1, b, a, c);
    } else return;
}
