#include "presets.h"

void preset::init() {

  cur = 0;
  byte flag;
  EEPROM_readAnything(0, flag);

  if (flag == 204) { // 11 00 11 00
    EEPROM_readAnything(1, presets[0]); //  first int in memory
    EEPROM_readAnything(1 + PRESET_SIZE, presets[1]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 2, presets[2]); //
    EEPROM_readAnything(1 + PRESET_SIZE * 3, presets[3]); //
    return;
  }

  presets[0].sp = 4300;
  presets[0].rot = 0;
  presets[0].acc = 5000;
  presets[0].dir = CW;

  presets[1].sp = 3300;
  presets[1].rot = 0;
  presets[1].acc = 5000;
  presets[1].dir = CW;

  presets[2].sp = 1000;
  presets[2].rot = 10000;
  presets[2].acc = 5000;
  presets[2].dir = CW;

  presets[3].sp = 2000;
  presets[3].rot = 20000;
  presets[3].acc = 5000;
  presets[3].dir = CW;

  flag = 204;
  EEPROM_writeAnything(0, flag);
  EEPROM_writeAnything(1, presets[0]); //  first byte in memory
  EEPROM_writeAnything(1 + PRESET_SIZE, presets[1]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 2, presets[2]); //
  EEPROM_writeAnything(1 + PRESET_SIZE * 3, presets[3]); //

}

void preset::next() {
  cur = (cur + 1) % NUM_PROGRAMS;
}

void preset::prev() {
  cur = (cur + 3) % NUM_PROGRAMS;
}

int preset::get_cur() {
  return cur;
}

Preset_st preset::get_cur_preset() {
  return presets[cur];
}

long preset::get_val(byte _pos) {
  long rez = 0;
  switch (_pos) {
  case SPEEED:
    rez = presets[cur].sp;
    break;
  case ROT:
    rez = presets[cur].rot;
    break;
  case ACC:
    rez = presets[cur].acc;
    break;
  case DIR:
    rez = presets[cur].dir;
    break;
  }
  return rez;
}

void preset::change_val(long _val, byte _pos) {
  switch (_pos) {
  case SPEEED:
    if (_val < SPEED_MIN)
      _val = SPEED_MIN;
    if (_val > SPEED_MAX)
      _val = SPEED_MAX;
    presets[cur].sp = _val;
    break;

  case ROT:
    if (_val < ROT_MIN)
      _val = ROT_MIN;
    if (_val > ROT_MAX)
      _val = ROT_MAX;
    presets[cur].rot = _val;
    break;

  case ACC:
    if (_val < ACC_MIN)
      _val = ACC_MIN;
    if (_val > ACC_MAX)
      _val = ACC_MAX;
    presets[cur].acc = _val;
    break;

  case DIR:
    if (_val == 0) {
      presets[cur].dir = CCW;
    } else {
      presets[cur].dir = CW;
    }
    break;
  }

}

void preset::change_val_u(long _val, byte _pos) {
  switch (_pos) {
  case SPEEED: {
    long tmp = presets[cur].sp + _val;
    if ((presets[cur].sp > SPEED_MAX) || tmp > SPEED_MAX) {
      presets[cur].sp = SPEED_MAX;
      break;
    }
    presets[cur].sp = tmp;
  }
    break;

  case ROT: {
    long tmp = presets[cur].rot + _val;
    if ((presets[cur].rot > ROT_MAX) || tmp > ROT_MAX) {
      presets[cur].rot = ROT_MAX;
      break;
    }
    presets[cur].rot = tmp;
  }
    break;

  case ACC: {
    long tmp = presets[cur].acc + _val;
    if ((presets[cur].acc > ACC_MAX) || tmp > ACC_MAX) {
      presets[cur].acc = ACC_MAX;
      break;
    }
    presets[cur].acc = tmp;
  }
    break;

  case DIR: {
    presets[cur].dir = CW;
  }
    break;
  }
}

void preset::change_val_d(long _val, byte _pos) {
  switch (_pos) {
  case SPEEED: {
    long tmp = presets[cur].sp - _val;
    if ((presets[cur].sp < SPEED_MIN) || tmp < SPEED_MIN) {
      presets[cur].sp = SPEED_MIN;
      break;
    }
    presets[cur].sp = tmp;
  }
    break;

  case ROT: {
    long tmp = presets[cur].rot - _val;
    if ((presets[cur].rot < ROT_MIN) || tmp < ROT_MIN) {
      presets[cur].rot = ROT_MIN;
      break;
    }
    presets[cur].rot = tmp;
  }
    break;

  case ACC: {
    long tmp = presets[cur].acc - _val;
    if ((presets[cur].acc < ACC_MIN) || tmp < ACC_MIN) {
      presets[cur].acc = ACC_MIN;
      break;
    }
    presets[cur].acc = tmp;
  }
    break;

  case DIR: {
    presets[cur].dir = CCW;
  }
    break;
  }
}

void preset::change_direction(int _dir) {
  presets[cur].dir = _dir;
}

