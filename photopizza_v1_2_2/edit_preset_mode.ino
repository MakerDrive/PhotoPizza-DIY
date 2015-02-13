void edit_preset_mode(){
long _val;

  switch(key){
  case 0:
//    menu_param_pos = 0;
    e_flag=0;
    show_curr_program(true);
    break;

  case BTN_POWER:  //exit without writing to mem
    menu_param_pos = 0;
    key=0;
    e_flag=0;
    cur_mode = MENU_MODE;
    show_curr_program(false);
    break;

  case BTN_FUNC: // write to mem and exit 
//    menu_param_pos = 0;
    key=0;
    e_flag=0;    
    cur_mode = MENU_MODE;
    update_preset();
    show_curr_program(false);
    break;

  case BTN_CH_U:
  case btnUP:
    e_flag=0;  
    menu_param_pos = (menu_param_pos+1)%4;
    show_curr_program(true);
    break;

  case BTN_CH_D:
  case btnDOWN:
    e_flag=0;  
    menu_param_pos = (menu_param_pos+3)%4;
    show_curr_program(true);   
    break;

  case BTN_VOL_D:
  case btnLEFT:
    e_flag=0;  
    value_d();
    show_curr_program(true); 
    break;

  case BTN_VOL_U:
  case btnRIGHT:
    e_flag=0;  
    value_u();
    show_curr_program(true); 
    break;

  case BTN_0:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_1:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 1;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_2:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 2;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_3:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 3;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_4:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 4;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_5:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 5;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_6:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 6;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_7:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 7;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_8:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 8;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_9:
    if(e_flag!=0){
      _val = programs.get_val(menu_param_pos)*10 + 9;
      programs.change_val(_val,menu_param_pos); 
      show_curr_program(true); 
    }
    break; 

  case BTN_EQ:
    if(e_flag==0){
      e_flag=1;
      _val = 0;
      programs.change_val(_val,menu_param_pos); 
    } else {
      e_flag=0;
    }
    show_curr_program(true); 
    break; 

  case BTN_ST:
    programs.change_val(0,menu_param_pos); 
    show_curr_program(true); 
    break; 

  case BTN_RW:
    _val = programs.get_val(menu_param_pos) /10;
    programs.change_val(_val,menu_param_pos); 
    show_curr_program(true); 
    break; 

  case BTN_FW:
    _val = programs.get_val(menu_param_pos) * 10;
    programs.change_val(_val,menu_param_pos); 
    show_curr_program(true); 
    break; 

  default:
    print_ir_error('#');
    break;
  }

}
