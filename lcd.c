/*	Filename:  lcd.c
 *	Date Created:  12/5/2018
 *	Company:  Valtronic, USA
 *	Project:   Lumitex OraGlo Control Board
 *	Project number:  DD400004
 *	Description:  Routines for controlling the LCD screen
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>

#include "mcc_generated_files/mcc.h"
#include "lcd.h"
#include "flashmem.h"
#include "ledboard.h"
#include "rgbled.h"
#include "Support_Functions.h"
#include "UI_Functions.h"
#include "Image_Arial20_Vert.h"

// Included for the sake of Test_5VDC().  Can remove
// this if we move Test_5VDC() elsewhere).
#include "Test_Functions.h"

#define    FCY    16000000UL  // required by libpic30.h    
#include <libpic30.h>

#define IMG_BUFFER_SIZE 640

#define USE_FLASH_DMA
#define USE_LCD_DMA

// Un-comment out the following line to disable interrupts during DMA transfers
#define DISABLE_INTERRUPTS_DURING_DMA

#define TFT_LCD_5VDC_LOW_WINDOW 0.001

#define TFT_LCD_5VDC_HIGH_WINDOW 0.200

static const uint16_t LCD_WIDTH  = 320;
static const uint16_t LCD_HEIGHT = 240;

static const uint16_t BKGRND_COLOR = 0x20E4; // 0x18A2;

/*  Have to add additional buffer entres so that we can clock in some
 *  extra data in DisplayImage(). See comments below for explanation. */
uint16_t img_buffer[IMG_BUFFER_SIZE+2];

static void LCDWriteDataDMA(const uint16_t *p_buffer, uint16_t num_ints);
static void InitLCDRegionWrite(uint32_t xpos, uint32_t ypos,
                                  uint32_t xdim, uint32_t ydim);
static bool LocateImage(uint32_t identifier, uint32_t *starting_page,
               uint32_t *xdim, uint32_t *ydim, uint32_t *xpos, uint32_t *ypos,
               bool print_out);
static void DisplayImage(uint32_t starting_page, uint32_t xdim, uint32_t ydim,
                         uint32_t xpos, uint32_t ypos);

static void LCDWriteData16bits(uint16_t data);


// TODO: Describe  at high level
void InitializeLCDDMA(void)
{
    //DMACONbits.DMAEN = 1;
    DMACONbits.PRSSEL = 1;
    DMAH = 0x5000; // From manual. Establish better limit if possible
    DMAL = 0x0;  // Set extra low to allow writing to SFRs
    DMACH1 = 0;
    DMACH1bits.SAMODE = 1; 
    DMACH1bits.DAMODE = 0;
    DMACH1bits.TRMODE = 2;
    DMACH1bits.SIZE = 0;
    DMACH1bits.NULLW = 0;
    DMADST1 = (uint16_t) &SPI3BUFL;
    SPI3CON1Lbits.MODE16 = 1; // Temporarily enable 16-bit data
                              // transfers as required for DMA
}


/* Description:  This function writes a command to the LCD
 * Preconditions:  SPI3 has been initialized and the digital IO bit that
 *                 drives the DC_LCD bit to the LCD has been initialized
 * Arguments:  Command bit field for the LCD.  See LCD datasheet for
 *             command definitions
 */
void LCDWriteCommand(uint8_t command)
{
    DC_LCD_SetLow();   // command = 0
    __delay32(120);
    SPI3_Exchange8bit(command);
    __delay32(120); 
}

/* Description:  This function writes data to the LCD
 * Preconditions:  SPI3 has been initialized and the digital IO bit that drives
 *                 the DC_LCD bit to the LCD has been initialized
 * Arguments:  Data bit field for the LCD, see LCD datasheet for command
 *             definitions and their associated data
 */
void LCDWriteData(uint16_t data)
{
    DC_LCD_SetHigh();    // Data = 1
    SPI3_Exchange8bit(data >> 8);
    SPI3_Exchange8bit(data);
}


static void LCDWriteData16bits(uint16_t data)
{
    DC_LCD_SetHigh();    // Data = 1
    while (SPI3STATLbits.SPITBF) ;
    SPI3BUFL = data;
    while (SPI3STATLbits.SPIRBE) ;
    SPI3BUFL;
    while (!SPI3STATLbits.SPITBE) ; // Wait for SPI buffer to empty
}


