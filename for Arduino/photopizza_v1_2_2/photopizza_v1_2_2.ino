#include <EEPROM.h>
#include "presetManager.h"

//#include "display_mgr.h"

/////////////
//need to include libs here, because of compile error in other files.
#include <AccelStepper.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Arduino.h>
#include "presetManager.h"

////////////

using namespace PhotoPizza;
static presetManager *presetMgr;
#include "IRReciever.h"

#include "lib.h"

////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);

  //delay(1000); //for debug
  libInit();
  IrInit();
  presetMgr = presetManager::get();
}

void yield() {
  presetMgr->loop(); //very carefully! do not allow delay call in presetMgr
}

void loop() {
  presetMgr->loop();
  libLoop();
}

