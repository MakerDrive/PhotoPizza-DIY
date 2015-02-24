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

////////////////////////////////////////////////////////////////////////////////////

///////////  Presets
preset programs;
Preset_st cur_preset;

extern byte cur_mode;

////////////////////////////////////////////////////////////////////////////////////

void setup(){

  Serial.begin(115200);

  programs.init();
  cur_preset = programs.get_cur_preset();

  // Show Logo
  Ir_init();
  say_hello();

  delay(2000);
  ////////////////////  

  show_curr_program(false);

}

void yield(){
  //TDOD: YIELD hook
}

void loop(){
  switch(cur_mode){
  case MENU_MODE:
    menu_mode();
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }
}