/* Description:  This function initializes the LCD.  This code was originally 
 *               copied from the crystalfontz example code
 * Preconditions:  SPI3 has been initialized and the digital IO bit that 
 *                 drives the DC_LCD bit to the LCD has been initialized
 */
// TODO: Replace the magic numbers with symbolic constants if we have time
void InitializeLCD(void)
{
	LCDWriteCommand(0x0028);    // VCOM OTP
	LCDWriteData(0x0006);       // Page 55-56 of SSD2119 datasheet

	LCDWriteCommand(0x0000);    // start Oscillator
	LCDWriteData(0x0001);       // Page 36 of SSD2119 datasheet

	LCDWriteCommand(0x0010);    // Sleep mode
	LCDWriteData(0x0000);       // Page 49 of SSD2119 datasheet. Sleep mode off.

	LCDWriteCommand(0x0001);    // Driver Output Control
	LCDWriteData(0x32EF);       // Page 36-39 of SSD2119 datasheet

	LCDWriteCommand(0x0002);    // LCD Driving Waveform Control
	LCDWriteData(0x0600);       // Page 40-42 of SSD2119 datasheet

	LCDWriteCommand(0x0003);    // Power Control 1
	LCDWriteData(0x6A38);       // Page 43-44 of SSD2119 datasheet

	LCDWriteCommand(0x0011);    // Entry Mode
	LCDWriteData(0x6870);       // Page 50-52 of SSD2119 datasheet, allows screen to flipped in X or Y direction

	LCDWriteCommand(0X000F);    // Gate Scan Position
	LCDWriteData(0x0000);       // Page 49 of SSD2119 datasheet

	LCDWriteCommand(0X000B);    // Frame Cycle Control
	LCDWriteData(0x5308);       // Page 45 of SSD2119 datasheet

	LCDWriteCommand(0x000C);    // Power Control 2
	LCDWriteData(0x0003);       // Page 47 of SSD2119 datasheet

	LCDWriteCommand(0x000D);    // Power Control 3
	LCDWriteData(0x000A);       // Page 48 of SSD2119 datasheet

	LCDWriteCommand(0x000E);    // Power Control 4
	LCDWriteData(0x2E00);       // Page 48 of SSD2119 datasheet

	LCDWriteCommand(0x001E);    // Power Control 5
	LCDWriteData(0x00B7);       // Page 55 of SSD2119 datasheet

	LCDWriteCommand(0x0025);    // Frame Frequency Control
	LCDWriteData(0x8000);       // Page 53 of SSD2119 datasheet

	LCDWriteCommand(0x0026);    // Analog setting
	LCDWriteData(0x3800);       // Page 54 of SSD2119 datasheet
	
	LCDWriteCommand(0x0027);    // Critical setting to avoid pixel defect
	LCDWriteData(0x0078);       // per solomon systech, apparently undocumented.

	LCDWriteCommand(0x004E);    // Ram Address Set
	LCDWriteData(0x0000);       // Page 58 of SSD2119 datasheet

	LCDWriteCommand(0x004F);    // Ram Address Set
	LCDWriteData(0x0000);       // Page 58 of SSD2119 datasheet

	LCDWriteCommand(0x0012);    // Sleep mode
	LCDWriteData(0x0D99);       // Page 49 of SSD2119 datasheet

	// Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
	LCDWriteCommand(0x0030);
	LCDWriteData(0x0000);

	LCDWriteCommand(0x0031);
	LCDWriteData(0x0104);

	LCDWriteCommand(0x0032);
	LCDWriteData(0x0100);

	LCDWriteCommand(0x0033);
	LCDWriteData(0x0305);

	LCDWriteCommand(0x0034);
	LCDWriteData(0x0505);

	LCDWriteCommand(0x0035);
	LCDWriteData(0x0305);

	LCDWriteCommand(0x0036);
	LCDWriteData(0x0707);

	LCDWriteCommand(0x0037);
	LCDWriteData(0x0300);

	LCDWriteCommand(0x003A);
	LCDWriteData(0x1200);

	LCDWriteCommand(0x003B);
	LCDWriteData(0x0800);		 

	LCDWriteCommand(0x0007);    // Display Control 
	LCDWriteData(0x0033);       // Page 45 of SSD2119 datasheet
	
    __delay32(600);

	LCDWriteCommand(0x0022);    // RAM data write/read
}

