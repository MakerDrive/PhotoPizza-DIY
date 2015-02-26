#include <EEPROM.h>
#include "presetManager.h"

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

void setup() {
  Serial.begin(115200);

  //delay(1000); //for debug
  libInit();
  IrInit();
}

void yield() {
  //TDOD: YIELD hook
}

void loop() {
  libLoop();
}

