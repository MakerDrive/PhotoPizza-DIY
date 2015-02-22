///////// Execute preset

#include "defines.h"

void execute_preset(){
  lcd.setCursor(0,1);
  lcd.print("Program started ");
  lcd.setCursor(13,0);
  lcd.print("P ");			 // P  - perform
  delay(500);                // IR sensetive
  exec_flag = true;
  start_interrupt = false;

  stepper.setCurrentPosition(0L);
  stepper.setAcceleration(cur_preset.acc);
 
  if(lcd_flag && cur_preset.acc==0){
    run_lcd_no_acc();
  }
  else if (lcd_flag && cur_preset.acc!=0){
    run_lcd_acc();
  }
  else if (!lcd_flag && cur_preset.acc==0){
    run_no_lcd_no_acc();
  }
  else{     // !lcd_flag && cur_preset.acc!=0
    run_no_lcd_acc();
  }

  start_interrupt = false;
  lcd.setCursor(0,1);
  lcd.print("Program finished");
  delay(1000);
  cur_mode = MENU_MODE;
  menu_param_pos = 0;
  show_curr_program(false);
}


void run_lcd_acc(){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(7,1);
  lcd.print("/");
  lcd.print(cur_preset.rot);

  long tmprot = cur_preset.rot*cur_preset.dir;
  float tmpsp =float( cur_preset.sp*cur_preset.dir);
  long tmpstop =cur_preset.dir* cur_preset.sp*cur_preset.sp/(2*cur_preset.acc); 
  long tmpstart = 2.5*tmpstop;
  stepper.setMaxSpeed(cur_preset.sp);
  stepper.moveTo(tmpstart*3);
  stepper.run();    // first 2 steps
  stepper.run();  
  // acceleration
  lcd.setCursor(14,0);
  lcd.print("A");
  if(cur_preset.dir==CCW){
    while((stepper.currentPosition() <= tmpstart)&& exec_flag){
      stepper.run();
    } 
  }
  else{
    while((stepper.currentPosition() >= tmpstart)&& exec_flag){
      stepper.run();
    } 
  }

  // move
  lcd.setCursor(14,0);
  lcd.print("R");
  if(tmprot==0){
    start_interrupt = true;
    while(true && exec_flag){
      lcd.setCursor(0,1);
      lcd.print(stepper.currentPosition()-tmpstart);
      stepper.runSpeed();  
    }

  }
  else{
    stepper.move(tmprot+tmpstop);
    stepper.setSpeed(tmpsp);

    if(cur_preset.dir==CCW){
      while((stepper.currentPosition() <= tmprot+tmpstart)&& exec_flag){
        lcd.setCursor(0,1);
        lcd.print(stepper.currentPosition()-tmpstart);
        stepper.runSpeed();
      }
    }
    else{
      while((stepper.currentPosition() >= tmprot+tmpstart)&& exec_flag){
        lcd.setCursor(0,1);
        lcd.print(stepper.currentPosition()-tmpstart);
        stepper.runSpeed();
      }      
    }
  }
  // stop
  lcd.setCursor(14,0);
  lcd.print("S");

  stepper.stop();
  stepper.runToPosition();  


};

void run_lcd_no_acc(){
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(7,1);
  lcd.print("/");
  lcd.print(cur_preset.rot);

  long tmprot = cur_preset.rot*cur_preset.dir;
  float tmpsp =float( cur_preset.sp*cur_preset.dir);

  stepper.setMaxSpeed(tmpsp);    

  lcd.setCursor(14,0);
  lcd.print("R");

  if(tmprot==0){  // infinite
    start_interrupt = true;
    stepper.moveTo(10*tmpsp);
    stepper.setSpeed(tmpsp);
    while(true && exec_flag){
      lcd.setCursor(0,1);
      lcd.print(stepper.currentPosition());
      stepper.runSpeed();
    }
  }
  else{
    stepper.moveTo(tmprot);
    stepper.setSpeed(tmpsp);
    while((stepper.currentPosition() != cur_preset.rot) && exec_flag){
      lcd.setCursor(0,1);
      lcd.print(stepper.currentPosition());
      stepper.runSpeed();
    }  
  }
};

void run_no_lcd_acc(){
  long tmprot = cur_preset.rot*cur_preset.dir;
  float tmpsp =float( cur_preset.sp*cur_preset.dir);
  long tmpstop =cur_preset.dir* cur_preset.sp*cur_preset.sp/(2*cur_preset.acc); 
  long tmpstart = 2.5*tmpstop;
  stepper.setMaxSpeed(cur_preset.sp);
  stepper.moveTo(tmpstart*3);
  stepper.run();    // first 2 steps
  stepper.run();  
  // acceleration
  lcd.setCursor(14,0);
  lcd.print("A");
  if(cur_preset.dir==CCW){
    while((stepper.currentPosition() <= tmpstart)&& exec_flag){
      stepper.run();
    } 
  }
  else{
    while((stepper.currentPosition() >= tmpstart)&& exec_flag){
      stepper.run();
    } 
  }

  // move
  lcd.setCursor(14,0);
  lcd.print("R");
  if(tmprot==0){
    start_interrupt = true;
    while(true && exec_flag){
      stepper.runSpeed();  
    }
  }
  else{
    stepper.move(tmprot+tmpstop);
    stepper.setSpeed(tmpsp);

    if(cur_preset.dir==CCW){
      while((stepper.currentPosition() <= tmprot+tmpstart)&& exec_flag){
        stepper.runSpeed();
      }
    }
    else{
      while((stepper.currentPosition() >= tmprot+tmpstart)&& exec_flag){
        stepper.runSpeed();
      }      
    }
  }
  // stop
  lcd.setCursor(14,0);
  lcd.print("S");

  stepper.stop();
  stepper.runToPosition();  

};

