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

  void nextPreset();
  void prevPreset();
  int getPresetNumber();
  preset getPreset();

  void nextParam();
  void prevParam();

  param getParamNumber(){
    return _curParam;
  }

  void resetParamNumber(){
     _curParam = SPEED;
  }

  long getValue(){
    return getValue(_curParam);
  }
  long getValue(param pos);

  void setValue(param pos, long val);
  void setValue(long val){
    setValue(_curParam, val);
  }

  void valueUp(param pos);
  void valueUp(){
    valueUp(_curParam);
  }

  void valueDown(param pos);
  void valueDown(){
    valueDown(_curParam);
  }

  void changeDirection(int dir);

  void save();

private:
  int _curPreset; // current preset
  preset _presets[NUM_PROGRAMS];
  param _curParam;
};

}

#endif

