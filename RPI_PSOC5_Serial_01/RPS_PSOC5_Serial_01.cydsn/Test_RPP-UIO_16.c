/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

// Function to blink an LED attached to an output channel
// Drives line high for a short time and then drives it low.
// The high level output turns on the LED.

#include <project.h>
#include "RasPi_Pins.h"

void blinkLED(uint8 RPI_Pin)
{
	setRasPiPin(RPI_Pin, 1);
    CyDelay(250);
	setRasPiPin(RPI_Pin, 0);
}

void testRPPUIO16(void)
{
// Set all of the RPI pins to outputs
    setRasPiPin(IO_2,0);
    setRasPiPin(IO_3,0);
    setRasPiPin(IO_4,0);
    setRasPiPin(IO_14,0);
    setRasPiPin(IO_15,0);
    setRasPiPin(IO_17,0);
    setRasPiPin(IO_18,0);
    setRasPiPin(IO_27,0);
    setRasPiPin(IO_22,0);
    setRasPiPin(IO_23,0);
    setRasPiPin(IO_24,0);
    setRasPiPin(IO_10,0);
    setRasPiPin(IO_9,0);
    setRasPiPin(IO_25,0);
    setRasPiPin(IO_11,0);
    setRasPiPin(IO_8,0);
    setRasPiPin(IO_7,0);
    setRasPiPin(IO_5,0);
    setRasPiPin(IO_6,0);
    setRasPiPin(IO_12,0);
    setRasPiPin(IO_13,0);
    setRasPiPin(IO_19,0);
    setRasPiPin(IO_16,0);
    setRasPiPin(IO_26,0);
    setRasPiPin(IO_20,0);
    setRasPiPin(IO_21,0);
    

// Blink all of the LEDs one at a time forever
// CTRL-C to exit which is not a particularly elegant exit strategy, but this is a demo program

    while (1)
    {
    	blinkLED(IO_2);
    	blinkLED(IO_3);
    	blinkLED(IO_4);
    	blinkLED(IO_14);
    	blinkLED(IO_15);
    	blinkLED(IO_17);
    	blinkLED(IO_18);
    	blinkLED(IO_27);
    	blinkLED(IO_22);
    	blinkLED(IO_23);
    	blinkLED(IO_24);
    	blinkLED(IO_10);
    	blinkLED(IO_9);
    	blinkLED(IO_25);
    	blinkLED(IO_11);
    	blinkLED(IO_8);
    	blinkLED(IO_7);
    	blinkLED(IO_5);
    	blinkLED(IO_6);
    	blinkLED(IO_12);
    	blinkLED(IO_13);
    	blinkLED(IO_19);
    	blinkLED(IO_16);
    	blinkLED(IO_26);
    	blinkLED(IO_20);
    	blinkLED(IO_21);
    }
}
/* [] END OF FILE */
