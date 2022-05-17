/*	Filename:  flashmem.c
 *	Date Created:  11/09/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Flash memory routines
 */

#include <stdio.h>
#include <string.h>

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#include "mcc_generated_files/mcc.h"
#include "flashmem.h"
#include "Support_Functions.h"

#define FLASH_MFG_ID 0xEF14

// Un-comment out the following line to disable interrupts during DMA transfers
#define DISABLE_INTERRUPTS_DURING_DMA

static const uint32_t EEPROM_DEFAULTS_PAGE = 512;

static const size_t EEPROM_FIELD_LENGTH = 16; // Must be multiple of 4
static const uint16_t NUM_EEPROM_FIELDS = 16;
static const uint16_t EEPROM_ADDR_CAL_CONST1 = 0x80;

// Address in FLASH where defaults are stored = EEPROM_DEFAULTS_PAGE*64
static const uint32_t FLASH_ADDR_OF_LED_EEPROM_DEFAULTS = 32768;   

static void EraseFlash(uint16_t starting_block, uint16_t num_blocks, bool print_out);
static void SPI2_ExchangeSwappedWords(uint8_t *pReceiveData, uint16_t num_longs);

/* Description:  This function tests the FLASH on the control board by reading the manufacturing ID and 
 *               comparing it to the expected value.
 * Preconditions:  The SPI module that communicates to the FLASH needs to be initialized
 *                 The digital output that controls the FLASH chip select needs to be initialized
 * Arguments:  The MFG ID is returned in the Data field
 *             Message should allow for up to 255 characters, test failure details will be placed here
 * Returns:  false = Fail, true = Pass
 */
bool TestFlash(uint16_t *data, char *message)
{
    uint32_t returned_data[2];
        
    SPI_SS_FLASH_SetLow();  //Take Select signal low    
    returned_data[0] = SPI2_Exchange32bit( 0x90000000 );   //send the command to get the MFG ID and Device ID
    returned_data[1] = SPI2_Exchange32bit( 0x00000000 );   //another transfer to read out the data
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    
    //printf("\n\r  Returned_Data[0] = 0x%08lX  Returned_Data[1] = 0x%08lX  ", Returned_Data[0], Returned_Data[1]);
    //strcpy(Data, "MFG ID");
    
    *data = (returned_data[1]>>16);

    //Compare to the expected FLASH MFG ID and Device ID
    bool pass = (FLASH_MFG_ID == *data);
    if (pass)
    {
        sprintf(message, "FLASH Passes");
    }
    else
    {
        sprintf(message, "Expected FLASH_MFG_ID = %08Xd", FLASH_MFG_ID);
    }    

    return pass;
}


/* Description:  This function reads the 3 status registers from the FLASH and optionally prints a summary to the console
 * Preconditions:  The SPI module that services the FLASH needs to be initialized
 *                The UART that services the console needs to be initialized.
 * Arguments:  print_out:  true = print summary of results to console
 * Returns:  32-bit unsigned integer containing the values stored within the
 *           the three eight-bit status registers.  This is padded to the left
 *           with zeroes, so the value returned is 00.SR3.SR2.SR1.
 */ 
uint32_t ReadFlashStatus(bool print_out)
{
    uint32_t returned_data[5] = {0};  //Temporary holding for data returned from the SPI interface
    
    SPI_SS_FLASH_SetLow();  //Take Select signal low    
    returned_data[0] = SPI2_Exchange32bit( 0x05000000 );  //Status register is read 3X since it is a 32-bit xfer
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    __delay_us(12); 
    
    SPI_SS_FLASH_SetLow();  //Take Select signal low    
    returned_data[1] = SPI2_Exchange32bit( 0x35000000 );  //Status register is read 3X since it is a 32-bit xfer
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    __delay_us(12); 
    
    SPI_SS_FLASH_SetLow();  //Take Select signal low    
    returned_data[2] = SPI2_Exchange32bit( 0x15000000 );  //Status register is read 3X since it is a 32-bit xfer
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    __delay_us(12);
    
    uint32_t status = (0x000000FF & returned_data[0])
                           | (0x0000FF00 & returned_data[1])
                           | (0x00FF0000 & returned_data[2]);
    
    if (print_out)
    {
        printf("\n\r Data = 0x%08lX", status);
        printf("\n\r---------------------------------------");
        printf("\n\rStatus Register 1");
        printf("\n\r BUSY = %ld,", ((0x00000001 & status) >> 0));
        printf("  1 = busy executing program/erase, most instructions ignored");
        printf("\n\r WEL  = %ld,", ((0x00000002 & status) >> 1));
        printf("  1 = write enable latch, device can be written and erased");
        printf("\n\r BP0  = %ld,", ((0x00000004 & status) >> 2));
        printf("  Block Protect Bits   000 = no portion protected, non-volatile");
        printf("\n\r BP1  = %ld,", ((0x00000008 & status) >> 3));  
        printf("\n\r BP2  = %ld,", ((0x00000010 & status) >> 4));
        printf("\n\r TB   = %ld,", ((0x00000020 & status) >> 5));
        printf("  Top/Bottom select for Block Protect method");
        printf("\n\r SEC  = %ld,", ((0x00000040 & status) >> 6));
        printf("  Sector/Block Protect Bit for Block Protect method");
        printf("\n\r SRP  = %ld,", ((0x00000080 & status) >> 7));
        printf("  Status Register Protect Bit, non-volatile");
        printf("\n\r---------------------------------------");
        printf("\n\rStatus Register 2");
        printf("\n\r SRL  = %ld,", ((0x00000100 & status) >> 8));
        printf("  Status Register Protect Bit, non-volatile");
        printf("\n\r QE   = %ld,", ((0x00000200 & status) >> 9));
        printf("  Quad Enable, non-volatile, 1 = Quad SPI mode");
        printf("\n\r (R)  = %ld,", ((0x00000400 & status) >> 10));
        printf("  Reserved");
        printf("\n\r LB1  = %ld,", ((0x00000800 & status) >> 11));
        printf("  Lock Bits, non-volatile OTP, 000 = unlocked");
        printf("\n\r LB2  = %ld,", ((0x00001000 & status) >> 12));
        printf("\n\r LB3  = %ld,", ((0x00002000 & status) >> 13));
        printf("\n\r CMP  = %ld,", ((0x00004000 & status) >> 14));
        printf("  Complement protect, non-volatile, protection option");
        printf("\n\r SUS  = %ld,", ((0x00008000 & status) >> 15));
        printf("  Suspend Status bit, 1 = Suspend instruction ");
        printf("\n\r---------------------------------------");
        printf("\n\rStatus Register 3");
        printf("\n\r (R)  = %ld,", ((0x00010000 & status) >> 16));
        printf("  Reserved");
        printf("\n\r (R)  = %ld,", ((0x00020000 & status) >> 17));
        printf("  Reserved");
        printf("\n\r WPS  = %ld,", ((0x00040000 & status) >> 18));
        printf("  Write protection selection, non-volatile");
        printf("\n\r (R)  = %ld,", ((0x00080000 & status) >> 19));
        printf("  Reserved");
        printf("\n\r (R)  = %ld,", ((0x00100000 & status) >> 20));
        printf("  Reserved");
        printf("\n\r DRV2 = %ld,", ((0x00200000 & status) >> 21));
        printf("  Driver strength for read operations");
        printf("\n\r DRV1 = %ld,", ((0x00400000 & status) >> 22));
        printf("  00=100, 01=75, 10=50, 11=25 (default)");
        printf("\n\r (R)  = %ld,", ((0x00800000 & status) >> 23));
        printf("  Reserved");
    }
    return status;   
}

