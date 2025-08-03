/* 
 * File:   main.h
 * Author: emertxe-69
 *
 * Created on 12 November, 2024, 5:04 PM
 */

#ifndef MAIN_H
#define	MAIN_H


#include <xc.h>
#include"clcd.h"
#include "matrix_keypad.h"
#include "microwave_def.h"
#include "timers.h"



#define SUCCESS             0x00
#define FAILURE             0xFF

#define POWER_ON_SCREEN     0x01
#define MENU_SCREEN         0x02

#define MICRO_MODE          0x11
#define GRILL_MODE          0x12
#define CONVECTION_MODE     0x13
#define START_MODE          0x14

#define RESET_NOTHING       0x22
#define RESET_MODE          0x33
#define DISPLAY_TIME        0x44
#define OPERATION_MODE      0x55

#define START               0xAA
#define STOP                0xBB
#define PAUSE               0xCC
#define PRE_HEAT            0xDD

#define BUZZER_DDR          TRISC1
#define BUZZER              RC1

#define FAN_DDR             TRISC2
#define FAN                 RC2
 
#endif	

