/*
 * usercode.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#include "usercode.h"
#include "stdbool.h"

char pc_message[INPUT_LENGTH];
char gps_message[INPUT_LENGTH];

volatile int pc_message_length = 0;
volatile int gps_message_length = 0;
volatile receiver_t PC_UART = IDLE;
volatile receiver_t GPS_UART = IDLE;

void handle_uart_interrupt_pc(char inchar)
{
    static int current_index = 0;

    if (inchar == '$')
    {
        current_index = 0;
        PC_UART = RECEIVING;
    }
    else if (inchar == '*')
    {
        pc_message_length = current_index;
        PC_UART = DONE;
    }
    else if (PC_UART == RECEIVING && current_index < INPUT_LENGTH)
    {
        pc_message[current_index] = inchar;
        current_index++;
    }
}

void handle_uart_interrupt_gps(char inchar)
{

}

bool is_same_string(const char str1[], const char str2[], int length)
{
    bool isSame = true;
    for (int i = 0; i < length; i++)
    {
        if (str1[i] != str2[i])
        {
            isSame = false;
            break;
        }
    }
    return isSame;
}

