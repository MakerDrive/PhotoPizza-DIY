///////// Execute preset

#include "defines.h"
#include "lib.h"

#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>
#include "presetManager.h"

#include "keyboard.h"

#define VER "V. 1.4.0"

using namespace PhotoPizza;
#if (BOARD_TYPE == BOARD_TYPE_NANO)
LiquidCrystal_I2C lcd(0x27,16,2); // select the pins used on the LCD panel
#elif (BOARD_TYPE == BOARD_TYPE_UNO)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
#endif

///////////  Presets
presetManager *presetMgr;

static void show_curr_program();

static void edit_preset_mode();
static void menu_mode();

static void sayHello();

void libInit(){
#if (BOARD_TYPE == BOARD_TYPE_NANO)
  Wire.begin();
  lcd.init();                      // initialize the lcd
  lcd.backlight();
#endif
  lcd.home();
  presetMgr = presetManager::get();
  sayHello();
  show_curr_program();
}

void sayHello() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("PhotoPizza DIY"));
  Serial.println(F("PhotoPizza DIY " VER));
  lcd.setCursor(0, 1);
  lcd.print(F(VER));
  delay(2000);
}

void libLoop(){
  presetMgr->loop();

  if (presetMgr->isEdit())
    edit_preset_mode();
  else
    menu_mode();
}

///////////////////////////////////////

///////// print info

void print_prog_num() {
  lcd.setCursor(0, 0);
  lcd.print(F("Program"));
  lcd.setCursor(8, 0);
  lcd.print((presetMgr->getPresetNumber() + 1));
}

void print_dir_small(int _dir) {
  lcd.setCursor(10, 0);
  if (_dir == CW) {
    lcd.print(F(">"));
  } else {
    lcd.print(F("<"));
  }
}

static void show_curr_program() {
  lcd.clear();
  print_prog_num();
  print_dir_small(presetMgr->getValue(DIR));


  IParam *ptr = presetMgr->getParam();

  if (presetMgr->isEdit()) {
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

void edit_preset_mode() {
  long val;
  int key = kbGetKey();

  switch (key) {
  case kbLeft: //exit without writing to mem
    presetMgr->discard();
    break;

  case kbOk: // write to mem and exit
    presetMgr->save(); //TODO: fix that!
    break;

  case kbDown:
    presetMgr->valueDown();
    break;

  case kbUp:
    presetMgr->valueUp();
    break;

  case kbClear:
    presetMgr->setValue(0);
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
        val = presetMgr->getValue() * 10 + key;
        presetMgr->setValue(val);
    }
    break;
  }
  if(key != kbNoKey)
    show_curr_program();
}

void menu_mode() {
  int key = kbGetKey();
  long val;

  switch (key) {
  case kbPwr:
    //presetMgr->run();
    break;
  case kbUp:
    presetMgr->nextParam();
    break;

  case kbDown:
    presetMgr->prevParam();
    break;

  case kbRight:
    presetMgr->nextPreset();
    break;

  case kbLeft:
    presetMgr->prevPreset();
    break;

  case kbOk:
    presetMgr->edit();
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
      presetMgr->edit();
      presetMgr->setValue(0);
      val = presetMgr->getValue() * 10 + key;
      presetMgr->setValue(val);
    }
    break;
  }

  if(key != kbNoKey && key != kbPwr)
    show_curr_program();

}

void libUpdateLCD(){
  //show_curr_program();
}

