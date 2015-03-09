/*
 * paramSteps.h
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMSTEPS_H_
#define PHOTOPIZZA_V1_2_2_PARAMSTEPS_H_

#include "defines.h"
#include "param.h"

namespace PhotoPizza {
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
    return F("steps");
  }

  virtual String ToString(bool shorten = false){
    long val = get();
    if(val == 0)
      return F("inf");
    else
      return (String) val;
  }
};
}



#endif /* PHOTOPIZZA_V1_2_2_PARAMSTEPS_H_ */
