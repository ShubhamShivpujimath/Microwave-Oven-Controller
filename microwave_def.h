#ifndef MICROWAVE_DEF_H
#define MICROWAVE_DEF_H

#include <xc.h>

#define _XTAL_FREQ 20000000UL   

#define KPD_ROW_TRIS    TRISB          
#define KPD_COL_TRIS    TRISB          
#define KPD_PORT        PORTB

#define KPD_LAT         PORTB

#define FAN             RA0
#define FAN_TRIS        TRISA0
#define BUZZER          RA1
#define BUZZER_TRIS     TRISA1
#define HEATER          RC2             
#define HEATER_TRIS     TRISC2

#define MAX_TIME_SEC   (10*60)  
#define PREHEAT_SEC    60       

typedef enum { MODE_NONE = 0, MODE_MICRO, MODE_GRILL, MODE_CONVECTION } oven_mode_t;

typedef enum {
    ST_BOOT,
    ST_MENU,
    ST_SET_TIME,
    ST_PREHEAT,
    ST_RUN,
    ST_DONE
} oven_state_t;


extern volatile unsigned char tick_flag;

#endif 