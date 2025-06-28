#include <msp430.h> 
#include <stdint.h>
#include "lcd.h"
#include <stdint.h>

#define LCD_REFRESH_DELAY   500         // LCD Refresh rate in milliseconds

#if _LCD_TYPE_ == 1
#define String1  "    Hello World    "
#define String2  "    Hai MSP430     "
#define String3  "    This is        "
#define String4  "    LCD TEST       "

#elif _LCD_TYPE_ == 2
#define String1  "    Hello World    "
#define String2  "    Hai MSP430     "
#endif



/*** Function    : stopWatchDog
**   Parameters  : None
**   Return      : None
**   Description : It will stop Watch Dog timer of MSP430
**/
void stopWatchDog(void)
{
    WDTCTL = WDTPW + WDTHOLD;         // Stop Watch Dog timer
}

/*** Function    : main
**   Parameters  : None
**   Return      : int
**   Description : It is entry point of program
**/
int main(void)
{

    stopWatchDog();
    lcdBegin();// Initiate the LCD

    P2DIR|=0x07;

    P1DIR|=0x40; //za buzzer P1.6
    P1SEL|=0x40;


    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    TACTL = TASSEL_2 + MC_1; // SMLCK, up mode
    TACCR0 = 250;
    TACCR1 = 0;
    TACCTL1 = OUTMOD_7;

    ADC10CTL0 = ADC10SHT_2 + ADC10ON;  // ADC ON, sample-and-hold time
    ADC10CTL1 = INCH_7;                // Use P1.7 (A7, channel 7)
    ADC10AE0 |= 0x80;// Enable analog input on P1.7 (hex 0x80 = BIT7)

    __delay_cycles(1000);

    while(1)
    {
        ADC10CTL0 |= ENC + ADC10SC;    // Start ADC conversion
        while (ADC10CTL1 & ADC10BUSY); // Wait for conversion
       int adcValue = ADC10MEM;       // Read ADC value

        if (adcValue > 240){
            P2OUT|=0x01;
            P2OUT&=~0x06;
            TACCR1 = 100;
        }
        else if (adcValue <= 240 && adcValue > 210){
            P2OUT|=0x04;
            P2OUT&=~0x03;
            TACCR1 ^= 10;
        }
        else {
           P2OUT&=~0x05;
           P2OUT|=0x02;
           TACCR1 = 0;
        }
        __delay_cycles(150000);

    }
}
