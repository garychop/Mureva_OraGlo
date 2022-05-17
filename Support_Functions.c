//	Filename:  Support_Functions.c
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains general functions to support the OraGlo project

#include "mcc_generated_files/mcc.h"
#include "Support_Functions.h"
#include "Test_Functions.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "float.h"

// Maximum length of a base-10 uint16_t and uint32_t,
// including null terminator and optional '+' sign
static const int MAX_LEN_UINT16 = 7;
static const int MAX_LEN_UINT32 = 12;

char GetChar(void)
{
    char c = UART1_Read(); 
    printf("%c", c);
    return c;
}

/*
 *  Returns 'true' when 'y' or 'Y' is entered,
 *  and 'false' when 'n' or 'N' is entered.
 */
bool GetYesOrNo(void)
{
    bool ret;
    bool done = false;
    char c;
    while (!done)
    {
        c = UART1_Read();
        switch (c)
        {
            case 'y':
            case 'Y':
                ret = true;
                done = true;
                break;
            case 'n':
            case 'N':
                ret = false;
                done = true;
                break;
            default: // Included to satisfy static analyzers
                break;
        }
    }
    printf("%c", c);
    return ret;
}

/*
 *  Returns 0 or 1 if '0' or '1' is pressed
 *  Returns -1 if 'x' or 'X' is pressed
 */
int16_t GetZeroOneOrCancel(void)
{
    int16_t ret;
    bool done = false;
    char c;
    while (!done)
    {
        c = UART1_Read();
        switch (c)
        {
            case '0':
                ret = 0;
                done = true;
                break;
            case '1':
                ret = 1;
                done = true;
                break;
            case 'x':
            case 'X':
                ret = -1;
                done = true;
                break;
            default: // Included to satisfy static analyzers
                break;
        }
    }
    printf("%c", c);
    return ret;
}

/*
 *   Returns the first character entered that's within the limits
 *   specified by the parameters.
 * 
 *   Unlike GetChar() and similar functions in this module, this
 *   routine does not print the entered character out. This is
 *   necessary so that the code will continue to work with the
 *   scripts that are used for downloading image data onto the device.
 */
char GetCharWithinLimits(char lower_bound, char upper_bound)
{
    char c = lower_bound - 1;
    while ((c < lower_bound) || (c > upper_bound))
    {
        c = UART1_Read(); 
    }
    return c;
}

/*
 * Description:  This function reads an unsigned 16-bit integer from the
 *               console (UART1) in the form of a character string.  It does
 *               NOT verify that the string constitutes a valid unsigned
 *               16-bit integer
 * Preconditions:  The UART that services the console needs to be initialized
 * Arguments:  void
 * Returns:  The value read as an unsigned 16-bit integer.
 */
uint16_t ReadUInt16(void)  
{
    char number[MAX_LEN_UINT16];
    ReadString(number, MAX_LEN_UINT16);
    return (uint16_t) strtol(number, NULL, 10); 
}

/*
 *  Description: Prompts the user for an unsigned 16-bit integer.
 *               Stores this value in 'value' unless the user cancels
 *               the operation by pressing 'x' or 'X'
 *  Parameters:
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled.  INPUT_VALID or INPUT_INVALID
 *                 depending on whether the result is a valid unsigned
 *                 16-bit integer or not
 */
input_status_t ReadUInt16OrCancel(uint16_t *value) 
{    
    char buffer[MAX_LEN_UINT16];
    ReadString(buffer, MAX_LEN_UINT16);
    input_status_t status;
    if (strlen(buffer) == 0)
        status = INPUT_CANCELED;
    else if (!strcmp(buffer, "x") || !strcmp(buffer, "X"))
    {
        status = INPUT_CANCELED;
    }
    else
    {
        char *endptr;
	int base = (buffer[1] == 'x' ? 16 : 10);
        int32_t val = strtol(buffer, &endptr, base);
        if ((*endptr != 0) || (val < 0) || (val > 0xFFFF))
        {
            status = INPUT_INVALID;
        }
        else 
        {
            status = INPUT_VALID;
            *value = (uint16_t) val;
        }
    }
    return status;
}

/*
 *  Description: Prompts the user for an unsigned 16-bit integer.
 *               Does not return until a valid value has been entered
 *               or the operation has been canceled by pressing 'x or 'X'
 *  Parameters:
 *     msg:      String containing a user-specified prompt to be displayed
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled; INPUT_VALID otherwise
 */
