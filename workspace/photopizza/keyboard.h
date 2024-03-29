/**
 * File: keyboard.h
 * Created on: 26 feb 2015 г.
 * Description:
 * PhotoPizza DIY is an open source project of 360° product photography turntable.
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>
 * Project Author: Vladimir Matiyasevith <vladimir.m@makerdrive.com>
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

#ifndef PHOTOPIZZA_V1_2_2_KEYBOARD_H_
#define PHOTOPIZZA_V1_2_2_KEYBOARD_H_

namespace PhotoPizza{
typedef enum {

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
}kbKeys;

kbKeys kbGetKey();
int kbGetNumericKey(int key);

}


#endif /* PHOTOPIZZA_V1_2_2_KEYBOARD_H_ */
