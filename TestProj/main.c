#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

enum Boolean {FALSE,TRUE};
typedef enum Boolean bool;

bool stringComp(const char str1[], const char str2[], int length);

int main()
{   bool bool1 = stringComp("ping","ping",4);
    bool bool2 = stringComp("pingo","pingu",4);
    bool bool3 = stringComp("ping","pong",4);

    printf("%d %d %d",bool1,bool2,bool3);
    return 0;
}

bool stringComp(const char str1[], const char str2[], int length)
{
  bool isSame = TRUE;
  for (int i = 0; i<length;i++)
  {
    if (str1[i] != str2[i])
    {
      isSame = FALSE;
      break;
    }
  }
  return isSame;
}
