#include <EEPROM.h>
#include "presets.h"

///////////  LED
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel
////////////////////////////////////////////////////////////////////////////////////

///////////  IR 
#define IR_BIT_LENGTH 32    // number of bits sent by IR remote
#define FirstLastBit 15     // divide 32 bits into two 15 bit chunks for integer variables. Ignore center two bits. they are all the same.
#define BIT_1 1500          // Binary 1 threshold (Microseconds)
#define BIT_0 450           // Binary 0 threshold (Microseconds)
#define BIT_START 4000      // Start bit threshold (Microseconds)

#define IR_PIN 2            // IR Sensor pin
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

////////    Stepper
#include <AccelStepper.h>
AccelStepper stepper(AccelStepper::DRIVER,12,13); 
////////////////////////////////////////////////////////////////////////////////////

void setup(){
  lcd.begin(16, 2);              

  key = 0; 
  cur_mode = MENU_MODE;
  e_flag=0;

  programs.init();
  cur_preset = programs.get_cur_preset();

  exec_flag = true;
  lcd_flag = false;
  start_interrupt = false;

  attachInterrupt(0, Ir_interrupt, RISING);
  interrupts();

  // Show Logo
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PhotoPizza DIY");
  lcd.setCursor(0,1);
  lcd.print("V. 1.2.2");  
  delay(2000);
  ////////////////////  

  // show preset 0 
  menu_param_pos = 0;
  show_curr_program(false);

}

void loop(){
  start_interrupt = false;
  key = get_key();
  action();
  delay(130);
}

void action(){
  switch(cur_mode){
  case MENU_MODE:
    menu_mode(); 
    break;
  case EDIT_MODE:
    edit_preset_mode();
    break;
  }
}

void Ir_interrupt(){
  if(start_interrupt){
    exec_flag = false;
    start_interrupt = false;
//    cur_mode = MENU_MODE;
//    key=0;
  }
}








