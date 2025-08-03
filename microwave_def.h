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
unsigned char power_on_screen(void);
void menu_screen(void);
unsigned char micro_mode(unsigned char key, unsigned char reset_flag);
unsigned char display_time(unsigned char key, unsigned char reset_flag);
unsigned char operation_mode(unsigned char key, unsigned char reset_flag);
unsigned char convection_mode(unsigned char key, unsigned char reset_flag);
unsigned char pre_heat_mode(unsigned char reset_flag);
unsigned char start_mode(unsigned char reset_flag);

#endif	/* MICROWAVE_DEF_H */


