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
#include "lcdIrController.h"
////////////

using namespace PhotoPizza;
static presetManager *presetMgr = NULL;
static lcdIrController controller;
#include "IRReciever.h"

////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  controller.init();
  presetMgr = presetManager::get();
}

void yield() {
  if(presetMgr)
    presetMgr->loop(); //very carefully! do not allow delay call in presetMgr
}

void loop() {
  presetMgr->loop();
  controller.loop();
}

