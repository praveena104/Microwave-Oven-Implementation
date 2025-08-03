#include <xc.h>
#include "main.h"

extern unsigned char sec,min,pre_time;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        // 1:16 prescale
        if (++count == 1250)
        {
            count = 0;
            if(sec-- == 0 && min >= 0)
            {
                sec = 59;
                min--;
            }
            
            if(pre_time > 0)
                pre_time--;
        }
        
        TMR2IF = 0;
    }
}
