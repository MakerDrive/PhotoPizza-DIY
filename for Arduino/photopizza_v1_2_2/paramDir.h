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
#include "param.h"

namespace PhotoPizza {

class paramDir : public EnumedParam {
public:
  paramDir() : paramDir(0){}
  paramDir(long val) {
    static enumParamMapItem dirMap[] = {
        {CW, F("CW")},
        {CCW, F("CCW")}
    };
    _valHiLimit = MAP_SIZE(dirMap) - 1;
    _map = dirMap;
    set(val);
  }

  virtual String getName(bool shorten = false){
    return F("dir");
  }
};
}



#endif /* PHOTOPIZZA_V1_2_2_PARAMDIR_H_ */
