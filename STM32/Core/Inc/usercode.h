/*
 * usercode.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#ifndef INC_USERCODE_H_
#define INC_USERCODE_H_

#define INPUT_LENGTH 20

enum Boolean {FALSE,TRUE};
typedef enum Boolean bool;

void handleUARTInterrupt(char inchar);
void handleMsg();

bool stringComp(char str1[], char str2[]);

#endif /* INC_USERCODE_H_ */
