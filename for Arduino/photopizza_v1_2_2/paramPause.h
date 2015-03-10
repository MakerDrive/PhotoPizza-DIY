/*
 * paramPause.h
 *
 *  Created on: 10 марта 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMPAUSE_H_
#define PHOTOPIZZA_V1_2_2_PARAMPAUSE_H_


#include "defines.h"
#include "param.h"

namespace PhotoPizza {
class paramPause : public LimitedParam {
public:
  paramPause() : paramPause(0){}
  paramPause(long val){
    _valStep = PAUSE_STEP;
    _valLoLimit = PAUSE_MIN;
    _valHiLimit = PAUSE_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return F("pause");
  }

  virtual String ToString(bool shorten = false){
    long val = get();
    if(val > 0)
      return (String)val;
    else if(val == 0)
      return F("none");
    else
      return F("?");
  }
};
}


#endif /* PHOTOPIZZA_V1_2_2_PARAMPAUSE_H_ */
