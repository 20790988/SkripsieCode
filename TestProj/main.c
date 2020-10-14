#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define TIMECODE_LENGTH 100
#define SINES_PER_CHARACTER 10
#define SINE_LENGTH 5

#define IGNORE_CHECKSUM

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

bool timecode_bool[TIMECODE_LENGTH * SINES_PER_CHARACTER];
void fill_bool(bool target[], uint32_t ones, uint32_t length);
void concat_timecode();
void insert_binary_into_string(char *p_timecode, uint32_t num, uint32_t len);

int main()
{
    uint32_t bin_max = 0xFF;
    uint32_t voltage_max = 3300;
    uint32_t voltage_pp = 500;
    uint32_t bin_pp = bin_max * voltage_pp / voltage_max;
    uint32_t length = 5;
    target
    target[0] = (bin_pp + 1) / 2;
    for (uint32_t i = 1; i < length; i++)
    {
        target[i] = (uint8_t) ((sin(i * 2 * M_PI / length) + 1)
                * ((bin_pp + 1) / 2));

        printf("%d\n",target[i]);
    }
    return 0;
}

void fill_bool(bool target[], uint32_t ones, uint32_t length)
{
    for (uint32_t i = 0; i<length; i++)
    {
        if (i<ones)
        {
            target[i] = true;
        }
        else
        {
            target[i] = false;
        }
    }
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

    for (int i = 0; i < TIMECODE_LENGTH; i++)
    {
        uint32_t position = i * SINES_PER_CHARACTER;
        switch (timecode[i])
        {
        case 'P':
            fill_bool(&timecode_bool[position],8,SINES_PER_CHARACTER);
            break;
        case 'I':
            fill_bool(&timecode_bool[position],2,SINES_PER_CHARACTER);
            break;
        case '0':
            fill_bool(&timecode_bool[position],2,SINES_PER_CHARACTER);
            break;
        case '1':
            fill_bool(&timecode_bool[position],5,SINES_PER_CHARACTER);
            break;
        default:
            fill_bool(&timecode_bool[position],10,SINES_PER_CHARACTER);
            //ENCODING ERROR
        }
    }
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