/*
 * Description:  This function reads the FLASH and optionally prints the data
 *               to the console.  The address, data, and num_longs parameters
 *               are all interpreted as operating on 32-bit data.  The FLASH
 *               is written and read on 32-bit (long) boundaries only.
 *               The W25Q16JV FLASH contains:
 *                   16,777,216 bits = 2,097,152 Bytes = 524,288 Longs
 *                       = 8192 pages = 32 Blocks
 *                   Page = 256 Bytes = 64 Longs
 *                   Block = 65,536 Bytes = 16,384 Longs
 * 
 *               Page    Address(dec)   Address(hex)
 *                0            0          0x000000
 *                1           64          0x000040
 *                2          128          0x000080
 *                3          192          0x0000C0
 *                4          256          0x000100
 *              ...          ...               ...
 *             8191      524,224          0x07FFCO
 * 
 * Preconditions:  The SPI module that services the FLASH needs to be initialized
 *                 The bit that controls the FLASH CS should be set high with a pullup.
 *                 The UART that services the console needs to be initialized.
 * Arguments:  address is the starting address in the FLASH from where data
 *               will be read from.  It is interpreted as a pointer to longs
 *               within the FLASH.  Even though the FLASH can be read/written
 *               on a byte boundary, these functions do not support that.  All 
 *               data read from or written to the FLASH will be on a 32-bit 
 *               b(4-byte) boundary.
 *             data holds all of the data read from the FLASH.   It must point
 *               to an area of RAM large enough to hold all of the data
 *               prescribed by argument num_longs (number of 32-bit words)
 *             num_longs is the number of 32-bit words
 *             print_out:  true = print summary of results to console
 * Returns:  void
 */    
void ReadFlash(uint32_t address, uint32_t *p_data, uint32_t num_longs, bool print_out)
{
    //This holds the command and the 24-bit address
    uint32_t command_address = ((address<<2) & 0x00FFFFFF) | 0x03000000;  //Address must be shifted left by 2 to get the Byte Address
    
    if (print_out)
    {
        printf("\n\r command_address = 0x%08lX   Num_Longs = %ld", command_address, num_longs);
    }
    
    SPI_SS_FLASH_SetLow();  //Take Select signal low 
    SPI2_Exchange32bit( command_address );  //write command and address to FLASH
    
    if (print_out)
    {
        printf("\n\r  Address         Data");
    }
    
    uint32_t i;
    for (i = 0; i < num_longs; i++)
    {
        *(p_data + i) = SPI2_Exchange32bit( 0x00000000 );  //read data from FLASH, 4 bytes at a time
        if (print_out)
        {
            printf("\n\r  %ld   0x%08lX    0x%08lX", (address+i), (address+i), *(p_data + i));
        }
    }
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    if (print_out)
    {
        printf("\n\r ");
    }
    
}

/*
 * Description:  Same as ReadFlash(), except that the two 16-bit components
 *               of the 32-bit words recorded are swapped.  For example,
 *               instead of setting p_data[n] to 0x5555AAAA, it would set
 *               this to 0xAAAA5555.
 */    
void ReadFlashWordsSwapped(uint32_t address, uint32_t *p_data, uint32_t num_longs)
{
    //This holds the command and the 24-bit address
    uint32_t command_address = ((address<<2) & 0x00FFFFFF) | 0x03000000;  //Address must be shifted left by 2 to get the Byte Address
    
    SPI_SS_FLASH_SetLow();  //Take Select signal low 
    SPI2_Exchange32bit(command_address);  //write command and address to FLASH
    
    SPI2_ExchangeSwappedWords((uint8_t *)p_data, num_longs);
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
}

/*
 * Description:  The LED board EEPROM contains 256 bytes, the default values
 *               of which are stored in flash memory. This function reads
 *               those values from flash memory and stores them in the 
 *               'data' parameter
 * Parameters:
 *    data:      Destination for EEPROM default values read
 */    
void ReadLEDBoardEEPROMDefaultsFromFlash(uint32_t *data)
{
    ReadFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS, data, 64, false);
}

/*
 * Description:  This function writes the FLASH and optionally prints debug info to the console.  The Address,
 *               Data, and Num_Longs are all interpreted as operating on 32-bit data.  The FLASH is written
 *               and read on 32-bit (long) boundaries only.
 *               The W25Q16JV FLASH contains:
 *               16,777,216 bits = 2,097,152 Bytes = 524,288 Longs = 8192 pages = 32 Blocks
 *               Page = 256 Bytes = 64 Longs
 *               Block = 65,536 Bytes = 16,384 Longs
 * 
 *               Page    Address(dec)   Address(hex)
 *                0            0          0x000000
 *                1           64          0x000040
 *                2          128          0x000080
 *                3          192          0x0000C0
 *                4          256          0x000100
 *              ...          ...               ...
 *             8191      524,224          0x07FFCO
 * 
 * Preconditions:  The SPI module that services the FLASH needs to be initialized
 *                 The bit that controls the FLASH CS should be set high with a pullup.
 *                 The UART that services the console needs to be initialized.
 * Arguments:  address is the starting address in the FLASH from which data
 *               will be written to.  It is interpreted as a pointer to longs
 *               within the FLASH.  Even though the FLASH can be read/written
 *               on a byte boundary, these functions do not support that.  All 
 *               data read from or written to the FLASH will be on a 32-bit 
 *               (4-byte) boundary.
 *             data holds all of the data read from the FLASH.   It must point
 *               to an area of RAM large enough to hold all of the data
 *               prescribed by argument num_longs (number of 32-bit words)
 *             num_longs is the number of 32-bit words
 *             print_out:  true = print summary of results to console
 * Returns:  void
 */
