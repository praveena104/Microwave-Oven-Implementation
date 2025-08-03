

/*
 * File:   main.c
 * Author: pravi
 *
 * Created on 3 January, 2025, 12:52 PM
 */


#include "main.h"

#pragma config WDTE = OFF

static void init_config(void) 
{
    // initialize CLCD
    init_clcd();
    // initialize Matrix keypad
    init_matrix_keypad();
    // initialize timer2
    init_timer2();
    // Enable GIE
    GIE = 1;
    // Enable PEIE
    PEIE = 1;
    
    // initialize buffer and Fan
    FAN_DDR = 0;
    FAN = 0;
    BUZZER_DDR = 0;
    BUZZER = 0;
    
}

void main(void) 
{
    unsigned char key;
    unsigned char operational_flag = POWER_ON_SCREEN, reset_flag;
    
    init_config();
    
    TMR2ON = 0;     // Initially turn OFF timer2
    
    while (1) 
    {
        key = read_matrix_keypad(STATE);        // read matrix key value
        
        if(operational_flag == MENU_SCREEN)     // if menu screen
        {
            if(key == 1)                        // 1 Micro mode
            {
                operational_flag = MICRO_MODE;
                reset_flag = RESET_MODE;
                clear_screen();
            } else if(key == 2)                 // 2 Grill mode
            {
                operational_flag = GRILL_MODE;
                clear_screen();
            } else if(key == 3)                 // 3 convection mode
            {
                operational_flag = CONVECTION_MODE;
                reset_flag = RESET_MODE;
                clear_screen();
            } else if(key == 4)                 // 4 Start mode
            {
                operational_flag = START_MODE;
                reset_flag = RESET_MODE;
                clear_screen();
            }
        } 

        switch(operational_flag)
        {
            case POWER_ON_SCREEN:           // power on screen 
                if(power_on_screen() == SUCCESS)
                {
                    clear_screen();
                    operational_flag = MENU_SCREEN;
                    continue;
                }
                break;
            case DISPLAY_TIME:          // Set time for process
                if(display_time(key, reset_flag) == SUCCESS)
                {
                    operational_flag = OPERATION_MODE;
                    reset_flag = RESET_MODE;
                    clear_screen();
                    TMR2ON = 1;
                    FAN = 1;
                    continue;
                }
                break;
            case OPERATION_MODE:        // perform the operation based on time set
                if(operation_mode(key, reset_flag) == SUCCESS)
                {
                    TMR2ON = 0;
                    clear_screen();
                    operational_flag = MENU_SCREEN;
                }
                break;
            case MENU_SCREEN:           // menu screen
                menu_screen();
                break;
            case MICRO_MODE:            //  Micro Mode
                operational_flag = micro_mode(key, reset_flag);
                reset_flag = RESET_MODE;
                clear_screen();
                //Add logic
                continue;
            case GRILL_MODE:            // Grill Mode
                operational_flag = DISPLAY_TIME;
                reset_flag = RESET_MODE;
                continue;
            case CONVECTION_MODE:       // Convection Mode
                switch(convection_mode(key, reset_flag))
                {
                    case PRE_HEAT:      // Pre Heat oven mode
                        clear_screen();
                        operational_flag = PRE_HEAT;
                        reset_flag = RESET_MODE;
                        TMR2ON = 1;
                        continue;
                    case RESET_MODE:    // Reset the convection mode
                        clear_screen();
                        operational_flag = CONVECTION_MODE;
                        reset_flag = RESET_MODE;
                        continue;
                }
                break;
            case START_MODE:            // Start mode for 30 seconds
                if(start_mode(reset_flag) == SUCCESS)
                {
                    operational_flag = OPERATION_MODE;
                    reset_flag = RESET_MODE;
                    TMR2ON = 1;
                    FAN = 1;
                    continue;
                }
                break;
            case PRE_HEAT:          // Pre Heat oven mode
                if(pre_heat_mode(reset_flag) == SUCCESS)
                {
                    operational_flag = DISPLAY_TIME;
                    reset_flag = RESET_MODE;
                    TMR2ON = 0;
                    clear_screen();
                    continue;
                }
                break;            
        }
        reset_flag = RESET_NOTHING;
    }
    return;
}
