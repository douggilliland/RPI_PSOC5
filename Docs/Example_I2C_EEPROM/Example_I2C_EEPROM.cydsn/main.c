//--------------------------------------------------------------------------
//
// Copyright 2008, Cypress Semiconductor Corporation.
//
// This software is owned by Cypress Semiconductor Corporation (Cypress)
// and is protected by and subject to worldwide patent protection (United
// States and foreign), United States copyright laws and international
// treaty provisions. Cypress hereby grants to licensee a personal,
// non-exclusive, non-transferable license to copy, use, modify, create
// derivative works of, and compile the Cypress Source Code and derivative
// works for the sole purpose of creating custom software in support of
// licensee product to be used only in conjunction with a Cypress integrated
// circuit as specified in the applicable agreement. Any reproduction,
// modification, translation, compilation, or representation of this
// software except as specified above is prohibited without the express
// written permission of Cypress.
//
// Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND,EXPRESS OR IMPLIED,
// WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Cypress reserves the right to make changes without further notice to the
// materials described herein. Cypress does not assume any liability arising
// out of the application or use of any product or circuit described herein.
// Cypress does not authorize its products for use as critical components in
// life-support systems where a malfunction or failure may reasonably be
// expected to result in significant injury to the user. The inclusion of
// Cypress' product in a life-support systems application implies that the
// manufacturer assumes all risk of such use and in doing so indemnifies
// Cypress against all charges.
//
// Use may be limited by and subject to the applicable Cypress software
// license agreement.
//
//
//--------------------------------------------------------------------------
//*****************************************************************************
//*****************************************************************************
//  FILENAME: main.c
//   Version: 1.0, Updated on 24 November 2009
//
//  DESCRIPTION: Main file of the Example_I2C_EEPROM Project.
//
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress MicroSystems 2000-2003. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
// 
//The project can be tested using a serial EEPROM 24C04.
// 
//CY8CKIT-001 Board Project
//
//Project Objective
//	    To demonstrate the interfacing of serial EEPROM with PSoC.
//
//Overview
//	This project is designed to demonstrate the interfacing of serial EEPROM with PSoC.
//	In this I2C protocol is used to control the transfer between the EEPROM and PSoC.
//
//	The following changes were made to the default settings in the Device Editor:
// 
//    	Select User Modules
//    	    o Place I2C Component 
//    	    o In this example, this Component is renamed as I2C
//    		o Place Character LCD Component
//			o Rename it as LCD
//		Configure these Components as explained in documentation.
//
// Operation
//	Upon program execution all hardware settings from the device configuration are loaded 
//	into the device and main.c is executed. Data Stored in the RAM (RAMBuffer) is transmitted
//	out of PSoC to be stored at address 0 of external EEPROM using I2C protocol.  Next the
//	internal subaddress of EEPROM is reset to 0 and data is read back
//
// Circuit Connections
// 	This example can be tested using the CCY8CKIT-001 board.  The following connections
// 	are to be made.  
// 
// 	PSoC
// 	--------
// 		P4[5] - SDA 			(Externally pull up to Vcc using 2.2K resistor)
// 		P4[4] - SCL 			(Externally pull up to Vcc using 2.2K resistor)
// 	
// 	24C04 (8 Pin PDIP) - EEPROM		
// 	---------------------------
// 		Pin 1,2,3	- Ground    (to make the 7 bit address as 1010 000)
// 		Pin 4		- Ground
// 		Pin 5		- SDA		Connect to P4[5] of PSoC
// 		Pin 6		- SCL		Connect to P4[4] of PSoC
// 		Pin 7 		- Ground
// 		Pin 8		- Vcc.	
//
//----------------------------------------------------------------------------


#include <device.h> 

//Word address
#define ADDR 0x00 

//Slave address
#define SLAVE_ADDR 0x50	

//Message is the data which has to be written to EEPROM. 
//First data in this array is Word address
uint8 cMessage[] = {ADDR, 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};

// buffer to save read back data
uint8 cRx_Buffer[15]; 

// Length of data to be written including word address
uint8 cLength = 0x11; 

uint8 cStatus,cIndex,cPosn;

//Main routine starts from here
void main()
{
	// enable global interrupts
	CYGlobalIntEnable;  
	 
	// enable I2C interrupts
	I2C_EnableInt();  
	
	//Start LCD
	LCD_Start(); 
	
	//Start I2C
	I2C_Start(); 
	
    for(;;)
    {
		I2C_MasterClearStatus(); /* Clear any previous status */
					
		//Following API writes the data from PSoC3 RAM buffer to EEPROM
		//SLAVE_ADDR is the slave address in this API
		//cMessage is the pointer to array which contains the data to be written to EEPROM.
		//cLength is the number of bytes which have to be written to EEPROM
		//I2C_MODE_COMPLETE_XFER, is to send data completly before sending stop command.
		
		I2C_MasterWriteBuf(SLAVE_ADDR, cMessage, cLength, I2C_MODE_COMPLETE_XFER);
		
		//wait until Transfer is complete
		while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT )==0); 
		
		// delay to complete the write operation(twr)
		CyDelay(10);
		
		//write a dummy byte to initialize the address word counter of 
		//eeprom to start address for read back operation. First location 
		//of cMessage array has word address.
		
		I2C_MasterWriteBuf(SLAVE_ADDR, cMessage, 1, I2C_MODE_COMPLETE_XFER);
		
		//wait until Transfer is complete
		while((I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT )==0);
		
		//Delay for setting address in EEPROM
		CyDelayUs(1);
		
		//Read the 16 bytes from slave, staring from word address specified by iMessage
		//SLAVE_ADDR is the slave address in this API
		//cRx_Buffer is the pointer to array where data has to be stored after reading from EEPROM.
		//cLength-1 is the number of bytes which have to be read from EEPROM
		//I2C_MODE_COMPLETE_XFER, is to read data completly before sending stop command.
		
		I2C_MasterReadBuf(SLAVE_ADDR, cRx_Buffer,cLength-1, I2C_MODE_COMPLETE_XFER );
				
		//wait until Transfer is complete
		while((I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT )==0); 
		
		//Display the data LCD. First 8 bytes in row 0 		
		for (cIndex=0,cPosn=0;cIndex<8;cIndex++,cPosn+=2)
		{
			LCD_Position(0,cPosn);
			LCD_PrintInt8(cRx_Buffer[cIndex]);
		}
		
		//Display next 8 bytes in row 1		
		for (cIndex=8,cPosn=0;cIndex<cLength-1;cIndex++,cPosn+=2)
		{
			LCD_Position(1,cPosn);
			LCD_PrintInt8(cRx_Buffer[cIndex]);
		}
	}
}

/* [] END OF FILE */
