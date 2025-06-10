#include <xc.h>
#include "main.h"
extern volatile unsigned int remaining_time;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    { 
        
        //1;16 prescale 
        if (++count == 1250)
        {
            if (remaining_time > 0)
            {
                remaining_time--; // Decrement remaining time
            }
          
            count = 0;
        }
        
        TMR2IF = 0;
    }
}