/* Description:  This function initializes the display position.  This 
 *               was originally copied from the crystalfontz example code.
 * Preconditions:  SPI3 has been initialized and the digital I/O bit that
 *                 drives the DC_LCD bit to the LCD has been initialized
 */
void LCDDisplayHome(void)
{
  	LCDWriteCommand(0x004E);    // RAM address set
	LCDWriteData(0x0000);       // Page 58 of SSD2119 datasheet
	LCDWriteCommand(0x004F);    // RAM address set
	LCDWriteData(0x0000);       // Page 58 of SSD2119 datasheet

	LCDWriteCommand(0x0044);    // Vertical RAM address position
	LCDWriteData(0xEF00);       // Page 57 of SSD2119 datasheet
	LCDWriteCommand(0x0045);    // Horizontal RAM address position 
	LCDWriteData(0x0000);       // Page 57 of SSD2119 datasheet
	LCDWriteCommand(0x0046);    // Horizontal RAM address position
	LCDWriteData(0x013F);       // Page 57 of SSD2119 datasheet

	LCDWriteCommand(0x0022);    // RAM data write/read  
}


/* Description:  This function initializes the display with striped colors, its a test image
 * this code was originally copied from the crystalfontz example code
 * Preconditions:  SPI3 has been initialized and the digital IO bit that drives the DC_LCD bit to the LCD has been initialized
 *
 * NOTE: Resetting the SPI3 interface appears to be necessary on occasion
 *       TODO: Figure out why if possible
 */
void DisplayTestImage(void)
{
    SPI3_Initialize();   
 	LCDDisplayHome();
    InitializeLCDDMA();   
    DC_LCD_SetHigh(); // Enable sending data to LCD flash
    
    memset(img_buffer, 0, 2*DMA_BUFFER_SIZE); // Set the pixel colors to black
    int i;
	for (i=0; i<LCD_WIDTH; i++)
	{
        int j;
        for (j=0; j < LCD_HEIGHT; j += DMA_BUFFER_SIZE)
        {        
            while (!SPI3STATLbits.SPITBE) ; // Wait for SPI buffer to empty
            LCDWriteDataDMA(img_buffer, DMA_BUFFER_SIZE); 
        }
	}
    SPI3CON1Lbits.MODE16 = 0; // Go back to using 8-bit data transfers
        
	LCDDisplayHome();
    
    SPI3CON1Lbits.MODE16 = 1; // Temporarily enable 16-bit data
                              // transfers as required for DMA
    DC_LCD_SetHigh(); // Enable sending data to LCD flash
	for (i=0; i<LCD_WIDTH; i++)
	{
        const uint16_t BLACK = 0x0000;
        const uint16_t BLUE	 = 0x001F;
        const uint16_t RED 	 = 0xF800;
        const uint16_t GREEN =  0x07E0;
        const uint16_t CYAN	 = 0x07FF;
        const uint16_t MAGENTA = 0xF81F;
        const uint16_t YELLOW  = 0xFFE0;
        const uint16_t WHITE   = 0xFFFF;
        
        uint16_t color = WHITE;
        bool new_color = true;
        // Change color when we reach the proper row
        switch (i)
        {   
            case 0:
                color = WHITE;
                break;  
            case 40:
                color = YELLOW;
                break;  
            case 80:
                color = MAGENTA;
                break;  
            case 120:
                color = RED;
                break;  
            case 160:
                color = CYAN;
                break;  
            case 200:
                color = GREEN;
                break;  
            case 240:
                color = BLUE;
                break;  
            case 280:
                color = BLACK;
                break;  
            default:
                new_color = false;
                break;
        }

        if (new_color)
        {
            int j;
            for (j=0; j<DMA_BUFFER_SIZE; j++)
            {
                img_buffer[j] = color;
            }    
        }
        int j;
		for (j=0; j<LCD_HEIGHT; j += DMA_BUFFER_SIZE)
		{
            while (!SPI3STATLbits.SPITBE) ; // Wait for SPI buffer to empty
            LCDWriteDataDMA(img_buffer, DMA_BUFFER_SIZE); 
		}
	}   
    while (!SPI3STATLbits.SPITBE) ;    
    SPI3CON1Lbits.MODE16 = 0; // Go back to using 8-bit data transfers
    //DMACONbits.DMAEN = 0;    
    SPI3_Initialize();
}