input_status_t WaitForValidUInt16(const char *msg, uint16_t *value) 
{ 
    input_status_t status = INPUT_INVALID;
    while (status == INPUT_INVALID)
    {
        printf(msg);
        status = ReadUInt16OrCancel(value);
        if (status == INPUT_INVALID)
        {
            printf("\r\nInput invalid\r\n");
        }
    }
    return status;
}

/*
 *  Description: Prompts the user for an unsigned 16-bit integer.
 *               Does not return until a valid value within the specified
 *               range has been entered or the operation has been canceled
 *               by pressing 'x or 'X'
 *  Parameters:
 *     msg:      String containing a user-specified prompt to be displayed
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled; INPUT_VALID otherwise
 */
input_status_t WaitForValidUInt16WithLimits(const char *msg, uint16_t *value,
                                     uint16_t lower_bound, uint16_t upper_bound) 
{ 
    input_status_t status = INPUT_INVALID;
    while (status == INPUT_INVALID)
    {
        printf(msg);
        status = ReadUInt16OrCancel(value);
	if (status == INPUT_CANCELED)
	{
            printf("\r\nInput canceled. ");
	}
        else if (status == INPUT_INVALID)
        {
            printf("\r\nInput invalid. ");
        }
        else if ((*value < lower_bound) || (*value > upper_bound))
        {
            printf("\r\nValue out of range. ");
            status = INPUT_INVALID;
        }
    }
    return status;
}

/*
 * Description:  This function reads an unsigned 32-bit integer from the
 *               console (UART1) in the form of a character string.  It does
 *               NOT verify that the string constitutes a valid unsigned
 *               32-bit integer
 * Preconditions:  The UART that services the console needs to be initialized
 * Arguments:  void
 * Returns:  The value read as an unsigned long (32-bit) integer. This will
 *           only work if the value entered does not exceed 0x7FFFFFFF, since
 *           strtol() generates a signed result.
 */
uint32_t ReadUInt32(void)  
{
    char number[MAX_LEN_UINT32];
    ReadString(number, MAX_LEN_UINT32);
    return (uint32_t) strtol(number, NULL, 10); 
}

/*
 *  Description: Prompts the user for an unsigned 32-bit integer.
 *               Stores this value in 'value' unless the user cancels
 *               the operation by pressing 'x' or 'X'
 *  Parameters:
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled.  INPUT_VALID or INPUT_INVALID
 *                 depending on whether the result is a valid unsigned
 *                 16-bit integer or not
 */
input_status_t ReadUInt32OrCancel(uint32_t *value) 
{    
    char buffer[MAX_LEN_UINT32];
    ReadString(buffer, MAX_LEN_UINT32);
    input_status_t status;
    if (!strcmp(buffer, "x") || !strcmp(buffer, "X"))
    {
        status = INPUT_CANCELED;
    }
    else
    {
        char *endptr;
        int32_t val = strtoul(buffer, &endptr, 10);
        if ((*endptr != 0) || (val < 0))
        {
            status = INPUT_INVALID;
        }
        else 
        {
            status = INPUT_VALID;
            *value = (uint32_t) val;
        }
    }
    return status;
}

/*
 *  Description: Prompts the user for an unsigned 32-bit integer.
 *               Does not return until a valid value has been entered
 *               or the operation has been canceled by pressing 'x or 'X'
 *  Parameters:
 *     msg:      String containing a user-specified prompt to be displayed
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled; INPUT_VALID otherwise
 */
input_status_t WaitForValidUInt32(const char *msg, uint32_t *value) 
{ 
    input_status_t status = INPUT_INVALID;
    while (status == INPUT_INVALID)
    {
        printf(msg);
        status = ReadUInt32OrCancel(value);
        if (status == INPUT_INVALID)
        {
            printf("\r\nInput invalid\r\n");
        }
    }
    return status;
}

/*
 *  Description: Prompts the user for an unsigned 32-bit integer.
 *               Does not return until a valid value within the specified
 *               range has been entered or the operation has been canceled
 *               by pressing 'x or 'X'
 *  Parameters:
 *     msg:      String containing a user-specified prompt to be displayed
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled; INPUT_VALID otherwise
 */
