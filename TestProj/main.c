#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define TIMECODE_LENGTH 100
#define IGNORE_CHECKSUM

static uint32_t hour = 0;
static uint32_t min = 0;
static uint32_t sec = 0;

static uint32_t year = 0;
static uint32_t month = 0;
static uint32_t day = 0;

static float lat = 0;
static float lon = 0;

void concat_timecode(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t seconds);
void print_b(uint32_t num, uint32_t nibbles);
bool is_checksum_good(char* givenString, uint32_t len);
void parse_nmea(char* GPSString, uint32_t GPSStringLen);
bool equals(char* str1, char* str2, uint32_t len, bool ignoreCase);
void parse_time(char* time);
void parse_date(char* time);
uint32_t parse_char(char c);
float parse_double(char* str,uint32_t len);
void copy_array_by_value(char* original, char* copy, uint32_t len);
void insert_binary_uint32_to_string(char* p_timecode, uint32_t num, uint32_t len);
uint32_t days_in_year(uint32_t year, uint32_t month, uint32_t day);

int main()
{
    //char str[] = "GPRMC,064951.000,A,2307.1256,N,0,E,0.03,165.48,260406,3.05,W,A*2C";
    //uint32_t GPSStringLen = 72;
    //parse_nmea(str,GPSStringLen);
    //char str[] = "P000I";
    //for (uint32_t i = 0; i<18;i++)
    //{
    //    insert_binary_uint32_to_string(&str[1], i, 3);
    //    puts(str);
    //}
    concat_timecode(2020,9,21,15,50,31);
    return 0;
}

void concat_timecode(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t seconds)
{
    char timecode[TIMECODE_LENGTH+1] =
    "P0000I000P0000I000IP0000I00IIP0000I0000P00IIIIIIIP0000I0000P000000000P000000000P000000000P00000000IP";

    insert_binary_uint32_to_string(&timecode[1],seconds%10,4);
    insert_binary_uint32_to_string(&timecode[6],seconds/10,3);

    insert_binary_uint32_to_string(&timecode[10],minute%10,4);
    insert_binary_uint32_to_string(&timecode[15],minute/10,3);

    insert_binary_uint32_to_string(&timecode[20],hour%10,4);
    insert_binary_uint32_to_string(&timecode[25],hour/10,2);

    uint32_t day_of_year = days_in_year(year,month,day);
    printf("days of year %d\n",day_of_year);
    insert_binary_uint32_to_string(&timecode[30],day_of_year%10,4);
    insert_binary_uint32_to_string(&timecode[35],(day_of_year%100)/10,4);
    insert_binary_uint32_to_string(&timecode[40],day_of_year/100,2);

    insert_binary_uint32_to_string(&timecode[50],year%10,4);
    insert_binary_uint32_to_string(&timecode[55],(year%100)/10,4);


    uint32_t seconds_of_day = hour*3600+minute*60+seconds;
    printf("seconds of day%d\n",seconds_of_day);
    for (uint32_t i = 0; i<=16; i++)
    {
        if (i<9)
        {
            timecode[80+i] = '0' + (seconds_of_day>>i & 1);
        }
        else
        {
            timecode[80+i+1] = '0' + (seconds_of_day>>i & 1);
        }
    }
    puts(timecode);
}

uint32_t days_in_year(uint32_t year, uint32_t month, uint32_t day)
{
    uint32_t days_in_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint32_t day_of_year  = day;
    for (uint32_t i = 0; i<month-1; i++)
    {
        day_of_year += days_in_month[i];
    }

    if (year % 4 == 0 && month>2)
    {
        day_of_year++;
    }
    return day_of_year;
}

void insert_binary_uint32_to_string(char* p_timecode, uint32_t num, uint32_t len)
{
    puts(p_timecode);
    printf("%d\n",num);
    printf("%d\n",len);


    if (num<10 && num < pow(2,len))
    {
        for (uint32_t i = 0; i<len; i++)
        {
            p_timecode[i] = '0' + (num>>i & 1);
        }
    }
    else
    {
        for (uint32_t i = 0; i<len; i++)
        {
            p_timecode[i] = 'E';
        }
    }
    puts(p_timecode);
    puts("");
}

void print_b(uint32_t num, uint32_t nibbles)
{
    uint32_t len = nibbles*4;
    char str[len+1];

    for (uint32_t i = 0; i<len; i++)
    {
        uint32_t index = len-i-1;
        str[len-i-1] = '0' + (num>>i & 1);
    }

    str[len] = '\0';

    for (uint32_t i = 0; i<len/4;i++)
    {
        char temp[5];
        temp[0] = str[i*4+0];
        temp[1] = str[i*4+1];
        temp[2] = str[i*4+2];
        temp[3] = str[i*4+3];
        temp[4] = '\0';
        printf("%s ",temp);
    }
    printf("\n");
}

bool is_checksum_good(char* givenString, uint32_t len)
{

#ifdef IGNORE_CHECKSUM
	return true;
#endif

	uint32_t i = 0;
	uint32_t checksum = givenString[0];
	char checksumstr[2];
	bool temp = false;

	for (i=1; givenString[i]!='*';i++)
	{
		checksum = checksum^givenString[i];
	}

	sprintf(checksumstr,"%.2x",checksum);

	temp = equals(checksumstr,&(givenString[len-2]),2,true);

	return temp;
}

