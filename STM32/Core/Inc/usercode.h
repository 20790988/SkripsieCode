/*
 * usercode.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#ifndef INC_USERCODE_H_
#define INC_USERCODE_H_

#include "stdbool.h"

#define INPUT_LENGTH 20

enum Receiver {IDLE,RECEIVING,DONE};
typedef enum Receiver receiver;

//int sine[8] = {2048,3251,2995,3996,2051,847,101,98};

void handleUARTInterrupt(char inchar);
void handleMsg(char str[], int length);

bool stringComp(const char str1[], const char str2[],int length);



#endif /* INC_USERCODE_H_ */
