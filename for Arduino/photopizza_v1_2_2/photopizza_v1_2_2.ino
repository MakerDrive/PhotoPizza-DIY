#include <EEPROM.h>
#include "presets.h"

//#include "display_mgr.h"

/////////////
//need to include libs here, because of compile error in other files.
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <Arduino.h>

////////////

#include "IRReciever.h"

#include "lib.h"

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

void setup(){

  key = 0; 
  cur_mode = MENU_MODE;
  e_flag=0;


  Serial.begin(115200);

  programs.init();
  cur_preset = programs.get_cur_preset();

  exec_flag = true;
  lcd_flag = false;
  start_interrupt = false;

  // Show Logo
  Ir_init();
  say_hello();
  Serial.println("Hello");
  delay(1000);
  ////////////////////  

  // show preset 0 
  menu_param_pos = 0;
  show_curr_program(false);

}

void yield(){
  //TDOD: YIELD hook
}

void loop(){
  //Serial.println((String) "l: " + millis());
  /*start_interrupt = false;
  key = get_key();
  switch(cur_mode){
  case MENU_MODE:
    menu_mode();
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }*/
  Ir_getKey();
  //Serial.println(get_key());
  //delay(1000);
}








