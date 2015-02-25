#ifndef presets_h
#define presets_h

#include "defines.h"
#include "eepromAnything.h"

namespace PhotoPizza {

typedef enum {
  SPEED,
  STEPS,
  ACC,
  DIR,
  PARAM_COUNT,
} param;

class preset {
public:
  long _speed; // speed
  long _steps; // rotaion
  long _acc; // acceleration
  int _dir; // -1 - clockwise , 1 - counterclockwise
};

class presetManager {
public:
  void init();

  void next();
  void prev();
  int getCur();
  preset get_cur_preset();

  long getValue(param pos);
  void setValue(param pos, long val);
  void valueUp(param pos);
  void valueDown(param pos);

  void changeDirection(int dir);

  void save();

private:
  int _cur; // current preset
  preset _presets[NUM_PROGRAMS];
};

}

#endif

