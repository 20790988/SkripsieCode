/*
 * usercode.c
 *
 *  Created on: Aug 8, 2020
 *      Author: Kobus
 */

#include "usercode.h"
#include "stdbool.h"
#include "stdint.h"
#include "stm32f3xx_hal.h"

char pc_message[INPUT_LENGTH];
char gps_message[INPUT_LENGTH];

char timecode[TIMECODE_LENGTH];
uint32_t timecode_pulse[TIMECODE_LENGTH];

volatile int pc_message_length = 0;
volatile int gps_message_length = 0;
volatile receiver_t PC_UART = IDLE;
volatile receiver_t GPS_UART = IDLE;

extern TIM_HandleTypeDef htim6;

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
    static int current_index = 0;

    if (inchar == '$')
    {
        current_index = 0;
       GPS_UART = RECEIVING;
    }
    else if (inchar == '*')
    {
        gps_message_length = current_index;
        GPS_UART = DONE;
    }
    else if (GPS_UART == RECEIVING && current_index < INPUT_LENGTH)
    {
        gps_message[current_index] = inchar;
        current_index++;
    }
}

void concat_timecode()
{
    static uint16_t seconds = 0;

    timecode[0] = 'P';

    for (int i = 0; i<TIMECODE_LENGTH-1; i++)
    {
        timecode[i+1] = '0' + (seconds>>i & 1);
    }

    for (int i = 0; i<TIMECODE_LENGTH;i++)
    {
        switch(timecode[i])
        {
        case 'P':
            timecode_pulse[i] = 310;
            break;
        case '0':
            timecode_pulse[i] = 0x000;
            break;
        case '1':
            timecode_pulse[i] = 620;
            break;
        default:
            timecode_pulse[i] = 0x000;
        }
    }

    seconds++;
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

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
    //HAL_DAC_Stop_DMA(hdac, DAC_CHANNEL_1);
    HAL_TIM_Base_Stop_IT(&htim6);
}
