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

/*template<class T>*/ class param {

public:
  virtual void up(){
    ++_val;
  }
  virtual void down(){
    --_val;
  }

  virtual String ToString(bool shorten = false){
    return (String) _val;
  }
  virtual String getName(bool shorten = false){
    return "param";
  }

  virtual bool set(long val){
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
  //static const long _valAlter = 6;
  long _val;
};

class paramSpeed : public param {
public:
  paramSpeed() : paramSpeed(1000){}
  paramSpeed(long val){
    _val = val;
  }

  virtual void up(){
    set(get() - get() % SPEED_STEP + SPEED_STEP);
  }
  virtual void down(){
    set(get() - get() % SPEED_STEP - SPEED_STEP);
  }

  virtual String getName(bool shorten = false){
    return "sp";
  }

  virtual bool set(long val){
    if (val < SPEED_MIN)
      val = SPEED_MIN;
    if (val > SPEED_MAX)
      val = SPEED_MAX;
    _val = val;
    return true;
  }
};

class paramSteps : public param {
public:
  paramSteps() : paramSteps(3200){}
  paramSteps(long val){
    _val = val;
  }

  virtual void up(){
    set(get() - get() % ROT_STEP + ROT_STEP);
  }
  virtual void down(){
    set(get() - get() % ROT_STEP - ROT_STEP);
  }

  virtual String getName(bool shorten = false){
    return "rot";
  }

  virtual bool set(long val){
    if (val < ROT_MIN)
      val = ROT_MIN;
    if (val > ROT_MAX)
      val = ROT_MAX;
    _val = val;
    return true;
  }
};

class paramAcc : public param {
public:
  paramAcc() : paramAcc(5000){}
  paramAcc(long val){
    _val = val;
  }

  virtual void up(){
    set(get() - get() % ACC_STEP + ACC_STEP);
  }
  virtual void down(){
    set(get() - get() % ACC_STEP - ACC_STEP);
  }

  virtual String getName(bool shorten = false){
    return "acc";
  }

  virtual bool set(long val){
    if (val < ACC_MIN)
      val = ACC_MIN;
    if (val > ACC_MAX)
      val = ACC_MAX;
    _val = val;
    return true;
  }
};

class paramDir : public param {
public:
  paramDir() : paramDir(CW){}
  paramDir(long val){
    _val = val;
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

  virtual bool set(long val){
    if (val < CW)
      val = CW;
    if (val > CCW)
      val = CCW;
    _val = val;
    return true;
  }
};

struct preset {
//public:
  paramSpeed  _speed; // speed
  paramSteps  _steps; // rotaion
  paramAcc    _acc; // acceleration
  paramDir    _dir; // -1 - clockwise , 1 - counterclockwise
};

class presetManager {
public:
  void init();

  void nextPreset();
  void prevPreset();
  int getPresetNumber();
  //presetStorage getPreset();

  void nextParam();
  void prevParam();

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
  presetStorage _presetStorage[NUM_PROGRAMS];
  preset _preset[NUM_PROGRAMS];
  paramType _curParam;
};

}

#endif

