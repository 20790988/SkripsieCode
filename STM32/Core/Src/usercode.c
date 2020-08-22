/*
 * usercode.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#include "usercode.h"
#include "stdbool.h"

char rxdata[INPUT_LENGTH];
volatile int rxlen = 0;
volatile receiver PC_UART = IDLE;

void handleUARTInterrupt(char inchar)
{
  static int msglength = 0;

  if (inchar == '$')
  {
    msglength = 0;
    PC_UART = RECEIVING;
  }
  else if (inchar == '*')
  {
    rxlen = msglength;
    PC_UART = DONE;
  }
  else if (PC_UART == RECEIVING && msglength < INPUT_LENGTH)
  {
    rxdata[msglength] = inchar;
    msglength++;
  }
}

bool stringComp(const char str1[], const char str2[], int length)
{
  bool isSame = true;
  for (int i = 0; i<length;i++)
  {
    if (str1[i] != str2[i])
    {
      isSame = false;
      break;
    }
  }
  return isSame;
}





