#ifndef presets_h
#define presets_h

#include "defines.h"
#include "eepromAnything.h"


struct Preset_st{
  long sp; // speed
  long rot; // rotaion
  long acc; // acceleration
  int  dir;   // -1 - clockwise , 1 - counterclockwise
};

class preset {
public:
  void init();

  void next();
  void prev();
  int get_cur();
  Preset_st get_cur_preset();

  long get_val(byte _pos);
  void change_val(long _val, byte _pos);
  void change_val_u(long _val, byte _pos);
  void change_val_d(long _val, byte _pos);

  void change_direction(int _dir);

private:
  int cur; // current preset
  Preset_st presets[NUM_PROGRAMS];
};

#endif





