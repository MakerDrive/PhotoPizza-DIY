///////// Execute preset

#include "defines.h"
#include "lib.h"

#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include "presetManager.h"

#include "keyboard.h"

#include <limits.h>

#define VER "V. 1.3.0"

using namespace PhotoPizza;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
AccelStepper stepper(AccelStepper::DRIVER, 12, 13);

///////////  Presets
presetManager presets;

static bool bRun = false;

static void prvExecutePreset();

int cur_mode = MENU_MODE;

static void show_curr_program();

static void edit_preset_mode();
static void menu_mode();

static void sayHello();

void libInit(){
  presets.init();
  sayHello();
  show_curr_program();
}

void sayHello() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PhotoPizza DIY");
  Serial.println("PhotoPizza DIY " VER);
  lcd.setCursor(0, 1);
  lcd.print(VER);
  delay(2000);
}

static void prvExecutePreset() {
  if(bRun){
    Serial.println("stopping");
    stepper.stop();
    lcd.setCursor(0, 1);
    lcd.print("Program stopping ");
    return;
  }

  bRun = true;
  lcd.setCursor(0, 1);
  lcd.print("Program started ");

  Serial.println("Run");
  long steps = presets.get()->_steps * presets.get()->_dir; //TODO: refactor getters (via local vars)
  Serial.println((String)"Accel" + presets.get()->_acc);
  Serial.println((String)"Steps" + steps);
  Serial.println((String)"Speed" + presets.get()->_speed);
  stepper.setCurrentPosition(0L);
  if(presets.get()->_acc == 0){
    stepper.setAcceleration(10000000); //no acc.
  }else
    stepper.setAcceleration(presets.get()->_acc);
  if(steps != 0){
    stepper.moveTo(steps);
  }else
    stepper.moveTo(LONG_MAX * presets.get()->_dir);
  stepper.setMaxSpeed(presets.get()->_speed);
}

void finishPreset(){
  lcd.setCursor(0, 1);
  lcd.print("Program finished");
  Serial.println("Finished");
  delay(1000);
  show_curr_program();
}

void libLoop(){
  if(!stepper.run() && bRun){
    bRun = false;
    finishPreset();
  }

  switch (cur_mode) {
  case MENU_MODE:
    menu_mode();
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }
}

///////////////////////////////////////

///////// print info

void print_prog_num() {
  lcd.setCursor(0, 0);
  lcd.print("Program");
  lcd.setCursor(8, 0);
  lcd.print((presets.getPresetNumber() + 1));
}

void print_dir_small(int _dir) {
  lcd.setCursor(10, 0);
  if (_dir == CW) {
    lcd.print(">");
  } else {
    lcd.print("<");
  }
}

static void show_curr_program() {
  lcd.clear();
  print_prog_num();
  print_dir_small(presets.getValue(DIR));

  if (cur_mode == EDIT_MODE) {
    lcd.setCursor(0, 1);
    lcd.print(">");
  } else {
    lcd.setCursor(0, 1);
    lcd.print(" ");
  }

  param *ptr = presets.getParam();

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
  case kbPwr: //exit without writing to mem
    presets.firstParam();
    cur_mode = MENU_MODE;
    break;

  case kbOk: // write to mem and exit
    presets.save();
    cur_mode = MENU_MODE;
    break;

  case kbDown:
    presets.valueDown();
    break;

  case kbUp:
    presets.valueUp();
    break;

  case kbClear:
    presets.setValue(0);
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
        val = presets.getValue() * 10 + key;
        presets.setValue(val);
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
    prvExecutePreset();
    break;
  case kbUp:
    presets.nextParam();
    break;

  case kbDown:
    presets.prevParam();
    break;

  case kbRight:
    presets.nextPreset();
    break;

  case kbLeft:
    presets.prevPreset();
    break;

  case kbOk:
    cur_mode = EDIT_MODE;
    break;

  default:
    key = kbGetNumericKey(key);
    if(key >= 0){
      cur_mode = EDIT_MODE;
      presets.setValue(0);
      val = presets.getValue() * 10 + key;
      presets.setValue(val);
    }
    break;
  }

  if(key != kbNoKey && key != kbPwr)
    show_curr_program();

}