/*
 *   If 'complete_blankout' is 'true', display is set to complete black.
 *   Otherwise, it's set to the standard background color used.
 */
void BlankOutDisplay(bool complete_blankout)
{
    const uint16_t BLACK = 0x0000;
    int i;
	for (i=0; i<DMA_BUFFER_SIZE; i++)
	{
        img_buffer[i] = (complete_blankout ? BLACK : BKGRND_COLOR);
    }
    if (complete_blankout)
    {
        TurnBacklightOff();
    }
    SPI3_Initialize();   
 	LCDDisplayHome();
    InitializeLCDDMA();   
    DC_LCD_SetHigh(); // Enable sending data to LCD flash
    
    SPI3CON1Lbits.MODE16 = 0; // Go back to using 8-bit data transfers
        
	LCDDisplayHome();
    
    SPI3CON1Lbits.MODE16 = 1; // Temporarily enable 16-bit data
                              // transfers as required for DMA
    DC_LCD_SetHigh(); // Enable sending data to LCD flash
	for (i=0; i<LCD_WIDTH; i++)
	{
        int j;
        for (j=0; j < LCD_HEIGHT; j += DMA_BUFFER_SIZE)
        {        
            while (!SPI3STATLbits.SPITBE) ; // Wait for SPI buffer to empty
            LCDWriteDataDMA(img_buffer, DMA_BUFFER_SIZE); 
        }
	}
    while (!SPI3STATLbits.SPITBE) ;    
    SPI3CON1Lbits.MODE16 = 0; // Go back to using 8-bit data transfers
    //DMACONbits.DMAEN = 0;    
    SPI3_Initialize();    
}

/* Description:  This function searches the FLASH for the desired image based
 *               on the identifier in the argument list.  If the image is found
 *               in the table of images, then the pixel data is read out of the
 *               flash memory and written into the LCD graphics RAM for display.
 * 
 * Preconditions:  SPI3 has been initialized and the digital IO bit that drives
 *                 the DC_LCD bit to the LCD has been initialized
 * Arguments:  Identifier is the image identification number, it is used to
 *             search for the image entry in the table of images
 * Returns:  void
 */
void WriteImageToLCD(uint32_t identifier, bool erase_previous, bool print_out)
{
    //Data that is stored for each entry in the FLASH
    uint32_t starting_page = 0;
    uint32_t xdim = 0;
    uint32_t ydim = 0;
    uint32_t xpos = 0;
    uint32_t ypos = 0;

    if (LocateImage(identifier, &starting_page, &xdim, &ydim, &xpos, &ypos, print_out))
    {
        /* The SSD2119 controller can be programmed to write pixels in any direction.
         * When ID[1:0] = 11 and AM=0, pixels are written from left to right and
         * top to bottom - assuming the LCD is oriented with the ribbon cable coming
         * out of the top.  The SSD2119 considers this Graphics RAM entry mode as: 
         * incrementing the horizontal and incrementing the vertical - which is logical.
         * 
         * 
         * The LCD considers the rightmost edge to be x=0 rather than the
         * leftmost edge.  To make the x-position more intuitive, we use
         * a linear transformation here. We do the same for the y-position
         * to flip the screen upside down.  To flip the LCD in the y direction,
         * just change the Entry Mode setting to ID=00 <--> 11 and comment out
         * or leave in the ypos statement below.  */
        //xpos = LCD_WIDTH - xpos - xdim;
        ypos = LCD_HEIGHT - ypos - ydim;
        
        if(erase_previous)
        {
            BlankOutDisplay(false);   //paint background color over entire display
        }
        
        DisplayImage(starting_page, xdim, ydim, xpos, ypos);
    }
    else
    {
        printf("\n\r Could not find the requested image #%ld.", identifier); 
    }
 
    SPI3_Initialize();   
}


