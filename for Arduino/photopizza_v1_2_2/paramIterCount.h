/*
 * paramIterCount.h
 *
 *  Created on: 10 марта 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMITERCOUNT_H_
#define PHOTOPIZZA_V1_2_2_PARAMITERCOUNT_H_

#include "defines.h"
#include "param.h"

namespace PhotoPizza {
class paramIterCount : public LimitedParam {
public:
  paramIterCount() : paramIterCount(0){}
  paramIterCount(long val){
    _valStep = ITER_COUNT_STEP;
    _valLoLimit = ITER_COUNT_MIN;
    _valHiLimit = ITER_COUNT_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return F("iter");
  }

  virtual String ToString(bool shorten = false){
    long val = get();
    if(val > 0)
      return (String)val;
    else if(val == 0)
      return F("inf");
    else
      return F("?");
  }
};
}

#endif /* PHOTOPIZZA_V1_2_2_PARAMITERCOUNT_H_ */