void WriteFlash(uint32_t address, uint32_t *p_data, uint32_t num_longs, bool print_out)
{
    uint32_t num_longs_in_first_page, num_longs_in_last_page, num_full_pages, num_longs_to_last_page;   
    
    //Number of longs available between the address and the next page boundary up
    //Calculate some boundaries for programming across pages
    //There are 64 longs per page (aka 256 Bytes per page)
    uint32_t num_longs_to_next_page = ((address & 0xFFFFFFC0) + 0x40) - address;
    
    if (num_longs_to_next_page < num_longs)  //Data is spread across (at least) 2 pages
    {
        num_longs_in_first_page = num_longs_to_next_page;
        num_longs_in_last_page = ((address + num_longs) & 0x0000003F);
        num_full_pages = ((((address + num_longs) & 0xFFFFFFC0) - ((address & 0xFFFFFFC0) + 0x40) ) >> 6);  //excluding the first page, which itself may be a full page but it is not counted in this number
        num_longs_to_last_page = num_longs_in_first_page + 64*num_full_pages;
    }
    else   //All data to be programmed resides in one page
    {
        num_longs_in_first_page = num_longs;
        num_longs_in_last_page = 0;
        num_full_pages = 0;
        num_longs_to_last_page = 0;
    }
    
    if (print_out)
    {
        printf("\n\r Writing FLASH");
        printf("\n\r Num_Longs_to_Next_Page = %ld   Num_Longs_in_First_Page = %ld ",
                num_longs_to_next_page, num_longs_in_first_page);
        printf("\n\r Num_Longs_in_Last_Page = %ld   Num_Full_Pages = %ld ",
                num_longs_in_last_page, num_full_pages);
        printf("\n\r Num_Longs_to_Last_Page = %ld", num_longs_to_last_page);
    }

    // Program the first page of data, this may be a full page or a partial.
    // Either way it is not included in the value of num_full_pages
    
    SPI2CON1Lbits.MODE32 = 0;   //temporarily put this into 8-bit mode, needed for the write enable command
    SPI_SS_FLASH_SetLow();  //Take Select signal low 
    SPI2_Exchange32bit( 0x00000006 );  //write enable command
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    SPI2CON1Lbits.MODE32 = 1;  //put it back into 32-bit mode
      
    //This holds the command and the 24-bit address
    uint32_t command_address = ((address<<2) & 0x00FFFFFF) | 0x02000000;   //OR in the page program command, shift address left by 2 to get the byte address
    SPI_SS_FLASH_SetLow();  //Take Select signal low 
    SPI2_Exchange32bit( command_address ); 
    
    uint32_t i;
    for (i = 0; i < num_longs_in_first_page; i++)
    {
        SPI2_Exchange32bit( *(p_data + i) );  //write data to FLASH, 4 bytes at a time
    }
        
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
        
    if (print_out)
    {
        printf("\n\rWriting data to FLASH at page 0x%08lX\n\r", (command_address & 0x00FFFFFF) );
    }
    uint32_t status = ReadFlashStatus(false); //This holds all 3 status registers
    while (0x00000001 & status)     //Do this while BUSY = 1
    {
        status = ReadFlashStatus(false);
    }
        
    //--------------------------------program all of the full pages--------------------------------
    uint32_t pages;   //loops through the number of full pages when programming
    for (pages=0; pages<num_full_pages; pages++)
    {
        SPI2CON1Lbits.MODE32 = 0;   //temporarily put this into 8-bit mode, needed for the write enable command
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit( 0x00000006 );  //write enable command
        SPI_SS_FLASH_SetHigh();  //Take Select signal high
        SPI2CON1Lbits.MODE32 = 1;  //put it back into 32-bit mode

        command_address = (((address + num_longs_to_next_page + pages*64)<<2) & 0x00FFFFFF) | 0x02000000;   //OR in the page program command, shift address left by 2 to get the byte address
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit( command_address ); 

        uint32_t i;
        for (i = 0; i < 64; i++)
        {
            SPI2_Exchange32bit( *(p_data + num_longs_to_next_page + pages*64 + i) );  //write data to FLASH, 4 bytes at a time
        }

        SPI_SS_FLASH_SetHigh();  //Take Select signal high

        if (print_out)
        {
            printf("\n\rWriting data to FLASH at page 0x%08lX\n\r", (command_address & 0x00FFFFFF) );
        }
        status = ReadFlashStatus(false);
        while (0x00000001 & status)     //Do this while BUSY = 1
        {
            status = ReadFlashStatus(false);
        }
    }
         
    //---------------------------------------------------------------------------------------------
           
    //Program the last page of data, if required
    if (num_longs_in_last_page>0)
    {
        SPI2CON1Lbits.MODE32 = 0;   //temporarily put this into 8-bit mode, needed for the write enable command
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit( 0x00000006 );  //write enable command
        SPI_SS_FLASH_SetHigh();  //Take Select signal high
        SPI2CON1Lbits.MODE32 = 1;  //put it back into 32-bit mode

        command_address = (((address + num_longs_to_last_page )<<2) & 0x00FFFFFF) | 0x02000000;   //OR in the page program command, shift address left by 2 to get the byte address
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit( command_address ); 

        uint32_t i;
        for (i = 0; i < num_longs_in_last_page; i++)
        {
            SPI2_Exchange32bit( *(p_data + num_longs_to_last_page + i ) );  //write data to FLASH, 4 bytes at a time
        }

        SPI_SS_FLASH_SetHigh();  //Take Select signal high
        if (print_out)
        {
            printf("\n\rWriting data to FLASH at page 0x%08lX\n\r", (command_address & 0x00FFFFFF) );
        }
        status = ReadFlashStatus(false);
        while (0x00000001 & status)     //Do this while BUSY = 1
        {
            status = ReadFlashStatus(false);
        }
    }
}

/*
 * Description:  Displays the LED board EEPROM default contents as stored
 *               in flash memory
 */    
void PrintLEDBoardEEPROMDefaultsFromFlash(void)
{
    char data[257];      //This holds the data for the EEPROM and/or FLASH defaults, init with spaces
    uint32_t long_data[64];   //This holds the data for the FLASH
    
    ReadFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS, long_data, 64, 0);
    int i;
    for (i=0; i<32; i++)   //copy from an unsigned long array to character array
    {
       data[i*4] = (char)((long_data[i] & 0xFF000000)>>24);
       data[i*4+1] = (char)((long_data[i] & 0x00FF0000)>>16);
       data[i*4+2] = (char)((long_data[i] & 0x0000FF00)>>8);
       data[i*4+3] = (char)((long_data[i] & 0x000000FF));
    }
    data[128]=0;  //Null terminate the character array to stop the printf at the appropriate character
    printf("\n\r\n\r      Company Name    Product Name    SubAssy Name    SubAssy P/N     SubAssy Rev     SubAssy S/N     Date of Test    Time of Test");
    printf(    "\n\r      00..............10..............20..............30..............40..............50..............60..............70..............");
    printf(    "\n\r      %s", data);

    for(i=32; i<64;i++)   //copy from an unsigned long array to character array
    {
        data[i*4] = (char)((long_data[i] & 0xFF000000)>>24);
        data[i*4+1] = (char)((long_data[i] & 0x00FF0000)>>16);
        data[i*4+2] = (char)((long_data[i] & 0x0000FF00)>>8);
        data[i*4+3] = (char)((long_data[i] & 0x000000FF));
    }
    data[256]=0;  //Null terminate the character array to stop the printf at the appropriate character
    printf("\n\r\n\r      Cal Const #1    Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        ");
    printf(    "\n\r      80..............90..............A0..............B0..............C0..............D0..............E0..............F0..............");
    printf(    "\n\r      %s", &data[128]);
    printf("\n\r");
}

/*
 * Description:  The LED board EEPROM contains 256 bytes, the default values
 *               of which are stored in flash memory. This function reads
 *               those values from flash memory and stores them in the 
 *               'data' parameter
 * Parameters:
 *    data:      Destination for EEPROM default values read
 */    
