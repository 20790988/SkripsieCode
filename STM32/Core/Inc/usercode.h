/*
 * usercode.h
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#ifndef INC_USERCODE_H_
#define INC_USERCODE_H_

#include "stdbool.h"
#include "stdint.h"

#define INPUT_LENGTH 100
#define PI 3.14159265358979323846

enum Receiver {IDLE,RECEIVING,DONE};
typedef enum Receiver receiver_t;

enum GPS_type {ADAFRUIT,NEO};
typedef enum GPS_type gps_t;

void handle_uart_interrupt_pc(char inchar);
void handle_uart_interrupt_gps(char inchar);

bool is_same_string(const char str1[], const char str2[],int length);

#endif /* INC_USERCODE_H_ */
