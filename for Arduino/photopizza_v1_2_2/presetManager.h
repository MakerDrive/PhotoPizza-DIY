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
} paramType;

struct presetStorage {
//public:
  long _speed; // speed
  long _steps; // rotaion
  long _acc; // acceleration
  int _dir; // -1 - clockwise , 1 - counterclockwise
};

class param {

public:

  param(){
    _valStep = 1;
    _val = 0;
    _valHiLimit = 100;
    _valLoLimit = 0;
  }

  virtual void up(){
    set(get() - get() % _valStep + _valStep);
  }
  virtual void down(){
    set(get() - get() % _valStep - _valStep);
  }

  virtual String ToString(bool shorten = false){
    return (String) _val;
  }
  virtual String getName(bool shorten = false){
    return "param";
  }

  virtual bool set(long val){
    if (val < _valLoLimit)
      val = _valLoLimit;
    if (val > _valHiLimit)
      val = _valHiLimit;
    _val = val;
    return true;
  }
  virtual long get() {
    return _val;
  }

  virtual long operator()() { //alias to get()
    return get(); //TODO: decrease stack usage???
  }

  virtual long operator()(long val) { //alias to set()
    set(val); //TODO: decrease stack usage???
  }

  virtual operator long() {
    return get();
  }

  virtual param& operator=( long val ) {
    set(val);
    return *this;
  }

  virtual bool isEnum(){
    return false;
  }

  virtual ~param(){};

protected:
  long _valHiLimit;
  long _valLoLimit;
  long _valStep;
  long _val;
};

class paramSpeed : public param {
public:
  paramSpeed() : paramSpeed(1000){}
  paramSpeed(long val){
    _val = val;
    _valStep = SPEED_STEP;
    _valLoLimit = SPEED_MIN;
    _valHiLimit = SPEED_MAX;
  }

  virtual String getName(bool shorten = false){
    return "speed";
  }
};

class paramSteps : public param {
public:
  paramSteps() : paramSteps(3200){}
  paramSteps(long val){
    _val = val;
    _valStep = ROT_STEP;
    _valLoLimit = ROT_MIN;
    _valHiLimit = ROT_MAX;
  }

  virtual String getName(bool shorten = false){
    return "steps";
  }

  virtual String ToString(bool shorten = false){
    if(_val == 0)
      return "inf";
    else
      return (String) _val;
  }
};

class paramAcc : public param {
public:
  paramAcc() : paramAcc(5000){}
  paramAcc(long val){
    _val = val;
    _valStep = ACC_STEP;
    _valLoLimit = ACC_MIN;
    _valHiLimit = ACC_MAX;
  }

  virtual String getName(bool shorten = false){
    return "accel";
  }

  virtual String ToString(bool shorten = false){
    if(_val > 0)
      return (String)_val;
    else if(_val == 0)
      return "inf";
    else
      return "?";
  }
};

class paramDir : public param {
public:
  paramDir() : paramDir(CW){}
  paramDir(long val){
    _val = val;
    _valLoLimit = CW;
    _valHiLimit = CCW;
  }

  virtual void up(){
    set(-get());
  }
  virtual void down(){
    set(-get());
  }

  virtual String getName(bool shorten = false){
    return "dir";
  }

  virtual String ToString(bool shorten = false){
    switch(_val){
      case CW: return "CW";
      case CCW: return "CCW";
      default : return "?";
    }
  }

  virtual bool set(long val){
    if (val != CW || val != CCW)
      val = CW;
    _val = val;
    return true;
  }
};

class preset {
public:
  paramSpeed  _speed; // speed
  paramSteps  _steps; // rotaion
  paramAcc    _acc; // acceleration
  paramDir    _dir; // -1 - clockwise , 1 - counterclockwise

  param& operator[] (const int nIndex){
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

private:
  static param _default;
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

  param* getParam();

  paramType getParamNumber(){
    return _curParam;
  }

  void firstParam(){
     _curParam = SPEED;
  }

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

  void changeDirection(int dir);

  void save();

private:
  int _curPreset; // current preset
//  presetStorage _presetStorage[NUM_PROGRAMS];
  preset _preset[NUM_PROGRAMS];
  paramType _curParam;
};

}

#endif

