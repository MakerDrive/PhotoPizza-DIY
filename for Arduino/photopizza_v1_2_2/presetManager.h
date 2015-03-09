#ifndef presets_h
#define presets_h

#include "defines.h"
#include "eepromAnything.h"

#include "param.h"
#include "paramAcc.h"
#include "paramSteps.h"
#include "paramSpeed.h"
#include "paramDir.h"

namespace PhotoPizza {

typedef enum {
  SPEED,
  STEPS,
  ACC,
  DIR,
  PARAM_COUNT,

  SAVED_PARAM = PARAM_COUNT,
} paramType;

struct presetStorageData {
  long _speed; // speed
  long _steps; // rotaion
  long _acc; // acceleration
};

struct presetStorage {
  byte flag;
  int  version;

  presetStorageData data[NUM_PROGRAMS];
};

class preset {
public:
  paramSpeed  _speed; // speed
  paramSteps  _steps; // rotaion
  paramAcc    _acc; // acceleration
  paramDir    _dir; // -1 - clockwise , 1 - counterclockwise

  /*preset(){
    //_dir();
  }*/

  IParam& operator[] (const int nIndex){
    switch (nIndex) {
    case SPEED:
      return _speed;
    case STEPS:
      return _steps;
    case ACC:
      return _acc;
    case DIR:
      return _dir;
    default:
      return _default;
    }
  }

  bool operator!=(presetStorageData & val) {
    if(_speed != val._speed)
       return true;
    if(_acc != val._acc)
       return true;
    if(_steps * _dir != val._steps)
       return true;

    return false;
  }

  operator presetStorageData() {
    //Serial.println("op = cast to psd");
    presetStorageData tmp;
    tmp._acc = _acc;
    tmp._speed = _speed;
    tmp._steps = _steps * _dir;
    return tmp;
  }

  preset& operator=(presetStorageData val) {
    /*Serial.println("op = psd");
    Serial.println((String)"Sp: " + val._speed);
    Serial.println((String)"acc: " + val._acc);
    Serial.println((String)"steps: " + val._steps);*/
    _speed = val._speed;
    _acc = val._acc;

    if(val._steps < 0)
      _dir.setByVal(CW);
    else
      _dir.setByVal(CCW);

    _steps = abs(val._steps);
    return *this;
  }

private:
  static LimitedParam _default;
};

class presetManager {
public:
  void init();

  void nextPreset();
  void prevPreset();
  int getPresetNumber();
  preset* get();

  void nextParam();
  void prevParam();

  IParam* getParam();

  paramType getParamNumber(){
    return _curParam;
  }

  void firstParam(){
     _curParam = SPEED;
  }

  void edit();
  bool isEdit();
  void discard();

  long getValue(){
    /*if(_edit)
        return getValue(PARAM_COUNT); //getting default param*/
    return getValue(_curParam);
  }
  long getValue(paramType pos);

  void setValue(paramType pos, long val);
  void setValue(long val){
    setValue(_curParam, val);
  }

  void valueUp(paramType pos);
  void valueUp(){
    valueUp(_curParam);
  }

  void valueDown(paramType pos);
  void valueDown(){
    valueDown(_curParam);
  }

  void changeDirection();

  void save(bool force = false);

private:
  int _curPreset; // current preset
//  presetStorage _presetStorage[NUM_PROGRAMS];
  preset _preset[NUM_PROGRAMS];
  paramType _curParam;
  bool _edit;
};

}

#endif