void StoreLEDBoardEEPROMDefaultsToFlash(void)
{
    const int BUFFER_LEN = 257;
    char data[BUFFER_LEN];       //This holds the data for the EEPROM and/or FLASH defaults, init with spaces
    uint32_t long_data[64];   //This holds the data for the FLASH
    
    EraseFlashPages(EEPROM_DEFAULTS_PAGE, EEPROM_DEFAULTS_PAGE);
    memset(data, 0x20, 257);
    printf("\n\r\n\r      Company Name    Product Name    SubAssy Name    SubAssy P/N     SubAssy Rev     SubAssy S/N     Date of Test    Time of Test");
    printf(    "\n\r      00..............10..............20..............30..............40..............50..............60..............70..............");
    printf(    "\n\rEnter>");
    ReadString(data, BUFFER_LEN);
    int i;
    for (i=0; i<32; i++)   //copy from character array to an unsigned long array
    {
        long_data[i] = (((unsigned long)data[i*4])<<24) | (((unsigned long)data[i*4+1])<<16) |
          (((unsigned long)data[i*4+2])<<8) | (((unsigned long)data[i*4+3]));
    }
    WriteFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS, long_data, 32, false);
    memset(data, 0x20, 257);
    printf("\n\r\n\r      Cal Const #1    Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        Reserved        ");
    printf(    "\n\r      80..............90..............A0..............B0..............C0..............D0..............E0..............F0..............");
    printf(    "\n\rEnter>");
    ReadString(data, BUFFER_LEN);   //Get string from the console (user entry)
    for (i=0; i<32; i++)   //copy from character array to an unsigned long array
    {
        long_data[i] = (((unsigned long)data[i*4])<<24) | (((unsigned long)data[i*4+1])<<16) |
          (((unsigned long)data[i*4+2])<<8) | (((unsigned long)data[i*4+3]));
    }
    WriteFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS+32, long_data, 32, false);  
    printf("\n\rDone\r\n");
}

/*
 * Description:  The LED board EEPROM contains 256 bytes, the default values
 *               of which are stored in flash memory.  One of these parameters
 *               is the default LED current, which will vary from one device
 *               to another. This function prompts the user for the required
 *               LED current and stores it in flash memory.  This value in 
 *               flash should later be transferred to the LED board EEPROM.
 */    
void StoreLEDBoardDefaultCurrentToFlash(void)
{
    uint8_t flash_data[EEPROM_FIELD_LENGTH * NUM_EEPROM_FIELDS];
    
    ReadFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS, (uint32_t *) flash_data, 64, 0);
    
    printf("\n\rEnter default current> ");
    char current_val[EEPROM_FIELD_LENGTH];
    ReadString(current_val, EEPROM_FIELD_LENGTH);   //Get string from the console (user entry)
    // Reverse the byte order for each four-byte word
    int i;
    for (i=0; i<EEPROM_FIELD_LENGTH; i+=4)
    {
        char c = current_val[i];
        current_val[i] = current_val[i+3];
        current_val[i+3] = c;
        c = current_val[i+1];
        current_val[i+1] = current_val[i+2];
        current_val[i+2] = c;
    }
    size_t length = min(strlen(current_val), EEPROM_FIELD_LENGTH);
    memcpy(&flash_data[EEPROM_ADDR_CAL_CONST1], "                ", EEPROM_FIELD_LENGTH);  //First you have to replace the field with spaces
    memcpy(&flash_data[EEPROM_ADDR_CAL_CONST1], current_val, length);     //Then write in the new value

    EraseFlashPages(EEPROM_DEFAULTS_PAGE, EEPROM_DEFAULTS_PAGE);
    WriteFlash(FLASH_ADDR_OF_LED_EEPROM_DEFAULTS, (uint32_t *) flash_data, 64, false);
    printf("\n\rDefault current value stored in flash\r\n");
}


/* Description:  This function erases pages, from one page up to any number that will fit in the
 *               the FLASH, excluding Block 1 which is used as a scratchpad for this function.
 * 
 *               The process:
 *               1. The arguments are verified to be reasonable, otherwise the function returns
 *               2. Erase Block 1
 *               3. Copy block_from to Block 1
 *               4. Erase block_from
 *               5. Copy all data that is not to be erased:  from Block 1 back to block_from
 *               6. Erase all full blocks (blocks that are completely full of data that needs to be erased)
 *               7. Erase Block 1 again
 *               8. Copy block_to to Block 1
 *               9. Erase block_to
 *               10. Copy all data that is not be erased:  from Block 1 back to block_to
 *               
 * 
 *               The W25Q16JV FLASH contains:
 *               16,777,216 bits = 2,097,152 Bytes = 524,288 Longs = 8192 pages = 32 Blocks
 *               Page = 256 Bytes = 64 Longs
 *               Block = 65,536 Bytes = 16,384 Longs = 256 Pages
 * 
 *               Page    Address(dec)   Address(hex)
 *                0            0          0x000000
 *                1           64          0x000040
 *                2          128          0x000080
 *                3          192          0x0000C0
 *                4          256          0x000100
 *              ...          ...               ...
 *             8191      524,224          0x07FFCO
 * 
 * Preconditions:  The SPI module that services the FLASH needs to be initialized
 *                 The bit that controls the FLASH CS should be set high with a pullup.
 *                 The UART that services the console needs to be initialized.
 * Arguments:  Page_From is the first page that will be erased
 *             Page_To is the last page that will be erased
 * Returns:  void
 */
