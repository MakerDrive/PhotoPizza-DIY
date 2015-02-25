#ifndef presets_h
#define presets_h

#include "defines.h"
#include "eepromAnything.h"


class preset{
public:
  long _speed;  // speed
  long _steps;  // rotaion
  long _acc;    // acceleration
  int  _dir;    // -1 - clockwise , 1 - counterclockwise
};

class presetManager {
public:
  void init();

  void next();
  void prev();
  int getCur();
  preset get_cur_preset();

  long getValue(byte _pos);
  void setValue(long _val, byte _pos);
  void valueUp(byte _pos);
  void valueDown(byte _pos);

  void change_direction(int _dir);

  void save();

private:
  int _cur; // current preset
  preset _presets[NUM_PROGRAMS];
};

#endif