input_status_t WaitForValidUInt32WithLimits(const char *msg, uint32_t *value,
                                     uint32_t lower_bound, uint32_t upper_bound) 
{ 
    input_status_t status = INPUT_INVALID;
    while (status == INPUT_INVALID)
    {
        printf(msg);
        status = ReadUInt32OrCancel(value);
        if (status == INPUT_INVALID)
        {
            printf("\r\nInput invalid. ");
        }
        else if (status == INPUT_CANCELED)
        {
            printf("\r\nInput canceled\r\n");
            break;
        }
        else if ((*value < lower_bound) || (*value > upper_bound))
        {
            printf("\r\nValue of %lu is out of range, must be between %lu and %lu  ", *value, lower_bound, upper_bound);
            status = INPUT_INVALID;
        }
    }
    return status;
}

/*
 * Description:  This function reads a floating point number from the console (UART1)
 * Preconditions:  The UART that services the console needs to be initialized
 * Arguments:  void
 * Returns:  The floating point value read
 */
double ReadFloat(void)   
{
    char Number[32]={32};  //maximum number of characters allowed to specify the floating point number, initialize array with ascii 32 (space)
    
    int i;   //working variable
    for (i=0; i<30; i++)
    { 
        char c = UART1_Read();  printf("%c",c);                 //print the character that was entered
        if (c == CR) { break;}                               //if a carriage return is entered, we're done polling for input
        Number[i] = c;
    }
    return atof(Number);       //convert from the character string to a float
}

/*
 *  Description: Prompts the user for a floating point value.
 *               Stores this value in 'value' unless the user cancels
 *               the operation by pressing 'x' or 'X'
 *  Parameters:
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled.  INPUT_VALID or INPUT_INVALID
 *                 depending on whether the result is a valid unsigned
 *                 16-bit integer or not
 */
input_status_t ReadFloatOrCancel(float *value) 
{    
    /* Using MAX_LEN_UINT32 as the buffer length even though we're
       reading a floating point value. The length is more than adequate.*/
    char buffer[MAX_LEN_UINT32];
    ReadString(buffer, MAX_LEN_UINT32);
    input_status_t status;
    if (!strcmp(buffer, "x") || !strcmp(buffer, "X"))
    {
        status = INPUT_CANCELED;
    }
    else
    {
        char *endptr;
        double val = strtod(buffer, &endptr);
        if ((*endptr != 0) || (val < -FLT_MAX) || (val > FLT_MAX))
        {
            status = INPUT_INVALID;
        }
        else 
        {
            status = INPUT_VALID;
            *value = (float) val;
        }
    }
    return status;
}

/*
 *  Description: Prompts the user for a floating point number.
 *               Does not return until a valid value has been entered
 *               or the operation has been canceled by pressing 'x or 'X'
 *  Parameters:
 *     msg:      String containing a user-specified prompt to be displayed
 *     value:    Pointer to variable in which value will be stored
 *  Returns      INPUT_CANCELED if canceled; INPUT_VALID otherwise
 */
input_status_t WaitForValidFloat(const char *msg, float *value) 
{ 
    input_status_t status = INPUT_INVALID;
    while (status == INPUT_INVALID)
    {
        printf(msg);
        status = ReadFloatOrCancel(value);
        if (status == INPUT_INVALID)
        {
            printf("\r\nInput invalid\r\n");
        }
    }
    return status;
}

/*
 * Description:  This function reads a string from the console and puts it into Data, null terminated, 
 *               returns the number of characters in the string (0 to 256)
 * Preconditions:  The UART that services the console needs to be initialized
 * Arguments:  Data needs to allow for 257 characters
 * Returns:  integer value equal to the number of characters in the string (0 to 256)
 */
int ReadString(char *data, uint16_t max_length)  
{
    int i = 0; // Index into 'data' array. Can't exceed max_length
    int j = 0; // Number of characters entered minus backspaces.
               // Helps us properly handle backspacing beyond max 'data' length
    bool exit_loop = false;
    while (!exit_loop)
    { 
        char c = GetChar();
        switch (c)
        {
            case CR: // carriage return was entered
                exit_loop = true;
                break;
            case 127: // backspace entered
                if (--j < (max_length-1))
                {
                    if (--i < 0)
                    {
                        i = 0; //Don't allow the backspace to drive i negative
                    }                                
                    data[i] = 0; //Replace deleted character with a null
                }
                break;
            default:
                j++;
                if (i < (max_length-1))
                {
                   if ((c>=32) && (c<=126))
                   {   
                       data[i++] = c;  //Only enter in printable characters
                   }
                }
                break;
        }
    }
    data[i] = 0;  //null terminate the string
    return i;
}

