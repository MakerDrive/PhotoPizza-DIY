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

#include <DelayRun.h>

namespace PhotoPizza {

#define ledPin 13
class relayPause: public DelayRun{
public:
  relayPause(unsigned long delayMs): DelayRun(delayMs, NULL){
    pinMode(ledPin, OUTPUT);
  };
  ~relayPause(){};
  void startDelayed(){
    digitalWrite(ledPin, HIGH);
    DelayRun::startDelayed();
  };
  virtual boolean operator()(){
    digitalWrite(ledPin, LOW);
    return true;
  }
};

class paramRun : public EnumedParam, public DelayRun{
public:

  paramRun() : paramRun(0){}
  paramRun(long val): DelayRun(0, NULL), _relay(100){//_iterRepeatTask(0, /*boolean (*)(Task*))*/NULL) {
    static enumParamMapItem map[] = {
        {0, F("")},
        {1, F(">>>")}
    };
    _valHiLimit = MAP_SIZE(map) - 1;
    _map = map;
    _run = false;
    _iterCount = 0;
    _delayTime = 0;
    _relayCycle = false;
    set(val);
  }

  virtual boolean operator()();

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
  bool _relayCycle;
  unsigned long _delayTime;
  void stopPreset();
  bool startMotor(Task *t);
  relayPause _relay;
  short _iterCount;
private:
};
}


#endif /* PHOTOPIZZA_V1_2_2_PARAMRUN_H_ */
