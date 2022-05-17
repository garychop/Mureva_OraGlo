/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the MPLAB(c) Code Configurator device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 4.45.7
        Device            :  PIC24FJ1024GA606
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.34
        MPLAB             :  MPLAB X v4.15

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

*/


/**
    Section: Includes
*/
#include <xc.h>
#include "pin_manager.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATB = 0x2000;
    LATC = 0x0000;
    LATD = 0x0200;
    LATE = 0x0000;
    LATF = 0x0000;
    LATG = 0x0200;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISB = 0x1FCB;
    TRISC = 0x5000;
    TRISD = 0x00E2;
    TRISE = 0x003F;
    TRISF = 0x007F;
    TRISG = 0x010C;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    IOCPDB = 0x0000;
    IOCPDC = 0x0000;
    IOCPDD = 0x0020;
    IOCPDE = 0x0000;
    IOCPDF = 0x0000;
    IOCPDG = 0x0000;
    IOCPUB = 0x0000;
    IOCPUC = 0x0000;
    IOCPUD = 0x0200;
    IOCPUE = 0x0000;
    IOCPUF = 0x0000;
    IOCPUG = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;
    ODCE = 0x0000;
    ODCF = 0x0000;
    ODCG = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSB = 0x1F03;
    ANSC = 0x0000;
    ANSD = 0x0000;
    ANSE = 0x0000;
    ANSG = 0x0000;


    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS

    RPOR12bits.RP25R = 0x000B;   //RD4->SPI2:SCK2OUT;
    RPOR13bits.RP26R = 0x0019;   //RG7->SPI3:SS3OUT;
    RPOR13bits.RP27R = 0x0003;   //RG9->UART1:U1TX;
    RPOR6bits.RP13R = 0x000D;   //RB2->OC1:OC1;
    RPOR11bits.RP23R = 0x0007;   //RD2->SPI1:SDO1;
    RPOR10bits.RP21R = 0x0017;   //RG6->SPI3:SDO3;
    RPOR5bits.RP11R = 0x0008;   //RD0->SPI1:SCK1OUT;
    RPINR22bits.SDI2R = 0x0014;   //RD5->SPI2:SDI2;
    RPINR18bits.U1RXR = 0x0013;   //RG8->UART1:U1RX;
    RPOR14bits.RP28R = 0x000E;   //RB4->OC2:OC2;
    RPOR9bits.RP18R = 0x000F;   //RB5->OC3:OC3;
    RPINR20bits.SDI1R = 0x000A;   //RF4->SPI1:SDI1;
    RPOR1bits.RP2R = 0x000A;   //RD8->SPI2:SDO2;
    RPINR7bits.IC1R = 0x0025;   //RC14->IC1:IC1;
    RPOR1bits.RP3R = 0x0018;   //RD10->SPI3:SCK3OUT;

    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    /****************************************************************************
     * Interrupt On Change for group IOCFF - flag
     ***************************************************************************/
	IOCFFbits.IOCFF2 = 0; // Pin : RF2
	IOCFFbits.IOCFF3 = 0; // Pin : RF3

    /****************************************************************************
     * Interrupt On Change for group IOCNF - negative
     ***************************************************************************/
	IOCNFbits.IOCNF2 = 0; // Pin : RF2
	IOCNFbits.IOCNF3 = 0; // Pin : RF3

    /****************************************************************************
     * Interrupt On Change for group IOCPF - positive
     ***************************************************************************/
	IOCPFbits.IOCPF2 = 1; // Pin : RF2
	IOCPFbits.IOCPF3 = 1; // Pin : RF3

    /****************************************************************************
     * Interrupt On Change for group PADCON - config
     ***************************************************************************/
	PADCONbits.IOCON = 1; 

    IEC1bits.IOCIE = 1; // Enable IOCI interrupt 
}

/* Interrupt service routine for the IOCI interrupt. */
//void __attribute__ (( interrupt, no_auto_psv )) _IOCInterrupt ( void )
//{
//    if(IFS1bits.IOCIF == 1)
//    {
//        // Clear the flag
//        IFS1bits.IOCIF = 0;
//        // interrupt on change for group IOCFF
//        if(IOCFFbits.IOCFF2 == 1)
//        {
//            IOCFFbits.IOCFF2 = 0;
//            // Add handler code here for Pin - RF2
//        }
//        if(IOCFFbits.IOCFF3 == 1)
//        {
//            IOCFFbits.IOCFF3 = 0;
//            // Add handler code here for Pin - RF3
//        }
//    }
//}
