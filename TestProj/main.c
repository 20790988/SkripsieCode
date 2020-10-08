#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define TIMECODE_LENGTH 100
#define IGNORE_CHECKSUM

#define PULSE_LENGTH 10

void concat_timecode();
void insert_binary_into_string(char *p_timecode, uint32_t num, uint32_t len);
uint32_t days_in_year(uint32_t year, uint32_t month, uint32_t day);

static uint32_t hour = 14;
static uint32_t min = 50;
static uint32_t sec = 12;

static uint32_t year = 2020;
static uint32_t month = 1;
static uint32_t day = 1;
char timecode[TIMECODE_LENGTH + 1] =
        "P0000I000P0000I000IP0000I00IIP0000I0000P00IIIIIIIP0000I0000P000000000P000000000P000000000P00000000IP";


int main()
{
    char str[] = "ABCDEFGHIJKLMNOP";
    char * p_strings[10];
    p_strings[1] = &str[5];
    puts(p_strings[1]);
    return 0;
}

void concat_timecode()
{

    insert_binary_into_string(&timecode[1], sec % 10, 4);
    insert_binary_into_string(&timecode[6], sec / 10, 3);

    insert_binary_into_string(&timecode[10], min % 10, 4);
    insert_binary_into_string(&timecode[15], min / 10, 3);

    insert_binary_into_string(&timecode[20], hour % 10, 4);
    insert_binary_into_string(&timecode[25], hour / 10, 2);

    uint32_t day_of_year = days_in_year(year, month, day);

    insert_binary_into_string(&timecode[30], day_of_year % 10, 4);
    insert_binary_into_string(&timecode[35], (day_of_year % 100) / 10, 4);
    insert_binary_into_string(&timecode[40], day_of_year / 100, 2);

    insert_binary_into_string(&timecode[50], year % 10, 4);
    insert_binary_into_string(&timecode[55], (year % 100) / 10, 4);

    uint32_t seconds_of_day = hour * 3600 + min * 60 + sec;

    for (uint32_t i = 0; i <= 16; i++)
    {
        if (i < 9)
        {
            timecode[80 + i] = '0' + (seconds_of_day >> i & 1);
        }
        else
        {
            timecode[80 + i + 1] = '0' + (seconds_of_day >> i & 1);
        }
    }

//    static uint8_t sine[SINE_LENGTH];
//
//    static bool is_generated = false;
//    if (is_generated == false)
//    {
//        generate_sine(sine, SINE_LENGTH);
//        is_generated = true;
//    }
//
//    for (int i = 0; i < TIMECODE_LENGTH; i++)
//    {
//        uint32_t position = i * PULSE_LENGTH;
//        switch (timecode[i])
//        {
//        case 'P':
//            copy_pulse(sine, &timecode_pulse[position], SINE_LENGTH, 8);
//            break;
//        case 'I':
//            copy_pulse(sine, &timecode_pulse[position], SINE_LENGTH, 2);
//            break;
//        case '0':
//            copy_pulse(sine, &timecode_pulse[position], SINE_LENGTH, 2);
//            break;
//        case '1':
//            copy_pulse(sine, &timecode_pulse[position], SINE_LENGTH, 5);
//            break;
//        default:
//            copy_pulse(sine, &timecode_pulse[position], SINE_LENGTH, 0);
//            //ENCODING ERROR
//        }
//    }
}

void insert_binary_into_string(char *p_timecode, uint32_t num, uint32_t len)
{

    if (num < 10 && num < pow(2, len))
    {
        for (uint32_t i = 0; i < len; i++)
        {
            p_timecode[i] = '0' + (num >> i & 1);
        }
    }
    else
    {
        for (uint32_t i = 0; i < len; i++)
        {
            p_timecode[i] = 'E';
        }
    }
}

uint32_t days_in_year(uint32_t year, uint32_t month, uint32_t day)
{
    uint32_t days_in_month[] =
            { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    uint32_t day_of_year = day;
    for (uint32_t i = 0; i < month - 1; i++)
    {
        day_of_year += days_in_month[i];
    }

    if (year % 4 == 0 && month > 2)
    {
        day_of_year++;
    }
    return day_of_year;
}