void EraseFlashPages(uint32_t page_from, uint32_t page_to)
{
    uint32_t data[64];  //Temporarily holds one page of data = 64 longs
    
    //Verify arguments are reasonable
    if ( ((page_from > 255) && (page_from < 512)) || (page_from > 8191) )
    {
        printf("\n\r Page number must not be in Block 1 or greater than 8191");
        return;
    }
    
    if ( ((page_to > 255) && (page_to < 512)) || (page_to > 8191) )
    {
        printf("\n\r Page number must not be in Block 1 or greater than 8191");
        return;
    }
    
    if ( page_from > page_to )
    {
        printf("\n\r Starting page is greater than ending page");
        return;
    }
    
    uint32_t first_page_of_block_from = page_from & 0xFFFFFF00;  //the first page within the first block that has data to be erased
    uint32_t first_page_of_block_to = page_to & 0xFFFFFF00;  //the first page within the last block that has data to be erased
    
    uint32_t block_from = ((page_from & 0xFFFFFF00) >> 8); //block number where page_from resides
    uint32_t block_to = ((page_to & 0xFFFFFF00) >> 8); //block number where page_to resides
    
    //Calculate the number of full blocks to be erased (blocks that are completely full of data to be erased)
    uint32_t number_of_full_blocks_to_be_erased;
    if (block_to > (block_from + 1)) {number_of_full_blocks_to_be_erased = block_to - block_from - 1;}  
    else {number_of_full_blocks_to_be_erased = 0;}
    
    EraseFlash(1, 1, 0);  //Erase Block 1
    
    //Copy block_from to Block 1
    uint32_t page;                   //Page number 
    for (page = 0; page<256; page++ )
    {
        ReadFlash(64*(first_page_of_block_from + page), data, 64, 0);  //Read from Block X
        WriteFlash(64*(page+256), data, 64, false);               //Write to Block 1
    }
    
    EraseFlash( (uint16_t)block_from, 1, 0);  //Erase block_from
    
    if (block_from == block_to)
    {
        
        //Now copy back only those pages that are not to be "erased"
        for (page = 0; page < (page_from & 0x000000FF); page++ )
        {
            ReadFlash(64*(page+256), data, 64, 0);                 //Read from Block 1
            WriteFlash(64*(page+first_page_of_block_from), data, 64, false);    //Write to block_from
        }

        for (page = ((page_to & 0x000000FF)+1); page < 256; page++ )
        {
            ReadFlash(64*(page+256), data, 64, 0);                 //Read from Block 1
            WriteFlash(64*(page+first_page_of_block_from), data, 64, false);    //Write to block_from
        }        
    }
    
    if (block_from < block_to)
    {
        //Now copy back only those pages that are not to be "erased"
        for (page = 0; page < (page_from & 0x000000FF); page++ )
        {
            ReadFlash(64*(page+256), data, 64, 0);                 //Read from Block 1
            WriteFlash(64*(page+first_page_of_block_from), data, 64, false);    //Write to block_from
        }
    }
    
    //Erase all full blocks (blocks completely full of data to be erased)
    if (number_of_full_blocks_to_be_erased > 0)
    {
        EraseFlash( (uint16_t)block_from + 1, (uint16_t)number_of_full_blocks_to_be_erased, 0);  
    }
   
    
    if (block_from < block_to)
    {
        
        EraseFlash(1, 1, 0);  //Erase Block 1

        //Copy block_to to Block 1
        for (page = 0; page<256; page++ )
        {
            ReadFlash(64*(first_page_of_block_to + page), data, 64, 0);  //Read from block_to
            WriteFlash(64*(page+256), data, 64, false);               //Write to Block 1
        }

        EraseFlash( (uint16_t)block_to, 1, 0);  //Erase block_to
  
        for (page = ((page_to & 0x000000FF)+1); page < 256; page++ )
        {
            ReadFlash(64*(page+256), data, 64, 0);                 //Read from Block 1
            WriteFlash(64*(page+first_page_of_block_to), data, 64, false);    //Write to block_to
        }        
        
    }

    printf("Erase operation complete\n\r");
}


/*  Description:  This function allows data to be erased from the FLASH, in page boundaries 
 *  Preconditions:  SPI module for the FLASH and LCD interfaces.  UART module will need initialized for com.
 *  Arguments:  void
 *  Returns:  void
 */
void EraseDataFromFlashInPages(void)
{
    printf("\n\r Erase pages from FLASH");
    uint32_t page_from;
    input_status_t status = WaitForValidUInt32(
                        "\n\r Enter starting page number or 'x' to cancel >",
                        &page_from);
    if (status == INPUT_CANCELED)
    {
        printf("\r\nOperation canceled\r\n");
    }
    else
    {
        uint32_t page_to;
        status = WaitForValidUInt32(
                        "\n\r Enter ending page number or 'x' to cancel >",
                        &page_to);
        if (status == INPUT_CANCELED)
        {
            printf("\r\nOperation canceled\r\n");
        }
        else
        {
            printf("\n\r");
            EraseFlashPages(page_from, page_to);
        }
    }
}


/*  Description:  This function reads the contents of the specified flash
 *                memory location
 */
void ReadDataFromFlash(void)
{
    uint32_t data[1024]={0};
    
    printf("\n\r Read data from FLASH");
    uint32_t address;
    input_status_t status = WaitForValidUInt32(
                                "\n\r Enter address (in decimal) or 'x' to cancel > ",
                                &address);
    if (status == INPUT_CANCELED)
    {
        printf("\r\nOperation canceled\r\n");
    }
    else
    {
        uint32_t num_longs;
        status = WaitForValidUInt32WithLimits(
                    "\n\r Enter number of 32-bit data entries to read (1 to 1024) or 'x' to cancel > ",
                    &num_longs, 0, 1024);
        if (status == INPUT_CANCELED)
        {
            printf("\r\nOperation canceled\r\n");
        }
        else
        {
            ReadFlash(address, data, num_longs, true);
        }
    }
}


/*  Description:  This function allows data to be written to the FLASH
 *  Preconditions:  SPI module for the FLASH and LCD interfaces.  UART module will need initialized for com.
 *  Arguments:  void
 *  Returns:  void
 */
void WriteDataToFlash(void)
{
    printf("\n\r Write data to FLASH");
    uint32_t address;
    input_status_t status = WaitForValidUInt32(
                                "\n\r Enter address (in decimal) or 'x' to cancel > ",
                                &address);    
    if (status == INPUT_CANCELED)
    {
        printf("\r\nOperation canceled\r\n");
    }
    else
    {
        uint32_t num_longs;
        status = WaitForValidUInt32WithLimits(
                    "\n\r Enter number of 32-bit values to write (0 to 1024) or 'x' to cancel > ",
                    &num_longs, 0, 1024);
        printf("\n\r");
        if ((status == INPUT_CANCELED) || (num_longs == 0))
        {
            printf("\r\nOperation canceled\r\n");
        }
        else
        {
            uint32_t data[1024]; 
            input_status_t status = INPUT_INVALID;
            uint32_t i;
            for (i=0; (i < num_longs) && (status != INPUT_CANCELED); i++)
            {
                status = INPUT_INVALID;
                while (status == INPUT_INVALID)
                {
                    printf("\n\rEnter value #%lu or 'x' to cancel >", i);
                    status = ReadUInt32OrCancel(data+i);
                    if (status == INPUT_INVALID)
                    {
                        printf("\r\nInvalid input");
                    }
                }
            }
            if (status == INPUT_CANCELED)
            {
                printf("\r\nOperation canceled\r\n");
            }
            else
            {
                WriteFlash(address, data, num_longs, false);
            }
        }
    }
}


/*  Description:  This function allows data to be erased from the FLASH 
 *  Preconditions:  SPI module for the FLASH and LCD interfaces.  UART module will need initialized for com.
 *  Arguments:  void
 *  Returns:  void
 */
void EraseDataFromFlash()
{
    printf("\n\r Erase 64kByte blocks from FLASH");
    uint16_t starting_block;
    input_status_t status = WaitForValidUInt16(
            "\n\r Enter starting block number or 'x' to cancel > ",
            &starting_block);
    if (status == INPUT_CANCELED) 
    {
        printf("\r\nOperation canceled\r\n");
    }
    else
    {
        uint16_t num_blocks;
        status = WaitForValidUInt16(
            "\n\r Enter the number of 64kByte blocks to erase or 'x' to cancel > ",
            &num_blocks);
        if ((status == INPUT_CANCELED) || (num_blocks == 0))
        {
            printf("\r\nOperation canceled\r\n");
        }
        else
        {
            printf("\n\r");
            EraseFlash((uint16_t) starting_block, (uint16_t) num_blocks, 1);
        }
    }
 }


