/**
 * File: presetManager.h
 * Created on: 26 feb 2015 г.
 * Description:
 * PhotoPizza DIY is an open source project of 360° product photography turntable.
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>
 * Project Author:
 * Project Site: PhotoPizza.org
 *
 * Copyright: 2015 MakerDrive
 * Copying permission statement:
 *  This file is part of PhotoPizza DIY.
 *
 *  PhotoPizza DIY is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PRESET_MANAGER_H
#define PRESET_MANAGER_H

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

struct presetStorageData {
  long _steps;  // rotaion
  long _accel;    // acceleration
  long _pause;
  int   _iter;
  short _speed; // speed
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
    if(_acc != val._accel)
       return true;
    if(_steps * _dir != val._steps)
       return true;
    if(_pause != val._pause)
      return true;
    if(_iter != val._iter)
        return true;

    return false;
  }

  operator presetStorageData() {
    //Serial.println("op = cast to psd");
    presetStorageData tmp;
    tmp._accel = _acc;
    tmp._speed = _speed;
    tmp._steps = _steps * _dir;
    tmp._iter = _iter;
    tmp._pause = _pause;
    return tmp;
  }

  preset& operator=(presetStorageData val) {
//    Serial.println(F("op = psd"));
//    Serial.println((String)F("Sp: ") + val._speed);
//    Serial.println((String)F("acc: ") + val._acc);
//    Serial.println((String)F("steps: ") + val._steps);
    _speed = val._speed;
    _acc = val._accel;
    _iter = val._iter;
    _pause = val._pause;

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

  preset::paramType getParamNumber(){
    return _curParam;
  }

  void firstParam(){
     _curParam = preset::FIRST_PARAM;
  }

  void edit();
  bool isEdit();
  void discard();

  void loop();

  long getValue();
  void setValue(long val);

  void valueUp();
  void valueDown();

  void changeDirection();
  void run();
  void stop();

  void update(){
    _update = true;
    if(!_preset._run.isRunning())
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
  preset _preset;
  preset::paramType _curParam;
  bool _edit;
  bool _update;
};

}

#endif

