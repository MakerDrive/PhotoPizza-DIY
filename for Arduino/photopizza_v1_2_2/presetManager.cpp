#include "presetManager.h"

void presetManager::init() {

  _cur = 0;
  byte flag;
  EEPROM_readAnything(0, flag);

  /*if (flag == 204) { // 11 00 11 00
    EEPROM_readAnything(1, presets[0]); //  first int in memory
    EEPROM_readAnything(1 + PRESET_SIZE, presets[1]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 2, presets[2]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 3, presets[3]); //
    return;
  }*/ //TODO: restore

  _presets[0]._speed = 4300;
  _presets[0]._steps = 3200;
  _presets[0]._acc = 5000;
  _presets[0]._dir = CW;

  _presets[1]._speed = 3300;
  _presets[1]._steps = 3200;
  _presets[1]._acc = 5000;
  _presets[1]._dir = CW;

  _presets[2]._speed = 1000;
  _presets[2]._steps = 10000;
  _presets[2]._acc = 5000;
  _presets[2]._dir = CW;

  _presets[3]._speed = 2000;
  _presets[3]._steps = 20000;
  _presets[3]._acc = 5000;
  _presets[3]._dir = CW;

  flag = 204;
  EEPROM_writeAnything(0, flag);
  EEPROM_writeAnything(1, _presets[0]); //  first byte in memory
  EEPROM_writeAnything(1 + PRESET_SIZE, _presets[1]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 2, _presets[2]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 3, _presets[3]); //

}

void presetManager::save() { // read mem -> check for changes -> write if changed => EEPROM live longer =)
  preset orig;
  EEPROM_readAnything((1 + getCur() * PRESET_SIZE), orig);

  if ((orig._speed != get_cur_preset()._speed) || (orig._steps != get_cur_preset()._steps) //TODO: refactor this
      || (orig._acc != get_cur_preset()._acc) || (orig._dir != get_cur_preset()._dir)) {
    EEPROM_writeAnything((1 + getCur() * PRESET_SIZE), get_cur_preset());
    byte f = 204;
    EEPROM_writeAnything(0, f);
  }
}

void presetManager::next() {
  _cur = (_cur + 1) % NUM_PROGRAMS;
}

void presetManager::prev() {
  _cur = (_cur + NUM_PROGRAMS - 1) % NUM_PROGRAMS;
}

int presetManager::getCur() {
  return _cur;
}

preset presetManager::get_cur_preset() {
  return _presets[_cur];
}

long presetManager::getValue(byte _pos) {
  long rez = 0;
  switch (_pos) {
  case SPEEED:
    rez = _presets[_cur]._speed;
    break;
  case ROT:
    rez = _presets[_cur]._steps;
    break;
  case ACC:
    rez = _presets[_cur]._acc;
    break;
  case DIR:
    rez = _presets[_cur]._dir;
    break;
  }
  return rez;
}

void presetManager::setValue(long _val, byte _pos) {
  switch (_pos) {
  case SPEEED:
    if (_val < SPEED_MIN)
      _val = SPEED_MIN;
    if (_val > SPEED_MAX)
      _val = SPEED_MAX;
    _presets[_cur]._speed = _val;
    break;

  case ROT:
    if (_val < ROT_MIN)
      _val = ROT_MIN;
    if (_val > ROT_MAX)
      _val = ROT_MAX;
    _presets[_cur]._steps = _val;
    break;

  case ACC:
    if (_val < ACC_MIN)
      _val = ACC_MIN;
    if (_val > ACC_MAX)
      _val = ACC_MAX;
    _presets[_cur]._acc = _val;
    break;

  case DIR:
    if (_val == 0) {
      _presets[_cur]._dir = CCW;
    } else {
      _presets[_cur]._dir = CW;
    }
    break;
  }

}

void presetManager::valueUp(byte _pos) {
  long _val = 0;
  switch (_pos) {
  case SPEEED: {
    _val = SPEED_STEP;
    long tmp = _presets[_cur]._speed + _val;
    if ((_presets[_cur]._speed > SPEED_MAX) || tmp > SPEED_MAX) {
      _presets[_cur]._speed = SPEED_MAX;
      break;
    }
    _presets[_cur]._speed = tmp;
  }
    break;

  case ROT: {
    _val = ROT_STEP;
    long tmp = _presets[_cur]._steps + _val;
    if ((_presets[_cur]._steps > ROT_MAX) || tmp > ROT_MAX) {
      _presets[_cur]._steps = ROT_MAX;
      break;
    }
    _presets[_cur]._steps = tmp;
  }
    break;

  case ACC: {
    _val = ACC_STEP;
    long tmp = _presets[_cur]._acc + _val;
    if ((_presets[_cur]._acc > ACC_MAX) || tmp > ACC_MAX) {
      _presets[_cur]._acc = ACC_MAX;
      break;
    }
    _presets[_cur]._acc = tmp;
  }
    break;

  case DIR: {
    _val = CW;
    _presets[_cur]._dir = CW;
  }
    break;
  }
}

void presetManager::valueDown(byte _pos) {
  long _val = 0;
  switch (_pos) {
  case SPEEED: {
    _val = SPEED_STEP;
    long tmp = _presets[_cur]._speed - _val;
    if ((_presets[_cur]._speed < SPEED_MIN) || tmp < SPEED_MIN) {
      _presets[_cur]._speed = SPEED_MIN;
      break;
    }
    _presets[_cur]._speed = tmp;
  }
    break;

  case ROT: {
    _val = ROT_STEP;
    long tmp = _presets[_cur]._steps - _val;
    if ((_presets[_cur]._steps < ROT_MIN) || tmp < ROT_MIN) {
      _presets[_cur]._steps = ROT_MIN;
      break;
    }
    _presets[_cur]._steps = tmp;
  }
    break;

  case ACC: {
    _val = ACC_STEP;
    long tmp = _presets[_cur]._acc - _val;
    if ((_presets[_cur]._acc < ACC_MIN) || tmp < ACC_MIN) {
      _presets[_cur]._acc = ACC_MIN;
      break;
    }
    _presets[_cur]._acc = tmp;
  }
    break;

  case DIR: {
    _val = CCW;
    _presets[_cur]._dir = CCW;
  }
    break;
  }
}

void presetManager::change_direction(int _dir) {
  _presets[_cur]._dir = _dir;
}