/*  For efficiency, this function assumes that it will only be used
 *  to display one image. That is, it will not be called with more
 *  than one image ID as the parameter.
 */
void DisplayImageAtRandomLocation(uint32_t identifier)
{
    // Data that is stored for each entry in the FLASH
    // If starting_page is set to ULONG_MAX, the image has not been found
    static uint32_t starting_page = ULONG_MAX;
    static uint32_t xdim = 0;
    static uint32_t ydim = 0;
    static uint32_t xpos = 0;
    static uint32_t ypos = 0;
    static uint16_t image_radius = 0;

    if (starting_page == ULONG_MAX)
    {
        SPI2_Initialize(); // Might not be helping
        if (LocateImage(identifier, &starting_page, &xdim, &ydim, &xpos, &ypos, false))
        {
            image_radius = sqrt(xdim*xdim + ydim*ydim)/2;
        }
        else
        {
            printf("\n\r Could not find the requested image #%ld.", identifier); 
        }
    }
    if (starting_page != ULONG_MAX)
    {
        const uint16_t VISIBLE_REGION_RADIUS = 116;
        const double TWO_PI = 2*3.1415926535;
        uint16_t r = rand() % (VISIBLE_REGION_RADIUS - image_radius);
        double theta = rand() * TWO_PI / INT_MAX;
        xpos = LCD_WIDTH/2  - r * cos(theta) - xdim/2;
        ypos = LCD_HEIGHT/2  - r * sin(theta) - ydim/2;
        DisplayImage(starting_page, xdim, ydim, xpos, ypos);
        /* Resetting the SPI3 interface appears to be necessary after
        multiple image writes. */
        // TODO: Figure out why if possible
        SPI3_Initialize();
    }
    // Reset SPI interface and clear overflow flag
    // TODO: Figure out why this is necessary. Without resetting,
    // subsequent flash read operations fail.  Perhaps this
    // is always needed after using DMA?
    SPI2CON1Lbits.SPIEN = 0;
    SPI2STATLbits.SPIROV = 0; 
    SPI2CON1Lbits.SPIEN = 1;
}


/*
 * Description:  This function tests that the TFT LCD is connected and
 *               determines pass/fail.  This is done by measuring the 5V rail
 *               with the LCD in a lower power mode, then measuring it again
 *               with the LCD fully powered up.  Since the 5V rail has a 1 ohm
 *               resistor in series and the LCD takes about 40mA of current
 *               typically, the difference in the two measurements should be
 *               around 40mV.
 * 
 *               There are some delays below to give time for the power supply
 *               to settle out after changing the power consumption of the LCD.
 *               Empirical measurements:  0.100S delay => 2mV difference,
 *                                        0.500S delay => 4mV difference, 
 *                                        1.0S delay => 8mV difference,
 *                                        4.0S delay => 8mV difference
 * Preconditions:  The SPI peripheral that services the TFT LCD must be
 *                 initialized along with the digital output bit that drives
 *                 the data/command input to the LCD.  This function will use
 *                 the basic user interface functions that talk to the TFT LCD.
 * Arguments:  'message' should allow for up to 255 characters.  Test failure
 *                 details will be placed here.
 *             'post' is 'true' if this test is being performed as part of POST.
 *                 During POST, we want to synchronize when the RGB LED and
 *                 the LCD are turned on and off.  (This results in some
 *                 coupling between this and the RGB LED test routine.  This
 *                 is an acceptable trade-off for the sake of performance and
 *                 visual aesthetics, though.)
 * Returns:  false = Fail, true = Pass
 */
