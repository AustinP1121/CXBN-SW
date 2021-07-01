#include <msp430.h> 


/**
 * MOREHEAD STATE UNIVERSITY
 * By:
 * Austin Pedigo
 * Elijah Gibbs
 *
 * Mixture of legacy and original code.
 */

u8 Payload_Identifier = A; // default is PLD A


unsigned char MST_Data[3], SLV_Data[3]; // declares SPI data frames (24 bits in length, 3 bytes)

unsigned char parity_bit (unsigned char* data_frame, unsigned int len) {
        // declares initial bit of an SPI frame
	// Even parity bit function
	// Input Byte array(data_frame) of length (len)
	// Output parity bit calculated over entire frame
	volatile unsigned int j,k;

	u16 temp = 0;
    for(j=0;j<(len-1);j++)
        for(k=0;k<8;k++)					// for all bits in byte
           temp ^= (data_frame[j] >> k) & BIT0;	// xor with next bit in byte
    for(k=1;k<8;k++)						// for most significant 7 bits in byte
        temp ^= (data_frame[len - 1] >> k) & BIT0 ;		// xor with next bit in byte
    return temp;												// return even parity bit
}

void Send_Frame(void){
        // send a frame of data over SPI line
        t=0;
        //UCB1IFG &= ~UCRXIFG;
        UCB1IE |= (UCRXIE + UCTXIE);                         // Enable USCI_B1 RX interrupt -- look in MSP430G2553 data sheet and look at pin declarations
        switch (Payload_Identifier){
            case A: // send frame to PLD A/1
                //P2IE &= ~BIT6;
                P5OUT &= ~BIT2;                             // Assert slave select A line low -- believe this is pin 2 of port 5, will need to modify for 2552
                UCB1TXBUF = MST_Data[0];                    // Transmit run character
                __bis_SR_register(GIE);                     // enable interrupts
                while(t<3);                             // Send rest of bytes

                P5OUT |= BIT2; // SS A back to high

                break;

            case B: // send frame to PLD B/2
                //P2IE &= ~BIT7;
                P5OUT &= ~BIT3;                             // Assert slave select B line low -- pin 4 of port 5, modify for 2552

                UCB1TXBUF = MST_Data[0];                    // Transmit run character
                __bis_SR_register(GIE);                     // enable interrupts
                while(t<3);                             // Send rest of bytes

                P5OUT |= BIT3;                              // SS B back to high
                break;

            default: break;
        }
        shift_data_array(); // apparently this fixes an issue in reading the array
        return;
    }


}

void M1770_Read_Event(void) {
    MST_Data[0] = BIT7;                          // Initialize MOSI data values
    MST_Data[1] = 0x00;
    MST_Data[2] = BIT0;
        
    Send_Frame();
}



void shift_data_array(void){
    // shifts data right one bit
    // corrects bug in reading array data
    int j;
    char tmp[3];
    for (j=0;j<3;j++)
        tmp[j]= SLV_Data[j];
    SLV_Data[0] = (tmp[0] >> 1);
    SLV_Data[1] = ((tmp[0] & BIT0) << 7) | (tmp[1] >> 1);
    SLV_Data[2] = ((tmp[1] & BIT0) << 7) | (tmp[2] >> 1);
}

void M1770_Read_Register (unsigned char highADDR, unsigned char lowADDR) {
	// Subroutine reads data in highADDR and lowADDR addresses
	// Stores data from slave in SLV_Data array (3 bytes)
    MST_Data[0] = BIT6 | (highADDR << 1) | (lowADDR >> 7); // Initialize MOSI data values
    MST_Data[1] = (lowADDR << 1);
    MST_Data[2] = 0x00;
    MST_Data[2] |= parity_bit(MST_Data, 3);

    M1770_Send_Frame();

    MST_Data[0] = BIT7;                     // Initialize MOSI data values
    MST_Data[1] = 0x00;
    MST_Data[2] = BIT0;
        
    Send_Frame();
}

void write_register(unsigned char highAddr, unsigned char lowAddr, unsigned char data) { 
        // easy way to write to registers

    MST_Data[0] = (highAddr << 1) | (lowAddr >> 7);
    MST_Data[1] = (lowAddr << 1) | (data >> 7);
    MST_Data[2] = (data) << 1;
    MST_Data[2] |= parity_bit(MST_Data, 3);

    Send_Frame(); //send command to slave
}

void enable_pixel(unsigned int pixel_addr) {
write_register(0x05, pixel_addr, ~0x10) // are these registers correct??? 

}

int main(void) 
{
        // will work on after setting up initial functions
        
    WDTCTL = WDTPW + WDTHOLD; //stop WD timer





    while(1) {




    }

}