void run_no_lcd_no_acc(){
  lcd.setCursor(14,0);
  lcd.print("R");

  long tmprot = cur_preset.rot*cur_preset.dir;
  float tmpsp =float( cur_preset.sp*cur_preset.dir);

  stepper.setMaxSpeed(tmpsp);    

  if(tmprot==0){  // infinite
    start_interrupt = true;
    stepper.moveTo(10*tmpsp);
    stepper.setSpeed(tmpsp);
    while(true && exec_flag){
      stepper.runSpeed();
    }
  }
  else{
    stepper.moveTo(tmprot);
    stepper.setSpeed(tmpsp);
    while((stepper.currentPosition() != cur_preset.rot) && exec_flag){
      stepper.runSpeed();
    }  
  }
};

///////////////////////////////////////

///////// edit preset

void value_u(){
  switch(menu_param_pos){
  case SPEEED:
    programs.change_val_u(SPEED_STEP,SPEEED);
    break;

  case ROT:
    programs.change_val_u(ROT_STEP,ROT);
    break;

  case ACC:
    programs.change_val_u(ACC_STEP,ACC);
    break;

  case DIR:
    programs.change_val_u(CW,DIR);
    break;
  }

  cur_preset = programs.get_cur_preset();
}

void value_d(){
  switch(menu_param_pos){
  case SPEEED:
    programs.change_val_d(SPEED_STEP,SPEEED);
    break;

  case ROT:
    programs.change_val_d(ROT_STEP,ROT);
    break;

  case ACC:
    programs.change_val_d(ACC_STEP,ACC);
    break;

  case DIR:
    programs.change_val_d(CCW,DIR);
    break;
  }  

  cur_preset = programs.get_cur_preset();
}

void update_preset(){  // read mem -> check for changes -> write if changed => EEPROM live longer =)
  Preset_st orig;
  EEPROM_readAnything((1 + programs.get_cur()*PRESET_SIZE) ,orig);

  if((orig.sp != cur_preset.sp) || (orig.rot != cur_preset.rot) || (orig.acc != cur_preset.acc)  || (orig.dir != cur_preset.dir) ){
    EEPROM_writeAnything((1 + programs.get_cur()*PRESET_SIZE) , cur_preset); 
    byte f = 204;
    EEPROM_writeAnything(0, f); 
  }
}

///////////////////////////////////////

///////// print info

void show_curr_program(boolean _is_edit){
  lcd.clear();
  print_prog_num();
  print_dir_small(cur_preset.dir);

  if(_is_edit){
    lcd.setCursor(14,0); 
    lcd.print("E");

    cur_preset = programs.get_cur_preset();
  }
  else{
    lcd.setCursor(14,0); 
    lcd.print("M");  
  }

  switch(menu_param_pos){
  case SPEEED:
    print_ul("sp", cur_preset.sp);
    break;

  case ROT:
    if(cur_preset.rot==0){
      lcd.setCursor(0,1); 
      lcd.print("rot");
      lcd.setCursor(4,1); 
      lcd.print("inf");
    }
    else{
      print_ul("rot", cur_preset.rot);
    }
    break;

  case ACC:
    print_ul("acc", cur_preset.acc);
    break;

  case DIR:
    print_dir(cur_preset.dir);
    break;
  }

}

void print_prog_num(){
  lcd.setCursor(0,0); 
  lcd.print("Program");
  lcd.setCursor(8,0);
  lcd.print((programs.get_cur()+1));
}

void print_ul(String _pref, long _value ){
  lcd.setCursor(0,1); 
  lcd.print(_pref);
  lcd.setCursor(4,1); 
  lcd.print(_value);
}

void print_dir(int _dir ){
  lcd.setCursor(0,1); 
  lcd.print("dir");
  lcd.setCursor(5,1); 
  if(_dir==CW){
    lcd.print("CW");
  }
  else{
    lcd.print("CCW");
  }
}

void print_dir_small(int _dir ){
  lcd.setCursor(10,0); 
  if(_dir==CW){
    lcd.print(">");
  }
  else{
    lcd.print("<");
  }
}

void print_ir_error(char _sym){
  lcd.setCursor(15,0);
  lcd.print(_sym);
  delay(1000);  
}

///////////////////////////////////////

///////// LCD Buttons
int read_LCD_buttons(){               // read the buttons
  int adc_key_in;
  adc_key_in = analogRead(0);       // read the value from the sensor 

  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result

  if (adc_key_in > 1000) return btnNONE; 

  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;  
  if (adc_key_in < 250)  return btnUP; 
  if (adc_key_in < 450)  return btnDOWN; 
  if (adc_key_in < 650)  return btnLEFT; 
  if (adc_key_in < 850)  return btnSELECT;  

  // For V1.0 comment the other threshold and use the one below:
  /*
     if (adc_key_in < 50)   return btnRIGHT;  
   if (adc_key_in < 195)  return btnUP; 
   if (adc_key_in < 380)  return btnDOWN; 
   if (adc_key_in < 555)  return btnLEFT; 
   if (adc_key_in < 790)  return btnSELECT;   
   */

  return btnNONE;                // when all others fail, return this.
}





