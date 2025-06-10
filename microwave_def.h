/* 
 * File:   microwave_def.h
 * Author: emertxe-69
 *
 * Created on 12 November, 2024, 5:03 PM
 */

#ifndef MICROWAVE_DEF_H
#define	MICROWAVE_DEF_H

/*Add Function Declarations*/
void clear_screen(void);
void power_on(void);
void menu_screen(void);
void micro_mode(void); 
void grill_mode(void);
void convection_mode(void); 
unsigned int set_time(void);
void display_time(void);
void start_timer(void);
void pause_timer(void);
void stop_timer(void);
void display_task_done(void);
void set_temperature(void);

#endif	/* MICROWAVE_DEF_H */

