#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void shift_out(uint8_t dat);
void print_b(int num, int len);

int main()
{

    return 0;
}

void shift_out(uint8_t dat)
{
	for (uint8_t i = 0; i<8; i++)
	{
	    //temp =
		//print()
	}
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
