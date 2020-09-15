#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#define TIMECODE_LENGTH 18
#define IGNORE_CHECKSUM

static int hour = 0;
static int min = 0;
static int sec = 0;

static int year = 0;
static int month = 0;
static int day = 0;

static float lat = 0;
static float lon = 0;

void concat_timecode(int seconds);
void print_b(int num, int nibbles);
bool is_checksum_good(char* givenString, int len);
void parse_nmea(char* GPSString, int GPSStringLen);
bool equals(char* str1, char* str2, int len, bool ignoreCase);
void parse_time(char* time);
void parse_date(char* time);
int parse_char(char c);
float parse_double(char* str,int len);
void copy_array_by_value(char* original, char* copy, int len);


int main()
{
    char str[] = "GPRMC,064951.000,A,2307.1256,N,12016.4438,E,0.03,165.48,260406,3.05,W,A*2C";
    int GPSStringLen = 72;
    parse_nmea(str,GPSStringLen);
    return 0;
}

void concat_timecode(int seconds)
{
    char timecode[] = "P00000000000000000";
    for (int i = 0; i<TIMECODE_LENGTH-1; i++)
    {
        timecode[i+1] = '0' + (seconds>>i & 1);
    }
    puts(timecode);
}

void print_b(int num, int nibbles)
{
    int len = nibbles*4;
    char str[len+1];

    for (int i = 0; i<len; i++)
    {
        int index = len-i-1;
        str[len-i-1] = '0' + (num>>i & 1);
    }

    str[len] = '\0';

    for (int i = 0; i<len/4;i++)
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

bool is_checksum_good(char* givenString, int len)
{

#ifdef IGNORE_CHECKSUM
	return true;
#endif

	int i = 0;
	int checksum = givenString[0];
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
void parse_nmea(char* GPSString, int GPSStringLen)
{
    int l_hour = 0;
    int l_min = 0;
    int l_sec = 0;

    int l_year = 0;
    int l_month = 0;
    int l_day = 0;

    float l_lat = 0;
    float l_lon = 0;
	char** strings = malloc((sizeof(char*))*11);

	int i = 0;
	int numStrings = 0;
	bool GPSFix = false;

	if(!equals("GPRMC",&GPSString[0],5,false))
	{
		return;
	}

	for(i = 0; i<GPSStringLen;i++)
	{
		if(GPSString[i]==',')
		{
			strings[numStrings] = &GPSString[i+1];
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
	if (strings[1]-strings[0] != 1)
	{
		parse_time(strings[0]);
	}
    printf("%d %d %d\n",hour,min,sec);

    //parse date
	if (strings[9]-strings[8] != 1)
	{
		parse_date(strings[8]);
	}
    printf("%d %d %d\n",day,month,year);


	//check if device has a gps fix
	if (strings[2]-strings[1] != 1)
	{
	    if (strings[1][0] == 'A')
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
	if (strings[3]-strings[2] != 1 && GPSFix)
	{
		lat = 10*parse_char(strings[2][0])+parse_char(strings[2][1])+ parse_double(&strings[2][2],strings[2]-strings[2]-3)/60.0;

		if (strings[4]-strings[3] != 1)
		{
			if(strings[3][0] == 'S')
			{
				lat *= -1;
			}
		}
	}
	printf("%f\n",lat);

	//parse longitude and checks burn conditions if it has gps fix
	if (strings[5]-strings[4] != 1 && GPSFix)
	{
		lon = 100*parse_char(strings[4][0])+10*parse_char(strings[4][1]) + parse_char(strings[4][2]) + parse_double(&strings[4][3],strings[5]-strings[4]-4)/60.0;

		if (strings[6]-strings[5] != 1)
		{
			if(strings[5][0] == 'W')
			{
				lon *= -1;
			}
		}
	}
	printf("%f\n",lon);

	free(strings);
}

//checks if two strings have the same characters up until length len
//if ignoreCase is true, capitals and lower case letters are considered equal
bool equals(char* str1, char* str2, int len, bool ignoreCase)
{
	int i = 0;
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

//extracts time from given string to local variables. Assumes format hhmmss
void parse_time(char* time)
{
	hour = 10*parse_char(time[0])+parse_char(time[1]);
	min = 10*parse_char(time[2])+parse_char(time[3]);
	sec = 10*parse_char(time[4])+parse_char(time[5]);
}

//extracts time from given string to local variables. Assumes format ddmmyy
void parse_date(char* date)
{
	day = 10*parse_char(date[0])+parse_char(date[1]);
	month = 10*parse_char(date[2])+parse_char(date[3]);
	year = 10*parse_char(date[4])+parse_char(date[5]);
}

//converts a character to its numerical equivalent for example '3' -> 3
int parse_char(char c)
{
	return (c-'0');
}

//converts a char array to a float. Works even if no decimal point is present
float parse_double(char* str,int len)
{
	int i = 0;
	int decimal = 0;
	bool decimalFlag = false;
	int whole = 0;
	int times = 10;

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

void copy_array_by_value(char* original, char* copy, int len)
{
	int i = 0;
	for (i = 0; i<len; i++)
	{
		copy[i] = original[i];
	}
}


