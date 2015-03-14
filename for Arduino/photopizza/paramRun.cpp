/**
 * File: paramRun.cpp
 * Created on: 09 mar 2015 г.
 * Description:
 * PhotoPizza DIY is an open source project of 360° product photography turntable.
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>
 * Project Author:
 * Project Site: PhotoPizza.org
 *
 * Copyright: 2015 MakerDrive
 * Copying permission statement:
 *  This file is part of PhotoPizza DIY.
 *
 *  PhotoPizza DIY is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "paramRun.h"
#include <AccelStepper.h>
#include <limits.h>

#include "presetManager.h"

namespace PhotoPizza{

AccelStepper stepper(AccelStepper::DRIVER, MOTOR_STP_PIN, MOTOR_DIR_PIN);

boolean paramRun::operator()(){
  if(_iterCount == 0 || _val == 0){
    _run = false;
    _val = 0;
    presetManager::get()->update();
    Serial.println(F("Finished"));
  }else
    startMotor(NULL);
  return true;
}

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

void paramRun::up(){
  paramSpeed *sp = &presetManager::get()->getPreset()->_speed;
  sp->up();
  stepper.setMaxSpeed((long)*sp);
}

void paramRun::down(){
  paramSpeed *sp = &presetManager::get()->getPreset()->_speed;
  sp->down();
  stepper.setMaxSpeed((long)*sp);
}

void paramRun::loop(){
  presetManager *pMgr = presetManager::get();
  if(!stepper.run() && _run){
    if(!Timer::isRunning()){
      setPeriodMs(pMgr->getPreset()->_pause);
      start();
    }
    if(!_relay.isRunning() && !_relayCycle && _val){
      _relay.start();
      _relayCycle = true;
    }
    if(!_val){
      Timer::stop(true);
      _relay.stop(true);
    }
  }
}

bool paramRun::startMotor(Task *t){
  preset *preset = presetManager::get()->getPreset();

  if(_iterCount >= 0)
    --_iterCount;

  _relayCycle = false;
  long steps = preset->_steps * preset->_dir; // / preset->_iter; //TODO: refactor getters (via local vars)
  Serial.println((String)F("Accel") + preset->_acc);
  Serial.println((String)F("Steps") + steps);
  Serial.println((String)F("Speed") + preset->_speed);
  stepper.setCurrentPosition(0L);
  if(preset->_acc == 0){
    stepper.setAcceleration(10000000); //no acc.
  }else
    stepper.setAcceleration(preset->_acc);
  if(steps != 0){
    stepper.moveTo(steps);
  }else
    stepper.moveTo(LONG_MAX * preset->_dir);
  stepper.setMaxSpeed(preset->_speed);

  return false;
}

void paramRun::edit() {
  if(_run)
    return;
  Serial.println(F("Run"));
  _val = 1;
  _run = true;
  _iterCount = presetManager::get()->getPreset()->_iter;
  startMotor(NULL);

  presetManager::get()->update();
}

};
