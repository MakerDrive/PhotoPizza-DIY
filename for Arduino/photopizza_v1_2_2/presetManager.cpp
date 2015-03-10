#include "presetManager.h"

using namespace PhotoPizza;

/* static */ LimitedParam preset::_default;
/* static */ paramRun preset::_run;
/* static */ presetManager *presetManager::_presetMgr = NULL;

static presetStorage ps;

#define EEPROM_FLAG 204
#define EEPROM_VER 4

presetManager::presetManager() {

  _curPreset = 0;
  _curParam = FIRST_PARAM;
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

  EEPROM_readAnything(0 + sizeof(ps) * num, ps);
  if (ps.flag == EEPROM_FLAG && ps.version == EEPROM_VER) { // 11 00 11 00
    if(set)
      _preset = ps.data;
    return true;
  }

  switch(num){
  case 0:
    _preset = (presetStorageData){4300, -3200, 5000};
    break;
  case 1:
    _preset = (presetStorageData){3300, -3200, 5000};
    break;
  case 2:
    _preset = (presetStorageData){1000, -10000, 5000};
    break;
  case 3:
    _preset = (presetStorageData){2000, -20000, 5000};
    break;
  }

  return false;
}

bool presetManager::savePreset(unsigned short num){
  if(num >= NUM_PROGRAMS)
    return false;

  ps.flag = EEPROM_FLAG;
  ps.version = EEPROM_VER;
  ps.data = _preset;
  Serial.println((String)F("Sp: ") + ps.data._speed);
  Serial.println((String)F("acc: ") + ps.data._acc);
  Serial.println((String)F("steps: ") + ps.data._steps);
  EEPROM_writeAnything(0 + sizeof(ps) * num, ps);

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
  _curParam = (paramType) ((_curParam + 1) % PARAM_COUNT);
}

void presetManager::prevParam(){
  _curParam = (paramType) ((_curParam + PARAM_COUNT - 1) % PARAM_COUNT);
}

IParam* presetManager::getParam(){
  return &(*getPreset())[_curParam];
}

void presetManager::edit(){
  LimitedParam *src = static_cast<LimitedParam *>(getParam());
  LimitedParam *dst = static_cast<LimitedParam *>(&(*getPreset())[SAVED_PARAM]);
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
  LimitedParam *src = static_cast<LimitedParam *>(&(*getPreset())[SAVED_PARAM]);
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

long presetManager::getValue(paramType pos) {
  return _preset[pos];
}

void presetManager::setValue(paramType pos, long val) {
  _preset[pos] = val;
}

void presetManager::valueUp(paramType pos) {
  _preset[pos].up();
}

void presetManager::valueDown(paramType pos) {
  _preset[pos].down();
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

