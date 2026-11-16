
#include "keypad.h"
#include "microwave_def.h"

static const char keymap[4][4] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

void kpd_init(void)
{
    KPD_ROW_TRIS &= 0x0F;   
    KPD_COL_TRIS |= 0x0F;   
    KPD_LAT |= 0xF0;        
}


char kpd_getkey(void)
{
    unsigned char row, col;
    static unsigned char last_state = 0;

    for (row = 0; row < 4; row++)
    {
        KPD_LAT |= 0xF0;                
        KPD_LAT &= ~(0x10 << row);       
        __delay_us(5);

        for (col = 0; col < 4; col++)
        {
            if (!(KPD_PORT & (1 << col)))
            {
                if (last_state) return 0;    
                __delay_ms(20);             
                if (KPD_PORT & (1 << col)) continue;
                last_state = 1;
                while (!(KPD_PORT & (1 << col)));  
                last_state = 0;
                KPD_LAT |= 0xF0;
                return keymap[row][col];
            }
        }
    }
    return 0;
}