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

void blinkLED(channel)
{
	GPIO.output(channel, 1)
    CyDelay(250);
	GPIO.output(channel, 0)
}

// Dir_Ctl bits
// 

void testRPPUIO16(void)
{
// Set all of the RPI pins to outputs
    Dir_Ctl_Write(0xff);
    DataOut_Port15_Write(0x10);
}
    
    
GPIO.output(I2C_3, 0)
GPIO.output(I2C_4, 0)
GPIO.output(UART_3, 0)
GPIO.output(UART_4, 0)
GPIO.output(IO4, 0)
GPIO.output(IO18, 0)
GPIO.output(IO17, 0)
GPIO.output(IO27, 0)
GPIO.output(IO23, 0)
GPIO.output(IO22, 0)
GPIO.output(IO24, 0)
GPIO.output(IO25, 0)
GPIO.output(SPI0_3, 0)
GPIO.output(SPI0_4, 0)
GPIO.output(SPI0_5, 0)
GPIO.output(SPI0_6, 0)
GPIO.output(SPI1_6, 0)
GPIO.output(IO5, 0)
GPIO.output(IO6, 0)
GPIO.output(IO12, 0)
GPIO.output(IO13, 0)
GPIO.output(IO19, 0)
GPIO.output(IO16, 0)
GPIO.output(IO26, 0)
GPIO.output(IO20, 0)
GPIO.output(IO21, 0)

# Blink all of the LEDs one at a time forever
# CTRL-C to exit which is not a particularly elegant exit strategy, but this is a demo program

while 1:
	blinkLED(IO21)
	blinkLED(SPI1_6)
	blinkLED(SPI0_3)
	blinkLED(SPI0_4)
	blinkLED(SPI0_5)
	blinkLED(SPI0_6)
	blinkLED(I2C_3)
	blinkLED(I2C_4)
	blinkLED(UART_3)
	blinkLED(UART_4)
	blinkLED(IO4)
	blinkLED(IO18)
	blinkLED(IO17)
	blinkLED(IO27)
	blinkLED(IO23)
	blinkLED(IO22)
	blinkLED(IO24)
	blinkLED(IO25)
	blinkLED(IO5)
	blinkLED(IO6)
	blinkLED(IO12)
	blinkLED(IO13)
	blinkLED(IO19)
	blinkLED(IO16)
	blinkLED(IO26)
	blinkLED(IO20)
}
/* [] END OF FILE */
