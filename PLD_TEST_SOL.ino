/*
  SSC at Morehead State University
  
  Authors: Austin Pedigo and Elijah Gibbs

  last update: 4/8/2021 (change after each revision) 
*/

#include <arduino.h>
#include <SPI.h>
#include <e.h>
#include <stdint.h>

enum PLD_registers
{

	STATUS_AND_RESET = 0x00, // displays status and ability to reset PLD - 
	MOD_STATUS = 0x07, // declares module status
	MOD_CONTROL = 0x08, // allows direct control of the module and FIFO
	GPIO_EV_FIFO_SLCT = 0x09, // determines the status of the GPIO pin
	


};


#define BIT0; // LSB
#define BIT1;
#define BIT2;
#define BIT3:
#define BIT4;
#define BIT5;
#define BIT6;
#define BIT7; // MSB

void write_reg(u8 data, u8 address_high, unsi) {




}