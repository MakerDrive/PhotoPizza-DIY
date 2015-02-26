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
    _tmpVal = 0;
    _valHiLimit = 100;
    _valLoLimit = 0;
    _edit = false;
  }

  virtual void up(){
    long tmp = get();
       tmp = tmp - tmp % _valStep + _valStep;
    if(tmp > _valHiLimit)
      tmp = _valLoLimit;
    set(tmp);
  }
  virtual void down(){
    long tmp = get();
      tmp = tmp - tmp % _valStep - _valStep;
    if(tmp < _valLoLimit)
      tmp = _valHiLimit;
    set(tmp);
  }

  virtual void edit(){
    _tmpVal = _val;
    _edit = true;
  }

  virtual bool save(){
    _edit = false;
    return set(_tmpVal);
  }

  virtual void discard(){
    _edit = false;
  }

  virtual String ToString(bool shorten = false){
    if(_edit)
      return (String) _tmpVal;
    else
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
    if(_edit)
      _tmpVal = val;
    else
      _val = val;
    return true;
  }
  virtual long get() {
    if(_edit)
      return _tmpVal;
    else
      return _val;
  }

  virtual operator long() {
    return get();
  }

  virtual param& operator=( long val ) {
    set(val);
    return *this;
  }

  virtual bool isEdit(){
    return _edit;
  }

  virtual ~param(){};

protected:
  long _valHiLimit;
  long _valLoLimit;
  long _valStep;
  long _val;
  bool _edit;
  long _tmpVal;
};

class paramSpeed : public param {
public:
  paramSpeed() : paramSpeed(1000){}
  paramSpeed(long val){
    _valStep = SPEED_STEP;
    _valLoLimit = SPEED_MIN;
    _valHiLimit = SPEED_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return "speed";
  }
};

class paramSteps : public param {
public:
  paramSteps() : paramSteps(3200){}
  paramSteps(long val){
    _valStep = ROT_STEP;
    _valLoLimit = ROT_MIN;
    _valHiLimit = ROT_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return "steps";
  }

  virtual String ToString(bool shorten = false){
    long val = get();
    if(val == 0)
      return "inf";
    else
      return (String) val;
  }
};

class paramAcc : public param {
public:
  paramAcc() : paramAcc(5000){}
  paramAcc(long val){
    _valStep = ACC_STEP;
    _valLoLimit = ACC_MIN;
    _valHiLimit = ACC_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return "accel";
  }

  virtual String ToString(bool shorten = false){
    long val = get();
    if(val > 0)
      return (String)val;
    else if(val == 0)
      return "inf";
    else
      return "?";
  }
};

#define PARAM_DIR_VAL_COUNT 2

typedef struct enumParamMap{
  long   value;
  char*  label;
};

class paramDir : public param {
public:
  paramDir() : paramDir(CW){}
  paramDir(long val): _map{
    {CW, "CW"}, {CCW, "CCW"}
  }{
    _valLoLimit = 0;
    _valHiLimit = PARAM_DIR_VAL_COUNT - 1;
    _valStep = 1;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return "dir";
  }

  virtual long get(){
    if(_edit)
      return _map[_tmpVal].value;
    else
      return _map[_val].value;
  }

  virtual String ToString(bool shorten = false){
    if(_edit)
       return _map[_tmpVal].label;
    else
      return _map[_val].label;
  }

protected:
  enumParamMap _map[PARAM_DIR_VAL_COUNT];
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

