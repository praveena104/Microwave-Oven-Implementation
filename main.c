/*
 * File:   main.c
 * Author: emertxe
 *
 * Created on 12 November, 2024, 4:42 PM
 */


#include "main.h"
#pragma config WDTE = OFF

static void init_config(void) {

    init_clcd();
    init_matrix_keypad();
    FAN_DDR = 0;
    FAN = 0;
    BUZZER_DDR = 0;
    BUZZER = 0;

    init_timer2();
    
    GIE = 1;
    PEIE = 1;
}
//extern unsigned int remaining_time = 0;
unsigned char operational_flag = POWER_ON_SCREEN;
void main(void) {
    init_config();
    unsigned char key;
    unsigned char reset_flag;
    volatile unsigned int remaining_time = 0;
    unsigned char is_paused = 0;
    while (1) {
        key = read_matrix_keypad(STATE);
        if (operational_flag == MENU_SCREEN) 
        {
            
            if (key == 1) 
            {
                /*Update flags and clear screen*/
                operational_flag = MICRO_MODE;
                 clear_screen();
            } 
            else if (key == 2) 
            {
                /*Update flags and clear screen*/
                operational_flag = GRILL_MODE;
                 clear_screen();
            } 
            else if (key == 3)
            {
                /*Update flags and clear screen*/
                operational_flag = CONVECTION_MODE;
                 clear_screen();
            } 
            else if (key == 4) 
            {
                /*Update flags and clear screen*/
                operational_flag = START_MODE;
                 clear_screen();
            }
        } 
       /* else if (operational_flag == DISPLAY_TIME) 
        {
            //Add logic
            display_time();
            operational_flag = MENU_SCREEN;
            
        } 
        else if (operational_flag == PAUSE) 
        {
            pause_timer();
            //Add logic
           // break;
        }*/

        switch (operational_flag) {
            case POWER_ON_SCREEN:
                //Add logic
                power_on();
                operational_flag = MENU_SCREEN;
                break;
            case MENU_SCREEN:
                //Add logic
              
                menu_screen();
                
                break;
            case DISPLAY_TIME:
                //add logic
               display_time();
                break;
            case MICRO_MODE:
                //Add logic
                micro_mode();
                break;
            case GRILL_MODE:
               //Add logic
                grill_mode();
                break;
            case CONVECTION_MODE:
              //add logic
                //clear_screen();
                convection_mode();
                break;
            case START_MODE:
              //add logic
                remaining_time = 30;
                is_paused=0;
                display_time();
                clear_screen();
                operational_flag = MENU_SCREEN;
                break;
            case STOP:
              //add logic
                stop_timer();
                clear_screen();
                operational_flag = MENU_SCREEN;
                break;
            case PAUSE:
             //add logic
                pause_timer();
                clear_screen();
                operational_flag = MENU_SCREEN;
                
                break;
            
        }
        reset_flag = RESET_NOTHING;
    }
    return;
}
