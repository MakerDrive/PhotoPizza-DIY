/*
 * lib.h
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_LIB_H_
#define PHOTOPIZZA_V1_2_2_LIB_H_

#include <Arduino.h>

void execute_preset();

void run_lcd_acc();

void run_lcd_no_acc();

void run_no_lcd_acc();

void run_no_lcd_no_acc();

void say_hello();
///////////////////////////////////////

///////// edit preset

void value_u();

void value_d();

void update_preset();
///////////////////////////////////////

///////// print info

void show_curr_program(bool _is_edit);

void print_prog_num();

void print_ul(String _pref, long _value );

void print_dir(int _dir );

void print_dir_small(int _dir );

void print_ir_error(char _sym);
///////////////////////////////////////

///////// LCD Buttons
int read_LCD_buttons();

void edit_preset_mode();

void menu_mode();


#endif /* PHOTOPIZZA_V1_2_2_LIB_H_ */
