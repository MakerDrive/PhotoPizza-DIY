/*
 * paramDir.h
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMDIR_H_
#define PHOTOPIZZA_V1_2_2_PARAMDIR_H_

#include "defines.h"
#include "param.h"

namespace PhotoPizza {
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
}



#endif /* PHOTOPIZZA_V1_2_2_PARAMDIR_H_ */
