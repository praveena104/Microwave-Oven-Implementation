
/* Microwave definition functions */

#include "main.h"

unsigned char min = 0, sec = 0, pre_time;
unsigned char flag = 0, count = 0;
unsigned char blink = 0, wait = 0;
unsigned long int temp;

/* clear the display screen */
void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
        __delay_us(500);
}

/* power on screen function */
unsigned char power_on_screen(void)
{
    for(char i=0; i<16; i++)        // display the progress bar on powering display 
    {
        clcd_putch(BAR , LINE1(i));
        __delay_us(300);
    }
    
    clcd_print("Powering On", LINE2(2));        // display powering on msg
    clcd_print("Microwave Oven", LINE3(1));
    
    for(char i=0; i<16; i++)        // display the progress bar on powering display 
    {
        clcd_putch(BAR , LINE4(i));
        __delay_ms(30);
    }
    
    __delay_ms(1000);
    return SUCCESS;
}

/* select main menu screen function */
void menu_screen()
{
    clcd_print("1.Micro", LINE1(0));
    clcd_print("2.Grill", LINE2(0));
    clcd_print("3.Convection", LINE3(0));
    clcd_print("4.Start", LINE4(0));
}

/* Set time display function */
unsigned char display_time(unsigned char key, unsigned char reset_flag)
{   
    if(reset_flag == RESET_MODE)
    {
        count = 0;
        flag = 0;
        min = 0;
        sec = 0;
        wait = 0;
        clcd_print("SET TIME <MM:SS>", LINE1(0));
        clcd_print("TIME:", LINE2(0));
        clcd_print("*:CLEAR  #:ENTER", LINE4(0));
        key = ALL_RELEASED;        
    }
    
    if(key == '#')      // start the process
    {
        return SUCCESS;
    }
    else if(key == '*')     // reset the min and sec
    {
        if(flag)
        {
            min = 0;
            count = 2;
        }
        else
        {
            sec = 0;
            count = 0;
        }
        key = ALL_RELEASED;     // key released to avoid collision
    }
    
    if(key != ALL_RELEASED)     // store the value to min and sec
    {
        if(flag == 0)           // sec field
        {
            if(count == 0 && key < 6)
            {
                count++;
                sec = key;
            }
            else if(count == 1)
            {
                count++;
                sec = sec * 10 + key;
            }
            if(count == 2)
            {
                flag = !flag;
                key = ALL_RELEASED;
            }
        }
        else                // minute field
        {
            if(count == 2)
            {
                count++;
                min = key;
            }
            else if(count == 3)
            {
                count++;
                min = min * 10 + key;
            }
            if(count == 4)
            {
                count = 0;
                flag = !flag;
            }
        }
    }
    
    if(wait++ == 200)       // blink function
    {
        wait = 0;
        blink = !blink;
        clcd_putch((min / 10) + '0', LINE2(5));
        clcd_putch((min % 10) + '0', LINE2(6));
        clcd_putch(':', LINE2(7));
        clcd_putch((sec / 10) + '0', LINE2(8));
        clcd_putch((sec % 10) + '0', LINE2(9));
        
        // blink logic
        if(blink)
        {
            if(flag)
                clcd_print("  ", LINE2(5)); // blink minutes
            else
                clcd_print("  ", LINE2(8)); // blink seconds
        }
    }
    return FAILURE;
}

/* perform operation based on time */
unsigned char operation_mode(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_MODE)        //reset the mode
    {
        clcd_print("TIME = ", LINE1(1));
        clcd_print("4.START/RESUME", LINE2(1));
        clcd_print("5.PAUSE", LINE3(1));
        clcd_print("6.STOP", LINE4(1));
    }
    
    if(key == 4 && TMR2ON && min < 90)      // add 30 seconds to the timer
    {   
        sec = sec + 30;     // increase the second
        
        if(sec > 59)        // if sec > 60, add 1 to min
        {
            min+=1;
            sec = sec % 60;
        }
    }
    else if(key == 4 && !TMR2ON)    // Resume the process
    {
        TMR2ON = 1;
        FAN = 1;
    }
    else if(key == 5)       // Pause the process
    {
        TMR2ON = 0;
        FAN = 0;
    }
    else if(key == 6)       // key 6 to stop the process immediately
    {
        FAN = 0;
        clear_screen();
        clcd_print("STOPPED!", LINE2(4));
        __delay_ms(500);
        return SUCCESS;
    }
    
    if(sec == 0 && min == 0)        // display the msg when time's up
    {
        FAN = 0;                    // Turn off the Fan
        clear_screen();
        clcd_print("TIME UP!!!", LINE2(3));     // display the msg 
        clcd_print("Enjoy your meal", LINE3(0));
        BUZZER = 1;         // Turn ON the buffer for 1 seconds
        __delay_ms(3000);
        BUZZER = 0;         // Turn OFF the buzzer
        return SUCCESS;     // return to main_menu
    }
    clcd_putch((min / 10) + '0', LINE1(8));     // display the time 
    clcd_putch((min % 10) + '0', LINE1(9));
    clcd_putch(':', LINE1(10));
    clcd_putch((sec / 10) + '0', LINE1(11));    // display the second
    clcd_putch((sec % 10) + '0', LINE1(12));
}


