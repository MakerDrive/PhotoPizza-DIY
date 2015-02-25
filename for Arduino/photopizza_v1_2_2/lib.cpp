///////// Execute preset

#include "defines.h"
#include "lib.h"

#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include "presetManager.h"

#include "IRReciever.h"

#define VER "V. 1.2.2"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // select the pins used on the LCD panel
AccelStepper stepper(AccelStepper::DRIVER, 12, 13);

///////////  Presets
presetManager presets;
preset cur_preset;

static bool bRun = false;

static void prvExecutePreset();

byte menu_param_pos;

boolean lcd_flag;

int key = 0; // Button code
byte e_flag = 0;

void libInit(){
  presets.init();
  sayHello();
  cur_preset = presets.get_cur_preset();
  show_curr_program(false);
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

void prvExecutePreset() {
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
  int steps = cur_preset._steps * cur_preset._dir;
  Serial.println((String)"Accel" + cur_preset._acc);
  Serial.println((String)"Steps" + steps);
  Serial.println((String)"Speed" + cur_preset._speed);
  stepper.setCurrentPosition(0L);
  if(cur_preset._acc == 0){
    stepper.setAcceleration(10000000); //no acc.
  }else
    stepper.setAcceleration(cur_preset._acc);
  stepper.moveTo(steps);
  stepper.setMaxSpeed(cur_preset._speed);
}

void finishPreset(){
  lcd.setCursor(0, 1);
  lcd.print("Program finished");
  Serial.println("Finished");
  delay(1000);
  menu_param_pos = 0;
  show_curr_program(false);
}

void libLoop(){
  int key = IrGetKey();
  if( key == BTN_POWER){
    prvExecutePreset();
  }

  if(!stepper.run() && bRun){
    bRun = false;
    finishPreset();
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

///////////////////////////////////////

///////// edit preset

void value_u() {
  presets.valueUp(menu_param_pos);
  cur_preset = presets.get_cur_preset();
}

void value_d() {
  presets.valueDown(SPEEED);
  cur_preset = presets.get_cur_preset();
}

///////////////////////////////////////

///////// print info

void show_curr_program(bool _is_edit) {
  lcd.clear();
  print_prog_num();
  print_dir_small(cur_preset._dir);

  if (_is_edit) {
    lcd.setCursor(14, 0);
    lcd.print("E");

    cur_preset = presets.get_cur_preset();
  } else {
    lcd.setCursor(14, 0);
    lcd.print("M");
  }

  switch (menu_param_pos) {
  case SPEEED:
    print_ul("sp", cur_preset._speed);
    break;

  case ROT:
    if (cur_preset._steps == 0) {
      lcd.setCursor(0, 1);
      lcd.print("rot");
      lcd.setCursor(4, 1);
      lcd.print("inf");
    } else {
      print_ul("rot", cur_preset._steps);
    }
    break;

  case ACC:
    print_ul("acc", cur_preset._acc);
    break;

  case DIR:
    print_dir(cur_preset._dir);
    break;
  }

}

void print_prog_num() {
  lcd.setCursor(0, 0);
  lcd.print("Program");
  lcd.setCursor(8, 0);
  lcd.print((presets.getCur() + 1));
}

void print_ul(String _pref, long _value) {
  lcd.setCursor(0, 1);
  lcd.print(_pref);
  lcd.setCursor(4, 1);
  lcd.print(_value);
}

void print_dir(int _dir) {
  lcd.setCursor(0, 1);
  lcd.print("dir");
  lcd.setCursor(5, 1);
  if (_dir == CW) {
    lcd.print("CW");
  } else {
    lcd.print("CCW");
  }
}

void print_dir_small(int _dir) {
  lcd.setCursor(10, 0);
  if (_dir == CW) {
    lcd.print(">");
  } else {
    lcd.print("<");
  }
}
///////////////////////////////////////

///////// LCD Buttons
int read_LCD_buttons() { // read the buttons
  int adc_key_in;
  adc_key_in = analogRead(0); // read the value from the sensor

  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  // We make this the 1st option for speed reasons since it will be the most likely result

  if (adc_key_in > 1000)
    return btnNONE;

  // For V1.1 us this threshold
  if (adc_key_in < 50)
    return btnRIGHT;
  if (adc_key_in < 250)
    return btnUP;
  if (adc_key_in < 450)
    return btnDOWN;
  if (adc_key_in < 650)
    return btnLEFT;
  if (adc_key_in < 850)
    return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
   if (adc_key_in < 50)   return btnRIGHT;
   if (adc_key_in < 195)  return btnUP; 
   if (adc_key_in < 380)  return btnDOWN; 
   if (adc_key_in < 555)  return btnLEFT; 
   if (adc_key_in < 790)  return btnSELECT;   
   */

  return btnNONE; // when all others fail, return this.
}

void edit_preset_mode() {
  long _val;
  key = IrGetKey();
  switch (key) {
  case 0:
//    menu_param_pos = 0;
    e_flag = 0;
    show_curr_program(true);
    break;

  case BTN_POWER: //exit without writing to mem
    menu_param_pos = 0;
    key = 0;
    e_flag = 0;
    show_curr_program(false);
    break;

  case BTN_FUNC: // write to mem and exit
//    menu_param_pos = 0;
    key = 0;
    e_flag = 0;
    presets.save();
    show_curr_program(false);
    break;

  case BTN_CH_U:
  case btnUP:
    e_flag = 0;
    menu_param_pos = (menu_param_pos + 1) % 4;
    show_curr_program(true);
    break;

  case BTN_CH_D:
  case btnDOWN:
    e_flag = 0;
    menu_param_pos = (menu_param_pos + 3) % 4;
    show_curr_program(true);
    break;

  case BTN_VOL_D:
  case btnLEFT:
    e_flag = 0;
    value_d();
    show_curr_program(true);
    break;

  case BTN_VOL_U:
  case btnRIGHT:
    e_flag = 0;
    value_u();
    show_curr_program(true);
    break;

  case BTN_0:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_1:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 1;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_2:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 2;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_3:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 3;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_4:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 4;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_5:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 5;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_6:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 6;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_7:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 7;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_8:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 8;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_9:
    if (e_flag != 0) {
      _val = presets.getValue(menu_param_pos) * 10 + 9;
      presets.setValue(_val, menu_param_pos);
      show_curr_program(true);
    }
    break;

  case BTN_EQ:
    if (e_flag == 0) {
      e_flag = 1;
      _val = 0;
      presets.setValue(_val, menu_param_pos);
    } else {
      e_flag = 0;
    }
    show_curr_program(true);
    break;

  case BTN_ST:
    presets.setValue(0, menu_param_pos);
    show_curr_program(true);
    break;

  case BTN_RW:
    _val = presets.getValue(menu_param_pos) / 10;
    presets.setValue(_val, menu_param_pos);
    show_curr_program(true);
    break;

  case BTN_FW:
    _val = presets.getValue(menu_param_pos) * 10;
    presets.setValue(_val, menu_param_pos);
    show_curr_program(true);
    break;

  default:
    Serial.println("Unknown key");
    break;
  }

}

void menu_mode() {
  key = IrGetKey();
  switch (key) {
  case 0: // first call, no key pressed yet
    menu_param_pos = 0;
    show_curr_program(false);
    break;
  case BTN_VOL_U:
    menu_param_pos = (menu_param_pos + 1) % 4;
    show_curr_program(false);
    break;

  case BTN_VOL_D:
    menu_param_pos = (menu_param_pos + 3) % 4;
    show_curr_program(false);
    break;

  case BTN_CH_U:
  case btnRIGHT:
    presets.next();
    cur_preset = presets.get_cur_preset();
    menu_param_pos = 0;
    show_curr_program(false);
    break;

  case BTN_CH_D:
  case btnLEFT:
    presets.prev();
    cur_preset = presets.get_cur_preset();
    menu_param_pos = 0;
    show_curr_program(false);
    break;

  case BTN_FW:
    presets.change_direction(CW);
    cur_preset = presets.get_cur_preset();
    show_curr_program(false);
    break;

  case BTN_RW:
    presets.change_direction(CCW);
    cur_preset = presets.get_cur_preset();
    show_curr_program(false);
    break;

  case BTN_FUNC:
    key = 0;
    edit_preset_mode();
    break;

  case BTN_PLAY:
  case btnUP:
    prvExecutePreset();
    break;

  default:
    Serial.println("Unknown key");
    break;
  }

}

