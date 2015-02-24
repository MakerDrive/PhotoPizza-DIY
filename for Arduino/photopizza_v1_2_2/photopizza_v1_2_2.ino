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
#include <float.h>

////////////////////////////////////////////////////////////////////////////////////

///////////  Presets
preset programs;
Preset_st cur_preset;

extern AccelStepper stepper;
extern LiquidCrystal lcd;

bool bRun = false;

////////////////////////////////////////////////////////////////////////////////////

void setup() {

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

void yield() {
  //TDOD: YIELD hook
}

void loop() {
  int key = Ir_getKey();
  if( key == BTN_POWER && !bRun){
    bRun = true;
    Serial.println("Run");
    Serial.println((String)"Accel" + cur_preset.acc);
    stepper.setCurrentPosition(0L);
    //stepper.setAcceleration(10000000); //no acc.//cur_preset.acc);
    stepper.setAcceleration(1000);

    stepper.setMaxSpeed(1000);///cur_preset.sp);
    stepper.moveTo(3200);
  }else if( key == BTN_POWER && bRun){
    Serial.println("stopping");
    stepper.stop();
  }

  if(!stepper.run() && bRun){
    Serial.println("stop");
    bRun = false;
  }

  /*switch (cur_mode) {
  case MENU_MODE:
    menu_mode();
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }*/
}

