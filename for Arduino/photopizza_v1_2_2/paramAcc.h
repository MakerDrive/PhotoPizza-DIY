/*
 * paramAcc.h
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMACC_H_
#define PHOTOPIZZA_V1_2_2_PARAMACC_H_

#include "defines.h"
#include "param.h"

namespace PhotoPizza {
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
    return F("accel");
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



#endif /* PHOTOPIZZA_V1_2_2_PARAMACC_H_ */
