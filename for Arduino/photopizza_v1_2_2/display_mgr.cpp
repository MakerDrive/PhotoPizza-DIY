/*
 * display_mgr.cpp
 *
 *  Created on: 22 февр. 2015 г.
 *      Author: romeo
 */

#include "display_mgr.h"

static LiquidCrystal lcd2(8, 9, 4, 5, 6, 7);

LiquidCrystal *getLCD(){
  return &lcd2;
}

//LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel


