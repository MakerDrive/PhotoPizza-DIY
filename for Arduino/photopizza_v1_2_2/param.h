/*
 * param.cpp
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAM_CPP_
#define PHOTOPIZZA_V1_2_2_PARAM_CPP_

namespace PhotoPizza {

class IParam {

public:

  virtual void up() = 0;
  virtual void down() = 0;
  virtual void edit() = 0;
  virtual bool save() = 0;
  virtual void discard() = 0;
  virtual String ToString() = 0;
  virtual const char* getName() = 0;
  virtual bool set(long val) = 0;
  virtual long get() = 0;
  virtual operator long() = 0;
  virtual IParam& operator=(long val) = 0;

  virtual bool isEdit() = 0;

  virtual ~IParam() {};

};

class param {

public:

  param() {
    _valStep = 1;
    _val = 0;
    _valHiLimit = 100;
    _valLoLimit = 0;
  }

  virtual void up() {
    long tmp = get(); //TODO: fix! works incorrect with enumed settings
    tmp = tmp - tmp % _valStep + _valStep;
    if (tmp > _valHiLimit)
      tmp = _valLoLimit;
    set(tmp);
  }
  virtual void down() {
    long tmp = get();
    tmp = tmp - tmp % _valStep - _valStep;
    if (tmp < _valLoLimit)
      tmp = _valHiLimit;
    set(tmp);
  }

  virtual void edit() {
  }

  virtual bool save() {
    return true;
  }

  virtual void discard() {
  }

  virtual String ToString(bool shorten = false) {
    return (String) _val;
  }
  virtual String getName(bool shorten = false) {
    return F("param");
  }

  virtual bool set(long val) {
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

  virtual operator long() {
    return get();
  }

  virtual param& operator=(long val) {
    set(val);
    return *this;
  }

  virtual bool isEdit() {
    return false;
  }

  virtual ~param() {
  }
  ;

protected:
  long _valHiLimit;
  long _valLoLimit;
  short _valStep;
  long _val;
};
}

#endif /* PHOTOPIZZA_V1_2_2_PARAM_CPP_ */
