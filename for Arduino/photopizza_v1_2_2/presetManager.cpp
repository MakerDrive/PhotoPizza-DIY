#include "presetManager.h"

using namespace PhotoPizza;

/* static */ param preset::_default;

static presetStorage ps;

#define EEPROM_FLAG 204
#define EEPROM_VER 3

void presetManager::init() {

  _curPreset = 0;
  _curParam = SPEED;
  byte flag;

  EEPROM_readAnything(0, ps);

  _preset[0] = {4300, 3200, 5000, CW };

  _preset[1] = {3300, 3200, 5000, CW};

  _preset[2] = {1000, 10000, 5000, CW};

  _preset[3] = {2000, 20000, 5000, CW};

  if (ps.flag == EEPROM_FLAG && ps.version == EEPROM_VER) { // 11 00 11 00
    Serial.println(F("Loading presets..."));
    for(int i = 0; i < NUM_PROGRAMS; ++i){
      _preset[i] = ps.data[i];
    }
    return;
  }

  ps.flag = EEPROM_FLAG;
  ps.version = EEPROM_VER;

  Serial.println(F("EEPROM data is invalid. Resetting..."));

  save(true);
}

void presetManager::save(bool force) { // read mem -> check for changes -> write if changed => EEPROM live longer =)

  bool update = force;
  for(int i = 0; i < NUM_PROGRAMS; ++i){
    if(_preset[i] != ps.data[i]){
      Serial.println(F("preset has changed, saving"));
      update = true;
      ps.data[i] = _preset[i];
      /*Serial.println((String)"Sp: " + ps.data[i]._speed);
      Serial.println((String)"acc: " + ps.data[i]._acc);
      Serial.println((String)"steps: " + ps.data[i]._steps);*/
    }
  }

  if(!update){
    Serial.println(F("EEPROM data is actual, not saving"));
    return;
  }else
    Serial.println(F("EEPROM data needs update, saving"));

  ps.flag = EEPROM_FLAG;
  ps.version = EEPROM_VER;

  EEPROM_writeAnything(0, ps);
}

void presetManager::nextParam(){
  _curParam = (paramType) ((_curParam + 1) % PARAM_COUNT);
}

void presetManager::prevParam(){
  _curParam = (paramType) ((_curParam + PARAM_COUNT - 1) % PARAM_COUNT);
}

param* presetManager::getParam(){
  return &(*get())[_curParam];
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