void DisableNonDMAInterrupts(void)
{
    IEC0bits.IC1IE = 0;
    IEC0bits.AD1IE = 0;
    IEC0bits.T2IE = 0;
    IEC1bits.MI2C1IE = 0;
    IEC1bits.IOCIE = 0;
    IEC3bits.RTCIE = 0;
}

void EnableNonDMAInterrupts(void)
{
    IEC0bits.IC1IE = 1;
    IEC0bits.AD1IE = 1;
    IEC0bits.T2IE = 1;
    IEC1bits.MI2C1IE = 1;
    IEC1bits.IOCIE = 1;
    IEC3bits.RTCIE = 1;
}

/*
 * CRC-8-Dallas/Maxim CRC table for its polynomial = x^8 + x^5 + x^4 + 1 = 0x31 (left-to-right), or 0x8c (right-to-left)
 */
static const uint8_t crc8_table[] = {0x00,0x5e,0xbc,0xe2,0x61,0x3f,0xdd,0x83,0xc2,0x9c,0x7e,0x20,0xa3,0xfd,0x1f,0x41,0x9d,0xc3,0x21,0x7f,0xfc,0xa2,0x40,0x1e,0x5f,0x01,0xe3,0xbd,0x3e,0x60,0x82,0xdc,0x23,0x7d,0x9f,0xc1,0x42,0x1c,0xfe,0xa0,0xe1,0xbf,0x5d,0x03,0x80,0xde,0x3c,0x62,0xbe,0xe0,0x02,0x5c,0xdf,0x81,0x63,0x3d,0x7c,0x22,0xc0,0x9e,0x1d,0x43,0xa1,0xff,0x46,0x18,0xfa,0xa4,0x27,0x79,0x9b,0xc5,0x84,0xda,0x38,0x66,0xe5,0xbb,0x59,0x07,0xdb,0x85,0x67,0x39,0xba,0xe4,0x06,0x58,0x19,0x47,0xa5,0xfb,0x78,0x26,0xc4,0x9a,0x65,0x3b,0xd9,0x87,0x04,0x5a,0xb8,0xe6,0xa7,0xf9,0x1b,0x45,0xc6,0x98,0x7a,0x24,0xf8,0xa6,0x44,0x1a,0x99,0xc7,0x25,0x7b,0x3a,0x64,0x86,0xd8,0x5b,0x05,0xe7,0xb9,0x8c,0xd2,0x30,0x6e,0xed,0xb3,0x51,0x0f,0x4e,0x10,0xf2,0xac,0x2f,0x71,0x93,0xcd,0x11,0x4f,0xad,0xf3,0x70,0x2e,0xcc,0x92,0xd3,0x8d,0x6f,0x31,0xb2,0xec,0x0e,0x50,0xaf,0xf1,0x13,0x4d,0xce,0x90,0x72,0x2c,0x6d,0x33,0xd1,0x8f,0x0c,0x52,0xb0,0xee,0x32,0x6c,0x8e,0xd0,0x53,0x0d,0xef,0xb1,0xf0,0xae,0x4c,0x12,0x91,0xcf,0x2d,0x73,0xca,0x94,0x76,0x28,0xab,0xf5,0x17,0x49,0x08,0x56,0xb4,0xea,0x69,0x37,0xd5,0x8b,0x57,0x09,0xeb,0xb5,0x36,0x68,0x8a,0xd4,0x95,0xcb,0x29,0x77,0xf4,0xaa,0x48,0x16,0xe9,0xb7,0x55,0x0b,0x88,0xd6,0x34,0x6a,0x2b,0x75,0x97,0xc9,0x4a,0x14,0xf6,0xa8,0x74,0x2a,0xc8,0x96,0x15,0x4b,0xa9,0xf7,0xb6,0xe8,0x0a,0x54,0xd7,0x89,0x6b,0x35};

uint8_t crc8(uint8_t *data, uint16_t len)
{
	uint8_t crc = 0;
	int i;
	for (i = 0; i < len; i++)
		crc = crc8_table[crc ^ data[i]];
	return crc;
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t len = strlen(s);
	if (len > maxlen)
		len = maxlen;
	return (len);
}
