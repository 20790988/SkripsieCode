/*
 * gps_parsing.h
 *
 *  Created on: Sep 22, 2020
 *      Author: Kobus
 */

#ifndef INC_GPS_PARSING_H_
#define INC_GPS_PARSING_H_

#define TIMECODE_LENGTH 100
#define PULSE_LENGTH 50
#define SINE_LENGTH 5

void concat_timecode();
void print_b(uint32_t num, uint32_t nibbles);
bool is_checksum_good(char* givenString, uint32_t len);
void parse_nmea(char* GPSString, uint32_t GPSStringLen);
bool equals(char* str1, char* str2, uint32_t len, bool ignoreCase);
void parse_time(char* time);
void parse_date(char* time);
uint32_t parse_char(char c);
float parse_double(char* str,uint32_t len);
void copy_array_by_value(char* original, char* copy, uint32_t len);
void insert_binary_into_string(char* p_timecode, uint32_t num, uint32_t len);
uint32_t days_in_year(uint32_t year, uint32_t month, uint32_t day);
void copy_pulse(uint8_t* original, uint8_t* copy, uint32_t len, uint32_t num_copies);
void generate_sine(uint8_t target[], uint32_t length);

#endif /* INC_GPS_PARSING_H_ */
