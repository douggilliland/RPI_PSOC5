/* ========================================
 *
 * Copyright LAND BOARDS, LLC, 2019
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Land Boards.
 *
 * ========================================
*/

#include <project.h>
#include "RPI_PSOC5.h"

#define LED_ON 1
#define LED_OFF 0


void testRPISPI8(void)
{
// Set all of the RPI pins to outputs
    setRasPiPinValue(IO_9,PIN_DM_STRONG);   // SPIMISO 
    setRasPiPinValue(IO_10,PIN_DM_STRONG);  // SPIMOSI
    setRasPiPinValue(IO_11,PIN_DM_STRONG);  // SPISCK
    setRasPiPinValue(IO_8,PIN_DM_STRONG);   // SPICE0
    setRasPiPinValue(IO_22,PIN_DM_STRONG);  // A0
    setRasPiPinValue(IO_27,PIN_DM_STRONG);  // A1
    setRasPiPinValue(IO_17,PIN_DM_STRONG);  // A2
    
// Set all of the RPI pins to low
    setRasPiPinValue(IO_9,LED_OFF);         // SPIMISO 
    setRasPiPinValue(IO_10,LED_OFF);        // SPIMOSI
    setRasPiPinValue(IO_11,LED_OFF);        // SPISCK
    setRasPiPinValue(IO_8,LED_OFF);         // SPICE0
    setRasPiPinValue(IO_22,LED_OFF);        // A0
    setRasPiPinValue(IO_27,LED_OFF);        // A1
    setRasPiPinValue(IO_17,LED_OFF);        // A2
    
// Blink all of the LEDs one at a time forever
// CTRL-C to exit which is not a particularly elegant exit strategy, but this is a demo program

    while (1)
    {
        setRasPiPinValue(IO_22,LED_OFF);    // A0
        setRasPiPinValue(IO_27,LED_OFF);    // A1
        setRasPiPinValue(IO_17,LED_OFF);    // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_ON);     // A0
        setRasPiPinValue(IO_27,LED_OFF);    // A1
        setRasPiPinValue(IO_17,LED_OFF);    // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_OFF);    // A0
        setRasPiPinValue(IO_27,LED_ON);     // A1
        setRasPiPinValue(IO_17,LED_OFF);    // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_ON);     // A0
        setRasPiPinValue(IO_27,LED_ON);     // A1
        setRasPiPinValue(IO_17,LED_OFF);    // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_OFF);    // A0
        setRasPiPinValue(IO_27,LED_OFF);    // A1
        setRasPiPinValue(IO_17,LED_ON);     // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_ON);     // A0
        setRasPiPinValue(IO_27,LED_OFF);    // A1
        setRasPiPinValue(IO_17,LED_ON);     // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_OFF);    // A0
        setRasPiPinValue(IO_27,LED_ON);     // A1
        setRasPiPinValue(IO_17,LED_ON);     // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
        
        setRasPiPinValue(IO_22,LED_ON);     // A0
        setRasPiPinValue(IO_27,LED_ON);     // A1
        setRasPiPinValue(IO_17,LED_ON);     // A2
     	blinkLED(IO_9);                     // SPIMISO 
    	blinkLED(IO_10);                    // SPIMOSI
    	blinkLED(IO_11);                    // SPISCK
    	blinkLED(IO_8);                     // SPICE0
    }
}

void debugRPISPI8(void)
{

}

/* [] END OF FILE */
