#include "main.h"

/*Add function definitions here*/
extern unsigned char operational_flag;
unsigned char time_str[6] = {'0', '0', ':', '0', '0'};
unsigned int time = 0;
int i;
unsigned char key, pos = 0,secs;
unsigned char temp_str[3]={'0','0','0'};
unsigned char time_field = 1; //1-seconds field 0-minute field
volatile unsigned int remaining_time = 0;
unsigned char is_paused = 0;

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

void power_on(void)
{
    for (i = 0; i < 16; i++) 
    {
        __delay_ms(200);
        clcd_putch(0xFF,LINE1(i));
    }
     
    clcd_print("Powering on",LINE2(3));
    clcd_print("Microwave Oven",LINE3(1));
    for (int i = 0; i < 16; i++) 
    {
        __delay_ms(200);
        clcd_putch(0xFF,LINE4(i));
    }
    __delay_ms(1500);
    clear_screen();
}
void menu_screen(void)
{
    
    clcd_print("1.Micro",LINE1(0));
    clcd_print("2.Grill",LINE2(0));
    clcd_print("3.Convection",LINE3(0));
    clcd_print("4.Start",LINE4(0));
}

void micro_mode(void) 
{
    clear_screen(); 
    clcd_print("Power = 900W", LINE2(0));
   __delay_ms(1000);
     
    set_time();
    if (key == '#')
    {
        display_time();
        display_task_done();
    }
   
}
void grill_mode(void) 
{
    clear_screen();
    set_time();
    if (key == '#')
    {
        display_time();
        display_task_done();
    }
}


void set_temperature(void)
{       //check output image
    clear_screen();
    clcd_print("SET Temp <*C>", LINE1(0));
    clcd_print("TEMP:", LINE2(0));
    clcd_putch(temp_str[0], LINE2(5));
    clcd_putch(temp_str[1], LINE2(6));
    clcd_putch(temp_str[2], LINE2(7));
    clcd_print("*:CLEAR", LINE4(0));
    clcd_print("#:ENTER", LINE4(9));
    unsigned char temp_field = 0; // 0-first digit, 1-second, 2-third digit
    while (1)
    {
        key = read_matrix_keypad(STATE);
        if (key >= 0 && key <= 9)
        {
            temp_str[temp_field] = key + '0';
            temp_field++;
            if (temp_field > 2)
                temp_field = 0; // Loop around digits
        }
        else if (key == '*')
        {
            // Clear current digit
            temp_str[temp_field] = '0';
        }
        else if (key == '#')
        {
            // Enter pressed, break
            break;
        }
        // Display blinking digit for current edit
        for (int i = 0; i < 3; i++) 
        {
            if (i == temp_field) 
            {
                clcd_putch(' ', LINE2(5 + i));
            } 
            else 
            {
                clcd_putch(temp_str[i], LINE2(5 + i));
            }
        }
        __delay_ms(500);
        for (int i = 0; i < 3; i++) 
        {
            clcd_putch(temp_str[i], LINE2(5 + i));
        }
        __delay_ms(500);
    }
    clear_screen();
}
            
void convection_mode(void) 
{
    clear_screen();
    set_temperature();
    clcd_print("Pre-heating", LINE1(3));
    clcd_print("Time left:", LINE2(0));
    clcd_putch('s', LINE2(14));
    secs=180;
    while(secs>0)
    {
        clcd_putch(secs/100+'0', LINE2(11));
        clcd_putch((secs/10)%10 +'0', LINE2(12));
        clcd_putch(secs%10 +'0', LINE2(13));
    
        __delay_ms(500);
    secs--;
    }
    
    set_time();
    
    if(key=='#')
    {
        display_time();
        display_task_done();
    }
}

