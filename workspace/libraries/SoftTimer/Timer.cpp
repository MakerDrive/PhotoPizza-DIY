/**
 * File: Timer.cpp
 * Description:
 * SoftTimer library is a lightweight but effective event based timeshare solution for Arduino.
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>, Balazs Kelemen <prampec+arduino@gmail.com>
 * Copyright: 2014 Roman Savrulin
 * Copying permission statement:
    This file is part of SoftTimer.

    SoftTimer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Timer.h"

#include <Arduino.h>

Timer::Timer(unsigned long delayMs, Timer* nextTask)
    : Task(delayMs, &(Timer::step)) {
  this->_nextTimer = _nextTimer;
  this->_state = STATE_STOPPED;
}

void Timer::start() {
  _state = STATE_RUNNING;
  SoftTimer.add(this, false);
}

void Timer::stop(bool launchTask){
  if(launchTask)
    step(this);

  // anyway do the following sequence
  this->_state = STATE_STOPPED;
  SoftTimer.remove(this);
}

/*static */ void Timer::step(Task* task) {
  Timer* dr = (Timer*)task;
  if(dr->_state == STATE_RUNNING) {
    // -- Remove me from Timer Manager.
    SoftTimer.remove(dr);
    dr->_state = STATE_STOPPED;

    boolean retVal = (*dr)();
    if(retVal) {
      if(dr->_nextTimer != NULL) {
        dr->_nextTimer->start();
      }
    }
  }else
    SoftTimer.remove(dr);
}
