/*
 * paramRun.cpp
 *
 *  Created on: 09 марта 2015 г.
 *      Author: romeo
 */

#include "paramRun.h"
#include <AccelStepper.h>
#include <limits.h>

#include "presetManager.h"

namespace PhotoPizza{

AccelStepper stepper(AccelStepper::DRIVER, MOTOR_STP_PIN, MOTOR_DIR_PIN);

void paramRun::stopPreset(){
  if(!_run)
      return;
  _val = 0;
  _delayTime = 0;
  Serial.println(F("stopping"));
  stepper.stop();
  presetManager::get()->update();
  return;
}

void paramRun::loop(){
  presetManager *pMgr = presetManager::get();
  if(!stepper.run() && _run){

    if(!_delay){
      unsigned long now = millis();
      _delayTime = now + pMgr->getPreset()->_pause;
        _ovr = false;

      if(_delayTime < now) //check for overflow
        _ovr = true;

      _delay = true;
    }

    if(_delay){

      //if(!_ovr){ //todo: handle overflow
        if(millis() <= _delayTime)
          return;

        _delay = false;

      if(_iterCount == 0 || _val == 0){
        _run = false;
        _val = 0;
        pMgr->update();
        Serial.println(F("Finished"));
      }else
        startMotor();
    }
  }
}

void paramRun::startMotor(){
  presetManager *presetMgr = presetManager::get();

  if(_iterCount >= 0)
    --_iterCount;

  long steps = presetMgr->getPreset()->_steps * presetMgr->getPreset()->_dir; //TODO: refactor getters (via local vars)
  Serial.println((String)F("Accel") + presetMgr->getPreset()->_acc);
  Serial.println((String)F("Steps") + steps);
  Serial.println((String)F("Speed") + presetMgr->getPreset()->_speed);
  stepper.setCurrentPosition(0L);
  if(presetMgr->getPreset()->_acc == 0){
    stepper.setAcceleration(10000000); //no acc.
  }else
    stepper.setAcceleration(presetMgr->getPreset()->_acc);
  if(steps != 0){
    stepper.moveTo(steps);
  }else
    stepper.moveTo(LONG_MAX * presetMgr->getPreset()->_dir);
  stepper.setMaxSpeed(presetMgr->getPreset()->_speed);
}

void paramRun::edit() {
  if(_run)
    return;
  Serial.println(F("Run"));
  _val = 1;
  _run = true;
  _iterCount = presetManager::get()->getPreset()->_iter;
  startMotor();

  presetManager::get()->update();
}

};