/* Description:  This function allows an image file (24-bit BMP) to be uploaded to the FLASH. It is designed to 
 *               work with a host-based utility such as the Python app that was written to support it.  The Python 
 *               app is run on the host PC that is connected to the Control Board via serial port.  The BMP file
 *               is input to the Python App.  The Python app is in the Python subdirectory.
 * Preconditions:  SPI module for the FLASH and LCD interfaces.  UART module will need initialized for com.
 * Arguments:  void
 * Returns:  void
 * 
 * 
 *   (row, col)
 * 
 *   (0,0)-----------------------------------------------------(320,0)
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *     |                                                           |
 *   (0,240)---------------------------------------------------(320,240)   
 * 
 *   Page = 256 Bytes = 128 Pixels = 64 Longs
 * 
 */  
void WriteImageToFlash(void)
{
    const uint32_t OFFSET = 54;      //This is the amount of header information in the bmp file, pixel data starts at 54

    IEC0bits.AD1IE = 0;   // Disabling ADC1 interrupt.
    IEC3bits.RTCIE = 0;   // Disabling RTCC interrupt

    
    /* We can use ReadUInt32() here because the data is expected to come
     * from an automated script rather than human input.  This script MUST
     * use valid numeric values, since ReadUInt32() does not check the
     * validity of the data received.  */
    printf("SEND_IDENTIFIER");  //This tells the host PC software to send the identifier of the image
    uint32_t identifier = ReadUInt32(); //Unique numerical identifier of the image
    printf("SEND_DESCRIPTION");  //This tells the host PC software to send the description of the image (255 char max)
    const int MAX_STR_LEN = 256;
    char description[MAX_STR_LEN];   //Description of image
    ReadString(description, MAX_STR_LEN);
    printf("SEND_STARTING_PAGE");  //This tells the host PC software to send the starting page in FLASH
    uint32_t starting_page = ReadUInt32();
    printf("SEND_XPOS");  //This tells the host PC software to send the XPOS for the image.
    uint32_t xpos = ReadUInt32();
    printf("SEND_YPOS");  //This tells the host PC software to send the YPOS for the image
    uint32_t ypos = ReadUInt32();
    printf("SEND_XDIM");  //This tells the host PC software to send the XDIM for the image
    uint32_t xdim = ReadUInt32();
    printf("SEND_YDIM");  //This tells the host PC software to send the YDIM for the image
    uint32_t ydim = ReadUInt32();
    printf("SEND_DATESTAMP");  //This tells the host PC software to send the current date
    char date_stamp[MAX_STR_LEN];   //Current date and time
    ReadString(date_stamp, MAX_STR_LEN);
    
    //Calculate the total number of complete pages (256 bytes or 64 Longs) contained in the image data
    //that will be written to the FLASH.  The pixel data will be compressed by this function before it 
    //is written to the FLASH
    uint32_t total_pixels = xdim*ydim;  //total number of pixels in the image
    uint32_t total_longs = total_pixels / 2;    //If Total_Pixels is odd, then we lose one pixel, but we account for that later
    uint32_t total_pages = total_longs / 64;    //Total_Pages does not account for partial pages, but we account for that later
   
    //BMP format will pad 0x00 bytes to the end of each row so that it ends on a 4 byte boundary
    //Calculate the number of bytes that the BMP image appended to each row
    //These bytes will be zeroes and they will need to be removed
    uint32_t bytes_to_remove_per_row = (((xdim*3) % 4) == 0 ? 0 : 4 - ((xdim*3) % 4));
    
    uint32_t rows_per_page = 128/xdim;   //this will truncate (round down) to the nearest integer
    uint32_t pages_per_row = xdim/128;   //this will truncate (round down) to the nearest integer
    uint32_t pixels_in_last_page = total_pixels - total_pages*128;
    
    printf("\n\rIdentifier = %ld", identifier);
    printf("\n\rDescription = %s", description);
    printf("\n\rStarting_Page = %ld", starting_page);
    printf("\n\rDateStamp = %s", date_stamp);
    printf("\n\rXPOS = %ld", xpos);
    printf("\n\rYPOS = %ld", ypos);
    printf("\n\rXDIM = %ld", xdim);
    printf("\n\rYDIM = %ld", ydim);
    printf("\n\rTotal_Pixels = %ld", total_pixels);
    printf("\n\rTotal_Longs = %ld", total_longs);
    printf("\n\rTotal_Pages = %ld", total_pages);
    printf("\n\rRows_per_Page = %ld", rows_per_page);
    printf("\n\rPages_per_Row = %ld", pages_per_row);
    printf("\n\rBytes_to_Remove_per_Row = %ld", bytes_to_remove_per_row);
    printf("\n\rPixels_in_Last_Page = %ld",pixels_in_last_page);
    printf("\n\rEnd of Report");

    //-----------------------------------------------------------------------------------------------
    //Write pages to the FLASH
    uint32_t pixel_counter = 0;   //initialize pixel counter to zero
    uint32_t byte_counter = 0;    //initialize byte counter to zero

    uint32_t flash_data[64];          //holds one page of data 
    uint32_t page;
    for (page=0; page<total_pages; page++) 
    {
        char c=0;
        while (c != 'N')     //wait until host is ready to send more data
        {
            c = UART1_Read();
        }    
        
        uint32_t bytes_to_request_per_page = 0;       //Number of bytes for host to send
        //Need 128 pixels for the FLASH plus all the zero padding that bmp format puts in there
        //Note that the worst case (most padding) occurs when the end of one row falls on the beginning of a page
        if (rows_per_page>0)
        {
            bytes_to_request_per_page = 3*128 + (rows_per_page + 1)*bytes_to_remove_per_row;
        }  
        //When there are multiple pages per row, only some pages will need extra data requested due to BMP padding
        if (pages_per_row>0)
        {
            bytes_to_request_per_page = 3*128 + bytes_to_remove_per_row*((page>0)&&((page % pages_per_row)==0));
        }
        
        uint32_t starting_index = OFFSET + byte_counter;  //starting index into the image file
        printf("S%ldQ%ldP%ldE", starting_index, bytes_to_request_per_page, page);  //Request enough data to fill one page 
        
        //Read all the data into a local array, which can be no larger than 128 pixels + 128*(3 Bytes)
        //We're doing this so the host can send all data immediately and we can consume it immediately
        //Don't have to worry about UART flow control
        uint8_t bytes_from_host[768];    //holds one page worth of bytes from the host, 1 page = 128 pixels, each pixel coming from the host is 3 Bytes and the maximum amount of BMP padding is 3 Bytes, so 128 * 6
        uint32_t i;
        
           
        for (i=0; i<bytes_to_request_per_page; i++)
        {
            bytes_from_host[i] = UART1_Read(); 
        }
        
        //Process 128 pixels, discard padding that the bmp file format inserted
        uint32_t byte_rollover_counter = 0;
        uint32_t pixel_rollover_counter = 0;
        uint32_t pixels[128]; //holds one page of pixels
        while (pixel_rollover_counter < 128)   //Stay in the loop until all 128 pixels have been read out
        {
            //Read in one pixel's worth of RGB data
            uint8_t B1 = bytes_from_host[byte_rollover_counter++];
            uint8_t G1 = bytes_from_host[byte_rollover_counter++];
            uint8_t R1 = bytes_from_host[byte_rollover_counter++];
            byte_counter += 3; 
            
            //Compress the 24 bit RGB data into 16 bit RGB data 
            pixels[pixel_rollover_counter] =     (((uint32_t)(B1))/8) +       //Blue   
                                                ((((uint32_t)(G1))/4)<<5) +  //Green
                                                ((((uint32_t)(R1))/8)<<11);  //Red


            //Remove any padding that the bmp format put in the file at the end of the row
            if ((pixel_counter % xdim) == (xdim - 1))   //detect the end of a row
            {
                uint32_t pad;
                for (pad=0; pad<bytes_to_remove_per_row; pad++) {byte_counter++; byte_rollover_counter++;}
            }
            pixel_counter++; pixel_rollover_counter++;    
        }
        //Now write the Pixel data to the FLASH
        for (i=0; i<64; i++)
        {
            flash_data[i] = ((pixels[2*i])<<16) + pixels[2*i+1];
        }
        WriteFlash((starting_page+page)*64, flash_data, 64, false);        
    }
    
    //-----------------------------------------------------------------------------------------------
    //Now write the last partial page of data to the FLASH
    //Number of bytes for host to send    
    uint32_t bytes_to_request_for_last_page = (xdim*3 + bytes_to_remove_per_row)*ydim - byte_counter;
    
    if (bytes_to_request_for_last_page != 0)
    {
        char c = 0;
        while (c != 'N')     //wait until host is ready to send more data
        {
            c = UART1_Read();
        }   

        uint32_t starting_index = OFFSET + byte_counter;  //starting index into the image file
        printf("S%ldQ%ldP%ldE", starting_index, bytes_to_request_for_last_page, total_pages);  //Request enough data to fill one page 

        //Read all the data into a local array, which can be no larger than 128 pixels + 128*(3 Bytes)
        //We're doing this so the host can send all data immediately and we can consume it immediately
        //Don't have to worry about UART flow control
        uint8_t bytes_from_host[768];    //holds one page worth of bytes from the host, 1 page = 128 pixels, each pixel coming from the host is 3 Bytes and the maximum amount of BMP padding is 3 Bytes, so 128 * 6
        uint32_t i;
        for (i=0; i<bytes_to_request_for_last_page; i++)
        {
            bytes_from_host[i] = UART1_Read(); 
        }

        //Process pixels, discard padding that the bmp file format inserted
        uint32_t pixel_rollover_counter = 0;   //initialize pixel counter to zero
        uint32_t byte_rollover_counter = 0;    //initialize byte counter to zero
        uint32_t pixels[128]; //holds one page of pixels
        while (pixel_rollover_counter < pixels_in_last_page)   //Stay in the loop until all pixels have been read out
        {
            //Read in one pixel's worth of RGB data
            uint8_t B1 = bytes_from_host[byte_rollover_counter++];
            uint8_t G1 = bytes_from_host[byte_rollover_counter++];
            uint8_t R1 = bytes_from_host[byte_rollover_counter++];
            byte_counter += 3;

            //Compress the 24 bit RGB data into 16 bit RGB data 
            pixels[pixel_rollover_counter] =  (((uint32_t)(B1))/8) +     //Blue   
                                             ((((uint32_t)(G1))/4)<<5) + //Green
                                             ((((uint32_t)(R1))/8)<<11); //Red

            //Remove any padding that the bmp format put in the file at the end of the row
            if ((pixel_counter % xdim) == (xdim - 1))   //detect the end of a row
            {
                uint32_t pad;
                for (pad=0; pad<bytes_to_remove_per_row; pad++) {byte_counter++; byte_rollover_counter++;}
            }
            pixel_counter++; pixel_rollover_counter++;
        }

        //Now write the Pixel data to the FLASH, convert pixels to longs by dividing by 2.  Adding 1 to this in case of padding
        for (i=0; i<((pixels_in_last_page/2)+1); i++) {flash_data[i] = ((pixels[2*i])<<16) + pixels[2*i+1];}

        WriteFlash((starting_page+total_pages)*64, flash_data, (pixels_in_last_page/2)+1, false);
    }
    
    //Now update the Image listing in the FLASH
    //Each image that is stored in FLASH has a corresponding entry in the Image List.  The Image List is also
    //stored in FLASH and kind of acts like a file allocation table.  Each entry in the Image List is 1 page = 64 longs.
    //We first have to search for the next available entry and then write the new entry
    uint32_t i;
    // TODO: Work around break statement below
    //bool unprogrammed_page_found = false; 
    for (i=0; (i<256) ; i++)
    {
        ReadFlash(i*64, flash_data, 1, 0);
        if (flash_data[0] == 0xFFFFFFFF)
        {
            break; // unprogrammed_page_found = true;
        }
    }
    
    //Write entry into the next unprogrammed page, each entry takes up one page and is defined, in order, as)
    //Identifier = 1 long
    //Description = 20 longs = 80 characters
    //DateStamp = 10 longs = 40 characters
    //Starting_Page = 1 long
    //Total_Pages = 1 long
    //XDIM = 1 long
    //YDIM = 1 long
    //XPOS = 1 long
    //YPOS = 1 long

    WriteFlash(i*64, &identifier, 1, false);
    WriteFlash(i*64+1, (uint32_t *)description, 20, false);
    WriteFlash(i*64+21, (uint32_t *)date_stamp, 10, false);
    WriteFlash(i*64+31, &starting_page, 1, false);
    WriteFlash(i*64+32, &total_pages, 1, false);
    WriteFlash(i*64+33, &xdim, 1, false);
    WriteFlash(i*64+34, &ydim, 1, false);
    WriteFlash(i*64+35, &xpos, 1, false);
    WriteFlash(i*64+36, &ypos, 1, false);

    IEC0bits.AD1IE = 1;   // Enabling ADC1 interrupt.
    IEC3bits.RTCIE = 1;   // Enabling RTCC interrupt
    
    //      01234567890123456789  Number of characters is important since the MCU
    //                            will send more characters after exiting this function
    printf("Completed Image Load");  //Tell host we are done requesting data
}


