/*
 * keyboard.h
 *
 *  Created on: 26 февр. 2015 г.
 *      Author: romeo
 */

#ifndef PHOTOPIZZA_V1_2_2_KEYBOARD_H_
#define PHOTOPIZZA_V1_2_2_KEYBOARD_H_

namespace PhotoPizza{
typedef enum kbKeys{

  kbNoKey = -1,  //!!! DO NOT SET NUMBERS OR CHANGE ORDER!

  kbBtn0 = 0, //numeric
  kbBtn1,
  kbBtn2,
  kbBtn3,
  kbBtn4,
  kbBtn5,
  kbBtn6,
  kbBtn7,
  kbBtn8,
  kbBtn9,

  kbPwr,
  kbMenu,

  kbBksp,
  kbClear,
  kbTest,

  kbOk,

  kbUp,
  kbDown,
  kbLeft,
  kbRight,



  kbBtnCount //!!! DO NOT SET NUMBERS OR CHANGE ORDER!
};

kbKeys kbGetKey();
int kbGetNumericKey(int key);

}


#endif /* PHOTOPIZZA_V1_2_2_KEYBOARD_H_ */
