/*
 * usercode.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#include "usercode.h"

volatile int msg_pending = FALSE;
char rxdata[INPUT_LENGTH];
volatile int rxlen = 0;

void handleUARTInterrupt(char inchar)
{
  rxdata[rxlen] = inchar;
  rxlen++;
  if (rxlen>=INPUT_LENGTH || inchar == '\r')
  {
    rxlen = 0;
    msg_pending = TRUE;
  }
}



