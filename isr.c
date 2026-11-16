#include "microwave_def.h"

volatile unsigned char tick_flag = 0;
static unsigned char ovf_count = 0;
#define OVERFLOWS_PER_SEC 76

void timer0_init(void)
{
    OPTION_REG = 0b00000111;  
    TMR0 = 0;
    T0IE = 1;
    GIE  = 1;
}

void __interrupt() isr(void)
{
    if (T0IF)
    {
        T0IF = 0;
        TMR0 = 0;
        if (++ovf_count >= OVERFLOWS_PER_SEC)
        {
            ovf_count = 0;
            tick_flag = 1;
        }
    }
}