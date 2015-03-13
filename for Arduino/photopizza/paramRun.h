/*
 * paramRun.h
 *
 *  Created on: 09 марта 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_PARAMRUN_H_
#define PHOTOPIZZA_V1_2_2_PARAMRUN_H_


#include <Timer.h>
#include "defines.h"
#include "param.h"


namespace PhotoPizza {

class relayPause: public Timer{
public:
  relayPause(unsigned long delayMs): Timer(delayMs, NULL){
    pinMode(RELAY_PIN, OUTPUT);
  };
  ~relayPause(){};
  void startDelayed(){
    digitalWrite(RELAY_PIN, HIGH);
    Timer::start();
  };
  virtual boolean operator()(){
    digitalWrite(RELAY_PIN, LOW);
    return true;
  }
};

class paramRun : public EnumedParam, public Timer{
public:

  paramRun() : paramRun(0){}
  paramRun(long val): Timer(0, NULL), _relay(RELAY_HOLD_TIMEOUT){
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

  virtual void up();
  virtual void down();

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