bool TestLCD(char *message, bool post)
{
    float value1;
    TurnBacklightOn();
    __delay_ms(1275);   //Add a little delay to allow the LCD to stabilize
    Test5VDC(&value1, NULL);    //Measure the 5V power supply
    
    //Put the LCD into "sleep"  mode by taking SLP = 1
    LCDWriteCommand(0x0010);    // Sleep mode
	LCDWriteData(0x0001);       // Page 49 of SSD2119 datasheet
    //Turn the display off
    LCDWriteCommand(0x0007);    // Display Control 5
	LCDWriteData(0x0000);       // Page 45 of SSD2119 datasheet
    if (post)
    {
        /* During POST, the red LED will be on because of the LED test.
         * We turn it off here instead of within the LED test to synchronize
         * the LED and LCD blinks. */        
        WriteRGBLED(LED_BLACK, 0, 0, 0, 0);   //Turn off the red LED
    }
    TurnBacklightOff();
    __delay_ms(1275);   //Add a little delay to allow the LCD to stabilize into the lower power mode
 
    float value2;
    Test5VDC(&value2, NULL);    //Measure the 5V power supply

    //Turn display back on and take it out of sleep mode
    LCDWriteCommand(0x0007);    // Display Control 
	LCDWriteData(0x0033);       // Page 45 of SSD2119 datasheet
    LCDWriteCommand(0x0010);    // Sleep mode
	LCDWriteData(0x0000);       // Page 49 of SSD2119 datasheet
    
    TurnBacklightOn();
    if (post)
    {
        //Restore the RGB LED light since it is turned off during POST
        WriteRGBLED(LED_CUSTOM, ReadUISetting(POWER_UP_STATE, COLOR_1_R),
                                ReadUISetting(POWER_UP_STATE, COLOR_1_TIME), 
                                ReadUISetting(POWER_UP_STATE, COLOR_2_R),
                                ReadUISetting(POWER_UP_STATE, COLOR_2_TIME)); 
    }

    sprintf(message, "LCD Test Value1 = %.3f  Value2 = %.3f", (double)value1, (double)value2);
    
    //Apply the pass/fail criteria to the values - a window of acceptance
    bool pass1 = (((value2 - value1) > TFT_LCD_5VDC_LOW_WINDOW) && ((value2 - value1) < TFT_LCD_5VDC_HIGH_WINDOW));
    if (!pass1)
    {
        strcat(message, "  Value2-Value1 failed.  ");
    }
    
    return pass1; 
}


void TurnBacklightOff(void)
{
    BACKLIGHT_OFF_SetLow();
}

void TurnBacklightOn(void)
{
    BACKLIGHT_OFF_SetHigh();
}

/* 
 *   Notifies the LCD which region the next image is to be written onto.
 *   Specified via the x- and y-coordinates as well as the dimensions
 */
static void InitLCDRegionWrite(uint32_t xpos, uint32_t ypos,
                                  uint32_t xdim, uint32_t ydim)
{
    LCDWriteCommand(0x004E);    // RAM address set
    LCDWriteData(xpos);    // Page 58 of SSD2119 datasheet        //Set starting address at lower right of screen
    LCDWriteCommand(0x004F);    // RAM address set
    LCDWriteData(ypos);    // Page 58 of SSD2119 datasheet        //Set starting address at lower right of screen

    LCDWriteCommand(0x0044);    // Vertical RAM address position
    LCDWriteData(ypos + ((ypos+ydim-1)<<8));       // Page 57 of SSD2119 datasheet
    LCDWriteCommand(0x0045);    // Horizontal RAM address position 
    LCDWriteData(xpos);       // Page 57 of SSD2119 datasheet
    LCDWriteCommand(0x0046);    // Horizontal RAM address position
    LCDWriteData(xpos+xdim-1);       // Page 57 of SSD2119 datasheet

    LCDWriteCommand(0x0022);    // RAM data write/read  
}


/* Description:  This function writes data to the LCD
 * Preconditions:  SPI3 has been initialized and the digital IO bit that
 *                 drives the DC_LCD bit to the LCD has been initialized
 * Arguments:  Data bit field for the LCD, see LCD datasheet for command
 *             definitions and their associated data
 */
static void LCDWriteDataDMA(const uint16_t *p_buffer, uint16_t num_ints)
{
    if (num_ints == DMA_BUFFER_SIZE)
    {
        DMASRC1 = (unsigned int) p_buffer;
        DMACNT1 = num_ints;

#ifdef DISABLE_INTERRUPTS_DURING_DMA
        INTCON2bits.GIE = 0;
#endif
        DMACH1bits.CHEN = 1;
        DMACH1bits.CHREQ = 1;
        while (DMACNT1) ;
#ifdef DISABLE_INTERRUPTS_DURING_DMA
        INTCON2bits.GIE = 1;
#endif
    }
    else
    {
        int j;
        for (j = 0; j<num_ints; j++)
        {
            LCDWriteData16bits(p_buffer[j]); 
        }
    }
}

