#include "presetManager.h"

using namespace PhotoPizza;

/* static */ param preset::_default;

void presetManager::init() {

  _curPreset = 0;
  _curParam = SPEED;
  byte flag;

  _preset[0] = {4300, 3200, 5000, CW };

  _preset[1] = {3300, 3200, 5000, CW};

  _preset[2] = {1000, 10000, 5000, CW};

  _preset[3] = {2000, 20000, 5000, CW};

  /*EEPROM_readAnything(0, flag);

  if (flag == 204) { // 11 00 11 00
    EEPROM_readAnything(1, _presetStorage[0]); //  first int in memory
    EEPROM_readAnything(1 + PRESET_SIZE, _presetStorage[1]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 2, _presetStorage[2]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 3, _presetStorage[3]); //
    return;
  } //TODO: restore

  _presetStorage[0]._speed = 4300;
  _presetStorage[0]._steps = 3200;
  _presetStorage[0]._acc = 5000;
  _presetStorage[0]._dir = CW;

  _presetStorage[1]._speed = 3300;
  _presetStorage[1]._steps = 3200;
  _presetStorage[1]._acc = 5000;
  _presetStorage[1]._dir = CW;

  _presetStorage[2]._speed = 1000;
  _presetStorage[2]._steps = 10000;
  _presetStorage[2]._acc = 5000;
  _presetStorage[2]._dir = CW;

  _presetStorage[3]._speed = 2000;
  _presetStorage[3]._steps = 20000;
  _presetStorage[3]._acc = 5000;
  _presetStorage[3]._dir = CW;

  flag = 204;
  EEPROM_writeAnything(0, flag);
  EEPROM_writeAnything(1, _presetStorage[0]); //  first byte in memory
  EEPROM_writeAnything(1 + PRESET_SIZE, _presetStorage[1]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 2, _presetStorage[2]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 3, _presetStorage[3]); //*/

}

void presetManager::save() { // read mem -> check for changes -> write if changed => EEPROM live longer =)
  /*presetStorage orig;
  EEPROM_readAnything((1 + getPresetNumber() * PRESET_SIZE), orig);

  if ((orig._speed != getPreset()._speed) || (orig._steps != getPreset()._steps) //TODO: refactor this
      || (orig._acc != getPreset()._acc) || (orig._dir != getPreset()._dir)) {
    EEPROM_writeAnything((1 + getPresetNumber() * PRESET_SIZE), getPreset());
    byte f = 204;
    EEPROM_writeAnything(0, f);
  }*/
}

void presetManager::nextParam(){
  _curParam = (paramType) ((_curParam + 1) % PARAM_COUNT);
}

void presetManager::prevParam(){
  _curParam = (paramType) ((_curParam + PARAM_COUNT - 1) % PARAM_COUNT);
}

void presetManager::nextPreset() {
  _curParam = SPEED;
  _curPreset = (_curPreset + 1) % NUM_PROGRAMS;
}

void presetManager::prevPreset() {
  _curParam = SPEED;
  _curPreset = (_curPreset + NUM_PROGRAMS - 1) % NUM_PROGRAMS;
}

int presetManager::getPresetNumber() {
  return _curPreset;
}

preset* presetManager::get() {
  return &_preset[_curPreset];
}

/*param* presetManager::getParam() {
  return &_preset[_curPreset];
}*/

long presetManager::getValue(paramType pos) {
  return _preset[_curPreset][pos];
}

void presetManager::setValue(paramType pos, long val) {
  _preset[_curPreset][pos] = val;
}

void presetManager::valueUp(paramType pos) {
  _preset[_curPreset][pos].up();
}

void presetManager::valueDown(paramType pos) {
  _preset[_curPreset][pos].down();
}

void presetManager::changeDirection(int dir) {
  _preset[_curPreset]._dir = dir;
}

