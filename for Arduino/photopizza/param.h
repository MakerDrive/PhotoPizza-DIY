/**
 * File: param.h
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

#ifndef PHOTOPIZZA_V1_2_2_PARAM_H_
#define PHOTOPIZZA_V1_2_2_PARAM_H_

#include <WString.h>
#include <Arduino.h>

namespace PhotoPizza {

class IParam {

public:

  virtual void up() = 0;
  virtual void down() = 0;
  virtual void edit() = 0;
  virtual bool save() = 0;
  virtual void discard() = 0;
  virtual String toString(bool shorten = false) = 0;
  //virtual String toString(long val) = 0;
  virtual String getName(bool shorten = false) = 0;
  virtual bool set(long val) = 0;
  virtual long get() = 0;
  virtual operator long() = 0;
  virtual IParam& operator=(long val) = 0;
  //virtual IParam& operator=(IParam& other) {};

  virtual bool isEdit() = 0;

  virtual ~IParam() {};

};

class LimitedParam: public IParam {

public:

  LimitedParam() {
    _valStep = 1;
    _val = 0;
    _valHiLimit = 100;
    _valLoLimit = 0;
    _editVal = 0;
    _edit = false;
  }

  virtual void up() {
    long tmp = _val; //TODO: fix! works incorrect with enumed settings
    if(_edit)
      tmp = _editVal;
    tmp = tmp - tmp % _valStep + _valStep;
    if (tmp > _valHiLimit)
      tmp = _valLoLimit;
    set(tmp);
  }

  virtual void down() {
    long tmp = _val;
    if(_edit)
        tmp = _editVal;
    tmp = tmp - tmp % _valStep - _valStep;
    if (tmp < _valLoLimit)
      tmp = _valHiLimit;
    set(tmp);
  }

  virtual void edit() {
    _editVal = (int)(*this);
    _edit = true;
  }

  virtual bool save() {
    _edit = false;
    set(_editVal);
    return true;
  }

  virtual void discard() {
    _edit = false;
  }

  virtual String toString(bool shorten = false) {
    if(_edit)
      return (String) _editVal;
    else
      return (String) _val;
  }

  virtual String getName(bool shorten = false) {
    return F("param");
  }

  virtual bool set(long val) {
    if(_edit){
      _editVal = val;
      return true;
    }
    if (val < _valLoLimit)
      val = _valLoLimit;
    if (val > _valHiLimit)
      val = _valHiLimit;
    _val = val;
    return true;
  }
  virtual long get() {
    if(_edit){
      return _editVal;
    }
    return _val;
  }

  virtual operator long() {
    return get();
  }

  virtual IParam& operator=(long val) {
    set(val);
    return *this;
  }

  virtual bool isEdit() {
    return _edit;
  }

  virtual ~LimitedParam() {};

protected:
  long _valHiLimit;
  long _valLoLimit;
  short _valStep;
  long _val;

  static long _editVal;
  static bool _edit;
};

class enumParamMapItem{
public:
  long   value;
  const __FlashStringHelper* label;
};

#ifndef MAP_SIZE(x)
#define MAP_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

class EnumedParam : public LimitedParam {
public:
  EnumedParam() : EnumedParam(0){}
  EnumedParam(long val): _map(NULL){
    _valStep = 1;
    _valLoLimit = 0;
    _valHiLimit = 100;
    this->set(val);
  }

  virtual long get(){
    return _val;
  }

  virtual operator long() {
    if(_map != NULL)
      return _map[_val].value;

    return _val;
  }

  virtual bool setByVal(long val){
    //Serial.println((String)F("SetByVal: ") + val);
    //Serial.println((String)F("_valHiLim: ") + _valHiLimit);
    if(!_map)
      return false;
    for(int i = 0; i<=_valHiLimit; i++){
      //Serial.println((String)F("Checking: ") + _map[i].value);
      if(_map[i].value == val){
        _val = i;
        //Serial.println(F("SetByVal: true"));
        return true;
      }
    }
    //Serial.println(F("SetByVal: false"));
    return false;
  }

  virtual String toString(long val){
    if(_map != NULL)
      return _map[val].label;

    return (String) val;
  }

protected:
  enumParamMapItem *_map;
};

}

#endif /* PHOTOPIZZA_V1_2_2_PARAM_CPP_ */