// Temporary function for debugging and diagnostics
void LCDTest(void)
{
    while (true)
    {
        WriteImageToLCD(2000, false, false);
        __delay_ms(250);
        WriteImageToLCD(3000, false, false);
        __delay_ms(250);
    }
}

/*
 *   Returns 'true' if image corresponding to 'identifier' is found in
 *     flash memory; 'false' otherwise
 *   If found, the starting page in flash memory, image dimensions, and
 *     image coordinates are stored via the subsequent parameters.
 *   Diagnostic info is printed out on the test interface if 'print_out'
 *     is set to 'true'.
 *  
 *  Changed to look for final entry. During development new images are
 *  appended at the end of the list and I want to get the latest dimensions.
 */
static bool LocateImage(uint32_t identifier, uint32_t *starting_page,
               uint32_t *xdim, uint32_t *ydim, uint32_t *xpos, uint32_t *ypos,
               bool print_out)
{
    int i;
    bool identifier_found = false;
    for (i=0; (i<256) /*&& !identifier_found */; i++)
    {
        uint32_t identifier_from_flash;
        ReadFlash(i*64, &identifier_from_flash, 1, 0);
        if (identifier == identifier_from_flash)     //Look for the image we want to write to the LCD
        {  
            ReadFlash(i*64+31, starting_page, 1, 0);
            ReadFlash(i*64+33, xdim, 1, 0);
            ReadFlash(i*64+34, ydim, 1, 0);
            ReadFlash(i*64+35, xpos, 1, 0);
            ReadFlash(i*64+36, ypos, 1, 0);
            identifier_found = true;
            if (print_out)
            {
                printf("\n\r Starting page = %lu  XDIM = %lu  YDIM = %lu  XPOS = %lu  YPOS = %lu",
                        *starting_page, *xdim, *ydim, *xpos, *ypos);
            }
       }
    }
    return identifier_found;
}

/*
 *   Displays the indicated image on the LCD
 * 
 *   Parameters:
 *     starting_page:  First page within flash memory at which image is stored
 *     xdim, ydim:     Dimensions in pixels
 *     xpos, ypos:     x- and y-coordinates
 */
static void DisplayImage(uint32_t starting_page, uint32_t xdim, uint32_t ydim,
                         uint32_t xpos, uint32_t ypos)
{
    InitLCDRegionWrite(xpos, ypos, xdim, ydim);
    
    
    // TODO: Initialize the DMA registers at startup instead
#ifdef USE_LCD_DMA
    InitializeLCDDMA();
#endif
    DC_LCD_SetHigh(); // Enable sending data to LCD flash
    __delay_us(1); // Not sure if this is necessary or helpful
    
    uint32_t flash_address = 0;
    uint32_t img_size = xdim*ydim;
    uint32_t i;
    uint32_t img_buffer_ndx = 0;
    for (i = 0; i<img_size; i+=IMG_BUFFER_SIZE)
    {
        uint32_t num_words_to_read = 
               ((img_size - img_buffer_ndx) < IMG_BUFFER_SIZE ?
                   (img_size - img_buffer_ndx) : IMG_BUFFER_SIZE);
        

        uint32_t num_longs_to_read;
        if(num_words_to_read % 2)
        {
            num_longs_to_read = num_words_to_read/2 + 1;
        }
        else
        {
            num_longs_to_read = num_words_to_read/2;
        }
         
        ReadFlash((64*starting_page + flash_address), (uint32_t *) img_buffer, num_longs_to_read, false);
 
        
        //Swap the words around prior to writing to the LCD
        uint16_t i, Holding_Reg;
        for(i=0; i<num_words_to_read; i=i+2)
        {
            Holding_Reg = img_buffer[i];
            img_buffer[i] = img_buffer[i+1];
            img_buffer[i+1] = Holding_Reg;
        }
        

        uint32_t j;

#ifdef USE_LCD_DMA
        /*  Here we start transferring data from img_buffer[1] because, as
         *  per comments above, we have to ignore the first 16-bit word.   */
        for (j = 0; j<num_words_to_read; j += DMA_BUFFER_SIZE)
        {
            while (!SPI3STATLbits.SPITBE) ; // Wait for SPI buffer to empty
            LCDWriteDataDMA(&img_buffer[j], 
                (num_words_to_read-j < DMA_BUFFER_SIZE ?
                    (num_words_to_read-j) : DMA_BUFFER_SIZE));
        }
#else  // else for USE_LCD_DMA
        for (j = 0; j<num_words_to_read; j++)
        {
            LCDWriteData(img_buffer[j+1]); 
        }
#endif // end of USE_LCD_DMA
        flash_address += IMG_BUFFER_SIZE/2;                
        img_buffer_ndx += IMG_BUFFER_SIZE;
    }
    
    while (!SPI3STATLbits.SPITBE) ;
    SPI3CON1Lbits.MODE16 = 0; // Go back to using 8-bit data transfers
    //DMACONbits.DMAEN = 0; 
}