/* micro_mode Function */
unsigned char micro_mode(unsigned char key, unsigned char reset_flag)
{
    if(reset_flag == RESET_MODE)
    {
        clcd_print("Power = 900W", LINE2(2));       // display the power value
        __delay_ms(1000);
        clear_screen();
        key = ALL_RELEASED;     // release the key to avoid collision
    }
    
    return DISPLAY_TIME;        // enter to set display time
}

/* convection mode set temperature */
unsigned char convection_mode(unsigned char key, unsigned char reset_flag)
{
    
    if(reset_flag == RESET_MODE)        // reset all
    {
        temp = 0;
        wait = 0;
        count = 0;
        blink = 0;
        clcd_print("SET TEMP < C>", LINE1(0));
        clcd_putch(DEGREE, LINE1(10));
        clcd_print("TEMP = 000", LINE2(0));
        clcd_print("*:CLEAR  #:ENTER", LINE4(0));
        key = ALL_RELEASED;     // release the key to avoid the collision
    }
    
    if(key == '*')          // reset the current temperature and count
    {
        temp = 0;
        count = 0;
    }
    else if(key == '#')     // enter to pre heat
    {
        if(temp == 0)       // if temp is 0, display msg enter minimum temp
        {
            clear_screen();
            clcd_print("Set Min Temp", LINE2(2));
            clcd_print("for Pre-Heating", LINE3(0));
            __delay_ms(800);
            return RESET_MODE; 
        }
        return PRE_HEAT;    // enter to pre heat function
    }
    else if(key != ALL_RELEASED && key != '*' && key != '#')    // enter the temp, store and display it
    {
        // maximum set upto 180 degree
        if(count == 0)
        {
            count++;
            temp = key;
        }
        else if(count == 1)
        {
            count++;
            temp = (temp * 10) + key;
        }
        else if(count == 2)
        {
            count++;
            temp = (temp * 10) + key;
        }
        
        if(count == 3)
            count = 0;
    }
    
    if(temp > 180)      // set limit for temperature upto 180c
    {
        clear_screen();
        clcd_print("Set Max Temp", LINE2(2));
        clcd_print("Upto ", LINE3(3));
        clcd_print("180", LINE3(8));
        clcd_putch(DEGREE, LINE3(11));
        clcd_putch('C', LINE3(12));
        __delay_ms(800);
        return RESET_MODE;
    }
    
    if(wait++ == 250)       // blink the temp field
    {
        wait = 0;
        blink = !blink;
        clcd_putch((temp / 100) + '0', LINE2(7));
        clcd_putch((temp / 10) % 10 + '0', LINE2(8));
        clcd_putch((temp % 10) + '0', LINE2(9));
    
        if(blink)
            clcd_print("   ", LINE2(7));
    }
    return FAILURE;
}

/* pre-Heat for the oven based on temperature */
unsigned char pre_heat_mode(unsigned char reset_flag)
{
    if(reset_flag == RESET_MODE)        // display the temp msg and time left
    {
        pre_time = 180;
        clcd_print("Pre-Heating", LINE1(2));
        clcd_print("Temp: ", LINE2(2));
        clcd_putch((temp / 100) + '0', LINE2(8));
        clcd_putch((temp / 10) % 10 + '0', LINE2(9));
        clcd_putch((temp % 10) + '0', LINE2(10));
        clcd_putch(DEGREE, LINE2(11));
        clcd_putch('C', LINE2(12));
        clcd_print("Time Left: ", LINE3(0));
    }
    
    while(pre_time)             // display pre_time 
    {
        clcd_putch((pre_time / 100) + '0', LINE3(11));
        clcd_putch((pre_time / 10) % 10 + '0', LINE3(12));
        clcd_putch((pre_time % 10) + '0', LINE3(13));
        clcd_putch('s', LINE3(14));
    }
    
    __delay_us(300);
    return SUCCESS;
}

/* Start mode for 30 seconds basic-heating */
unsigned char start_mode(unsigned char reset_flag)     
{
    if(reset_flag == RESET_MODE)
    {
        sec = 30;       // reset the time 30 seconds
        min = 0;
    }
    
    __delay_us(100);
    return SUCCESS;
}
