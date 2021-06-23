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


unsigned char MST_Data[3], SLV_Data[3]; // declares SPI data frames (24 bits in length, 3 for each throughput)

unsigned char parity_bit (unsigned char* data_frame, unsigned int len); // gives context for the parity bit in a data frame

void M1770_Send_Frame(void){ // send a frame of data over SPI line
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

void write_register_M1762(unsigned char highAddr, unsigned char lowAddr, unsigned char data) { // easy way to writte to registers

    MST_Data[0] = (highAddr << 1) | (lowAddr >> 7);
    MST_Data[1] = (lowAddr << 1) | (data >> 7);
    MST_Data[2] = (data) << 1;
    MST_Data[2] |= parity_bit(MST_Data, 3);

    M1770_Send_Frame(); //send command to slave
}



int main(void) // will work on after setting up initial functions
{

    WDTCTL = WDTPW + WDTHOLD; //stop WD timer





    while(1) {




    }

}