unsigned int set_time(void) 
{
    clear_screen();
    clcd_print("SET TIME <MM:SS>", LINE1(0));
    clcd_print("TIME:", LINE2(0));
    clcd_putch(time_str[0], LINE2(5));
    clcd_putch(time_str[1], LINE2(6));
    clcd_putch(':',LINE2(7));
    clcd_putch(time_str[3], LINE2(8));
    clcd_putch(time_str[4], LINE2(9));
    clcd_print("*:CLEAR", LINE4(0));
    clcd_print("#:ENTER", LINE4(9));

    while (1) 
    {
        
        if (time_field == 1) 
        { // Seconds field
            
            clcd_putch(time_str[3], LINE2(8));
            clcd_putch(time_str[4], LINE2(9));
            __delay_ms(500); // Blink effect
            
            clcd_putch(' ', LINE2(8)); // Clear seconds
            clcd_putch(' ', LINE2(9));
            __delay_ms(500);
        } 
        else 
        { // // Minutes field
            clcd_putch(time_str[0], LINE2(5));
            clcd_putch(time_str[1], LINE2(6));
            __delay_ms(500); // Blink effect
            clcd_putch(' ', LINE2(5)); // Clear minutes
            clcd_putch(' ', LINE2(6));
            __delay_ms(500);
        }
        
        key = read_matrix_keypad(STATE);
        
        if (key >= 0 && key <= 9) 
        {
            if (time_field == 1) 
            { // Setting seconds
                if(time_str[3]=='0')
                {
                time_str[3] = key + '0';
                }
                else
                {
                time_str[4] = key + '0';// Set second digit
                time_field = 0; // Move to minutes field
                }
            } 
            else 
            { // Setting minutes
                if(time_str[0]=='0')
                {
                time_str[0] = key + '0'; 
                }
                else
                {
                time_str[1] = key + '0'; // Set minute digit
                time_field = 1; // Reset to seconds field
            }
            }
        } 

        else if (key == '*') 
        {
            // Clear current field
            if (time_field == 1) 
            {
                time_str[3] = '0'; // Clear seconds
                time_str[4] = '0';
            } 
            else 
            {
                time_str[0] = '0'; // Clear minutes
                time_str[1] = '0';
            }
        }
        else if (key == '#') 
        {
           
            remaining_time = ((time_str[0] - '0') * 10 + (time_str[1] - '0')) * 60 + 
                             ((time_str[3] - '0') * 10 + (time_str[4] - '0'));
             //display_time();
            operational_flag = DISPLAY_TIME;
            break;
        }
        
           
    }
    return remaining_time; 
}
void display_time(void) 
{
    clear_screen();
    TMR2ON = 1; // Start timer hardware
    is_paused = 0;
    while (remaining_time > 0) 
    {
      
        time_str[0] = ((remaining_time / 60) / 10) + '0';
        time_str[1] = ((remaining_time / 60) % 10) + '0';
        time_str[3] = ((remaining_time % 60) / 10) + '0';
        time_str[4] = ((remaining_time % 60) % 10) + '0';
        clcd_print("Time:", LINE1(0));
        clcd_print(time_str, LINE1(5)); // Display remaining time in MM:SS format
        clcd_print("4.START/RESUME", LINE2(2));
        clcd_print("5.PAUSE", LINE3(2));
        clcd_print("6.STOP", LINE4(2));
        __delay_ms(1000); // Update every second
        if (!is_paused) 
        {
        remaining_time--; // Decrement remaining time
        }
        FAN = 1;
        key = read_matrix_keypad(STATE);
    if (key == 4) 
    { 
        if (is_paused) 
        {
            //is_paused = 0; 
            start_timer(); 
        }
    } 
    else if (key == 5) 
    { 
        pause_timer(); 
        
        clcd_print("Time:", LINE1(0));
        clcd_print(time_str, LINE1(5));
        clcd_print("4.START/RESUME", LINE2(2));
        clcd_print("5.PAUSE", LINE3(2));
        clcd_print("6.STOP", LINE4(2));
    } 
    else if (key == 6) 
    { 
        stop_timer();
        break; //stop displaying when stopped
    }
    }
    FAN = 0;
    BUZZER = 1;
    display_task_done(); // Notify that the task is done
    BUZZER = 0;
    clear_screen();
    operational_flag = MENU_SCREEN;
}

void start_timer(void)
{
    is_paused = 0;
    TMR2ON = 1;
   /* while (remaining_time > 0 && !is_paused) 
    {
        //clear_screen();
       
        __delay_ms(1000);
        remaining_time--;
    }
    if (remaining_time == 0) 
    {
        FAN = 0;
        BUZZER = 1;
        display_task_done(); 
    }
    TMR2ON = 0; */
}
void pause_timer(void)
{
    is_paused = 1; 
    TMR2ON = 0;
    FAN = 0;
}
void stop_timer(void)
{
 remaining_time = 0; 
 TMR2ON = 0;
 FAN = 0;
 BUZZER = 1;
 
}

void display_task_done(void)
{
    clear_screen();
    clcd_print("TIME UP!!!",LINE2(4));
    clcd_print("Enjoy your meal",LINE3(0));
    __delay_ms(3000);
    clear_screen();
    
}