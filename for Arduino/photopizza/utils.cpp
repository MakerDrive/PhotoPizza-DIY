/*
 * utils.cpp
 *
 *  Created on: 10 марта 2015 г.
 *      Author: romeo
 */

#include <Arduino.h>

#include "utils.h"

int freeRam (){
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