/* Description:  Reads the specified number of 16-bit integers from flash
 *               memory and copies them to the RAM.  This is done via
 *               DMA for the sake of speed.
 * Preconditions:  DMA controller must be initialized.  Ideally, SPI port 2
 *                 should also be initialized, although in the current
 *                 implementation, we re-initalize it at the start of this
 *                 function.
 * Parameters:
 *     address:    Flash memory address for source data
 *     p_data:     Pointer to destination within RAM
 *     num_ints:   Number of 16-bit values to copy
 */
void ReadFlashDMA(uint32_t address, uint16_t *p_data, uint16_t num_ints)
{  
    // This holds the command and the 24-bit address
    uint32_t command_address = ((address<<1) & 0x00FFFFFF) | 0x03000000;  
    
    SPI2_Initialize(); // This is probably overkill. Not sure why this
                       // is needed to prevent hanging.
                       // TODO: Identify which resets are needed
    SPI_SS_FLASH_SetLow();  //Take Select signal low 
    SPI2_Exchange32bit(command_address);  //write command and address to FLASH
    
#ifdef DISABLE_INTERRUPTS_DURING_DMA
    INTCON2bits.GIE = 0;
#endif
    // If we don't set PRSSEL here, we occasionally get blank
    // data.  Don't know why, since this flag shouldn't be changing.
    DMACONbits.PRSSEL = 1;
    // For some reason, we have to set DMAH and DMAL here to
    // prevent the images from being garbled.  Heaven knows why,
    DMAH = 0x5000; // From manual. See if correct.
    DMAL = 0x0;  // Set extra low
    DMACH0 = 0;
    DMACH0bits.CHEN = 0;
    
    DMASRC0 = (uint16_t) &SPI2BUFL;
    /* For some reason, the first 16 ints are garbage.  Until we figure out
       why, let's just empty the buffer. */
    volatile uint16_t dummy[DMA_BUFFER_SIZE];
    DMADST0 = (unsigned short int) dummy;
    DMACNT0 = DMA_BUFFER_SIZE;
    DMACH0bits.SIZE = 0;
    DMACH0bits.TRMODE = 2;
    DMACH0bits.SAMODE = 0; 
    DMACH0bits.DAMODE = 1;
    DMACH0bits.NULLW = 1;
    SPI2CON1Lbits.ENHBUF = 1;
    SPI2CON1Lbits.MODE32 = 0;
    SPI2CON1Lbits.MODE16 = 1;

    __delay_ms(1);
    while (!SPI2STATLbits.SPITBE) ;
    DMACH0bits.CHEN = 1;
    DMAINT0bits.DONEIF = 0;
    DMACH0bits.CHREQ = 1;
    while (DMACNT0) ;

    IO_RE7_SetHigh();
    
    uint16_t i;
    for (i = 0; i < num_ints; i += DMA_BUFFER_SIZE)
    {
        DMACNT0 = ((num_ints - i) < DMA_BUFFER_SIZE ? (num_ints - i) : DMA_BUFFER_SIZE);
        DMADST0 = (unsigned short int) &p_data[i];
        
        while (!SPI2STATLbits.SPITBE) ;
        DMACH0bits.CHEN = 1;
        DMAINT0bits.DONEIF = 0;
        DMACH0bits.CHREQ = 1;
        while (DMACNT0) ;
    }
    
    IO_RE7_SetLow();
    
#ifdef DISABLE_INTERRUPTS_DURING_DMA
    INTCON2bits.GIE = 1;
#endif
    SPI_SS_FLASH_SetHigh();  //Take Select signal high
    SPI2CON1Lbits.MODE32 = 1;
    SPI2CON1Lbits.MODE16 = 0;
   
    /* Reset SPI interface and clear overflow flag.
     * NOTE: This may have fixed the problem with occasional misreads,
     *       thus rendering the other safeguards (re-trying apparently
     *       invalid reads) unnecessary.  We'll keep those other 
     *       safeguards in place for now, though.
     * TODO: Figure out why this is necessary. Without resetting,
     * subsequent flash read operations fail.  Perhaps this
     * is always needed after using DMA?  */
    SPI2CON1Lbits.SPIEN = 0;
    SPI2STATLbits.SPIROV = 0; 
    SPI2CON1Lbits.SPIEN = 1;
}

// ---------------- PRIVATE FUNCTIONS ----------------

/*   
 *   This is meant to be a more efficient variation on the automatically
 *   generated SPI2_Exchange() function in spi2.c.  It has been
 *   implemented here to avoid modifying the automatically generated files.
 * 
 *   Because it closely parallels SPI2_Exchange(), the naming convention
 *   has been loosened for this function.
 */
static void SPI2_ExchangeSwappedWords(uint8_t *p_recvdata, uint16_t num_longs)
{
    uint8_t i;
    for (i=0; i<num_longs; i++)
    {
        while (SPI2STATLbits.SPITBF) ;

        SPI2BUFL = 0; 
        SPI2BUFH = 0; 

        while (SPI2STATLbits.SPIRBE) ;

        // Appears that SPI2BUFL should be read first. Verify this.
        *((uint16_t*) (p_recvdata+4*i+2)) = SPI2BUFL;
        *((uint16_t*) (p_recvdata+4*i)) = SPI2BUFH;
    }
}


/* Description:  This function erases the FLASH and optionally prints debug info to the console.  
 *              The FLASH is erased one block at a time, each block is 1/32 of the entire memory
 *              
 *              The W25Q16JV FLASH contains:
 *              16,777,216 bits = 2,097,152 Bytes = 524,288 Longs = 8192 pages = 32 Blocks
 *              Page = 256 Bytes = 64 Longs
 *               Block = 65,536 Bytes = 16,384 Longs
 * 
 *               Page    Address(dec)   Address(hex)
 *                0            0          0x000000
 *                1           64          0x000040
 *                2          128          0x000080
 *                3          192          0x0000C0
 *                4          256          0x000100
 *              ...          ...               ...
 *             8191      524,224          0x07FFCO
 * 
 * Preconditions:  The SPI module that services the FLASH needs to be initialized
 *                 The bit that controls the FLASH CS should be set high with a pullup.
 *                 The UART that services the console needs to be initialized.
 * Arguments:  starting_block is the first block to be erased, it can have a value from 0 to 31
 *             num_blocks is the number of blocks to erase, it can have a value from 1 to 32
 *             print_out:  1 = print summary of results to console
 * Returns:  void
 */
static void EraseFlash(uint16_t starting_block, uint16_t num_blocks, bool print_out)
{
    if (starting_block > 31)
    { 
        starting_block = 31;
    }
    if (num_blocks > 32)
    { 
        num_blocks = 32;
    }
    if ((starting_block + num_blocks) > 32)
    {
        num_blocks = 32 - starting_block;  //Otherwise the erase command will wrap around
    }  
    
    int i;      //general working variable for loops
    for (i = 0; i<num_blocks; i++ )
    {
        SPI2CON1Lbits.MODE32 = 0;   //temporarily put this into 8-bit mode, needed for the write enable command
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit(0x00000006);  //write enable command
        SPI_SS_FLASH_SetHigh();  //Take Select signal high
        SPI2CON1Lbits.MODE32 = 1;  //put it back into 32-bit mode

        //starting_block = 0 to 31
        int block_num = starting_block+i;
        uint32_t address = 65536*(block_num);
        uint32_t command_address = (address & 0x00FFFFFF) | 0xD8000000;
        SPI_SS_FLASH_SetLow();  //Take Select signal low 
        SPI2_Exchange32bit(command_address); 
        SPI_SS_FLASH_SetHigh();  //Take Select signal high

        if (print_out)
        {
            printf("\n\rErasing FLASH Block %d starting at address 0x%08lX\n\r", block_num, address);
        }
        uint32_t status = ReadFlashStatus(false); //This holds all 3 status registers
        while (0x00000001 & status)     //Do this while BUSY = 1
        {
            if (print_out)
            {
                printf("Erasing FLASH Block %d starting at address 0x%08lX\n\r", block_num, address);
            }
            status = ReadFlashStatus(false);
            __delay_ms(12);   //add a little delay so we don't print a whole bunch of messages to the screen
        }
    }
}

