/*
 * param.cpp
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAM_CPP_
#define PHOTOPIZZA_V1_2_2_PARAM_CPP_

namespace PhotoPizza {
class param {

public:

  param() {
    _valStep = 1;
    _val = 0;
    _tmpVal = 0;
    _valHiLimit = 100;
    _valLoLimit = 0;
    _edit = false;
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
    _tmpVal = _val;
    _edit = true;
  }

  virtual bool save() {
    _edit = false;
    return set(_tmpVal);
  }

  virtual void discard() {
    _edit = false;
  }

  virtual String ToString(bool shorten = false) {
    if (_edit)
      return (String) _tmpVal;
    else
      return (String) _val;
  }
  virtual String getName(bool shorten = false) {
    return "param";
  }

  virtual bool set(long val) {
    if (val < _valLoLimit)
      val = _valLoLimit;
    if (val > _valHiLimit)
      val = _valHiLimit;
    if (_edit)
      _tmpVal = val;
    else
      _val = val;
    return true;
  }
  virtual long get() {
    if (_edit)
      return _tmpVal;
    else
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
    return _edit;
  }

  virtual ~param() {
  }
  ;

protected:
  long _valHiLimit;
  long _valLoLimit;
  long _valStep;
  long _val;
  bool _edit;
  long _tmpVal;
};
}

#endif /* PHOTOPIZZA_V1_2_2_PARAM_CPP_ */
