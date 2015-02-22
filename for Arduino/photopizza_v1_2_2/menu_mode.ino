#include "defines.h"

void menu_mode(){
  switch(key){
  case 0:   // first call, no key pressed yet
    menu_param_pos = 0;
    show_curr_program(false);
    break;
  case BTN_VOL_U:
    menu_param_pos = (menu_param_pos+1)%4;
    show_curr_program(false);
    break;

  case BTN_VOL_D:
    menu_param_pos = (menu_param_pos+3)%4;
    show_curr_program(false);   
    break;

  case BTN_CH_U:
  case btnRIGHT:
    programs.next();
    cur_preset = programs.get_cur_preset();
    menu_param_pos = 0;
    show_curr_program(false);
    break;

  case BTN_CH_D:
  case btnLEFT:
    programs.prev();
    cur_preset = programs.get_cur_preset();
    menu_param_pos = 0;
    show_curr_program(false);
    break;

  case BTN_FW:
    programs.change_direction(CW);
    cur_preset = programs.get_cur_preset();
    show_curr_program(false);
    break;

  case BTN_RW:
    programs.change_direction(CCW);
    cur_preset = programs.get_cur_preset();
    show_curr_program(false);
    break;

  case BTN_FUNC:
    cur_mode = EDIT_MODE;
    key = 0;
    edit_preset_mode();
    break; 

  case BTN_PLAY:
  case btnUP:
    cur_mode = EXEC_MODE;
    execute_preset();
    break; 

  default:
    print_ir_error('#');
    break;
  } 

}









