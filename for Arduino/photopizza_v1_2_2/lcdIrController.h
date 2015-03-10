/*
 * lib.h
 *
 *  Created on: 23 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_LCDIRCONTROLLER_H_
#define PHOTOPIZZA_V1_2_2_LCDIRCONTROLLER_H_

#include "presetManager.h"

namespace PhotoPizza {

class lcdIrController{

public:
void loop();
void init();

private:
void showProgram();

void editMode();
void menuMode();

void sayHello();
void printProgNum();
void printDir(int dir);

private:
static presetManager *_presetMgr;
};

}

#endif /* PHOTOPIZZA_V1_2_2_LCDIRCONTROLLER_H_ */
