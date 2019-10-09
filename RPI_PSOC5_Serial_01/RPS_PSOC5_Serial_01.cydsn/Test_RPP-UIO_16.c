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

void blinkLED(uint8 RPI_Pin, uint8 val)
{
	setRasPiPin(RPI_Pin, 1);
    CyDelay(250);
	setRasPiPin(RPI_Pin, 0);
}

// Dir_Ctl bits
// 

#define PORT0_OUT   0x01
#define PORT1_OUT   0x02
#define PORT2_OUT   0x04
#define PORT3_OUT   0x08
#define PORT4_OUT   0x10
#define PORT5_OUT   0x20
#define PORT6_OUT   0x40
#define PORT12_OUT  0x80

void testRPPUIO16(void)
{
// Set all of the RPI pins to outputs
    Dir_Ctl_Write(Dir_Ctl_Read() | PORT1_OUT | PORT2_OUT | PORT5_OUT | PORT12_OUT);   // Port 1,2,5,6 = outputs

// Blink all of the LEDs one at a time forever
// CTRL-C to exit which is not a particularly elegant exit strategy, but this is a demo program

    while (1)
    {
    	blinkLED(IO21);
    	blinkLED(SPI1_6);
    	blinkLED(SPI0_3);
    	blinkLED(SPI0_4);
    	blinkLED(SPI0_5);
    	blinkLED(SPI0_6);
    	blinkLED(I2C_3);
    	blinkLED(I2C_4);
    	blinkLED(UART_3);
    	blinkLED(UART_4);
    	blinkLED(IO4);
    	blinkLED(IO18);
    	blinkLED(IO17);
    	blinkLED(IO27);
    	blinkLED(IO23);
    	blinkLED(IO22);
    	blinkLED(IO24);
    	blinkLED(IO25);
    	blinkLED(IO5);
    	blinkLED(IO6);
    	blinkLED(IO12);
    	blinkLED(IO13);
    	blinkLED(IO19);
    	blinkLED(IO16);
    	blinkLED(IO26);
    	blinkLED(IO20);
    }
}
/* [] END OF FILE */
