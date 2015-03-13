///////// Execute preset

#include "lcdIrController.h"

#include "defines.h"
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include "presetManager.h"

#include "IRReciever.h"
#include "keyboard.h"

#define VER "V. 1.5.1"

namespace PhotoPizza {

#if (BOARD_TYPE == BOARD_TYPE_NANO) //TODO: move lcd to class member???
static LiquidCrystal_I2C lcd(0x27,16,2); // select the pins used on the LCD panel
#elif (BOARD_TYPE == BOARD_TYPE_UNO)
static LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
#endif

///////////  Presets
/* static */ presetManager *lcdIrController::_presetMgr;

void lcdIrController::init(){
#if (BOARD_TYPE == BOARD_TYPE_NANO)
  Wire.begin();
  lcd.init();                      // initialize the lcd
  lcd.backlight();
#endif
  lcd.home();
  _presetMgr = presetManager::get();
  sayHello();
  showProgram();
  IrInit();
}

void lcdIrController::sayHello() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("PhotoPizza DIY"));
  Serial.println(F("PhotoPizza DIY " VER));
  lcd.setCursor(0, 1);
  lcd.print(F(VER));
  delay(2000);
}

void lcdIrController::loop(){

  if (_presetMgr->isEdit())
    editMode();
  else
    menuMode();

  if(_presetMgr->isUpdated())
    showProgram();
}

void lcdIrController::printProgNum() {
  lcd.setCursor(0, 0);
  lcd.print(F("Program"));
  lcd.setCursor(8, 0);
  lcd.print((_presetMgr->getPresetNumber() + 1));
}

void lcdIrController::printDir(int dir) {
  lcd.setCursor(10, 0);
  if (dir == CW) {
    lcd.print(F(">"));
  } else {
    lcd.print(F("<"));
  }
}

void lcdIrController::showProgram() {
  lcd.clear();
  printProgNum();
  printDir(_presetMgr->getValue(DIR));


  IParam *ptr = _presetMgr->getParam();

  if (_presetMgr->isEdit()) {
    lcd.setCursor(0, 1);
    lcd.print(F(">"));
  } else {
    lcd.setCursor(0, 1);
    lcd.print(F(" "));
  }

  lcd.setCursor(1, 1);
  lcd.print(ptr->getName());
  lcd.setCursor(7, 1);
  lcd.print(ptr->ToString());
}

///////////////////////////////////////

void lcdIrController::menuMode() {
  int key = kbGetKey();
  long val;

  switch (key) {
  case kbPwr:
    _presetMgr->stop();
    _presetMgr->run();
    break;
  case kbUp:
    _presetMgr->nextParam();
    break;

  case kbDown:
    _presetMgr->prevParam();
    break;

  case kbRight:
    _presetMgr->nextPreset();
    break;

  case kbLeft:
    _presetMgr->prevPreset();
    break;

  case kbOk:
    _presetMgr->edit();
    break;

  case kbClear:
    _presetMgr->edit();
    _presetMgr->setValue(0);
    break;

  case kbBksp:
    _presetMgr->changeDirection();
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
      _presetMgr->edit();
      _presetMgr->setValue(0);
      val = _presetMgr->getValue() * 10 + key;
      _presetMgr->setValue(val);
    }
    break;
  }

  if(key != kbNoKey && key != kbPwr)
    showProgram();
}

void lcdIrController::editMode() {
  long val;
  int key = kbGetKey();

  switch (key) {
  case kbPwr:
    _presetMgr->stop();
    _presetMgr->run();
    break;
  case kbLeft:
    _presetMgr->discard();
    break;

  case kbOk:
    _presetMgr->save();
    break;

  case kbDown:
    _presetMgr->valueDown();
    break;

  case kbUp:
    _presetMgr->valueUp();
    break;

  case kbClear:
    _presetMgr->setValue(0);
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
        val = _presetMgr->getValue() * 10 + key;
        _presetMgr->setValue(val);
    }
    break;
  }
  if(key != kbNoKey)
    showProgram();
}

}

