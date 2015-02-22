#include <EEPROM.h>
#include "presets.h"

#include "display_mgr.h"

#include <AccelStepper.h>
#include <LiquidCrystal.h>

///////////  LED
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
////////////////////////////////////////////////////////////////////////////////////

int key;                    // Button code
////////////////////////////////////////////////////////////////////////////////////


///////////  Presets
preset programs;
Preset_st cur_preset;

byte cur_mode;
byte menu_param_pos;
byte e_flag;

volatile boolean exec_flag;
volatile boolean start_interrupt;

boolean lcd_flag;
////////////////////////////////////////////////////////////////////////////////////

////////    Stepper

AccelStepper stepper(AccelStepper::DRIVER,12,13); 
////////////////////////////////////////////////////////////////////////////////////

void setup(){
  lcd.begin(16, 2);

  getLCD();

  key = 0; 
  cur_mode = MENU_MODE;
  e_flag=0;

  programs.init();
  cur_preset = programs.get_cur_preset();

  exec_flag = true;
  lcd_flag = false;
  start_interrupt = false;

  attachInterrupt(0, Ir_interrupt, RISING);
  interrupts();

  // Show Logo
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PhotoPizza DIY");
  lcd.setCursor(0,1);
  lcd.print("V. 1.2.2");  
  delay(2000);
  ////////////////////  

  // show preset 0 
  menu_param_pos = 0;
  show_curr_program(false);

}

void loop(){
  start_interrupt = false;
  key = get_key();
  action();
  delay(130);
}

void action(){
  switch(cur_mode){
  case MENU_MODE:
    menu_mode(); 
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }
}

void Ir_interrupt(){
  if(start_interrupt){
    exec_flag = false;
    start_interrupt = false;
//    cur_mode = MENU_MODE;
//    key=0;
  }
}








