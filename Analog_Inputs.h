//	Filename:  Analog_Inputs.h
//	Author:  Paul Wittibschlager
//	Date Created:  08/27/2018
//	Company:  Valtronic, USA
//	Project:   Lumitex OraGlo Control Board
//	Project number:  DD400004
//	Description:  This file contains the #defines and declarations for the analog acquisition system
//                This includes the fan tachometer input, which we will measure the frequency
//
//  Development environment:  MPLAB v4.20  XC16 v1.35  MCC (Code Configurator) v3
//                            com.microchip.mcc.mcu16.Mcu16PeripheralLibrary v1.55



// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations



//volatile = tells compiler that this variable is not guaranteed to retain its value between successive accesses.  The prevents the compiler from optimizing
//redundant references because it may alter the behavior of the code.  Recommended  usage for global variables that will be accessed by interrupt service 
//routines is to declare them as volatile.  static = variable is given permanent storage for the duration of the program, even when declared within a
//function.  static variables can always be accessed by a pointer, even outside of a function where it was declared.  static variables are guaranteed to 
//retain their value between calls to the function (unless modified by a pointer access).
volatile static int ADC1_Total_Accum=0;  

volatile static unsigned long Analog_Ch8_Accum=0;
volatile static unsigned long Analog_Ch8_Average=0;
volatile static float Analog_Ch8=0;

volatile static unsigned long Analog_Ch9_Accum=0;
volatile static unsigned long Analog_Ch9_Average=0;
volatile static float Analog_Ch9=0;

volatile static unsigned long Analog_Ch10_Accum=0;
volatile static unsigned long Analog_Ch10_Average=0;
volatile static float Analog_Ch10=0;

volatile static unsigned long Analog_Ch11_Accum=0;
volatile static unsigned long Analog_Ch11_Average=0;
volatile static float Analog_Ch11=0;

volatile static unsigned long Analog_Ch12_Accum=0;
volatile static unsigned long Analog_Ch12_Average=0;
volatile static float Analog_Ch12=0;

#define ADC_REF 2.5
#define ADC_AVERAGES 100

volatile static unsigned long Fan_Tach_RPM=0;
volatile static unsigned long Fan_Tach_Value1=0;
volatile static unsigned long Fan_Tach_Value2=0;
volatile static unsigned long Fan_Tach_Difference=0;


volatile static unsigned int Pushbutton_1=0;
volatile static unsigned int Pushbutton_2=0;

volatile static int Update_Time=0;


// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

