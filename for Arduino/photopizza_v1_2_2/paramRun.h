/*
 * paramRun.h
 *
 *  Created on: 09 марта 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMRUN_H_
#define PHOTOPIZZA_V1_2_2_PARAMRUN_H_


#include "defines.h"
#include "param.h"
#include "param.h"
#include "lib.h"

namespace PhotoPizza {

class paramRun : public EnumedParam {
public:
  paramRun() : paramRun(0){}
  paramRun(long val) {
    static enumParamMapItem map[] = {
        {0, F("")},
        {1, F(">>>")}
    };
    _valHiLimit = MAP_SIZE(map) - 1;
    _map = map;
    _run = false;
    set(val);
  }

  virtual void up(){}
  virtual void down(){}

  virtual void edit();

  virtual bool save(){
    stopPreset();
    return true;
  }

  virtual void discard(){
    stopPreset();
  }

  virtual bool isEdit(){
    return _run || _val != 0;
  }

  virtual bool isRunning(){
    return _run;
  }

  virtual bool set(long int val){ return true;}

  virtual String getName(bool shorten = false){
    return F("Run");
  }
  void loop();

private:
  bool _run;
  //void finishPreset();
  void stopPreset();
private:
};
}


#endif /* PHOTOPIZZA_V1_2_2_PARAMRUN_H_ */
