#ifndef presets_h
#define presets_h

#include "defines.h"
#include "eepromAnything.h"

#include "param.h"
#include "paramAcc.h"
#include "paramSteps.h"
#include "paramSpeed.h"
#include "paramDir.h"
#include "paramRun.h"
#include "paramIterCount.h"
#include "paramPause.h"

namespace PhotoPizza {

typedef enum {
  FIRST_PARAM = 0,
  RUN = FIRST_PARAM,  // command-parameter
  STEPS,
  SPEED,
  ITER_COUNT,
  PAUSE,
  ACC,
  DIR,
  PARAM_COUNT,

  SAVED_PARAM,       //parameter where we save value before editing
} paramType;

struct presetStorageData {
  long _speed; // speed
  long _steps; // rotaion
  long _acc; // acceleration
};

struct presetStorage {
  byte flag;
  int  version;

  presetStorageData data;
};

class preset {
public:
  paramSpeed     _speed; // speed
  paramSteps     _steps; // rotaion
  paramAcc       _acc; // acceleration
  paramDir       _dir; // -1 - clockwise , 1 - counterclockwise
  paramIterCount _iter;
  paramPause     _pause;

  static paramRun _run;

  IParam& operator[] (const int nIndex){
    switch (nIndex) {
    case RUN:
      return _run;
    case SPEED:
      return _speed;
    case ITER_COUNT:
      return _iter;
    case PAUSE:
        return _pause;
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
    /*Serial.println(F("op = psd"));
    Serial.println((String)F("Sp: ") + val._speed);
    Serial.println((String)F("acc: ") + val._acc);
    Serial.println((String)F("steps: ") + val._steps);*/
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
  static presetManager* get();

  void nextPreset();
  void prevPreset();
  int getPresetNumber();
  preset* getPreset();


  void nextParam();
  void prevParam();

  IParam* getParam();

  paramType getParamNumber(){
    return _curParam;
  }

  void firstParam(){
     _curParam = FIRST_PARAM;
  }

  void edit();
  bool isEdit();
  void discard();

  void loop();

  long getValue(){
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
  void run();
  void stop();

  void update(){
    _update = true;
    if(!_preset[0]._run.isRunning())
      _edit = false;
  }
  bool isUpdated(){
    if(_update){
      _update = false;
      return true;
    }
    return false;
  }

  void save(bool force = false);

private:
  bool loadPreset(unsigned short num, bool set = false);
  bool savePreset(unsigned short num);
  static presetManager *_presetMgr;
  presetManager();
  int _curPreset; // current preset
  preset _preset[NUM_PROGRAMS];
  paramType _curParam;
  bool _edit;
  bool _update;
};

}

#endif

