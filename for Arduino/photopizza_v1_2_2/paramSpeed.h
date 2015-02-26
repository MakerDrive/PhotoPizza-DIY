/*
 * paramSpeed.h
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMSPEED_H_
#define PHOTOPIZZA_V1_2_2_PARAMSPEED_H_

#include "defines.h"
#include "param.h"

namespace PhotoPizza {
class paramSpeed : public param {
public:
  paramSpeed() : paramSpeed(1000){}
  paramSpeed(long val){
    _valStep = SPEED_STEP;
    _valLoLimit = SPEED_MIN;
    _valHiLimit = SPEED_MAX;
    this->set(val);
  }

  virtual String getName(bool shorten = false){
    return "speed";
  }
};
}



#endif /* PHOTOPIZZA_V1_2_2_PARAMSPEED_H_ */