//checks if the given string is a GPGGA message
//if so, extracts the time, latitude, longitude and altitude to local variables
void parse_nmea(char* GPSString, uint32_t GPSStringLen)
{
    uint32_t l_hour = 0;
    uint32_t l_min = 0;
    uint32_t l_sec = 0;

    uint32_t l_year = 0;
    uint32_t l_month = 0;
    uint32_t l_day = 0;

    float l_lat = 0;
    float l_lon = 0;

	char** p_strings = malloc((sizeof(char*))*11);

	uint32_t i = 0;
	uint32_t numStrings = 0;
	bool GPSFix = false;

	if(!equals("GPRMC",&GPSString[0],5,false))
	{
		return;
	}

	for(i = 0; i<GPSStringLen;i++)
	{
		if(GPSString[i]==',')
		{
			p_strings[numStrings] = &GPSString[i+1];
			numStrings++;
		}
	}
	/*
	 *  0 - hhmmss.sss
	 *  1 - 'A'ctive or 'V'oid
	 *  2 - ddmm.mmmmm lat
	 *  3 - N or S
	 *  4 - dddmm.mmmm long
	 *  5 - E or W
	 *  6 - x.xx knots
	 *  7 - xxx.xx degrees course
	 *  8 - ddmmyy
	 *  9 - xx.x degrees magnetic variation
	 *  10 - E or W magnetic variation
	 */

	//parse time
	if (p_strings[1]-p_strings[0] != 1)
	{
	    char* time = p_strings[0];
		l_hour = 10*parse_char(time[0])+parse_char(time[1]);
        l_min = 10*parse_char(time[2])+parse_char(time[3]);
        l_sec = 10*parse_char(time[4])+parse_char(time[5]);
	}
    printf("%d %d %d\n",l_hour,l_min,l_sec);

    //parse date
	if (p_strings[9]-p_strings[8] != 1)
	{
	    char* p_date = p_strings[8];
        l_day = 10*parse_char(p_date[0])+parse_char(p_date[1]);
        l_month = 10*parse_char(p_date[2])+parse_char(p_date[3]);
        l_year = 10*parse_char(p_date[4])+parse_char(p_date[5]);
	}
    printf("%d %d %d\n",l_day,l_month,l_year);


	//check if device has a gps fix
	if (p_strings[2]-p_strings[1] != 1)
	{
	    if (p_strings[1][0] == 'A')
        {
            GPSFix = true;
            puts("fix!");
        }
        else
        {
            GPSFix = false;
            puts("no fix");
        }
	}

	//parse latitude if it has gps fix
	if (p_strings[3]-p_strings[2] != 1 && GPSFix)
	{
		l_lat = 10*parse_char(p_strings[2][0])+parse_char(p_strings[2][1])+ parse_double(&p_strings[2][2],p_strings[2]-p_strings[2]-3)/60.0;

		if (p_strings[4]-p_strings[3] != 1)
		{
			if(p_strings[3][0] == 'S')
			{
				l_lat *= -1;
			}
		}
	}
	printf("%f\n",l_lat);

	//parse longitude if it has gps fix
	if (p_strings[5]-p_strings[4] != 1 && GPSFix)
	{
		l_lon = 100*parse_char(p_strings[4][0])+10*parse_char(p_strings[4][1]) + parse_char(p_strings[4][2]) + parse_double(&p_strings[4][3],p_strings[5]-p_strings[4]-4)/60.0;

		if (p_strings[6]-p_strings[5] != 1)
		{
			if(p_strings[5][0] == 'W')
			{
				l_lon *= -1;
			}
		}
	}
	printf("%f\n",l_lon);

	free(p_strings);

    hour = l_hour;
    min = l_min;
    sec = l_sec;

    year = l_year;
    month = l_month;
    day = l_day;

    lat = l_lat;
    lon = l_lon;
}

//checks if two strings have the same characters up until length len
//if ignoreCase is true, capitals and lower case letters are considered equal
bool equals(char* str1, char* str2, uint32_t len, bool ignoreCase)
{
	uint32_t i = 0;
	bool equal = true;
	char c1,c2;


	for (i = 0; i<len;i++)
	{
		if (!ignoreCase)
		{
			if (str1[i] != str2[i])
			{
				equal = false;
				break;
			}
		}
		else
		{
			c1 = str1[i];
			c2 = str2[i];
			if (c1>='a' && c1<='z')
			{
				c1 -= 32;
			}

			if (c2>='a' && c2<='z')
			{
				c2 -= 32;
			}

			if (c1 != c2)
			{
				equal = false;
				break;
			}
		}
	}

	return equal;
}

//converts a character to its numerical equivalent for example '3' -> 3
uint32_t parse_char(char c)
{
	return (c-'0');
}

//converts a char array to a float. Works even if no decimal pouint32_t is present
float parse_double(char* str,uint32_t len)
{
	uint32_t i = 0;
	uint32_t decimal = 0;
	bool decimalFlag = false;
	uint32_t whole = 0;
	uint32_t times = 10;

	if (str == NULL)
	{
		return 0.0;
	}

	for (i = 0; i<len;i++)
	{
		if(str[i]== '.')
		{
			decimalFlag = true;
			times = 1;
			continue;
		}

		if (decimalFlag)
		{
		    decimal *= 10;
			decimal += parse_char(str[i]);
			times *= 10;
		}
		else
		{
			whole *= 10;
			whole += parse_char(str[i]);
		}
	}

	return whole+(decimal/(float)times);
}

void copy_array_by_value(char* original, char* copy, uint32_t len)
{
	uint32_t i = 0;
	for (i = 0; i<len; i++)
	{
		copy[i] = original[i];
	}
}


