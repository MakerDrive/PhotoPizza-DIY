#include "presetManager.h"
#include <avr/pgmspace.h>

using namespace PhotoPizza;

/* static */ LimitedParam preset::_default;
/* static */ paramRun preset::_run;
/* static */ presetManager *presetManager::_presetMgr = NULL;

static presetStorage ps;

#define EEPROM_FLAG 204
#define EEPROM_VER 5

const PROGMEM presetStorageData psData[NUM_PROGRAMS] = {
    {-3100, 5000, 100,  0, 1000},
    { 3200, 5000, 200, 10, 2000},
    {-3300, 5000, 300, 20, 3000},
    {-3400, 5000, 400, 30, 4300},
};

presetManager::presetManager() {

  _curPreset = 0;
  _curParam = preset::FIRST_PARAM;
  _edit = false;
  _update = false;

  Serial.println(F("Validating presets..."));
  for(int i = 0; i < NUM_PROGRAMS; ++i){
    if (!loadPreset(i, true)) {
      Serial.println((String) F("Data for preset ") + i + F(" is invalid, saving default"));
      savePreset(i);
    }
  }

  loadPreset(0, true);

  //_run = &(_preset[0]._run);//&(getPreset()->_run);
}

bool presetManager::loadPreset(unsigned short num, bool set){
  if(num >= NUM_PROGRAMS)
    return false;

  bool valid = true;

  EEPROM_readAnything(sizeof(presetStorage) * num, ps);
  if (ps.flag != EEPROM_FLAG || ps.version != EEPROM_VER){
    valid = false;
    Serial.println((String)F("Loading def preset ") + num + F(" from ") + (uint_farptr_t)&(psData[num]));
    memcpy_PF((void *)&ps.data, (uint_farptr_t)&(psData[num]), sizeof(presetStorageData));
  }

//  Serial.println((String)F("Sp: ") + ps.data._speed);
//  Serial.println((String)F("acc: ") + ps.data._acc);
//  Serial.println((String)F("steps: ") + ps.data._steps);

  if(set)
    _preset = ps.data;
  return valid;
}

bool presetManager::savePreset(unsigned short num){
  if(num >= NUM_PROGRAMS)
    return false;

  ps.flag = EEPROM_FLAG;
  ps.version = EEPROM_VER;
  ps.data = _preset;
//  Serial.println((String)F("Sp: ") + ps.data._speed);
//  Serial.println((String)F("acc: ") + ps.data._acc);
//  Serial.println((String)F("steps: ") + ps.data._steps);
  EEPROM_writeAnything(sizeof(presetStorage) * num, ps);

  return true;
}

void presetManager::loop(){
  _preset._run.loop();
}

void presetManager::save(bool force) { // read mem -> check for changes -> write if changed => EEPROM live longer =)

  _edit = false;
  getParam()->save();

  loadPreset(_curPreset, false);
  if(_preset != ps.data || force){
    Serial.println((String)F("preset ") + _curPreset + F(" has changed, saving"));
    savePreset(_curPreset);
  }
}

void presetManager::nextParam(){
  _curParam = (preset::paramType) ((_curParam + 1) % preset::PARAM_COUNT);
}

void presetManager::prevParam(){
  _curParam = (preset::paramType) ((_curParam + preset::PARAM_COUNT - 1) % preset::PARAM_COUNT);
}

IParam* presetManager::getParam(){
  return &(*getPreset())[_curParam];
}

void presetManager::edit(){
  LimitedParam *src = static_cast<LimitedParam *>(getParam());
  LimitedParam *dst = static_cast<LimitedParam *>(&(*getPreset())[preset::SAVED_PARAM]);
  *dst = *src; //save param we are changing*/
  getParam()->edit();
  _edit = true;
}

bool presetManager::isEdit(){
  return _edit;
}

void presetManager::discard(){
  getParam()->discard();
  LimitedParam *dst = static_cast<LimitedParam *>(getParam());
  LimitedParam *src = static_cast<LimitedParam *>(&(*getPreset())[preset::SAVED_PARAM]);
  *dst = *src; //load saved param from default value*/
  _edit = false;
}

void presetManager::nextPreset() {
  firstParam();
  _curPreset = (_curPreset + 1) % NUM_PROGRAMS;
  loadPreset(_curPreset, true);
}

void presetManager::prevPreset() {
  firstParam();
  _curPreset = (_curPreset + NUM_PROGRAMS - 1) % NUM_PROGRAMS;
  loadPreset(_curPreset, true);
}

int presetManager::getPresetNumber() {
  return _curPreset;
}

preset* presetManager::getPreset() {
  return &_preset;
}

long presetManager::getValue() {
  return _preset[_curParam];
}

void presetManager::setValue(long val) {
  _preset[_curParam] = val;
}

void presetManager::valueUp() {
  _preset[_curParam].up();
}

void presetManager::valueDown() {
  _preset[_curParam].down();
}

void presetManager::changeDirection() {
  _preset._dir.up();
  save();
  update();
}

void presetManager::run(){
  _edit = true;
  firstParam();
  _preset._run.edit();
}

void presetManager::stop(){
  _edit = false;
  firstParam();
  _preset._run.discard();
}

presetManager * presetManager::get(){
  if(_presetMgr == NULL){
    static presetManager presetMgr;
    _presetMgr = &presetMgr;
  }

  return _presetMgr;
}

