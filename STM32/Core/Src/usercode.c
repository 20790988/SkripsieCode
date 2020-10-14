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
#include "gps_parsing.h"

char pc_message[INPUT_LENGTH];
char gps_message[INPUT_LENGTH];

uint8_t test_code1[20] = { 0, 13, 26, 39, 52, 65, 78, 91, 104, 117, 130, 143,
        156, 169, 182, 195, 208, 221, 234, 247 };
uint8_t test_code2[20] = { 247, 234, 221, 208, 195, 182, 169, 156, 143, 130,
        117, 104, 91, 78, 65, 52, 39, 26, 13, 0 };

extern char timecode[];
extern uint32_t timecode_pulse[];

volatile int pc_message_length = 0;
volatile int gps_message_length = 0;
volatile receiver_t PC_UART = IDLE;
volatile receiver_t GPS_UART = IDLE;

extern DAC_HandleTypeDef hdac1;
extern DAC_HandleTypeDef hdac2;
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

void DAC_startup()
{
    HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, test_code1, 20, DAC_ALIGN_8B_R);
}


void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    static uint32_t toggle = 0;
    if (hdac == &hdac1)
    {
        HAL_TIM_Base_Stop_IT(&htim6);
    }
    else if (hdac == &hdac2)
    {
        if (toggle == 0)
        {
            HAL_DAC_Stop_DMA(&hdac2, DAC_CHANNEL_1);
            HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, test_code1, 20, DAC_ALIGN_8B_R);
            toggle = 1;
        }
        else
        {
            HAL_DAC_Stop_DMA(&hdac2, DAC_CHANNEL_1);
            HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, test_code2, 20, DAC_ALIGN_8B_R);
            toggle = 0;
        }

    }


}

void HAL_DAC_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac)
{
    if (hdac == &hdac1)
   {

   }
   else if (hdac == &hdac2)
   {
   }
}

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{

}

