#include "msp.h"

int main(void)
{
    // Hold the watchdog

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    P1->DIR = ~BIT1;   // Set P1.1 input
    P1->OUT = BIT1;    // Set pull-up mode
    P1->REN = BIT1;    // Enable pull-up resistor
    P1->SEL0 = 0;      // Select GPIO
    P1->SEL1 = 0;      // Select GPIO


    P1->DIR |= BIT0;    // P1.0 set as input

    P1->IES = BIT1; // Interrupt on high-to-low transition
    P1->IFG = 0;    // Clear all P1 interrupt flags
    P1->IE = BIT1;  // Enable interrupt for P1.1

    // Enable Port 1 interrupt on the NVIC
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);

    while(1){
        __sleep();        // Enter LPM0 mode. Execution stops here.
        __no_operation(); // Just added here to have a line (to step over during debugging)
    }

}

/* Port1 ISR */
void PORT1_IRQHandler(void)
{
    // Check if P1.1 interrupt triggered
    if(P1->IFG & BIT1){
        // Toggle the output on the LED
        P1->OUT ^= BIT0;
    }

    // clear the flag
    P1->IFG &= ~BIT1;
}