/* Description:  This function displays the given text string on the LCD, starting at the
 *               given position.
 * 
 * Preconditions:  LCD interface must be initialized
 * Arguments:  p_String is a pointer to the null-terminated text string
 *             xpos and ypos is the coordinates on the LCD: (0,0) = upper left corner
 * Returns:  void
 */
void DisplayText(const char *p_String, uint16_t xpos, uint16_t ypos)
{
    unsigned char char_count;         //general working variable
    unsigned char Width_Array_Index; //Index into the character Width array for a given character
    uint32_t Image_Array_Index;      //Index into the Image array, points to a given pixel
    uint16_t pixel_count;            //counts the pixels in a given character image
    uint16_t Number_Of_pixels;       //Number of pixels in a given character image
    
    uint16_t xpos_char;       //sets the xpos of the next character to be displayed
    uint16_t ypos_char;       //sets the ypos of the next character to be displayed
    uint16_t xdim_char = 0;        //sets the xdim of the next character to be displayed
    uint16_t ydim_char = 0;        //sets the ydim of the next character to be displayed
    
    //The font is held in an array, Image_Arial20_Vert[97280] which is an array of 16-bit color pixels
    //The one dimensional array holds the bit map of all ASCII characters between 32d and 126d inclusive
    //Each character is contained in an image of 32 x 32 pixels, since there are 95 total characters,
    //the array is 32 x 32 x 95 = 97280 pixels
    //The characters have different widths though, and the width of each character is held in another 
    //array, Image_Arial20_Widths[95]
    
    xpos_char = xpos;
    ypos_char = LCD_HEIGHT - ypos;
    ydim_char = 32;
    
    for(char_count=0; char_count<255; char_count++)
    {
        if (*(p_String+char_count) == 0) {break;}  //If there are no more characters to display, then we're done
        Width_Array_Index = 126 - *(p_String+char_count);  //Note that the array is reverse ordered with ASCII codes 126 downto 32
        xdim_char = (uint16_t) Image_Arial20_Widths[Width_Array_Index];
        //Determine if there is enough room on the display for the next character
        if ((xpos_char+xdim_char) > LCD_WIDTH) {break;}  //If there is no more room on the LCD for the next character, then we're done
        
        Number_Of_pixels = xdim_char*ydim_char;
        Image_Array_Index = 1024*(uint32_t)Width_Array_Index;   //32*32 = 1024
        
        //printf("\n\r char_count = %i   ASCII code of %c is %i   Width_Array_Index = %i   xdim_char = %i   xpos_char = %i    Image_Array_Index = %ld", char_count, *(p_String+char_count), *(p_String+char_count), Width_Array_Index, xdim_char, xpos_char, Image_Array_Index );
        //Print one character to the LCD
        InitLCDRegionWrite(xpos_char, ypos_char, xdim_char, ydim_char);   
        for(pixel_count = 1; pixel_count <= Number_Of_pixels; pixel_count++)
        {            
            //printf("\n\r Image_Array_Index = %ld ", Image_Array_Index);
            LCDWriteData(Image_Arial20_Vert[Image_Array_Index]);
            Image_Array_Index = Image_Array_Index + 1;
            if((pixel_count % xdim_char) == 0) {Image_Array_Index = Image_Array_Index + 32 - xdim_char;}
        }

        xpos_char = xpos_char + xdim_char;  // update the position for the next character
    }
 
}