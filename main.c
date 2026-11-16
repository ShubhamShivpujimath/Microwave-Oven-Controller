/*
 NAME: SHUBHAM SHIVPUJIMATH
 * DESCRIPTION: MICROWAVE OVEN
 */
#include <xc.h>
#include <stdio.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#include "microwave_def.h"
#include "clcd.h"
#include "keypad.h"

void timer0_init(void);   

static void buzz(unsigned int ms)
{
    BUZZER = 1;
    while (ms--) __delay_ms(1);
    BUZZER = 0;
}

int main(void)
{
    oven_state_t state = ST_BOOT;
    oven_mode_t  mode  = MODE_NONE;
    oven_mode_t  last_mode = MODE_NONE;

    unsigned int  set_sec = 0;
    unsigned int  digits[4] = {0, 0, 0, 0};
    unsigned char digit_idx = 0;

    unsigned int  remain_sec = 0;
    unsigned int  last_set_sec = 0;
    unsigned char paused = 0;
    char key;
    char buf[17];

    FAN_TRIS = 0;    FAN = 0;
    BUZZER_TRIS = 0; BUZZER = 0;
    HEATER_TRIS = 0; HEATER = 0;

    init_clcd();
    kpd_init();
    timer0_init();

    while (1)
    {
        switch (state)
        {
        
        case ST_BOOT:
            clear_screen();
            clcd_print("Powering on",    LINE1(0));
            clcd_print("Microwave oven", LINE2(0));
            __delay_ms(1500);
            clear_screen();
            state = ST_MENU;
            break;

        
        case ST_MENU:
            clcd_print("1.Micro",      LINE1(0));
            clcd_print("2.Grill",      LINE2(0));
            clcd_print("3.Convection", LINE3(0));
            clcd_print("4.Start",      LINE4(0));

            key = kpd_getkey();
            if (key == '1' || key == '2' || key == '3')
            {
                mode = (key == '1') ? MODE_MICRO :
                       (key == '2') ? MODE_GRILL : MODE_CONVECTION;
                digit_idx = 0;
                digits[0] = digits[1] = digits[2] = digits[3] = 0;
                clear_screen();
                state = ST_SET_TIME;
            }
            else if (key == '4' && last_mode != MODE_NONE)
            {
                mode = last_mode;
                remain_sec = last_set_sec;
                clear_screen();
                state = (mode == MODE_MICRO) ? ST_RUN : ST_PREHEAT;
                if (state == ST_PREHEAT) remain_sec = PREHEAT_SEC;
                paused = 0;
            }
            break;

  
        case ST_SET_TIME:
            clcd_print("SET TIME <MM:SS>", LINE1(0));
            sprintf(buf, "TIME:%d%d:%d%d", digits[0], digits[1], digits[2], digits[3]);
            clcd_print(buf, LINE2(0));
            clcd_print("*:CLEAR  #:ENTER", LINE4(0));

            key = kpd_getkey();
            if (key >= '0' && key <= '9')
            {
                if (digit_idx < 4) digits[digit_idx++] = key - '0';
            }
            else if (key == '*')
            {
                digit_idx = 0;
                digits[0] = digits[1] = digits[2] = digits[3] = 0;
            }
            else if (key == '#')
            {
                unsigned int mm = digits[0] * 10 + digits[1];
                unsigned int ss = digits[2] * 10 + digits[3];
                set_sec = mm * 60 + ss;
                if (set_sec > MAX_TIME_SEC) set_sec = MAX_TIME_SEC;
                if (set_sec == 0) break;

                last_mode = mode;
                last_set_sec = set_sec;
                remain_sec = set_sec;
                paused = 0;
                clear_screen();

                if (mode == MODE_MICRO)
                {
                    state = ST_RUN;
                }
                else
                {
                    remain_sec = PREHEAT_SEC;
                    state = ST_PREHEAT;
                }
            }
            break;

       
        case ST_PREHEAT:
            FAN = 1;
            clcd_print("Pre-heating", LINE1(0));
            sprintf(buf, "Time left: %03us", remain_sec);
            clcd_print(buf, LINE2(0));

            if (tick_flag)
            {
                tick_flag = 0;
                if (remain_sec > 0) remain_sec--;
                else
                {
                    remain_sec = last_set_sec;
                    clear_screen();
                    state = ST_RUN;
                }
            }
            break;

       
        case ST_RUN:
            sprintf(buf, "TIME = %02u:%02u", remain_sec / 60, remain_sec % 60);
            clcd_print(buf,              LINE1(0));
            clcd_print("4.START/RESUME", LINE2(0));
            clcd_print("5.PAUSE",        LINE3(0));
            clcd_print("6.STOP",         LINE4(0));

            key = kpd_getkey();
            if (key == '5')      { paused = 1; HEATER = 0; FAN = 0; }
            else if (key == '4') { paused = 0; }
            else if (key == '6')
            {
                HEATER = 0; FAN = 0;
                clear_screen();
                state = ST_MENU;
                break;
            }

            if (!paused)
            {
                HEATER = 1;
                FAN = 1;
                if (tick_flag)
                {
                    tick_flag = 0;
                    if (remain_sec > 0) remain_sec--;
                    else
                    {
                        HEATER = 0;
                        FAN = 0;
                        clear_screen();
                        state = ST_DONE;
                    }
                }
            }
            break;

        
        case ST_DONE:
            clcd_print("Cooking",   LINE1(0));
            clcd_print("Complete!", LINE2(0));
            for (unsigned char i = 0; i < 3; i++)
            {
                buzz(200);
                __delay_ms(200);
            }
            __delay_ms(1000);
            clear_screen();
            state = ST_MENU;
            break;
        }
    }
}