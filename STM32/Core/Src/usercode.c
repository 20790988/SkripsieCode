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

uint32_t timecode_index = 0;

static uint8_t sine[SINE_LENGTH];
static uint8_t not_sine[SINE_LENGTH];
static uint8_t *p_next_DAC_output = sine;

extern char timecode[];
extern bool timecode_bool[];

volatile int pc_message_length = 0;
volatile int gps_message_length = 0;
volatile receiver_t PC_UART = IDLE;
volatile receiver_t GPS_UART = IDLE;

volatile bool should_concat_timecode = false;

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

void setup_sine()
{
    generate_sine(sine, SINE_LENGTH);
    for (uint32_t i = 0; i < SINE_LENGTH; i++)
    {
        not_sine[i] = 38 / 2;
    }
}

void handle_DAC_Callback(bool from_interrupt)
{
    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1);

    if (from_interrupt)
    {
        HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, sine, SINE_LENGTH,
                DAC_ALIGN_8B_R);
        HAL_TIM_Base_Start_IT(&htim6);
        timecode_index = 1;
    }
    else
    {
        HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, p_next_DAC_output, SINE_LENGTH,
                DAC_ALIGN_8B_R);
        timecode_index++;
    }

    if (timecode_index > TIMECODE_LENGTH*SINES_PER_CHARACTER)
    {
        HAL_TIM_Base_Stop_IT(&htim6);
        p_next_DAC_output = sine;
        should_concat_timecode = true;
    }
    else
    {
        if (timecode_bool[timecode_index])
        {
            p_next_DAC_output = sine;
        }
        else
        {
            p_next_DAC_output = not_sine;
        }
    }

}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac)
{
    static uint32_t toggle = 0;

    if (hdac == &hdac1)
    {
        handle_DAC_Callback(false);
    }
    else if (hdac == &hdac2)
    {
        if (toggle == 0)
        {
            HAL_DAC_Stop_DMA(&hdac2, DAC_CHANNEL_1);
            HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, test_code1, 20,
                    DAC_ALIGN_8B_R);
            toggle = 1;
        }
        else
        {
            HAL_DAC_Stop_DMA(&hdac2, DAC_CHANNEL_1);
            HAL_DAC_Start_DMA(&hdac2, DAC_CHANNEL_1, test_code2, 20,
                    DAC_ALIGN_8B_R);
            toggle = 0;
        }

    }
}

void HAL_DAC_ErrorCallbackCh1(DAC_HandleTypeDef *hdac)
{
    int i = HAL_DAC_GetError(&hdac1);
}

void HAL_DAC_DMAUnderrunCallbackCh1(DAC_HandleTypeDef *hdac)
{
    int i = HAL_DAC_GetError(&hdac1);
}

void generate_sine(uint8_t target[], uint32_t length)
{
    uint32_t bin_max = 0xFF;
    uint32_t voltage_max = 3300;
    uint32_t voltage_pp = 500;
    uint32_t bin_pp = bin_max * voltage_pp / voltage_max;

    target[0] = (bin_pp + 1) / 2;
    for (uint32_t i = 1; i < length; i++)
    {
        target[i] = (uint16_t) ((sin(i * 2 * M_PI / length) + 1)
                * ((bin_pp + 1) / 2));
    }
}
