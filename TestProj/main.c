#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum Boolean {FALSE,TRUE};
typedef enum Boolean bool;

bool stringComp(const char str1[], const char str2[], int length);
void print_b(int num, int nibbles);

int main()
{
    int hour = 21;
    int min = 52;
    int sec = 10;
    char string[10];
    sprintf(string,"%2d %2d %2d\n",hour,min,sec);
    for (int i = 0; i<9;i++)
    {
        print_b(string[i],2);
    }
    return 0;
}

char* encode(int hour, int min, int sec)
{

}

void print_b(int num, int nibbles)
{
    int len = nibbles*4;
    char str[len+1];

    for (int i = 0; i<len; i++)
    {
        int index = len-i-1;
        str[len-i-1] = '0' + (num>>i & 1);
    }

    str[len] = '\0';

    for (int i = 0; i<len/4;i++)
    {
        char temp[5];
        temp[0] = str[i*4+0];
        temp[1] = str[i*4+1];
        temp[2] = str[i*4+2];
        temp[3] = str[i*4+3];
        temp[4] = '\0';
        printf("%s ",temp);
    }
    printf("\n");
}

